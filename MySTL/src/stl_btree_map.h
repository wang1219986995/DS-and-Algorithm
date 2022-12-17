#ifndef MYSTL_STL_BTREE_MAP_H
#define MYSTL_STL_BTREE_MAP_H

#include "stl_pair.h"
#include <functional>
namespace MySTL
{



template <class Key, class Tp, class Compare, class Alloc, int TargetNodeSize=256>
class btree_map
{
public:
    typedef Key key_type;
    typedef Tp data_type;
    typedef Tp mapped_type;
    typedef pair<const Key, Tp> value_type;
    typedef Compare key_compare;


private:
    typedef btree<key_type, value_type,
    std::_Select1st<value_type>, key_compare, Alloc, TargetNodeSize> Rep_type;

    Rep_type M_t;

public:


};
























}













#endif //MYSTL_STL_BTREE_MAP_H
