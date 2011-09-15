#ifndef CREEK_KDTREE_H
#define CREEK_KDTREE_H

#include <iterator>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <assert.h>

#include "fwd.h"
#include "iterator.h"
#include "kdtree_traits.h"

#include <iostream>

namespace creek{
    template<
        typename T,
        std::size_t D,
        typename Access = kdtree_default,
        typename Compare = kdtree_default,
        typename C = typename kdtree_traits<T>::component_type,
        typename Allocator = std::allocator<T>>
    class kdtree
    {
    private:
        typedef kdtree self_type;
        struct node
        {
            T value;
            node* parent;
            node* left;
            node* right;
        };
        
        template<typename Tp>
        struct is_specialized
        {
            static bool const value
                = !std::is_base_of<detail::kdtree_traits_dummy_base, kdtree_traits<Tp>>::value;
        };
        friend class binary_tree_traits<self_type>;
        
    public:
        typedef T value_type;
        typedef C component_type;
        typedef std::size_t size_type;
        typedef Allocator allocator_type;
        static size_type const dimension = D;
        
    private:
        typedef node* node_pointer;
        typedef typename allocator_type::template rebind<node>::other
            node_allocator_type;
        size_type m_size;
        node* m_root;
        node m_dummy_obj;
        node m_foot_obj;
        node* const m_dummy;
        node* const m_foot;
        node_allocator_type m_allocator;
        Access m_access;
        Compare m_compare;
        
    public:
        explicit kdtree(Access _access = Access(), Compare _comp = Compare())
            : m_size(),
              m_root(),
              m_dummy_obj(node{value_type(), 0, 0, &m_foot_obj}),
              m_foot_obj(node{value_type(), &m_dummy_obj, 0, 0}),
              m_dummy(&m_dummy_obj),
              m_foot(&m_foot_obj),
              m_allocator(),
              m_access(_access),
              m_compare(_comp)
        {
        }
        
        template<typename RAIterator>
        kdtree(RAIterator _first, RAIterator _last, Access _access = Access(), Compare _comp = Compare())
            : m_size(),
              m_root(),
              m_dummy_obj(node{value_type(), 0, 0, &m_foot_obj}),
              m_foot_obj(node{value_type(), &m_dummy_obj, 0, 0}),
              m_dummy(&m_dummy_obj),
              m_foot(&m_foot_obj),
              m_allocator(),
              m_access(_access),
              m_compare(_comp)
        {
            m_dummy->right = m_foot;
            m_foot->parent = m_dummy;
            try{
                auto const& pred = compare(size_type(0));
                std::sort(_first, _last, std::cref(pred));
                size_type mid = (_last - _first) / 2;
                m_root = create_node(*(_first + mid));
                m_dummy->left = m_root;
                m_root->parent = m_dummy;
                m_root->left = construct(_first, _first + mid, 1, m_root);
                m_root->right = construct(_first + mid + 1, _last, 1, m_root);
            }
            catch(...){
                clear();
                throw;
            }
        }
        
        kdtree(self_type const& _other)
            : m_size(),
              m_root(),
              m_dummy_obj(node{value_type(), 0, 0, &m_foot_obj}),
              m_foot_obj(node{value_type(), &m_dummy_obj, 0, 0}),
              m_dummy(&m_dummy_obj),
              m_foot(&m_foot_obj),
              m_allocator(),
              m_access(),
              m_compare()
        {
            m_dummy->right = m_foot;
            m_foot->parent = m_dummy;
            *this = _other;
        }
        
        ~kdtree()
        {
            clear();
        }
        
        self_type& operator=(self_type const& _other)
        {
            if(&_other == this) return *this;
            clear();
            if(_other.empty()) return *this;
            auto it = _other.pre_order_begin();
            auto end = _other.pre_order_end();
            
            m_root = create_node(*it, m_dummy, 0, 0);
            m_dummy->left = m_root;
            auto jt = pre_order_iterator(m_root);
            while(it != end){
                auto n = it.base();
                auto cur = jt.base();
                if(n->left != 0)
                    cur->left = create_node(n->left->value, cur, 0, 0);
                if(n->right != 0)
                    cur->right = create_node(n->right->value, cur, 0, 0);
                ++it;
                ++jt;
            }
            return *this;
        }
        
        bool operator==(self_type const& _other) const
        {
            if(this == &_other) return true;
            if(size() != _other.size()) return false;
            if(size() == 0) return true;
            auto it = pre_order_begin();
            auto end = pre_order_end();
            auto jt = _other.pre_order_begin();
            auto const empty = const_pre_order_iterator();
            if(*it != *jt)
            while(it != end){
                auto lefta = left_child(it);
                auto righta = right_child(it);
                auto leftb = left_child(jt);
                auto rightb = right_child(jt);
                if(lefta != empty){
                    if(leftb == empty) return false;
                    else if(*lefta != *leftb) return false;
                }else{
                    if(leftb != empty) return false;
                }
                if(righta != empty){
                    if(rightb == empty) return false;
                    else if(*righta != *rightb) return false;
                }else{
                    if(rightb != empty) return false;
                }
                ++it;
                ++jt;
            }
            return true;
        }
        
        bool operator!=(self_type const& _other) const
        {
            return !(*this == _other);
        }
        
        void swap(self_type& _other)
        {
            std::swap(m_size, _other.m_size);
            std::swap(m_root, _other.m_root);
            std::swap(m_allocator, _other.m_allocator);
            std::swap(m_access, _other.m_access);
            std::swap(m_compare, _other.m_compare);
            m_root->parent = m_dummy;
            m_dummy->left = m_root;
            _other.m_root->parent = _other.m_dummy;
            _other.m_dummy->left = _other.m_root;
        }
        
        void testout()
        {
            std::cout << "root " << m_root << std::endl;
            if(!m_root) return;
            std::cout << "root:" << std::endl;
            testout(m_root);
        }
        
        void testout(node_pointer p)
        {
            if(!p) return;
            std::cout << p << "value: " << p->value << " left: " << p->left << " right: " << p->right << std::endl;
            testout(p->left);
            testout(p->right);
        }
        
        void set_access(Access _a)
        {
            m_access = _a;
        }
        
        void set_compare(Compare _c)
        {
            m_compare = _c;
        }
        
        void insert(value_type const& _a)
        {
            node_pointer cur = m_root;
            if(m_root == 0){
                m_root = create_node(_a, m_dummy, 0, 0);
                m_dummy->left = m_root;
                return;
            }
            size_type lv = 0;
            while(1){
                auto const& pred = compare(lv);
                node_pointer& next =
                    (pred(_a, cur->value)) ?
                    cur->left :
                    cur->right;
                if(next == 0){
                    next = create_node(_a, cur, 0, 0);
                    break;
                }else{
                    cur = next;
                }
                lv = (lv + 1) % dimension;
            }
            return;
        }
        
        void optimise()
        {
            std::vector<value_type, allocator_type> v(in_order_begin(), in_order_end());
            self_type t(v.begin(), v.end());
            swap(t);
        }
        
        void clear()
        {
            if(m_root != 0) erase(m_root);
        }
        
        template<typename Tag>
        void erase(tree_iterator<true, Tag, self_type> it)
        {
            erase(it.base());
        }
        
        size_type size() const
        {
            return m_size;
        }
        
        size_type max_size() const
        {
            return m_allocator.max_size();
        }
        
        bool empty() const
        {
            return (m_size == 0);
        }
        
        template<typename F>
        void apply_within_range(value_type const& _bound1, value_type const& _bound2, F const& f)
        {
            apply_within_range(0, m_root, _bound1, _bound2, std::cref(f));
        }
        
    private:
        bool within(value_type const& _a, value_type const& _bound1, value_type const& _bound2)
        {
            bool within = true;
            for(size_type i = 0; ((i < dimension) && within); ++i){
                auto const& pred = compare(i);
                within &= !pred(_a, _bound1) && pred(_a, _bound2);
            }
            return within;
        }
        
        template<typename F>
        void apply_within_range(size_type _lv, node_pointer _a, value_type const& _bound1, value_type const& _bound2, F f)
        {
            if(_a == 0) return;
            auto const& pred = compare(_lv % dimension);
            if(pred(_a->value, _bound1)){
                apply_within_range(_lv + 1, _a->right, _bound1, _bound2, f);
            }else if(pred(_a->value, _bound2)){
                if(within(_a->value, _bound1, _bound2)) f(_a->value);
                apply_within_range(_lv + 1, _a->left, _bound1, _bound2, f);
                apply_within_range(_lv + 1, _a->right, _bound1, _bound2, f);
            }else{
                apply_within_range(_lv + 1, _a->left, _bound1, _bound2, f);
            }
        }
    
    public:
        // pre order
        typedef tree_iterator<false, pre_order_tag, self_type> pre_order_iterator;
        typedef tree_iterator<true,  pre_order_tag, self_type> const_pre_order_iterator;
        typedef tree_iterator<false, reverse_tag<pre_order_tag>, self_type> reverse_pre_order_iterator;
        typedef tree_iterator<true,  reverse_tag<pre_order_tag>, self_type> const_reverse_pre_order_iterator;
        // in order
        typedef tree_iterator<false, in_order_tag, self_type> in_order_iterator;
        typedef tree_iterator<true,  in_order_tag, self_type> const_in_order_iterator;
        typedef tree_iterator<false, reverse_tag<in_order_tag>, self_type> reverse_in_order_iterator;
        typedef tree_iterator<true,  reverse_tag<in_order_tag>, self_type> const_reverse_in_order_iterator;
        // post order
        typedef tree_iterator<false, post_order_tag, self_type> post_order_iterator;
        typedef tree_iterator<true,  post_order_tag, self_type> const_post_order_iterator;
        typedef tree_iterator<false, reverse_tag<post_order_tag>, self_type> reverse_post_order_iterator;
        typedef tree_iterator<true,  reverse_tag<post_order_tag>, self_type> const_reverse_post_order_iterator;
        
        // pre order
        const_pre_order_iterator pre_order_begin() const
        {
            return const_pre_order_iterator(m_root);
        }
        
        const_pre_order_iterator pre_order_end() const
        {
            if(m_root == 0) return const_pre_order_iterator(0);
            return const_pre_order_iterator(m_foot);
        }
        
        pre_order_iterator pre_order_begin()
        {
            auto it = ((self_type const&)*this).pre_order_begin();
            return pre_order_iterator(it.base());
        }
        
        pre_order_iterator pre_order_end()
        {
            auto it = ((self_type const&)*this).pre_order_end();
            return pre_order_iterator(it.base());
        }
        
        const_reverse_pre_order_iterator pre_order_rbegin() const
        {
            return const_reverse_pre_order_iterator(pre_order_end().base());
        }
        
        const_reverse_pre_order_iterator pre_order_rend() const
        {
            return const_reverse_pre_order_iterator(pre_order_begin().base());
        }
        
        reverse_pre_order_iterator pre_order_rbegin()
        {
            return reverse_pre_order_iterator(pre_order_end().base());
        }
        
        reverse_pre_order_iterator pre_order_rend()
        {
            return reverse_pre_order_iterator(pre_order_begin().base());
        }
        
        // in order
        const_in_order_iterator in_order_begin() const
        {
            auto b = m_root;
            while(b != 0){
                auto c = b->left;
                if(c == 0) return const_in_order_iterator(b);
                else b = c;
            }
            return const_in_order_iterator(0);
        }
        
        const_in_order_iterator in_order_end() const
        {
            if(m_root == 0) return const_in_order_iterator(0);
            return const_in_order_iterator(m_dummy);
        }
        
        in_order_iterator in_order_begin()
        {
            auto it = ((self_type const&)*this).in_order_begin();
            return in_order_iterator(it.base());
        }
        
        in_order_iterator in_order_end()
        {
            auto it = ((self_type const&)*this).in_order_end();
            return in_order_iterator(it.base());
        }
        
        const_reverse_in_order_iterator in_order_rbegin() const
        {
            return const_reverse_in_order_iterator(in_order_end().base());
        }
        
        const_reverse_in_order_iterator in_order_rend() const
        {
            return const_reverse_in_order_iterator(in_order_begin().base());
        }
        
        reverse_in_order_iterator in_order_rbegin()
        {
            return reverse_in_order_iterator(in_order_end().base());
        }
        
        reverse_in_order_iterator in_order_rend()
        {
            return reverse_in_order_iterator(in_order_begin().base());
        }
        
        // post order
        const_post_order_iterator post_order_begin() const
        {
            auto b = m_root;
            while(b != 0){
                auto c = b->left;
                c = (c != 0) ? c : b->right;
                if(c == 0) return const_post_order_iterator(b);
                else b = c;
            }
            return const_post_order_iterator(0);
        }
        
        const_post_order_iterator post_order_end() const
        {
            if(m_root == 0) return const_post_order_iterator(0);
            return const_post_order_iterator(m_foot);
        }
        
        post_order_iterator post_order_begin()
        {
            auto it = ((self_type const&)*this).post_order_begin();
            return post_order_iterator(it.base());
        }
        
        post_order_iterator post_order_end()
        {
            auto it = ((self_type const&)*this).post_order_end();
            return post_order_iterator(it.base());
        }
        
        const_reverse_post_order_iterator post_order_rbegin() const
        {
            return const_reverse_post_order_iterator(post_order_end().base());
        }
        
        const_reverse_post_order_iterator post_order_rend() const
        {
            return const_reverse_post_order_iterator(post_order_begin().base());
        }
        
        reverse_post_order_iterator post_order_rbegin()
        {
            return reverse_post_order_iterator(post_order_end().base());
        }
        
        reverse_post_order_iterator post_order_rend()
        {
            return reverse_post_order_iterator(post_order_begin().base());
        }
        
    private:
        template<typename RAIterator>
        node_pointer
        construct(RAIterator _first, RAIterator _last, size_type _level, node_pointer _parent)
        {
            if(_first == _last) return 0;
            auto const& pred = compare(_level % dimension);
            if(dimension != 1) std::sort(_first, _last, std::cref(pred));
            size_type mid = (_last - _first) / 2;
            node_pointer tmp = create_node(*(_first + mid));
            tmp->left = construct(_first, _first + mid, _level + 1, tmp);
            tmp->right = construct(_first + mid + 1, _last, _level + 1, tmp);
            tmp->parent = _parent;
            return tmp;
        }
        
        template<typename Tp>
        inline static typename std::enable_if<
            std::is_same<Tp, Tp>::value &&
            std::is_same<Access, kdtree_default>::value, component_type>::type
        access(Tp const& _v, size_type _i)
        {
            return kdtree_traits<value_type>::access(_v, _i);
        }
        
        template<typename Tp>
        inline typename std::enable_if<
            std::is_same<Tp, Tp>::value &&
            !std::is_same<Access, kdtree_default>::value, component_type>::type
        access(Tp const& _v, size_type _i) const
        {
            return m_access(_v, _i);
        }
        
        struct compare_impl1;
        struct compare_impl2;
        struct compare_impl3;
        struct compare_impl4;
        
        template<typename Tp>
        inline static typename std::enable_if<
            std::is_same<Access, kdtree_default>::value &&
            std::is_same<Compare, kdtree_default>::value &&
            std::is_same<Tp, size_type>::value,
            compare_impl1>::type
        compare(Tp _i)
        {
            return compare_impl1(_i);
        }
        
        template<typename Tp>
        inline typename std::enable_if<
            !std::is_same<Access, kdtree_default>::value &&
            std::is_same<Compare, kdtree_default>::value &&
            std::is_same<Tp, size_type>::value,
            compare_impl2>::type
        compare(Tp _i) const
        {
            return compare_impl2(m_access, _i);
        }
        
        template<typename Tp>
        inline typename std::enable_if<
            std::is_same<Access, kdtree_default>::value &&
            !std::is_same<Compare, kdtree_default>::value &&
            std::is_same<Tp, size_type>::value,
            compare_impl3>::type
        compare(Tp _i) const
        {
            return compare_impl3(m_compare, _i);
        }
        
        template<typename Tp>
        inline typename std::enable_if<
            !std::is_same<Access, kdtree_default>::value &&
            !std::is_same<Compare, kdtree_default>::value &&
            std::is_same<Tp, size_type>::value,
            compare_impl4>::type
        compare(Tp _i) const
        {
            return compare_impl4(m_access, m_compare, _i);
        }
        
        struct compare_impl1
        {
            typedef bool result_type;
            compare_impl1(size_type _i) : m_axis(_i) {}
            bool operator()(value_type const& _a, value_type const& _b) const
            { return (access(_a, m_axis) < access(_b, m_axis)); }
            size_type const m_axis;
        };
        
        struct compare_impl2
        {
            typedef bool result_type;
            compare_impl2(Access const& _acc, size_type _i) : m_acc(_acc), m_axis(_i) {}
            bool operator()(value_type const& _a, value_type const& _b) const
            { return (m_acc(_a, m_axis) < m_acc(_b, m_axis)); }
            Access const& m_acc;
            size_type const m_axis;
        };
        
        struct compare_impl3
        {
            typedef bool result_type;
            compare_impl3(Compare _comp, size_type _i) : m_comp(_comp), m_axis(_i) {}
            bool operator()(value_type const& _a, value_type const& _b) const
            { return m_comp(access(_a, m_axis), access(_b, m_axis)); }
            Compare const& m_comp;
            size_type const m_axis;
        };
        
        struct compare_impl4
        {
            typedef bool result_type;
            compare_impl4(Access const& _acc, Compare const& _comp, size_type _i)
                : m_acc(_acc), m_comp(_comp), m_axis(_i) {}
            bool operator()(value_type const& _a, value_type const& _b) const
            { return m_comp(m_acc(_a, m_axis), m_acc(_b, m_axis)); }
            Access const& m_acc;
            Compare const& m_comp;
            size_type const m_axis;
        };
        
        node_pointer create_node(value_type const& _v)
        {
            node_pointer tmp = m_allocator.allocate(1);
            try{
                m_allocator.construct(tmp, node{_v, 0, 0, 0});
            }
            catch(...){
                m_allocator.deallocate(tmp, 1);
                throw;
            }
            ++m_size;
            return tmp;
        }
        
        node_pointer create_node(
            value_type const& _v,
            node_pointer _parent, node_pointer _left, node_pointer _right)
        {
            node_pointer tmp = m_allocator.allocate(1);
            try{
                m_allocator.construct(tmp, node{_v, _parent, _left, _right});
            }
            catch(...){
                m_allocator.deallocate(tmp, 1);
                throw;
            }
            ++m_size;
            return tmp;
        }
        
        void destroy_and_deallocate(node_pointer _node)
        {
            m_allocator.destroy(_node);
            m_allocator.deallocate(_node, 1);
            --m_size;
        }
        
        void erase(node_pointer _a)
        {
            assert(_a != 0 && _a != m_dummy && _a != m_foot);
            node_pointer b = _a;
            while(1){
                node_pointer c = (b->left != 0) ? (b->left) : (b->right);
                if(c != 0) b = c;
                else break;
            }
            
            auto it = post_order_iterator(b);
            auto end = post_order_iterator(_a);
            while(it != end){
                auto tmp = it;
                ++it;
                destroy_and_deallocate(tmp.base());
            }
            
            node_pointer p = _a->parent;
            if(p != 0){
                if(p->left == _a) p->left = 0;
                else if(p->right == _a) p->right = 0;
            }
            if(_a == m_root) m_root = 0;
            destroy_and_deallocate(_a);
        }
    };
    
    template<typename T, std::size_t D, typename C, typename A, typename Cm>
    struct binary_tree_traits<kdtree<T, D, C, A, Cm>>
    {
        typedef typename kdtree<T, D, C, A, Cm>::node_pointer sub_iterator;
        typedef typename kdtree<T, D, C, A, Cm>::value_type value_type;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static value_type& dereference(sub_iterator _a)
        {
            return _a->value;
        }
        
        static sub_iterator parent(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return _a->parent;
        }
        
        static sub_iterator left_child(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return _a->left;
        }
        
        static sub_iterator right_child(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return _a->right;
        }
    };
}
#endif
