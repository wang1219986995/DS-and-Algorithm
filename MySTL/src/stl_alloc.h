//
// Created by WangJuncheng on 2022/8/14.
//

#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


namespace MySTL{

class malloc_alloc {
public:
    static void *allocate(size_t n)
    {
        void* result = malloc(n);
        if( 0 == result)
        {
            fprintf(stderr, "Out of memory.");
            exit(1);
        }
        return result;
    }

    static void deallocate(void* p, size_t) { free(p);}
    static void* reallocate(void* p, size_t , size_t new_sz)
    {
        void* result = realloc(p, new_sz);
        if(0 == result)
        {
            fprintf(stderr, "Out of memory.");
            exit(1);
        }
        return result;
    }
};


enum { ALIGN = 8};
enum { MAX_BYTES = 128};
enum { NFREELISTS = 16}; // _MAX_BYTES/_ALIGN

class default_alloc
{
private:
    static size_t S_round_up(size_t bytes)
    {return (bytes + (size_t)ALIGN-1) & ~((size_t)ALIGN-1);}


    union Obj{
        union Obj* M_free_list_link;
        char M_client_data[1];
    };

    static Obj* volatile S_free_list[NFREELISTS];

    static size_t S_freelist_index(size_t bytes)
    {return ((bytes) + (size_t)ALIGN-1)/(size_t)ALIGN-1;}

    static void* S_refill(size_t n);
    static char* S_chunk_alloc(size_t size, int& nobjs);

    // Chunk allocation state.
    static char* S_start_free;
    static char* S_end_free;
    static size_t S_heap_size;

public:
    static void* allocate(size_t n)
    {
        void* ret = 0;
        if(n > MAX_BYTES)
            ret = malloc_alloc::allocate(n);
        else
        {
            Obj* volatile* my_free_list = S_free_list + S_freelist_index(n);
            Obj* result = *my_free_list;
            if(0 == result) ret = S_refill(S_round_up(n));
            else
            {
                *my_free_list = result->M_free_list_link;
                ret = result;
            }
        }
        return ret;
    }

    static void deallocate(void *p, size_t n)
    {
        if(n > (size_t)MAX_BYTES)
            malloc_alloc::deallocate(p, n);
        else
        {
            Obj* volatile* my_free_list = S_free_list + S_freelist_index(n);
            ((Obj*)p)->M_free_list_link = *my_free_list;
            *my_free_list = (Obj*)p;
        }
    }

    static void* reallocate(void *p, size_t old_sz, size_t new_sz);
};

typedef default_alloc alloc;

char* default_alloc::S_start_free = 0;
char* default_alloc::S_end_free = 0;
size_t default_alloc::S_heap_size = 0;
typename default_alloc::Obj* volatile default_alloc::S_free_list[] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

char *default_alloc::S_chunk_alloc(size_t size, int &nobjs) {
    char *result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = S_end_free - S_start_free;

    if (bytes_left >= total_bytes) {
        result = S_start_free;
        S_start_free += total_bytes;
        return result;
    } else if (bytes_left >= size)
    {
        nobjs = (int)(bytes_left/size);
        total_bytes = size * nobjs;
        result = S_start_free;
        S_start_free += total_bytes;
        return result;
    }
    else
    {
        size_t bytes_to_get = 2 * total_bytes + S_round_up(S_heap_size >> 4);
        if(bytes_left > 0)
        {
            Obj* volatile* my_free_list = S_free_list + S_freelist_index(bytes_left);
            ((Obj*)S_start_free)->M_free_list_link = *my_free_list;
            *my_free_list = (Obj*)S_start_free;
        }
        S_start_free = (char*)malloc(bytes_to_get);
        if(0 == S_start_free)
        {
            size_t i;
            Obj* volatile* my_free_list;
            Obj* p;
            for(i = size; i <= (size_t)MAX_BYTES; i+= (size_t)ALIGN)
            {
                my_free_list = S_free_list + S_freelist_index(i);
                p = *my_free_list;
                if( 0 != p)
                {
                    *my_free_list = p->M_free_list_link;
                    S_start_free = (char*)p;
                    S_end_free = S_start_free + i;
                    return (S_chunk_alloc(size, nobjs));
                }
            }
            S_end_free = 0;
            S_start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        S_heap_size += bytes_to_get;
        S_end_free = S_start_free + bytes_to_get;
        return (S_chunk_alloc(size, nobjs));
    }
}


void* default_alloc::S_refill(size_t n)
{
    int nobjs = 20;
    char* chunk = S_chunk_alloc(n, nobjs);
    Obj* volatile* my_free_list;
    Obj* result;
    Obj* current_Obj;
    Obj* next_obj;
    int i;
    if(1 == nobjs)  return(chunk);
    my_free_list = S_free_list + S_freelist_index(n);

    result = (Obj*)chunk;
    *my_free_list = next_obj = (Obj*)(chunk + n);
    for(i = 1; ;i++)
    {
        result = (Obj*)chunk;
        *my_free_list = next_obj = (Obj*)(chunk + n);
        if(nobjs - 1 == i)
        {
            current_Obj->M_free_list_link = 0;
            break;
        }
        else    current_Obj->M_free_list_link = next_obj;
    }
    return result;
}

void* default_alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
{
    void* result;
    size_t copy_sz;
    if(old_sz > (size_t)MAX_BYTES && new_sz > (size_t)MAX_BYTES)
        return (realloc(p, new_sz));

    if(S_round_up(old_sz) == S_round_up(new_sz))    return p;
    result = allocate(new_sz);
    copy_sz = new_sz > old_sz ? old_sz : new_sz;
    memcpy(result, p, copy_sz);
    deallocate(p, old_sz);
    return result;
}

template <class Tp>
class allocator{
    typedef alloc Alloc;
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef Tp*         pointer;
    typedef const Tp*   const_pointer;
    typedef Tp&         reference;
    typedef const Tp&   const_reference;
    typedef Tp          value_type;

//TODO: 这个有用么？
    template <class Tp1> struct rebind {
        typedef allocator<Tp1> other;
    };

    allocator() noexcept {}
    allocator(const allocator&) noexcept {}
    template<class Tp1> allocator(const allocator<Tp1>&) noexcept {}
    ~allocator() noexcept {}

    pointer address(reference x) const {    return &x;  }
    const_pointer address(const_reference x) const {    return &x;  }

    //TODO: 这里的const void* = 0 好像也没用啊
    pointer allocate(size_type n, const void* = 0){
        return n != 0 ? static_cast<pointer>(Alloc::allocate(n * sizeof(Tp))) : 0;
    }

    void deallocate(pointer p, size_type n)
    { Alloc::deallocate(p, n * sizeof(Tp)); }

    size_type max_size() const noexcept
    {   return size_t(-1)/sizeof(Tp);   }

    void construct(pointer p, const_reference val)
    { new(p) Tp(val); }
    void destroy(pointer p) { p->~Tp();}
};


template<>
class allocator<void>
{
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
    typedef void        value_type;

    template<class Tp> struct rebind{
        typedef allocator<Tp> other;
    };
};


template <class Tp, class Allocator>
struct Alloc_traits{
    static const bool S_instalceless = false;
};

template <class Tp>
struct Alloc_traits<Tp, malloc_alloc>
{
    static const bool S_instanceless = true;
    typedef malloc_alloc allocator_type;
};

template <class Tp>
struct Alloc_traits<Tp, default_alloc>
{
    static const bool S_instanceless = true;
    typedef default_alloc allocator_type;
};












}


#endif //MYSTL_ALLOC_H
