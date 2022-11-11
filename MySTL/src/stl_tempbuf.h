#ifndef MYSTL_STL_TEMPBUF_H
#define MYSTL_STL_TEMPBUF_H

#include <stddef.h>
#include "stl_pair.h"
#include "type_traits.h"
#include "stl_uninitialized.h"
namespace MySTL
{
template <class Tp>
pair<Tp*, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, Tp*)
{

}

template <class Tp>
inline pair<Tp*, ptrdiff_t > get_temporary_buffer(ptrdiff_t len)
{
    return __get_temporary_buffer(len, (Tp*)0);
}



template <class Tp>
inline pair<Tp*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, Tp*)
{
    return __get_temporary_buffer(len, (Tp*) 0);
}

template <class Tp>
void return_temporary_buffer(Tp* p)
{
    free(p);
}


template <class ForwardIterator, class Tp>
class Temporary_buffer
{
private:
    ptrdiff_t M_original_len;
    ptrdiff_t M_len;
    Tp* M_buffer;

    void M_allocate_buffer()
    {
        M_original_len = M_len;
        M_buffer = 0;
        if(M_len > (ptrdiff_t)(INT_MAX / sizeof(Tp)))
            M_len = INT_MAX / sizeof(Tp);

        while(M_len > 0)
        {
            M_buffer = (Tp*) malloc(M_len * sizeof(Tp));
            if(M_buffer)
                break;
            M_len /= 2;
        }
    }

    void M_initialize_buffer(const Tp&, __true_type) {}
    void M_initialize_buffer(const Tp& val, __false_type)
    { uninitialized_fill_n(M_buffer, M_len, val); }

public:
    ptrdiff_t size() const { return M_len; }
    ptrdiff_t requested_size() const { return M_original_len; }
    Tp* begin() { return M_buffer; }
    Tp* end() { return M_buffer + M_len; }

    Temporary_buffer(ForwardIterator first, ForwardIterator last)
    {
        typedef typename type_traits<Tp>::is_POD_type Trivial;
        try
        {
            M_len = 0;
            distance(first, last, M_len);
            M_allocate_buffer();
            if(M_len > 0)
                M_initialize_buffer(*first, Trivial());
        }
        catch (...)
        {
            free(M_buffer);
            M_buffer = 0;
            M_len = 0;
            throw;
        }
    }

private:
    Temporary_buffer(const Temporary_buffer&) {}
    void operator=(const Temporary_buffer& ) {}
};

template <class ForwardIterator, class Tp = typename iterator_traits<ForwardIterator>::value_type>
struct temporary_buffer : public Temporary_buffer<ForwardIterator, Tp>
{
    temporary_buffer(ForwardIterator first, ForwardIterator last)
    : Temporary_buffer<ForwardIterator, Tp>(first, last) {}
    ~temporary_buffer() {}
};

} // namespace


#endif //MYSTL_STL_TEMPBUF_H
