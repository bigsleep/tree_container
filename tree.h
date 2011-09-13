//-----------------------------------------------------------
//    tree.h
//-----------------------------------------------------------
#ifndef CREEK_TREE_H
#define CREEK_TREE_H

#include <iterator>
#include <list>
#include <algorithm>
#include <cstddef>
#include <assert.h>

#include "fwd.h"
#include "iterator.h"

namespace creek
{
    //-----------------------------------------------------------
    //    class tree
    //-----------------------------------------------------------
    template<typename Tp, typename Allocator = std::allocator<Tp> >
    class tree
    {
    public:
        typedef tree self_type;
        typedef Tp value_type;
        typedef Allocator allocator_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;
        typedef unsigned int size_type;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
    
    private:
        struct node
        {
            typedef typename Allocator::template rebind<node*>::other allocator_type;
            typedef std::list<node*> list_type;
            typedef typename list_type::iterator sub_iterator;
            Tp m_value;
            sub_iterator m_parent;
            sub_iterator m_older_sibling;
            sub_iterator m_younger_sibling;
            list_type m_children;
        };
        
        typedef node* node_pointer;
        typedef typename Allocator::template rebind<node>::other node_allocator;
        typedef typename node::list_type list_type;
        typedef typename node::sub_iterator sub_iterator;
        friend class multiway_tree_traits<tree<value_type, allocator_type>>;
        friend class tree_iterator_traits<tree_iterator<true, child_order_tag, self_type>>;
        friend class tree_iterator_traits<tree_iterator<false, child_order_tag, self_type>>;
        
        list_type m_root_list;
        sub_iterator m_root;
        sub_iterator m_foot;
        node m_foot_obj;
        size_type m_size;
        node_allocator m_alloc;
        
    public:
        tree()
            : m_root_list(), m_root(m_root_list.begin()),
            m_foot(m_root), m_foot_obj(), m_size(), m_alloc()
        {}
        
        tree(tree const& _other)
            : m_root_list(), m_root(m_root_list.begin()),
                m_foot(m_root), m_foot_obj(), m_size(), m_alloc()
        {
            (*this) = _other;
        }
        
        ~tree()
        { clear(); }
        
        //---- size
        size_type size() const
        { return m_size; }
        
        //---- empty
        bool empty() const
        { return (m_size == 0); }
        
        //---- insert
        template<typename Iterator>
        Iterator insert(Iterator _pos, const value_type& _val)
        {
            sub_iterator node1 = _pos.base();
            assert(node1 != sub_iterator());
            assert(!((node1 == m_foot) && m_root != m_foot));
            
            if(m_size == 0){
                node* root_tmp = create_node(_val);
                m_root_list.push_back(root_tmp);
                m_root_list.push_back(&m_foot_obj);
                m_root = m_root_list.begin();
                m_foot = (++(m_root_list.begin()));
                (*m_root)->m_younger_sibling = m_foot;
                m_foot_obj.m_older_sibling = m_root;
                return Iterator(m_root);
            }else{
                list_type&
                    list1 = (node1 == m_root)
                        ? m_root_list
                        : (*((*node1)->m_parent))->m_children;
                node_pointer tmp = create_node(
                    _val,
                    (*node1)->m_parent,
                    (*node1)->m_older_sibling,
                    (*node1)->m_younger_sibling);
                list1.insert(node1, tmp);
                sub_iterator it = node1;
                --it;
                tmp->m_children.splice(tmp->m_children.begin(), list1, node1);
                
                if((*it)->m_older_sibling != sub_iterator())
                    (*((*it)->m_older_sibling))->m_younger_sibling = it;
                if((*it)->m_younger_sibling != sub_iterator())
                    (*((*it)->m_younger_sibling))->m_older_sibling = it;
                
                tmp->m_children.front()->m_parent = it;
                tmp->m_children.front()->m_older_sibling = sub_iterator();
                tmp->m_children.front()->m_younger_sibling = sub_iterator();
                
                if(node1 == m_root) m_root = it;
                return Iterator(it);
            }
        }
        
        //---- append child
        template<typename Iterator>
        Iterator append_child(Iterator _pos, const value_type& _val)
        {
            sub_iterator node1 = _pos.base();
            assert(node1 != sub_iterator());
            assert(node1 != m_foot);
            
            node_pointer tmp = create_node(_val, node1);
            if(!((*node1)->m_children.empty())){
                sub_iterator tmp_back = (*node1)->m_children.end();
                --tmp_back;
                tmp->m_older_sibling = tmp_back;
                (*node1)->m_children.push_back(tmp);
                (*tmp_back)->m_younger_sibling = (--((*node1)->m_children.end()));
                
            }else{
                (*node1)->m_children.push_back(tmp);
            }
            
            return Iterator(--((*node1)->m_children.end()));
        }
        
        //---- append child tree
        template<typename Iterator>
        void append_child_tree(Iterator _pos, const self_type& _subtree)
        {
            sub_iterator node1 = _pos.base();
            assert(node1 != sub_iterator());
            assert(node1 != m_foot);
            
            const self_type& cp1 = (&_subtree != this) ? _subtree : self_type(_subtree);
            
            const_pre_order_iterator iother = cp1.pre_order_begin(), other_end = cp1.pre_order_end();
            pre_order_iterator iself(node1);
            iself = append_child(iself, *iother);
            while(iother != other_end){
                auto chi = child_order_begin(iother);
                auto chi_end = child_order_end(iother);
                while(chi != chi_end){
                    append_child(iself, *chi);
                    ++chi;
                }
                ++iother;
                ++iself;
            }
        }
        
        //---- insert child
        template<typename Iterator, typename Iterator2>
        Iterator insert_child(Iterator _parent, Iterator2 _child_pos, const value_type& _val)
        {
            sub_iterator parent = _parent.base();
            sub_iterator node1 = _child_pos.base();
            assert(parent != sub_iterator());
            assert(node1 != sub_iterator());
            assert(node1 != m_foot);
            assert(node1 != m_root);
            assert(_parent.base() == creek::parent(_child_pos).base());
            
            node_pointer tmp = create_node(
                _val,
                parent,
                (*node1)->m_older_sibling,
                node1);
            sub_iterator it = (*parent)->m_children.insert(node1, tmp);
            
            if((*node1)->m_older_sibling != sub_iterator())
                (*((*node1)->m_older_sibling))->m_younger_sibling = it;
            (*node1)->m_older_sibling = it;
            
            return Iterator(it);
        }
        
        //---- insert child
        template<typename Iterator, typename Iterator2>
        Iterator insert_child_tree(Iterator _parent, Iterator2 _child_pos, self_type const& _other)
        {
            sub_iterator parent = _parent.base();
            sub_iterator node1 = _child_pos.base();
            assert(parent != sub_iterator());
            assert(node1 != sub_iterator());
            assert(node1 != m_foot);
            assert(node1 != m_root);
            assert(_parent.base() == creek::parent(_child_pos).base());
            
            self_type const& cp1 = (&_other != this) ? _other : self_type(_other);
            const_pre_order_iterator iother = cp1.pre_order_begin();
            const_pre_order_iterator other_end = cp1.pre_order_end();
            
            pre_order_iterator iself(node1);
            iself = insert_child(_parent, iself, *iother);
            while(iother != other_end){
                auto chi = child_order_begin(iother);
                auto chi_end = child_order_end(iother);
                while(chi != chi_end){
                    append_child(iself, *chi);
                    ++chi;
                }
                ++iother;
                ++iself;
            }
            return Iterator(--node1);
        }
        
        template<typename Iterator>
        void erase(Iterator _pos)
        {
            if(empty()) return;
            static sub_iterator const empty = sub_iterator();
            sub_iterator node1 = _pos.base();
            assert(node1 != empty);
            assert(node1 != m_foot);
            
            sub_iterator b = node1;
            while(!(**b).m_children.empty()){
                b = (**b).m_children.begin();
            }
            
            post_order_iterator it(b);
            post_order_iterator end(node1);
            while(it != end){
                node_pointer c = *(it.base());
                destroy_and_deallocate(c);
                ++it;
            }
            
            sub_iterator parentnode = (*node1)->m_parent;
            if(parentnode != empty){
                if((*node1)->m_older_sibling != empty)
                    (*((*node1)->m_older_sibling))->m_younger_sibling = (*node1)->m_younger_sibling;
                if((*node1)->m_younger_sibling != empty)
                    (*((*node1)->m_younger_sibling))->m_older_sibling = (*node1)->m_older_sibling;
                (*parentnode)->m_children.erase(node1);
                destroy_and_deallocate(*node1);
            }else{
                assert(node1 == m_root);
                destroy_and_deallocate(*node1);
                m_root_list.clear();
                m_root = m_root_list.begin();
                m_foot = m_root_list.begin();
                m_foot_obj = node();
            }
        }
        
        //---- clear
        void clear()
        {
            if(m_root == m_foot)
                return;
            erase(pre_order_iterator(m_root));
        }
        
        //---- operator=
        self_type& operator=(self_type const& _other)
        {
            if(&_other == this) return *this;
            clear();
            if(_other.m_size == 0) return *this;
            else
            {
                const_pre_order_iterator iother = _other.pre_order_begin(), other_end = _other.pre_order_end();
                pre_order_iterator iself = this->insert(this->pre_order_begin(), *iother);
                while(iother != other_end){
                    auto chi = child_order_begin(iother);
                    auto chi_end = child_order_end(iother);
                    while(chi != chi_end){
                        append_child(iself, *chi);
                        ++chi;
                    }
                    ++iother;
                    ++iself;
                }
                return *this;
            }
        }
        
        void swap(self_type& _other)
        {
            m_root_list.swap(_other.m_root_list);
            std::swap(m_root, _other.m_root);
            std::swap(m_foot, _other.m_foot);
            std::swap(m_foot_obj, _other.m_foot_obj);
            std::swap(m_size, _other.m_size);
            std::swap(m_alloc, _other.m_alloc);
            (*m_foot) = &m_foot_obj;
            (*_other.m_foot) = &(_other.m_foot_obj);
        }
        
        //
        template<typename Iterator>
        self_type get_subtree(Iterator _pos) const
        {
            sub_iterator node1 = _pos.base();
            assert(node1 != sub_iterator());
            assert(node1 != m_foot);
            sub_iterator const empty = sub_iterator();
            
            self_type subcopy;
            if(m_size == 0) return subcopy;
            
            const_pre_order_iterator isub(node1), isub_end(empty);
            {
                sub_iterator cur_node(node1);
                while(cur_node != empty){
                    if((*cur_node)->m_younger_sibling != empty){
                        isub_end = const_pre_order_iterator((*cur_node)->m_younger_sibling);
                        break;
                    }
                    cur_node = (*cur_node)->m_parent;
                }
            }
            assert(isub_end.base() != empty);
            
            pre_order_iterator icopy = subcopy.pre_order_begin();
            icopy = subcopy.insert(icopy, *isub);
            
            while(isub != isub_end){
                auto chi = child_order_begin(isub);
                auto chi_end = child_order_end(isub);
                while(chi != chi_end){
                    subcopy.append_child(icopy, *chi);
                    ++chi;
                }
                ++isub;
                ++icopy;
            }
            return subcopy;
        }
        
    private:
        //---- create node
        node_pointer create_node(node const& _node)
        {
            node_pointer tmp = m_alloc.allocate(1);
            try{
                m_alloc.construct(tmp, _node);
            }
            catch(...){
                m_alloc.deallocate(tmp, 1);
                throw;
            }
            ++m_size;
            return tmp;
        }
        
        node_pointer create_node(
            value_type const& _val = value_type(),
            sub_iterator _parent = sub_iterator(),
            sub_iterator _older = sub_iterator(),
            sub_iterator _younger = sub_iterator())
        {
            node_pointer tmp = m_alloc.allocate(1);
            try{
                m_alloc.construct(tmp, node{_val, _parent, _older, _younger, list_type()});
            }
            catch(...){
                m_alloc.deallocate(tmp, 1);
                throw;
            }
            ++m_size;
            return tmp;
        }
        
        //---- destroy and deallocate
        void destroy_and_deallocate(node_pointer _node)
        {
            m_alloc.destroy(_node);
            m_alloc.deallocate(_node, 1);
            --m_size;
        }
        
    public:
        //---- iterators
        typedef tree_iterator<false, pre_order_tag,   self_type> pre_order_iterator;
        typedef tree_iterator<true,  pre_order_tag,   self_type> const_pre_order_iterator;
        typedef tree_iterator<false, post_order_tag,  self_type> post_order_iterator;
        typedef tree_iterator<true,  post_order_tag,  self_type> const_post_order_iterator;
        typedef tree_iterator<false, child_order_tag, self_type> child_order_iterator;
        typedef tree_iterator<true,  child_order_tag, self_type> const_child_order_iterator;
        
        typedef tree_iterator<false, reverse_tag<pre_order_tag>,   self_type> reverse_pre_order_iterator;
        typedef tree_iterator<true,  reverse_tag<pre_order_tag>,   self_type> const_reverse_pre_order_iterator;
        typedef tree_iterator<false, reverse_tag<post_order_tag>,  self_type> reverse_post_order_iterator;
        typedef tree_iterator<true,  reverse_tag<post_order_tag>,  self_type> const_reverse_post_order_iterator;
        typedef tree_iterator<false, reverse_tag<child_order_tag>, self_type> reverse_child_order_iterator;
        typedef tree_iterator<true,  reverse_tag<child_order_tag>, self_type> const_reverse_child_order_iterator;
        
        //---- pre order begin end
        pre_order_iterator pre_order_begin()
        { return pre_order_iterator(m_root); }
        
        pre_order_iterator pre_order_end()
        { return pre_order_iterator(m_foot); }
        
        const_pre_order_iterator pre_order_begin() const
        { return const_pre_order_iterator(m_root); }
        
        const_pre_order_iterator pre_order_end() const
        { return const_pre_order_iterator(m_foot); }
        
        //---- pos torder begin end
        post_order_iterator post_order_begin()
        {
            if(empty()) return post_order_iterator(m_root);
            sub_iterator tmp = m_root;
            while(!((*tmp)->m_children.empty()))
                tmp = (*tmp)->m_children.begin();
            return post_order_iterator(tmp);
        }
        
        post_order_iterator post_order_end()
        { return post_order_iterator(m_foot); }
        
        const_post_order_iterator post_order_begin() const
        {
            if(m_root == m_foot) return post_order_iterator(m_root);
            sub_iterator tmp = m_root;
            while(!((*tmp)->m_children.empty()))
                tmp = (*tmp)->m_children.begin();
            return const_post_order_iterator(tmp);
        }
        
        const_post_order_iterator post_order_end() const
        { return const_post_order_iterator(m_foot); }
        
        //---- pre order rbegin rend
        reverse_pre_order_iterator pre_order_rbegin()
        { return reverse_pre_order_iterator(pre_order_end().base()); }
        const_reverse_pre_order_iterator pre_order_rbegin() const
        { return const_reverse_pre_order_iterator(pre_order_end().base()); }
        reverse_pre_order_iterator pre_order_rend()
        { return reverse_pre_order_iterator(pre_order_begin().base()); }
        const_reverse_pre_order_iterator pre_order_rend() const
        { return const_reverse_pre_order_iterator(pre_order_begin().base()); }
        
        //---- post order rbegin rend
        reverse_post_order_iterator post_order_rbegin()
        { return reverse_post_order_iterator(post_order_end().base()); }
        const_reverse_post_order_iterator post_order_rbegin() const
        { return const_reverse_post_order_iterator(post_order_end().base()); }
        reverse_post_order_iterator post_order_rend()
        { return reverse_post_order_iterator(post_order_begin().base()); }
        const_reverse_post_order_iterator post_order_rend() const
        { return const_reverse_post_order_iterator(post_order_begin().base()); }
    };
    
    //-----------------------------------------------------------
    //    multiway tree traits
    //-----------------------------------------------------------
    template<typename T, typename A>
    struct multiway_tree_traits<tree<T, A>>
    {
        typedef typename tree<T, A>::sub_iterator sub_iterator;
        typedef typename tree<T, A>::value_type value_type;
        
        static value_type& dereference(sub_iterator _a)
        {
            return (**_a).m_value;
        }
        
        static sub_iterator parent(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return (**_a).m_parent;
        }
        
        static sub_iterator first_child(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return ((**_a).m_children.empty())
                ? sub_iterator()
                : (**_a).m_children.begin();
        }
        
        static sub_iterator last_child(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return ((**_a).m_children.empty())
                ? sub_iterator()
                : (--((**_a).m_children.end()));
        }
        
        static sub_iterator older_sibling(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return (**_a).m_older_sibling;
        }
        
        static sub_iterator younger_sibling(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return (**_a).m_younger_sibling;
        }
    };
    
    //-----------------------------------------------------------
    //    child order traits
    //-----------------------------------------------------------
    template<bool IsConst, typename T, typename A>
    struct tree_iterator_traits<
        tree_iterator<IsConst, child_order_tag, tree<T, A>>>
    {
        typedef tree<T, A> tree_type;
        typedef std::ptrdiff_t difference_type;
        typedef typename tree_type::value_type value_type;
        typedef typename
            std::conditional<IsConst, value_type const*, value_type*>::type pointer;
        typedef typename
            std::conditional<IsConst, value_type const&, value_type&>::type reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef typename tree_type::sub_iterator sub_iterator;
        
        static sub_iterator base(sub_iterator _a)
        {
            return _a;
        }
        
        static reference dereference(sub_iterator _a)
        {
            assert(_a != sub_iterator());
            return (**_a).m_value;
        }
        
        static sub_iterator& increment(sub_iterator& _a)
        {
            return (++_a);
        }
        
        static sub_iterator& decrement(sub_iterator& _a)
        {
            return (--_a);
        }
    };
    
    template<bool IsConst, typename Traversal, typename T, typename A>
    tree_iterator<IsConst, child_order_tag, tree<T, A>>
    child_order_begin(tree_iterator<IsConst, Traversal, tree<T, A>> _it)
    {
        typedef tree_iterator_traits<tree_iterator<IsConst, child_order_tag, tree<T, A>>>
            traits_type;
        auto a = _it.base();
        return tree_iterator<IsConst, child_order_tag, tree<T, A>>((**a).m_children.begin());
    }
    
    template<bool IsConst, typename Traversal, typename T, typename A>
    tree_iterator<IsConst, child_order_tag, tree<T, A>>
    child_order_end(tree_iterator<IsConst, Traversal, tree<T, A>> _it)
    {
        typedef tree_iterator_traits<tree_iterator<IsConst, child_order_tag, tree<T, A>>>
            traits_type;
        auto a = _it.base();
        return tree_iterator<IsConst, child_order_tag, tree<T, A>>((**a).m_children.end());
    }
    
    template<bool IsConst, typename Traversal, typename T, typename A>
    tree_iterator<IsConst, reverse_tag<child_order_tag>, tree<T, A>>
    child_order_rbegin(tree_iterator<IsConst, Traversal, tree<T, A>> _it)
    {
        return tree_iterator<IsConst, reverse_tag<child_order_tag>, tree<T, A>>(child_order_end(_it).base());
    }
    
    template<bool IsConst, typename Traversal, typename T, typename A>
    tree_iterator<IsConst, reverse_tag<child_order_tag>, tree<T, A>>
    child_order_rend(tree_iterator<IsConst, Traversal, tree<T, A>> _it)
    {
        return tree_iterator<IsConst, reverse_tag<child_order_tag>, tree<T, A>>(child_order_begin(_it).base());
    }
    
    //-----------------------------------------------------------
    //    output_dot
    //-----------------------------------------------------------
    template<typename Tree>
    void output_dot(Tree const& tree0, std::ostream& out,
        std::string const& graph_name = std::string("G"))
    {
        if(!out) return;
        typename Tree::const_pre_order_iterator it = tree0.pre_order_begin(), end = tree0.pre_order_end();
        out << "digraph " << "\"" << graph_name << "\" {\n";
        while(it != end){
            typename Tree::const_child_order_iterator child = child_order_begin(it),
                child_end = child_order_end(it);
            out << "\t" << "N" << *(it.base()) << "[ label=\"" << *it << "\" ];\n";
            while(child != child_end){
                out << "\t" << "N" << *(it.base()) << " -> " << "N" << *(child.base());
                out << ";\n";
                ++child;
            }
            ++it;
        }
        out << "}\n";
    }
}//---- namespace creek

#endif



