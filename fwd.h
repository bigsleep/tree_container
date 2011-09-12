#ifndef CREEK_TREE_FWD_H
#define CREEK_TREE_FWD_H

namespace creek
{
    namespace detail
    {
        struct binary_tree_traits_dummy_base {};
        struct multiway_tree_traits_dummy_base {};
    };
    
    struct pre_order_tag {};
    struct in_order_tag {};
    struct post_order_tag {};
    struct level_order_tag {};
    struct child_order_tag {};
    
    template<typename Tag>
    struct reverse_tag {};
    
    template<bool IsConst, typename TraversalTag, typename TreeType>
    struct tree_iterator;
    
    template<typename Iterator, typename Sfinae = void>
    struct tree_iterator_traits {};
    
    template<typename Tree>
    struct binary_tree_traits
        : public detail::binary_tree_traits_dummy_base
    {};
    
    template<typename Tree>
    struct is_binary_tree
    {
        static bool const value =
            !std::is_base_of<
                detail::binary_tree_traits_dummy_base,
                binary_tree_traits<Tree>>::value;
    };
    
    template<typename Tree>
    struct multiway_tree_traits
        : public detail::multiway_tree_traits_dummy_base
    {};
    
    template<typename Tree>
    struct is_multiway_tree
    {
        static bool const value =
            !std::is_base_of<
                detail::multiway_tree_traits_dummy_base,
                multiway_tree_traits<Tree>>::value;
    };
    
    template<typename Iterator>
    struct is_reverse_iterator
        : public std::false_type {};
    
    template<bool IsConst, typename Tag, typename Tree>
    struct is_reverse_iterator<tree_iterator<IsConst, reverse_tag<Tag>, Tree>>
        : public std::true_type {};
};
#endif
