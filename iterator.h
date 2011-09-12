#ifndef CREEK_TREE_ITERATOR_H
#define CREEK_TREE_ITERATOR_H

#include "iterator_traits.h"

namespace creek{
    template<bool IsConst, typename TraversalTag, typename TreeType>
    class tree_iterator
        : public std::iterator<
            typename tree_iterator_traits<tree_iterator<IsConst, TraversalTag, TreeType>>::iterator_category,
            typename tree_iterator_traits<tree_iterator<IsConst, TraversalTag, TreeType>>::value_type,
            typename tree_iterator_traits<tree_iterator<IsConst, TraversalTag, TreeType>>::difference_type,
            typename tree_iterator_traits<tree_iterator<IsConst, TraversalTag, TreeType>>::pointer,
            typename tree_iterator_traits<tree_iterator<IsConst, TraversalTag, TreeType>>::reference>
    {
    private:
        typedef tree_iterator self_type;
        typedef tree_iterator_traits<self_type> traits_type;
    
    public:
        typedef typename tree_iterator_traits<self_type>::iterator_category iterator_category;
        typedef typename tree_iterator_traits<self_type>::value_type value_type;
        typedef typename tree_iterator_traits<self_type>::difference_type difference_type;
        typedef typename tree_iterator_traits<self_type>::pointer pointer;
        typedef typename tree_iterator_traits<self_type>::reference reference;
        static bool const is_const = IsConst;
        typedef TraversalTag traversal_tag;
        typedef TreeType tree_type;
        typedef typename traits_type::sub_iterator sub_iterator;
        
    private:
        sub_iterator m_current;
        friend class tree_iterator<!is_const, traversal_tag, tree_type>;
    
    public:
        tree_iterator() : m_current() {}
        
        explicit tree_iterator(sub_iterator _node)
            : m_current(_node)
        {}
        
        tree_iterator(tree_iterator<false, traversal_tag, tree_type> const& _other)
            : m_current(_other.m_current) {}
        
        reference operator*() const
        {
            return traits_type::dereference(m_current);
        }
        
        pointer operator->() const
        {
            return &(operator*());
        }
        
        self_type& operator++()
        {
            traits_type::increment(m_current);
            return *this;
        }
        
        self_type operator++(int)
        {
            self_type tmp(*this);
            ++(*this);
            return tmp;
        }
        
        // ---- bidirectional or random_access
        self_type& operator--()
        {
            traits_type::decrement(m_current);
            return *this;
        }
        
        // ---- bidirectional or random_access
        self_type operator--(int)
        {
            self_type tmp(*this);
            --(*this);
            return tmp;
        }
        
        // ---- random_access
        self_type operator+(difference_type _n) const
        {
            return self_type(traits_type::next(m_current, _n));
        }
        
        // ---- random_access
        self_type& operator+=(difference_type _n)
        {
            m_current = traits_type::next(m_current, _n);
            return (*this);
        }
        
        // ---- random_access
        self_type operator-(difference_type _n) const
        {
            return self_type(traits_type::prev(m_current, _n));
        }
        
        // ---- random_access
        self_type& operator-=(difference_type _n) const
        {
            m_current = traits_type::prev(m_current, _n);
            return (*this);
        }
        
        // ---- random_access
        reference operator[](difference_type _n) const
        {
            return traits_type::dereference(traits_type::next(m_current, _n));
        }
        
        sub_iterator base() const
        {
            return traits_type::base(m_current);
        }
        
        template<bool IsC>
        bool operator==(tree_iterator<IsC, traversal_tag, tree_type> const& _a) const
        {
            return (m_current == _a.m_current);
        }
        
        template<bool IsC>
        bool operator!=(tree_iterator<IsC, traversal_tag, tree_type> const& _a) const
        {
            return !(*this == _a);
        }
    };
    
    template<bool IsConstA, bool IsConstB, typename Tag, typename Tree>
    bool operator>(tree_iterator<IsConstA, Tag, Tree> const& _a,
        tree_iterator<IsConstB, Tag, Tree> const& _b)
    {
        return !tree_iterator_traits<tree_iterator<IsConstA, Tag, Tree>>::less(_a.base(), _b.base());
    }
    
    template<bool IsConstA, bool IsConstB, typename Tag, typename Tree>
    bool operator>=(tree_iterator<IsConstA, Tag, Tree> const& _a,
        tree_iterator<IsConstB, Tag, Tree> const& _b)
    {
        typedef tree_iterator_traits<tree_iterator<IsConstA, Tag, Tree>> traits_type;
        return (!traits_type::less(_a.base(), _b.base()) || (_a == _b));
    }
    
    template<bool IsConstA, bool IsConstB, typename Tag, typename Tree>
    bool operator<(tree_iterator<IsConstA, Tag, Tree> const& _a,
        tree_iterator<IsConstB, Tag, Tree> const& _b)
    {
        return tree_iterator_traits<tree_iterator<IsConstA, Tag, Tree>>::less(_a.base(), _b.base());
    }
    
    template<bool IsConstA, bool IsConstB, typename Tag, typename Tree>
    bool operator<=(tree_iterator<IsConstA, Tag, Tree> const& _a,
        tree_iterator<IsConstB, Tag, Tree> const& _b)
    {
        typedef tree_iterator_traits<tree_iterator<IsConstA, Tag, Tree>> traits_type;
        return (traits_type::less(_a.base(), _b.base()) || (_a == _b));
    }
    
    template<bool IsConstA, bool IsConstB, typename Tag, typename Tree>
    typename tree_iterator<IsConstA, Tag, Tree>::difference_type
    operator-(tree_iterator<IsConstA, Tag, Tree> const& _a,
        tree_iterator<IsConstB, Tag, Tree> const& _b)
    {
        return tree_iterator_traits<tree_iterator<IsConstA, Tag, Tree>>::difference(_a.base(), _b.base());
    }
    
    template<bool IsConstA, bool IsConstB, typename Tag, typename Tree>
    tree_iterator<IsConstB, Tag, Tree> operator+(
        typename tree_iterator<IsConstA, Tag, Tree>::difference_type _n,
        tree_iterator<IsConstB, Tag, Tree> const& _b)
    {
        typedef tree_iterator_traits<tree_iterator<IsConstA, Tag, Tree>> traits_type;
        return tree_iterator<IsConstB, Tag, Tree>(traits_type::next(_b.base(), _n));
    }
}
#endif

