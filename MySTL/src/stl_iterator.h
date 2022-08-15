//
// Created by GW00243464 on 2022/8/15.
//

#ifndef MYSTL_STL_ITERATOR_H
#define MYSTL_STL_ITERATOR_H

#include "stl_iterator_base.h"


namespace MySTL{

template <class Container>
class back_insert_iterator{
protected:
    Container* container;
public:
    typedef Container container_type;
    typedef output_iterator_tag iterator_category;

    explicit back_insert_iterator(Container& x) : container(&x){}
    back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
            { container->push_back(value);};
    back_insert_iterator<Container>& operator*() {return *this;}
    back_insert_iterator<Container>& operator++() {return *this;}
    back_insert_iterator<Container>& operator++(int) {return *this;}
};


template <class Container>
inline output_iterator_tag iterator_category(const back_insert_iterator<Container>&)
{   return output_iterator_tag();   }

template <class Container>
inline back_insert_iterator<Container> back_insert(Container& x)
{   return back_insert_iterator<Container>(x);  }








}
#endif //MYSTL_STL_ITERATOR_H
