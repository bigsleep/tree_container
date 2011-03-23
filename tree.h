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

namespace creek
{
    //-----------------------------------------------------------
    //    conditional
    //-----------------------------------------------------------
    template<bool Condition, typename T1, typename T2>
    struct conditional;
    
    template<typename T1, typename T2>
    struct conditional<true, T1, T2>
    {
        typedef T1 type;
    };
    
    template<typename T1, typename T2>
    struct conditional<false, T1, T2>
    {
        typedef T2 type;
    };
    
    //-----------------------------------------------------------
    //    traverse tag
    //-----------------------------------------------------------
    struct preorder_tag {};
    struct postorder_tag {};
    struct child_tag{};
    
    
    //-----------------------------------------------------------
    //    tree_node
    //-----------------------------------------------------------
    struct tree_node_base
    {
        typedef tree_node_base self_type;
        typedef std::list<tree_node_base*>::iterator iterator;
        typedef std::list<tree_node_base*>::const_iterator const_iterator;
        
        tree_node_base()
            : m_parent(), m_older_sibling(), m_younger_sibling(), m_children()
        {}
        
        explicit tree_node_base(
            iterator _parent,
            iterator _older = iterator(),
            iterator _younger = iterator())
            : m_parent(_parent),
              m_older_sibling(_older),
              m_younger_sibling(_younger),
              m_children()
        {}
        
        std::list<tree_node_base*>::iterator m_parent;
        std::list<tree_node_base*>::iterator m_older_sibling;
        std::list<tree_node_base*>::iterator m_younger_sibling;
        std::list<tree_node_base*> m_children;
    };
    
    template<typename Tp>
    struct tree_node : public tree_node_base
    {
        tree_node() : tree_node_base(), m_value()
        {}
        
        explicit tree_node(iterator _parent, Tp const& _val = Tp())
            : tree_node_base(_parent), m_value(_val) {}
        
        tree_node(
            iterator _parent,
            iterator _older,
            iterator _younger,
            Tp const& _val = Tp())
            : tree_node_base(_parent, _older, _younger), m_value(_val) {}
        
        Tp& get_property()
        { return m_value; }
        Tp const& get_property() const
        { return m_value; }
        void set_property(Tp const& _val)
        { m_value = _val; }
        
        Tp m_value;
    };
    
    template<typename TraverseTag>
    tree_node_base::iterator tree_traverse_increment(tree_node_base::iterator node0);
    template<typename TraverseTag>
    tree_node_base::const_iterator tree_traverse_increment(tree_node_base::const_iterator node0);
    template<typename TraverseTag>
    tree_node_base::iterator tree_traverse_decrement(tree_node_base::iterator node0);
    template<typename TraverseTag>
    tree_node_base::const_iterator tree_traverse_decrement(tree_node_base::const_iterator node0);
    
    //---- preorder
    template<>
    tree_node_base::iterator tree_traverse_increment<preorder_tag>(tree_node_base::iterator node0);
    template<>
    tree_node_base::const_iterator tree_traverse_increment<preorder_tag>(tree_node_base::const_iterator node0);
    template<>
    tree_node_base::iterator tree_traverse_decrement<preorder_tag>(tree_node_base::iterator node0);
    template<>
    tree_node_base::const_iterator tree_traverse_decrement<preorder_tag>(tree_node_base::const_iterator node0);
    
    //---- postorder
    template<>
    tree_node_base::iterator tree_traverse_increment<postorder_tag>(tree_node_base::iterator node0);
    template<>
    tree_node_base::const_iterator tree_traverse_increment<postorder_tag>(tree_node_base::const_iterator node0);
    template<>
    tree_node_base::iterator tree_traverse_decrement<postorder_tag>(tree_node_base::iterator node0);
    template<>
    tree_node_base::const_iterator tree_traverse_decrement<postorder_tag>(tree_node_base::const_iterator node0);
    
    //---- child
    template<>
    tree_node_base::iterator tree_traverse_increment<child_tag>(tree_node_base::iterator node0);
    template<>
    tree_node_base::const_iterator tree_traverse_increment<child_tag>(tree_node_base::const_iterator node0);
    template<>
    tree_node_base::iterator tree_traverse_decrement<child_tag>(tree_node_base::iterator node0);
    template<>
    tree_node_base::const_iterator tree_traverse_decrement<child_tag>(tree_node_base::const_iterator node0);
    
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
        
        typedef tree_node<Tp> node_type;
        typedef tree_node_base node_base;
        typedef node_type* node_pointer;
        typedef typename Allocator::template rebind<node_type>::other node_allocator;
        
    private:
        std::list<node_base*> m_root_list;
        std::list<node_base*>::iterator m_root;
        std::list<node_base*>::iterator m_foot;
        node_base m_foot_obj;
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
            if(_other.size() != 0){
                const_preorder_iterator iother = _other.preorder_begin(), other_end = _other.preorder_end();
                
                preorder_iterator iself = this->insert(this->preorder_begin(), *iother);
                while(iother != other_end){
                    const_child_iterator chi = iother.begin();
                    const_child_iterator chi_end = iother.end();
                    while(chi != chi_end){
                        append_child(iself, *chi);
                        ++chi;
                    }
                    ++iother;
                    ++iself;
                }
            }
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
            node_base::iterator node1 = _pos.base();
            assert(node1 != node_base::iterator());
            assert(!((node1 == m_foot) && m_root != m_foot));
            
            if(m_size == 0){
                node_type* root_tmp = create_node(
                    node_type(
                        node_base::iterator(),
                        node_base::iterator(),
                        node_base::iterator(),
                        _val));
                m_root_list.push_back(root_tmp);
                m_root_list.push_back(&m_foot_obj);
                m_root = m_root_list.begin();
                m_foot = (++(m_root_list.begin()));
                (*m_root)->m_younger_sibling = m_foot;
                m_foot_obj.m_older_sibling = m_root;
                return Iterator(m_root);
            }else{
                std::list<node_base*>&
                    list1 = (node1 == m_root)
                        ? m_root_list
                        : (*((*node1)->m_parent))->m_children;
                node_pointer tmp = create_node(
                    node_type(
                        (*node1)->m_parent,
                        (*node1)->m_older_sibling,
                        (*node1)->m_younger_sibling, _val));
                list1.insert(node1, tmp);
                node_base::iterator it = node1;
                --it;
                tmp->m_children.splice(tmp->m_children.begin(), list1, node1);
                
                if((*it)->m_older_sibling != node_base::iterator())
                    (*((*it)->m_older_sibling))->m_younger_sibling = it;
                if((*it)->m_younger_sibling != node_base::iterator())
                    (*((*it)->m_younger_sibling))->m_older_sibling = it;
                
                tmp->m_children.front()->m_parent = it;
                tmp->m_children.front()->m_older_sibling = node_base::iterator();
                tmp->m_children.front()->m_younger_sibling = node_base::iterator();
                
                if(node1 == m_root) m_root = it;
                return Iterator(it);
            }
        }
        
        //---- append child
        template<typename Iterator>
        Iterator append_child(Iterator _pos, const value_type& _val)
        {
            node_base::iterator node1 = _pos.base();
            assert(node1 != node_base::iterator());
            assert(node1 != m_foot);
            
            node_pointer tmp = create_node(node1, _val);
            if(!((*node1)->m_children.empty())){
                node_base::iterator tmp_back = (*node1)->m_children.end();
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
            node_base::iterator node1 = _pos.base();
            assert(node1 != node_base::iterator());
            assert(node1 != m_foot);
            
            const self_type& cp1 = (&_subtree != this) ? _subtree : self_type(_subtree);
            
            const_preorder_iterator iother = cp1.preorder_begin(), other_end = cp1.preorder_end();
            preorder_iterator iself(node1);
            iself = append_child(iself, *iother);
            while(iother != other_end){
                const_child_iterator chi = iother.begin(),
                    chi_end = iother.end();
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
            node_base::iterator parent = _parent.base();
            node_base::iterator node1 = _child_pos.base();
            assert(parent != node_base::iterator());
            assert(node1 != node_base::iterator());
            assert(node1 != m_foot);
            assert(node1 != m_root);
            assert(_parent == _child_pos.get_parent());
            
            node_pointer tmp = create_node(
                node_type(
                    parent,
                    (*node1)->m_older_sibling,
                    node1, _val));
            node_base::iterator it = (*parent)->m_children.insert(node1, tmp);
            
            if((*node1)->m_older_sibling != node_base::iterator())
                (*((*node1)->m_older_sibling))->m_younger_sibling = it;
            (*node1)->m_older_sibling = it;
            
            return Iterator(it);
        }
        
        //---- insert child
        template<typename Iterator, typename Iterator2>
        Iterator insert_child_tree(Iterator _parent, Iterator2 _child_pos, self_type const& _other)
        {
            node_base::iterator parent = _parent.base();
            node_base::iterator node1 = _child_pos.base();
            assert(parent != node_base::iterator());
            assert(node1 != node_base::iterator());
            assert(node1 != m_foot);
            assert(node1 != m_root);
            assert(_parent == _child_pos.get_parent());
            
            self_type const& cp1 = (&_other != this) ? _other : self_type(_other);
            const_preorder_iterator iother = cp1.preorder_begin();
            const_preorder_iterator other_end = cp1.preorder_end();
            
            preorder_iterator iself(node1);
            iself = insert_child(_parent, iself, *iother);
            while(iother != other_end){
                const_child_iterator chi = iother.begin();
                const_child_iterator chi_end = iother.end();
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
            node_base::iterator node1 = _pos.base();
            assert(node1 != node_base::iterator());
            assert(node1 != m_foot);
            node_base::iterator const empty = node_base::iterator();
            
            if(!(*node1)->m_children.empty()){
                node_base::iterator cur_node = (--((*node1)->m_children.end()));
                while(1){
                    if(cur_node == node1) break;
                    if((*cur_node)->m_children.empty()){
                        node_base::iterator nodetmp = cur_node;
                        cur_node = (*cur_node)->m_parent;
                        destroy_and_deallocate(static_cast<node_type*>(*nodetmp));
                        (*cur_node)->m_children.pop_back();
                        if(!((*cur_node)->m_children.empty()))
                            cur_node = (--((*cur_node)->m_children.end()));
                    }else{
                        cur_node = (--((*cur_node)->m_children.end()));
                    }
                }
            }
            node_base::iterator parentnode = (*node1)->m_parent;
            if(parentnode != empty){
                if((*node1)->m_older_sibling != empty)
                    (*((*node1)->m_older_sibling))->m_younger_sibling = (*node1)->m_younger_sibling;
                if((*node1)->m_younger_sibling != empty)
                    (*((*node1)->m_younger_sibling))->m_older_sibling = (*node1)->m_older_sibling;
                (*parentnode)->m_children.erase(node1);
                destroy_and_deallocate(static_cast<node_type*>(*node1));
            }else{
                assert(node1 == m_root);
                destroy_and_deallocate(static_cast<node_type*>(*node1));
                m_root_list.clear();
                m_root = m_root_list.begin();
                m_foot = m_root_list.begin();
                m_foot_obj = node_base();
            }
        }
        
        //---- clear
        void clear()
        {
            if(m_root == m_foot)
                return;
            erase(preorder_iterator(m_root));
        }
        
        //---- operator=
        self_type& operator=(self_type const& _other)
        {
            if(&_other == this) return *this;
            clear();
            if(_other.m_size == 0) return *this;
            else
            {
                const_preorder_iterator iother = _other.preorder_begin(), other_end = _other.preorder_end();
                preorder_iterator iself = this->insert(this->preorder_begin(), *iother);
                while(iother != other_end){
                    const_child_iterator chi = iother.begin();
                    const_child_iterator chi_end = iother.end();
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
            node_base::iterator node1 = _pos.base();
            assert(node1 != node_base::iterator());
            assert(node1 != m_foot);
            node_base::iterator const empty = node_base::iterator();
            
            self_type subcopy;
            if(m_size == 0) return subcopy;
            
            const_preorder_iterator isub(node1), isub_end(empty);
            {
                node_base::iterator cur_node(node1);
                while(cur_node != empty){
                    if((*cur_node)->m_younger_sibling != empty){
                        isub_end = const_preorder_iterator((*cur_node)->m_younger_sibling);
                        break;
                    }
                    cur_node = (*cur_node)->m_parent;
                }
            }
            assert(isub_end.base() != empty);
            
            preorder_iterator icopy = subcopy.preorder_begin();
            icopy = subcopy.insert(icopy, *isub);
            
            while(isub != isub_end){
                const_child_iterator chi = isub.begin();
                const_child_iterator chi_end = isub.end();
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
        node_pointer create_node(node_type const& _node)
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
        
        node_pointer create_node(node_base::iterator _parent = node_base::iterator(), value_type const& _val = value_type())
        {
            node_pointer tmp = m_alloc.allocate(1);
            try{
                m_alloc.construct(tmp, node_type(_parent, _val));
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
        template<bool IsConst, typename TraverseTag>
        class tree_iterator;
        template<typename Ite>
        class tree_reverse_iterator;
        
        typedef tree_iterator<false, preorder_tag> preorder_iterator;
        typedef tree_iterator<true, preorder_tag> const_preorder_iterator;
        typedef tree_iterator<false, postorder_tag> postorder_iterator;
        typedef tree_iterator<true, postorder_tag> const_postorder_iterator;
        typedef tree_iterator<false, child_tag> child_iterator;
        typedef tree_iterator<true, child_tag> const_child_iterator;
        
        typedef tree_reverse_iterator<preorder_iterator> reverse_preorder_iterator;
        typedef tree_reverse_iterator<const_preorder_iterator> const_reverse_preorder_iterator;
        typedef tree_reverse_iterator<postorder_iterator> reverse_postorder_iterator;
        typedef tree_reverse_iterator<const_postorder_iterator> const_reverse_postorder_iterator;
        typedef tree_reverse_iterator<child_iterator> reverse_child_iterator;
        typedef tree_reverse_iterator<const_child_iterator> const_reverse_child_iterator;
        
        //---- preorder begin end
		preorder_iterator preorder_begin()
		{ return preorder_iterator(m_root); }
		
		preorder_iterator preorder_end()
		{ return preorder_iterator(m_foot); }
		
		const_preorder_iterator preorder_begin() const
		{ return const_preorder_iterator(m_root); }
		
		const_preorder_iterator preorder_end() const
		{ return const_preorder_iterator(m_foot); }
		
		//---- postorder begin end
		postorder_iterator postorder_begin()
		{
			if(m_root == m_foot) return postorder_iterator(m_root);
			node_base::iterator tmp = m_root;
			while(!((*tmp)->m_children.empty()))
				tmp = (*tmp)->m_children.begin();
			return postorder_iterator(tmp);
		}
		
		postorder_iterator postorder_end()
		{ return postorder_iterator(m_foot); }
		
		const_postorder_iterator postorder_begin() const
		{
			if(m_root == m_foot) return postorder_iterator(m_root);
			node_base::iterator tmp = m_root;
			while(!((*tmp)->m_children.empty()))
				tmp = (*tmp)->m_children.begin();
			return const_postorder_iterator(tmp);
		}
		
		const_postorder_iterator postorder_end() const
		{ return const_postorder_iterator(m_foot); }
        
        //---- preorder rbegin rend
		reverse_preorder_iterator preorder_rbegin()
		{ return reverse_preorder_iterator(preorder_end()); }
		const_reverse_preorder_iterator preorder_rbegin() const
		{ return const_reverse_preorder_iterator(preorder_end()); }
		reverse_preorder_iterator preorder_rend()
		{ return reverse_preorder_iterator(preorder_begin()); }
		const_reverse_preorder_iterator preorder_rend() const
		{ return const_reverse_preorder_iterator(preorder_begin()); }
        
        //---- postorder rbegin rend
		reverse_postorder_iterator postorder_rbegin()
		{ return reverse_postorder_iterator(postorder_end()); }
		const_reverse_postorder_iterator postorder_rbegin() const
		{ return const_reverse_postorder_iterator(postorder_end()); }
		reverse_postorder_iterator postorder_rend()
		{ return reverse_postorder_iterator(postorder_begin()); }
		const_reverse_postorder_iterator postorder_rend() const
		{ return const_reverse_postorder_iterator(postorder_begin()); }
		
        //-----------------------------------------------------------
        //    tree_iterator
        //-----------------------------------------------------------
        template<bool IsConst, typename TraverseTag>
        class tree_iterator
        {
        public:
            static bool const is_const_iterator = IsConst;
            typedef tree_iterator self_type;
            typedef TraverseTag traverse_tag;
            typedef tree::value_type value_type;
            typedef std::ptrdiff_t difference_type;
            typedef typename
                conditional<IsConst, value_type const*, value_type*>::type pointer;
            typedef typename
                conditional<IsConst, value_type const&, value_type&>::type reference;
            typedef std::bidirectional_iterator_tag iterator_category;
            
            typedef tree_iterator<IsConst, child_tag> dual_iterator;
            typedef tree_reverse_iterator<dual_iterator> dual_reverse_iterator;
            typedef typename
                conditional<IsConst, node_type const*, node_type*>::type node_pointer;
            typedef typename
                conditional<IsConst, node_type const&, node_type&>::type node_reference;
            typedef typename
                conditional<IsConst, node_base const*, node_base*>::type nodebase_pointer;
            typedef typename
                conditional<IsConst, node_base const&, node_base&>::type nodebase_reference;
            typedef node_base::iterator nodebase_iterator;
        
        private:
            nodebase_iterator m_current;
        
        public:
            tree_iterator() : m_current() {}
            
            explicit tree_iterator(nodebase_iterator _node)
                : m_current(_node)
            {}
            
            tree_iterator(tree_iterator<false, TraverseTag> const& _other)
                : m_current(_other.base()) {}
            
            //---- operators
            reference operator*() const
            {
                return ((static_cast<node_pointer>(*m_current))->get_property());
            }
            
            pointer operator->() const
            {
                return &(operator*());
            }
            
            self_type& operator++()
            {
                assert(m_current != nodebase_iterator());
                m_current = tree_traverse_increment<traverse_tag>(m_current);
                return *this;
            }
            
            self_type operator++(int)
            {
                assert(m_current != nodebase_iterator());
                self_type tmp(*this);
                ++(*this);
                return tmp;
            }
            
            self_type& operator--()
            {
                assert(m_current != nodebase_iterator());
                m_current = tree_traverse_decrement<traverse_tag>(m_current);
                return *this;
            }
            
            self_type operator--(int)
            {
                assert(m_current != nodebase_iterator());
                self_type tmp(*this);
                --(*this);
                return tmp;
            }
            
            template<bool B, typename Tag>
            bool operator==(tree_iterator<B, Tag> const& _other) const
            {
                return (m_current == _other.base());
            }
        
            template<bool B, typename Tag>
            bool operator!=(tree_iterator<B, Tag> const& _other) const
            {
                return (m_current != _other.base());
            }
            
            nodebase_iterator base() const
            {
                return m_current;
            }
            
            bool has_child() const
            {
                assert(m_current != nodebase_iterator());
                return !((*m_current)->m_children.empty());
            }
            
            self_type get_parent() const
            {
                assert(m_current != nodebase_iterator());
                return self_type((*m_current)->m_parent);
            }
            
            //---- dual_iterator
            inline dual_iterator begin() const
            {
                assert(m_current != nodebase_iterator());
                return dual_iterator((*m_current)->m_children.begin());
            }
            
            inline dual_iterator end() const
            {
                assert(m_current != nodebase_iterator());
                return dual_iterator((*m_current)->m_children.end());
            }
            
            inline dual_reverse_iterator rbegin() const
            {
                return dual_reverse_iterator(end());
            }
            
            inline dual_reverse_iterator rend() const
            {
                return dual_reverse_iterator(begin());
            }
        };
        
        //-----------------------------------------------------------
        //    tree_reverse_iterator
        //-----------------------------------------------------------
        template<typename Iterator>
	    class tree_reverse_iterator : public std::reverse_iterator<Iterator>
	    {
	    public:
		    typedef tree_reverse_iterator self_type;
		    typedef std::reverse_iterator<Iterator> base_iterator_type;
		    typedef typename Iterator::dual_iterator dual_iterator;
		    typedef typename Iterator::dual_reverse_iterator dual_reverse_iterator;
		    typedef typename Iterator::nodebase_pointer nodebase_pointer;
		    
		    //---- constructor
		    tree_reverse_iterator()
			    : base_iterator_type() {}
		
		    tree_reverse_iterator(Iterator _pos)
			    : base_iterator_type(_pos) {}
		
		    template<typename Ite>
		    tree_reverse_iterator(tree_reverse_iterator<Ite> const& _other)
			    : base_iterator_type(_other.base()) {}
		
		    //---- begin end
		    dual_iterator begin() const
		    {
			    Iterator tmp = base_iterator_type::base();
			    --tmp;
			    return tmp.begin();
		    }
		
		    dual_iterator end() const
		    {
			    Iterator tmp = base_iterator_type::base();
			    --tmp;
			    return tmp.end();
		    }
		
		    dual_reverse_iterator rbegin() const
		    {
			    Iterator tmp = base_iterator_type::base();
			    --tmp;
			    return tmp.rbegin();
		    }
		
		    dual_reverse_iterator rend() const
		    {
			    Iterator tmp = base_iterator_type::base();
			    --tmp;
			    return tmp.rend();
		    }
		    
		    typename Iterator::nodebase_iterator base() const
		    {
		        Iterator tmp = base_iterator_type::base();
			    --tmp;
			    return tmp.base();
		    }
		    
            bool has_child() const
            {
			    Iterator tmp = base_iterator_type::base();
			    --tmp;
                return (tmp.has_child());
            }
	    };//---- class reverse_iterator
    };
    
	//-----------------------------------------------------------
	//    output_dot
	//-----------------------------------------------------------
	template<typename Tree>
	void output_dot(Tree const& tree0, std::ostream& out,
		std::string const& graph_name = std::string("G"))
	{
		if(!out) return;
		typename Tree::const_preorder_iterator it = tree0.preorder_begin(), end = tree0.preorder_end();
		out << "digraph " << "\"" << graph_name << "\" {\n";
		while(it != end){
			typename Tree::const_child_iterator child = it.begin(),
				child_end = it.end();
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



