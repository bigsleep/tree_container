#ifndef CREEK_TREE_ITERATOR_TRAITS_H
#define CREEK_TREE_ITERATOR_TRAITS_H

#include <iterator>
#include <type_traits>
#include <cstddef>

#include <iostream>

#include "fwd.h"

namespace creek{
    // ---- pre order, binary
    template<bool IsConst, typename Tree>
    struct tree_iterator_traits<
        tree_iterator<IsConst, pre_order_tag, Tree>,
        typename std::enable_if<is_binary_tree<Tree>::value, void>::type>
    {
        typedef std::ptrdiff_t difference_type;
        typedef typename Tree::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Tree tree_type;
        typedef typename binary_tree_traits<Tree>::sub_iterator sub_iterator;
        typedef binary_tree_traits<Tree> traits_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static reference dereference(sub_iterator _a)
        {
            return traits_type::dereference(_a);
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            auto left = traits_type::left_child(_a);
            if(left != sub_iterator()){
                _a = left;
                return _a;
            }
            
            auto right = traits_type::right_child(_a);
            if(right != sub_iterator()){
                _a = right;
                return _a;
            }
            
            auto c = _a;
            while(c != sub_iterator()){
                auto d = get_right_sibling(c);
                if(d != sub_iterator() && d != c){
                    _a = d;
                    return _a;
                }
                c = traits_type::parent(c);
            }
            
            return _a;
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            auto left = get_left_sibling(_a);
            if(left != sub_iterator() && left != _a){
                auto b = left;
                while(1){
                    auto c = traits_type::right_child(b);
                    c = (c != sub_iterator()) ? c : traits_type::left_child(b);
                    if(c == sub_iterator()){
                        _a = b;
                        return _a;
                    }else{
                        b = c;
                    }
                }
            }
            
            auto parent = traits_type::parent(_a);
            if(parent != sub_iterator()){
                _a = parent;
                return _a;
            }
            
            return _a;
        }
    
    private:
        static sub_iterator get_left_sibling(sub_iterator _a)
        {
            auto p = traits_type::parent(_a);
            if(p == sub_iterator()) return sub_iterator();
            else return traits_type::left_child(p);
        }
        
        static sub_iterator get_right_sibling(sub_iterator _a)
        {
            auto p = traits_type::parent(_a);
            if(p == sub_iterator()) return sub_iterator();
            else return traits_type::right_child(p);
        }
    };
    
    // ---- pre order, multiway
    template<bool IsConst, typename Tree>
    struct tree_iterator_traits<
        tree_iterator<IsConst, pre_order_tag, Tree>,
        typename std::enable_if<is_multiway_tree<Tree>::value, void>::type>
    {
        typedef std::ptrdiff_t difference_type;
        typedef typename Tree::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Tree tree_type;
        typedef typename multiway_tree_traits<Tree>::sub_iterator sub_iterator;
        typedef multiway_tree_traits<Tree> traits_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static reference dereference(sub_iterator _a)
        {
            return traits_type::dereference(_a);
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            auto b = traits_type::first_child(_a);
            if(b != sub_iterator()){
                _a = b;
                return _a;
            }
            
            auto c = _a;
            while(c != sub_iterator()){
                auto d = traits_type::younger_sibling(c);
                if(d != sub_iterator()){
                    _a = d;
                    return _a;
                }
                c = traits_type::parent(c);
            }
            
            return _a;
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            auto left = traits_type::older_sibling(_a);
            if(left != sub_iterator()){
                auto b = left;
                while(1){
                    auto c = traits_type::last_child(b);
                    if(c == sub_iterator()){
                        _a = b;
                        return _a;
                    }else{
                        b = c;
                    }
                }
            }
            
            auto parent = traits_type::parent(_a);
            if(parent != sub_iterator()){
                _a = parent;
                return _a;
            }
            
            return _a;
        }
    };
    
    // ---- in order
    template<bool IsConst, typename Tree>
    struct tree_iterator_traits<
        tree_iterator<IsConst, in_order_tag, Tree>,
        typename std::enable_if<is_binary_tree<Tree>::value, void>::type>
    {
        typedef std::ptrdiff_t difference_type;
        typedef typename Tree::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Tree tree_type;
        typedef typename binary_tree_traits<Tree>::sub_iterator sub_iterator;
        typedef binary_tree_traits<Tree> traits_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static reference dereference(sub_iterator _a)
        {
            return traits_type::dereference(_a);
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            auto right = traits_type::right_child(_a);
            if(right != sub_iterator()){
                auto b = right;
                while(1){
                    auto c = traits_type::left_child(b);
                    if(c != sub_iterator()){
                        b = c;
                    }else{
                        _a = b;
                        return _a;
                    }
                }
            }
            
            auto b = _a;
            while(b != sub_iterator()){
                auto c = traits_type::parent(b);
                if(traits_type::left_child(c) == b){
                    _a = c;
                    return _a;
                }else{
                    b = c;
                }
            }
            
            return _a;
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            auto left = traits_type::left_child(_a);
            if(left != sub_iterator()){
                auto b = left;
                while(1){
                    auto c = traits_type::right_child(b);
                    if(c != sub_iterator()){
                        b = c;
                    }else{
                        _a = b;
                        return _a;
                    }
                }
            }
            auto b = _a;
            while(b != sub_iterator()){
                auto c = traits_type::parent(b);
                if(traits_type::right_child(c) == b){
                    _a = c;
                    return _a;
                }else{
                    b = c;
                }
            }
            
            return _a;
        }
    
    private:
        static sub_iterator get_left_sibling(sub_iterator _a)
        {
            auto p = traits_type::parent(_a);
            if(p == sub_iterator()) return sub_iterator();
            else return traits_type::left_child(p);
        }
        
        static sub_iterator get_right_sibling(sub_iterator _a)
        {
            auto p = traits_type::parent(_a);
            if(p == sub_iterator()) return sub_iterator();
            else return traits_type::right_child(p);
        }
    };
    
    // ---- post order, binary
    template<bool IsConst, typename Tree>
    struct tree_iterator_traits<
        tree_iterator<IsConst, post_order_tag, Tree>,
        typename std::enable_if<is_binary_tree<Tree>::value, void>::type>
    {
        typedef std::ptrdiff_t difference_type;
        typedef typename Tree::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Tree tree_type;
        typedef typename binary_tree_traits<Tree>::sub_iterator sub_iterator;
        typedef binary_tree_traits<Tree> traits_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static reference dereference(sub_iterator _a)
        {
            return traits_type::dereference(_a);
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            auto right = get_right_sibling(_a);
            if(right != sub_iterator() && right != _a){
                auto b = right;
                while(1){
                    auto c = traits_type::left_child(b);
                    c = (c != sub_iterator()) ? c : traits_type::right_child(b);
                    if(c != sub_iterator()){
                        b = c;
                    }else{
                        _a = b;
                        return _a;
                    }
                }
            }
            
            auto parent = traits_type::parent(_a);
            if(parent != sub_iterator()){
                _a = parent;
                return _a;
            }
            
            return _a;
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            auto right = traits_type::right_child(_a);
            if(right != sub_iterator()){
                _a = right;
                return _a;
            }
            
            auto left = traits_type::left_child(_a);
            if(left != sub_iterator()){
                _a = left;
                return _a;
            }
            
            auto b = _a;
            while(b != sub_iterator()){
                auto c = get_left_sibling(b);
                if(c != sub_iterator() && c != b){
                    _a = c;
                    return _a;
                }else{
                    b = traits_type::parent(b);
                }
            }
            
            return _a;
        }
    
    private:
        static sub_iterator get_left_sibling(sub_iterator _a)
        {
            auto p = traits_type::parent(_a);
            if(p == sub_iterator()) return sub_iterator();
            else return traits_type::left_child(p);
        }
        
        static sub_iterator get_right_sibling(sub_iterator _a)
        {
            auto p = traits_type::parent(_a);
            if(p == sub_iterator()) return sub_iterator();
            else return traits_type::right_child(p);
        }
    };
    
    // ---- post order, multiway
    template<bool IsConst, typename Tree>
    struct tree_iterator_traits<
        tree_iterator<IsConst, post_order_tag, Tree>,
        typename std::enable_if<is_multiway_tree<Tree>::value, void>::type>
    {
        typedef std::ptrdiff_t difference_type;
        typedef typename Tree::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Tree tree_type;
        typedef typename multiway_tree_traits<Tree>::sub_iterator sub_iterator;
        typedef multiway_tree_traits<Tree> traits_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static reference dereference(sub_iterator _a)
        {
            return traits_type::dereference(_a);
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            auto right = traits_type::younger_sibling(_a);
            if(right != sub_iterator()){
                auto b = right;
                while(1){
                    auto c = traits_type::first_child(b);
                    if(c != sub_iterator()){
                        b = c;
                    }else{
                        _a = b;
                        return _a;
                    }
                }
            }
            
            auto parent = traits_type::parent(_a);
            if(parent != sub_iterator()){
                _a = parent;
                return _a;
            }
            
            return _a;
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            auto last = traits_type::last_child(_a);
            if(last != sub_iterator()){
                _a = last;
                return _a;
            }
            
            auto b = _a;
            while(b != sub_iterator()){
                auto c = traits_type::older_sibling(b);
                if(c != sub_iterator()){
                    _a = c;
                    return _a;
                }else{
                    b = traits_type::parent(b);
                }
            }
            
            return _a;
        }
    };
    
    // ---- reverse
    template<bool IsConst, typename Traversal, typename Tree, typename Void>
    struct tree_iterator_traits<
        tree_iterator<IsConst, reverse_tag<Traversal>, Tree>, Void>
    {
        typedef std::ptrdiff_t difference_type;
        typedef typename Tree::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Tree tree_type;
        typedef typename tree_iterator<IsConst, Traversal, Tree>::sub_iterator sub_iterator;
        typedef tree_iterator_traits<tree_iterator<IsConst, Traversal, Tree>> traits_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            auto b = tree_iterator<IsConst, Traversal, Tree>(_a);
            return (--b).base();
        }
        
        static reference dereference(sub_iterator _a)
        {
            auto b = tree_iterator<IsConst, Traversal, Tree>(_a);
            return *(--b);
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            auto b = tree_iterator<IsConst, Traversal, Tree>(_a);
            _a = (--b).base();
            return _a;
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            auto b = tree_iterator<IsConst, Traversal, Tree>(_a);
            _a = (++b).base();
            return _a;
        }
    };
    
    // ---- left child
    template<bool IsConst, typename Tag, typename Tree>
    typename std::enable_if<
        is_binary_tree<Tree>::value, 
        tree_iterator<IsConst, Tag, Tree>>::type
    left_child(tree_iterator<IsConst, Tag, Tree> _i)
    {
        typedef binary_tree_traits<Tree> traits_type;
        return tree_iterator<IsConst, Tag, Tree>(traits_type::left_child(_i.base()));
    }
    
    // ---- right child
    template<bool IsConst, typename Tag, typename Tree>
    typename std::enable_if<
        is_binary_tree<Tree>::value, 
        tree_iterator<IsConst, Tag, Tree>>::type
    right_child(tree_iterator<IsConst, Tag, Tree> _i)
    {
        typedef binary_tree_traits<Tree> traits_type;
        return tree_iterator<IsConst, Tag, Tree>(traits_type::right_child(_i.base()));
    }
    
    // ---- parent
    template<bool IsConst, typename Tag, typename Tree>
    typename std::enable_if<
        is_binary_tree<Tree>::value, 
        tree_iterator<IsConst, Tag, Tree>>::type
    parent(tree_iterator<IsConst, Tag, Tree> _i)
    {
        typedef binary_tree_traits<Tree> traits_type;
        return tree_iterator<IsConst, Tag, Tree>(traits_type::parent(_i.base()));
    }
    
    // ---- first child
    template<bool IsConst, typename Tag, typename Tree>
    typename std::enable_if<
        is_multiway_tree<Tree>::value,
        tree_iterator<IsConst, Tag, Tree>>::type
    first_child(tree_iterator<IsConst, Tag, Tree> _i)
    {
        typedef multiway_tree_traits<Tree> traits_type;
        return tree_iterator<IsConst, Tag, Tree>(traits_type::first_child(_i.base()));
    }
    
    // ---- last child
    template<bool IsConst, typename Tag, typename Tree>
    typename std::enable_if<
        is_multiway_tree<Tree>::value,
        tree_iterator<IsConst, Tag, Tree>>::type
    last_child(tree_iterator<IsConst, Tag, Tree> _i)
    {
        typedef multiway_tree_traits<Tree> traits_type;
        return tree_iterator<IsConst, Tag, Tree>(traits_type::last_child(_i.base()));
    }
    
    // ---- parent
    template<bool IsConst, typename Tag, typename Tree>
    typename std::enable_if<
        is_multiway_tree<Tree>::value, 
        tree_iterator<IsConst, Tag, Tree>>::type
    parent(tree_iterator<IsConst, Tag, Tree> _i)
    {
        typedef multiway_tree_traits<Tree> traits_type;
        return tree_iterator<IsConst, Tag, Tree>(traits_type::parent(_i.base()));
    }
}
#endif
