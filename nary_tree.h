#ifndef creek_NARY_TREE_H
#define creek_NARY_TREE_H

#include <vector>
#include <cstddef>
#include <iterator>
#include <utility>
#include <cassert>
#include <stdexcept>
#include <limits>

namespace creek{
    
    namespace nary{
        inline std::size_t _pow(std::size_t _x, std::size_t _i)
        {
            std::size_t const size_pre_max = std::numeric_limits<std::size_t>::max() / _x;
            std::size_t tmp = 1;
            while(_i > 0){
                if(tmp > size_pre_max){
                    throw std::length_error("creek::nary::_pow()");
                }
                tmp *= _x;
                --_i;
            }
            return tmp;
        }
        
        inline std::size_t calc_node_size(std::size_t _n, std::size_t _level)
        {
            return ((_pow(_n, _level) - 1) / (_n - 1));
        }
        
        template<typename T, typename A>
        struct node
        {
            typedef node self_type;
            typedef T value_type;
            typedef typename A::template rebind<self_type>::other node_allocator_type;
            typedef typename std::vector<self_type, node_allocator_type>::iterator iterator;
            
            value_type m_value;
            std::size_t const m_level;
            iterator const m_parent;
            iterator const m_child_begin;
            iterator const m_child_end;
            
            node(value_type const& _v, std::size_t _lv, iterator _p, iterator _b, iterator _e)
                : m_value(_v), m_level(_lv), m_parent(_p), m_child_begin(_b), m_child_end(_e)
            {}
            
            node(node&& _other)
                : m_value(std::move(_other.m_value)),
                m_level(_other.m_level),
                m_parent(_other.m_parent),
                m_child_begin(_other.m_child_begin),
                m_child_end(_other.m_child_end)
            {}
            
            node() = delete;
            node(node const&) = delete;
            node& operator=(node const&) = delete;
            
            node& operator=(node&& _other)
            {
                std::swap(*this, _other);
                return *this;
            }
            
            value_type& get_property()
            { return m_value; }
            
            value_type const& get_property() const
            { return m_value; }
        };
    }//---- nary
    
    template<std::size_t N, typename T, typename Allocator = std::allocator<T>>
    class nary_tree
    {
    private:
        template<bool IsConst>
        class tree_iterator;
        
        template<bool IsConst>
        class tree_reverse_iterator;
    
    public:
        static std::size_t const arity;
        typedef nary_tree self_type;
        typedef Allocator allocator_type;
        typedef nary::node<T, allocator_type> node_type;
        typedef typename Allocator::template rebind<node_type>::other node_allocator_type;
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type const* const_pointer;
        typedef value_type& reference;
        typedef value_type const&const_reference;
        typedef tree_iterator<false> iterator;
        typedef tree_iterator<true> const_iterator;
        typedef tree_reverse_iterator<false> reverse_iterator;
        typedef tree_reverse_iterator<true> const_reverse_iterator;
        typedef std::size_t size_type;
        typedef std::vector<node_type, node_allocator_type> storage_type;
        typedef typename storage_type::iterator sub_iterator;
        typedef typename storage_type::const_iterator const_sub_iterator;
        
    private:
        std::size_t m_level;
        storage_type m_nodes;
    
    public:
        
        // constructor
        nary_tree()
            : m_level(0), m_nodes()
        {}
        
        nary_tree(std::size_t _level, value_type const& _v )
            : m_level(_level), m_nodes()
        {
            try{
                m_nodes.reserve(nary::calc_node_size(N, m_level));
            
                sub_iterator it_parent = m_nodes.begin(),
                    it_child = it_parent + (N + 1);
                
                // root
                m_nodes.push_back(node_type(_v, 0, sub_iterator(), it_parent + 1, it_child));
                
                for(std::size_t i = 1; i < m_level; ++i){
                    std::size_t sibling_size = nary::_pow(N, i);
                    for(std::size_t j = 0; j < sibling_size; ++j){
                        if(i < m_level - 1){
                            m_nodes.push_back(node_type(_v, i, it_parent, it_child, it_child + N));
                            it_child += N;
                        }else{
                            m_nodes.push_back(node_type(_v, i, it_parent, sub_iterator(), sub_iterator()));
                        }
                        if(((j + 1) % N) == 0)
                            ++it_parent;
                    }
                }
            }
            catch(std::length_error const& _e){
                throw std::length_error("creek::nary_tree::nary_tree()");
            }
        }
        
        // copy constructor
        nary_tree(self_type const& _other)
            : m_level(_other.m_level), m_nodes()
        {
            if(this != &_other){
                std::size_t const newsize = _other.size();
                
                try{
                    m_nodes.reserve(newsize);
                }
                catch(std::length_error const& _e){
                    throw std::length_error("creek::nary_tree::nary_tree(nary_tree const&)");
                }
                m_nodes.clear();
                sub_iterator root = m_nodes.begin();
                const_sub_iterator other_root = _other.m_nodes.begin(), oit = other_root;
                sub_iterator const empty;
                
                for(std::size_t i = 0; i < newsize; ++i){
                    sub_iterator parent =
                        ((*oit).m_parent != empty) ?
                        (root + ((*oit).m_parent - other_root)) : empty;
                    sub_iterator child_begin =
                        ((*oit).m_child_begin != empty) ?
                        (root + ((*oit).m_child_begin - other_root)) : empty;
                    sub_iterator child_end =
                        ((*oit).m_child_end != empty) ?
                        (root + ((*oit).m_child_end - other_root)) : empty;
                    m_nodes.push_back(node_type((*oit).get_property(), (*oit).m_level, parent, child_begin, child_end));
                    ++oit;
                }
            }
        }
        
        // move constructor
        nary_tree(self_type&& _other)
            : m_level(_other.m_level), m_nodes(std::forward(_other.m_nodes))
        {}
        
        // copy assignment operator
        self_type& operator=(self_type const& _other)
        {
            if(this == &_other) return *this;
            std::size_t const newsize = _other.size();
            
            try{
                m_nodes.reserve(newsize);
            }
            catch(std::length_error const& _e){
                throw std::length_error("creek::nary_tree::operator=(nary_tree const&)");
            }
            m_nodes.clear();
            sub_iterator root = m_nodes.begin();
            const_sub_iterator other_root = _other.m_nodes.begin(), oit = other_root;
            sub_iterator const empty;
            
            m_level = _other.m_level;
            
            for(std::size_t i = 0; i < newsize; ++i){
                sub_iterator parent =
                    ((*oit).m_parent != empty) ?
                    (root + ((*oit).m_parent - other_root)) : empty;
                sub_iterator child_begin =
                    ((*oit).m_child_begin != empty) ?
                    (root + ((*oit).m_child_begin - other_root)) : empty;
                sub_iterator child_end =
                    ((*oit).m_child_end != empty) ?
                    (root + ((*oit).m_child_end - other_root)) : empty;
                m_nodes.push_back(node_type((*oit).get_property(), (*oit).m_level, parent, child_begin, child_end));
                ++oit;
            }
            return *this;
        }
        
        // move assignment operator
        self_type& operator=(self_type&& _other)
        {
            if(this == &_other) return *this;
            m_level = _other.m_level;
            std::swap(m_nodes, _other.m_nodes);
            return *this;
        }
        
        // iterators
        iterator begin()
        { return iterator(m_nodes.begin()); }
    
        iterator end()
        { return iterator(m_nodes.end()); }
        
        const_iterator begin() const
        { return const_iterator(m_nodes.begin()); }
    
        const_iterator end() const
        { return const_iterator(m_nodes.end()); }
        
        reverse_iterator rbegin()
        { return reverse_iterator(m_nodes.end()); }
    
        reverse_iterator rend()
        { return reverse_iterator(m_nodes.begin()); }
        
        const_reverse_iterator rbegin() const
        { return const_reverse_iterator(m_nodes.end()); }
    
        const_reverse_iterator rend() const
        { return const_reverse_iterator(m_nodes.begin()); }
        
        size_type size() const
        { return m_nodes.size(); }
        
        size_type level() const
        { return m_level; }
        
        // element access
        value_type& operator[](std::size_t _pos)
        { return m_nodes[_pos].get_property(); }
        
        value_type const& operator[](std::size_t _pos) const
        { return m_nodes[_pos].get_property(); }
        
        value_type& at(std::size_t _pos)
        try
        { return m_nodes.at(_pos).get_property(); }
        catch(std::out_of_range const& _e)
        { throw std::out_of_range("creek::nary_tree::at()"); }
        
        value_type const& at(std::size_t _pos) const
        try
        { return m_nodes.at(_pos).get_property(); }
        catch(std::out_of_range const& _e)
        { throw std::out_of_range("creek::nary_tree::at()"); }
        
    private:
        
        //-----------------------------------------------------------
        //    tree_iterator
        //-----------------------------------------------------------
        template<bool IsConst>
        class tree_iterator
        {
        public:
            static bool const is_const_iterator = IsConst;
            typedef tree_iterator self_type;
            typedef nary_tree::value_type value_type;
            typedef std::ptrdiff_t difference_type;
            typedef typename
                std::conditional<IsConst, value_type const*, value_type*>::type pointer;
            typedef typename
                std::conditional<IsConst, value_type const&, value_type&>::type reference;
            typedef std::random_access_iterator_tag iterator_category;
            typedef typename
                std::conditional<IsConst,
                    typename std::vector<node_type>::const_iterator,
                    typename std::vector<node_type>::iterator
                >::type sub_iterator;
            
        private:
            sub_iterator m_current;
            friend class tree_iterator<!IsConst>;
        
        public:
            
            tree_iterator() : m_current() {}
            
            explicit tree_iterator(sub_iterator _it)
                : m_current(_it)
            {}
            
            tree_iterator(tree_iterator<false> const& _other)
                : m_current(_other.m_current) {}
            
            // forward iterator requirements
            reference operator*() const
            {
                assert(m_current != sub_iterator());
                return ((*m_current).get_property());
            }
            
            pointer operator->() const
            {
                assert(m_current != sub_iterator());
                return &(operator*());
            }
            
            self_type& operator++()
            {
                ++m_current;
                return *this;
            }
            
            self_type operator++(int)
            {
                self_type tmp(*this);
                ++(*this);
                return tmp;
            }
            
            // bidirectional iterator requirements
            self_type& operator--()
            {
                --m_current;
                return *this;
            }
            
            self_type operator--(int)
            {
                self_type tmp(*this);
                --(*this);
                return tmp;
            }
            
            // random access iterator requirements
            reference operator[](difference_type _n) const
            { return m_current[_n]; }

            self_type& operator+=(difference_type _n)
            {
                m_current += _n;
                return *this;
            }

            self_type operator+(difference_type _n) const
            { return self_type(m_current + _n); }

            self_type& operator-=(difference_type _n)
            {
                m_current -= _n;
                return *this;
            }

            self_type operator-(difference_type _n) const
            { return self_type(m_current - _n); }
            
            template<bool B>
            difference_type operator-(tree_iterator<B> const& _other) const
            { return difference_type(m_current - _other.m_current); }
            
            // comparison
            template<bool B>
            bool operator==(tree_iterator<B> const& _other) const
            {
                return (m_current == _other.m_current);
            }
        
            template<bool B>
            bool operator!=(tree_iterator<B> const& _other) const
            {
                return (m_current != _other.m_current);
            }
            
            template<bool B>
            bool operator<(tree_iterator<B> const& _other) const
            {
                return (m_current < _other.m_current);
            }
        
            template<bool B>
            bool operator>(tree_iterator<B> const& _other) const
            {
                return (m_current > _other.m_current);
            }
            
            template<bool B>
            bool operator<=(tree_iterator<B> const& _other) const
            {
                return (m_current <= _other.m_current);
            }
        
            template<bool B>
            bool operator>=(tree_iterator<B> const& _other) const
            {
                return (m_current >= _other.m_current);
            }
            
            // other
            self_type get_parent() const
            {
                assert(m_current != sub_iterator());
                return self_type((*m_current).m_parent);
            }
            
            self_type begin() const
            {
                assert(m_current != sub_iterator());
                return self_type((*m_current).m_child_begin);
            }
            
            self_type end() const
            {
                assert(m_current != sub_iterator());
                return self_type((*m_current).m_child_end);
            }
            
            tree_reverse_iterator<IsConst> rbegin() const
            {
                return reverse_iterator(end());
            }
            
            tree_reverse_iterator<IsConst> rend() const
            {
                return reverse_iterator(begin());
            }
            
            std::size_t level() const
            {
                assert(m_current != sub_iterator());
                return (*m_current).m_level;
            }
        };
        
        //-----------------------------------------------------------
        //    tree_reverse_iterator
        //-----------------------------------------------------------
        template<bool IsConst>
	    class tree_reverse_iterator : public std::reverse_iterator<tree_iterator<IsConst>>
	    {
	    public:
		    typedef tree_reverse_iterator self_type;
		    typedef std::reverse_iterator<tree_iterator<IsConst>> base_type;
		    typedef typename tree_iterator<IsConst>::sub_iterator sub_iterator;
	    
	    private:
	        friend class tree_reverse_iterator<!IsConst>;
	        
	    public:
		    // constructor
		    tree_reverse_iterator()
			    : base_type() {}
		
		    tree_reverse_iterator(tree_iterator<IsConst> const& _pos)
			    : base_type(_pos) {}
		
		    tree_reverse_iterator(tree_reverse_iterator<false> const& _other)
			    : base_type(_other.tree_reverse_iterator<false>::base_type::base()) {}
		    
		    // begin end
		    tree_iterator<IsConst> begin() const
		    {
			    tree_iterator<IsConst> tmp(base_type::base());
			    --tmp;
			    return tmp.begin();
		    }
		
		    tree_iterator<IsConst> end() const
		    {
			    tree_iterator<IsConst> tmp(base_type::base());
			    --tmp;
			    return tmp.end();
		    }
		    
		    // rbegin rend
		    self_type rbegin() const
		    {
			    tree_iterator<IsConst> tmp(base_type::base());
			    --tmp;
			    return tmp.rbegin();
		    }
		
		    self_type rend() const
		    {
			    tree_iterator<IsConst> tmp(base_type::base());
			    --tmp;
			    return tmp.rend();
		    }
	    };//---- class reverse_iterator
    };
    
    template<std::size_t N, typename T, typename A>
    std::size_t const nary_tree<N, T, A>::arity = N;
}
#endif





