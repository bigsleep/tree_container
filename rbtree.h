#ifndef CREEK_RBTREE_H
#define CREEK_RBTREE_H

#include <iterator>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <assert.h>

#include "fwd.h"
#include "iterator.h"

namespace creek{
    struct rbtree_default {};
    
    template<
        typename T,
        typename Compare = rbtree_default,
        typename Allocator = std::allocator<T>>
    class rbtree
    {
        typedef rbtree self_type;
        enum class color_t
        {
            red, black
        };
        
        enum class branch_t
        {
            left, right
        };
        
        struct node
        {
            T value;
            color_t color;
            node* parent;
            node* left;
            node* right;
        };
           
        friend class binary_tree_traits<self_type>;
        
    public:
        typedef T value_type;
        typedef std::size_t size_type;
        typedef Allocator allocator_type;
        
    private:
        typedef node* node_pointer;
        typedef typename allocator_type::template rebind<node>::other
            node_allocator_type;
        size_type m_size;
        node* m_root;
        node m_dummy_obj;
        node m_foot_obj;
        node* m_dummy;
        node* m_foot;
        node_allocator_type m_allocator;
        Compare m_compare;
        
    public:
        explicit rbtree(Compare _cmp = Compare())
            : m_size(),
              m_root(),
              m_dummy_obj(node{value_type(), color_t::black, 0, 0, &m_foot_obj}),
              m_foot_obj(node{value_type(), color_t::black, &m_dummy_obj, 0, 0}),
              m_dummy(&m_dummy_obj),
              m_foot(&m_foot_obj),
              m_allocator(),
              m_compare(_cmp)
        {
        }
        
        rbtree(self_type const& _other)
            : m_size(),
              m_root(),
              m_dummy_obj(node{value_type(), color_t::black, 0, 0, &m_foot_obj}),
              m_foot_obj(node{value_type(), color_t::black, &m_dummy_obj, 0, 0}),
              m_dummy(&m_dummy_obj),
              m_foot(&m_foot_obj),
              m_allocator(),
              m_compare()
        {
            *this = _other;
        }
        
        ~rbtree()
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
            
            m_root = create_node(*it, color_t::black, m_dummy, 0, 0);
            m_dummy->left = m_root;
            auto jt = pre_order_iterator(m_root);
            while(it != end){
                auto n = it.base();
                auto cur = jt.base();
                if(n->left != 0)
                    cur->left = create_node(n->left->value, n->left->color, cur, 0, 0);
                if(n->right != 0)
                    cur->right = create_node(n->right->value, n->right->color, cur, 0, 0);
                ++it;
                ++jt;
            }
            m_compare = _other.m_compare;
            m_allocator = _other.m_allocator;
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
            while(it != end){
                if(*it != *jt) return false;
                if(it.base()->color != jt.base()->color)
                    return false; 
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
            std::swap(m_compare, _other.m_compare);
            m_root->parent = m_dummy;
            m_dummy->left = m_root;
            _other.m_root->parent = _other.m_dummy;
            _other.m_dummy->left = _other.m_root;
        }
        
        void insert(value_type const& _a)
        {
            if(empty()){
                m_root = create_node(_a, color_t::black, m_dummy, 0, 0);
                m_root->parent = m_dummy;
                m_dummy->left = m_root;
                return;
            }
            node_pointer cur = m_root;
            while(1){
                branch_t branch = (compare(_a, cur->value)) ? branch_t::left : branch_t::right;
                node_pointer& next = (branch == branch_t::left) ? cur->left : cur->right;
                if(next == 0){
                    next = create_node(_a, color_t::red, cur, 0, 0);
                    if(cur->color == color_t::black)
                        return;
                    else if(cur->parent->left == cur)
                        insert_left(cur, branch, _a);
                    else
                        insert_right(cur, branch, _a);
                    return;
                }else{
                    cur = next;
                }
            }
        }
        
        bool check_invariant() const
        {
            try{
                check(m_root);
            }
            catch(std::exception const& e){
                return false;
            }
            catch(...){
                return false;
            }
            return true;
        }
        
        void clear()
        {
            if(empty()) return;
            node_pointer b = m_root;
            while(1){
                node_pointer c = (b->left != 0) ? (b->left) : (b->right);
                if(c != 0) b = c;
                else break;
            }
            
            auto it = post_order_iterator(b);
            auto end = post_order_iterator(m_root);
            while(it != end){
                auto tmp = it;
                ++it;
                destroy_and_deallocate(tmp.base());
            }
            destroy_and_deallocate(m_root);
            m_root = 0;
            m_dummy->left = 0;
        }
        
        template<typename Iterator>
        void erase(Iterator _a)
        {
            node_pointer cur = _a.base();
            assert(cur != 0 && cur != m_dummy && cur != m_foot);
            if(cur->left == 0 && cur->right == 0){
                node_pointer parent = cur->parent;
                bool left = parent->left == cur;
                bool black = cur->color == color_t::black;
                if(cur->parent->left == cur) cur->parent->left = 0;
                else cur->parent->right = 0;
                if(cur == m_root) m_root = 0;
                destroy_and_deallocate(cur);
                if(black){
                    if(left) balance_left(parent);
                    else     balance_right(parent);
                }
            }else if(cur->left == 0 || cur->right == 0){
                node_pointer parent = cur->parent;
                node_pointer b = (cur->left != 0) ? cur->left : cur->right;
                bool left = parent->left == cur;
                bool black = cur->color == color_t::black;
                if(left) parent->left = b;
                else     parent->right = b;
                b->parent = parent;
                destroy_and_deallocate(cur);
                if(black){
                    if(left) balance_left(parent);
                    else     balance_right(parent);
                }
            }else{
                node_pointer next = cur->right;
                while(next->left != 0) next = next->left;
                std::swap(cur->value, next->value);
                node_pointer parent = next->parent;
                bool left = parent->left == next;
                bool black = next->color == color_t::black;
                if(parent->left == next) parent->left = next->right;
                else parent->right = next->right;
                if(next->right == 0){
                    destroy_and_deallocate(next);
                    if(black){
                        if(left) balance_left(parent);
                        else     balance_right(parent);
                    }
                }else{
                    node_pointer b = next->right;
                    b->parent = parent;
                    destroy_and_deallocate(next);
                    if(black){
                        if(left) balance_left(parent);
                        else     balance_right(parent);
                    }
                }
            }
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
        
    private:
        void rotate_right(node_pointer _a)
        {
            assert(_a != 0 && _a->left != 0);
            node_pointer left = _a->left;
            _a->left = left->right;
            if(_a->left != 0) _a->left->parent = _a;
            left->right = _a;
            left->parent = _a->parent;
            _a->parent = left;
            if(left->parent != 0){
                if(left->parent->left == _a) left->parent->left = left;
                else left->parent->right = left;
            }
            std::swap(_a->color, left->color);
            if(_a == m_root)
                m_root = left;
        }
        
        void rotate_left(node_pointer _a)
        {
            assert(_a != 0 && _a->right != 0);
            node_pointer right = _a->right;
            _a->right = right->left;
            if(_a->right != 0) _a->right->parent = _a;
            right->left = _a;
            right->parent = _a->parent;
            _a->parent = right;
            if(right->parent != 0){
                if(right->parent->left == _a) right->parent->left = right;
                else right->parent->right = right;
            }
            std::swap(_a->color, right->color);
            if(_a == m_root)
                m_root = right;
        }
        
        void change_color(node_pointer _a)
        {
            assert(_a != 0 && _a->left != 0 && _a->right != 0);
            _a->color = color_t::red;
            _a->left->color = color_t::black;
            _a->right->color = color_t::black;
        }
        
        void insert_left(node_pointer _parent, branch_t _b, value_type const& _a)
        {
            assert(_parent != 0);
            node_pointer grand_parent = _parent->parent;
            node_pointer uncle = grand_parent->right;
            if(uncle != 0 && uncle->color == color_t::red){
                change_color(grand_parent);
                if(grand_parent == m_root){
                    m_root->color = color_t::black;
                    return;
                }else{
                    node_pointer ggp = grand_parent->parent;
                    if(ggp->color == color_t::black)
                        return;
                    node_pointer gggp = ggp->parent;
                    branch_t branch = (ggp->left == grand_parent) ?
                        branch_t::left : branch_t::right;
                    if(gggp->left == ggp){
                        insert_left(ggp, branch, _a);
                    }else{
                        insert_right(ggp, branch, _a);
                    }
                }
            }else{
                if(_b == branch_t::left){
                    rotate_right(grand_parent);
                    return;
                }else{
                    rotate_left(_parent);
                    rotate_right(grand_parent);
                    return;
                }
            }
        }
        
        void insert_right(node_pointer _parent, branch_t _b, value_type const& _a)
        {
            assert(_parent != 0);
            node_pointer grand_parent = _parent->parent;
            node_pointer uncle = grand_parent->left;
            if(uncle != 0 && uncle->color == color_t::red){
                change_color(grand_parent);
                if(grand_parent == m_root){
                    m_root->color = color_t::black;
                    return;
                }else{
                    node_pointer ggp = grand_parent->parent;
                    if(ggp->color == color_t::black)
                        return;
                    node_pointer gggp = ggp->parent;
                    branch_t branch = (ggp->left == grand_parent) ?
                        branch_t::left : branch_t::right;
                    if(gggp->left == ggp){
                        insert_left(ggp, branch, _a);
                    }else{
                        insert_right(ggp, branch, _a);
                    }
                }
            }else{
                if(_b == branch_t::right){
                    rotate_left(grand_parent);
                    return;
                }else{
                    rotate_right(_parent);
                    rotate_left(grand_parent);
                    return;
                }
            }
        }
        
        void balance_left(node_pointer _a)
        {
            assert(_a != 0);
            if(_a->left == m_root){
                return;
            }else if(_a->right == 0){
                if(_a->parent->left == _a->parent)
                    balance_left(_a->parent);
                else
                    balance_right(_a->parent);
            }else{
                node_pointer parent = _a;
                node_pointer bro = parent->right;
                if(bro->color == color_t::red){
                    rotate_left(parent);
                    balance_left(parent);
                }else if(bro->right != 0 && bro->right->color == color_t::red){
                    rotate_left(parent);
                    bro->right->color = color_t::black;
                }else if(bro->left != 0 && bro->left->color == color_t::red){
                    rotate_right(bro);
                    rotate_left(parent);
                    bro->color = color_t::black;
                }else if(parent->color == color_t::red){
                    bro->color = color_t::red;
                    parent->color = color_t::black;
                }else{
                    bro->color = color_t::red;
                    if(parent->parent->left == parent) balance_left(parent->parent);
                    else balance_right(parent->parent);
                }
            }
        }
        
        void balance_right(node_pointer _a)
        {
            assert(_a != 0);
            if(_a->right == m_root){
                return;
            }else if(_a->left == 0){
                if(_a->parent->left == _a->parent)
                    balance_left(_a->parent);
                else
                    balance_right(_a->parent);
            }else{
                node_pointer parent = _a;
                node_pointer bro = parent->left;
                if(bro->color == color_t::red){
                    rotate_right(parent);
                    balance_right(parent);
                }else if(bro->left != 0 && bro->left->color == color_t::red){
                    rotate_right(parent);
                    bro->left->color = color_t::black;
                }else if(bro->right != 0 && bro->right->color == color_t::red){
                    rotate_left(bro);
                    rotate_right(parent);
                    bro->color = color_t::black;
                }else if(parent->color == color_t::red){
                    bro->color = color_t::red;
                    parent->color = color_t::black;
                }else{
                    bro->color = color_t::red;
                    if(parent->parent->left == parent) balance_left(parent->parent);
                    else balance_right(parent->parent);
                }
            }
        }
        
        static size_type check(node_pointer _a)
        {
            if(_a == 0) return 0;
            if(_a->color == color_t::red){
                if(_a->left != 0 && _a->left->color == color_t::red) throw std::runtime_error("red");
                if(_a->right != 0 && _a->right->color == color_t::red) throw std::runtime_error("red");
            }
            size_type left = check(_a->left);
            size_type right = check(_a->right);
            if(left != right) throw std::runtime_error("black");
            
            if(_a->color == color_t::black) return left + 1;
            else return left;
        }
        
        template<typename Tp>
        inline static typename std::enable_if<
            std::is_same<Tp, Tp>::value &&
            std::is_same<Compare, rbtree_default>::value, bool>::type
        compare(Tp const& _a, Tp const& _b)
        {
            return (_a < _b);
        }
        
        template<typename Tp>
        inline typename std::enable_if<
            std::is_same<Tp, Tp>::value &&
            !std::is_same<Compare, rbtree_default>::value, bool>::type
        compare(Tp const& _a, Tp const& _b) const
        {
            return m_compare(_a, _b);
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
        node_pointer create_node(value_type const& _v)
        {
            node_pointer tmp = m_allocator.allocate(1);
            try{
                m_allocator.construct(tmp, node{_v, color_t::black, 0, 0, 0});
            }
            catch(...){
                m_allocator.deallocate(tmp, 1);
                throw;
            }
            ++m_size;
            return tmp;
        }
        
        node_pointer create_node(
            value_type const& _v, color_t _color,
            node_pointer _parent, node_pointer _left, node_pointer _right)
        {
            node_pointer tmp = m_allocator.allocate(1);
            try{
                m_allocator.construct(tmp, node{_v, _color, _parent, _left, _right});
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
    
    public:
        void output_dot(std::ostream& out, std::string const& graph_name = std::string("G")) const
        {
            if(!out) return;
            auto it = pre_order_begin();
            auto end = pre_order_end();
            out << "digraph " << "\"" << graph_name << "\" {\n";
            while(it != end){
                out << "\t" << "N" << it.base() << "[ label=\"" << *it << "\", ";
                if(it.base()->color == color_t::black){
                    out << "style = filled, fillcolor = \"#cccccc\"];\n";
                }else{
                    out << "style = filled, fillcolor = \"#CC9999\"];\n";
                }
                auto left = left_child(it);
                auto right = right_child(it);
                if(left.base() != 0){
                    out << "\t" << "N" << it.base() << " -> " << "N" << left.base();
                    out << ";\n";
                }
                if(right.base() != 0){
                    out << "\t" << "N" << it.base() << " -> " << "N" << right.base();
                    out << ";\n";
                }
                ++it;
            }
            out << "}\n";
        }
    };
    
    template<typename T, typename C, typename A>
    struct binary_tree_traits<rbtree<T, C, A>>
    {
        typedef typename rbtree<T, C, A>::node_pointer sub_iterator;
        typedef typename rbtree<T, C, A>::value_type value_type;
        
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


