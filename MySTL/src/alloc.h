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
    //TODO: 待实现
    static void* allocate(size_t n)
    {

    }


    static void deallocate(void *p, size_t n)
    {

    }


    static void* reallocate(void *p, size_t old_sz, size_t new_sz);
};

typedef default_alloc alloc;


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














}


#endif //MYSTL_ALLOC_H
