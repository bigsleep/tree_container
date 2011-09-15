#ifndef CREEK_KDTREE_TRAITS_H
#define CREEK_KDTREE_TRAITS_H

#include <array>
#include <utility>
#include <type_traits>

namespace creek
{
    struct kdtree_default {};
    
    namespace detail
    {
        struct kdtree_traits_dummy_base {};
        
        template<typename T>
        struct kdtree_component_type_traits;
        
        template<typename T>
        struct kdtree_access_traits;
        
        struct kdtree_traits_dummy_type
        {
            template<typename T>
            operator T()
            {}
        };
    }
    
    template<typename T>
    struct kdtree_traits : detail::kdtree_traits_dummy_base
    {
        typedef typename T::value_type component_type;
    };
}

#define CREEK_KDTREE_DETAIL_ACCESS(__type, __access) \
    namespace creek{ namespace detail{\
    template<> \
    struct kdtree_access_traits< __type > \
    { \
        inline static auto get(__type const& _v, std::size_t i) \
            -> decltype(_v __access)\
        { \
            return ( _v __access ); \
        } \
    }; }}

#define CREEK_KDTREE_DETAIL_COMPONENT_TYPE(__type, __ctype) \
    namespace creek{ namespace detail{\
    template<> \
    struct kdtree_component_type_traits< __type > \
    { typedef typename std::remove_cv<typename std::remove_reference< __ctype >::type>::type type; };}}

#define CREEK_KDTREE_DETAIL_SPECIALIZATION(__type) \
    namespace creek{ \
    template<> \
    struct kdtree_traits< __type > \
    { \
        typedef typename detail::kdtree_component_type_traits< __type >::type component_type; \
        inline static component_type const& access(__type const& _v, std::size_t _i) \
        { \
            return detail::kdtree_access_traits< __type >::get(_v, _i); \
        } \
    }; }

#define CREEK_KDTREE_SPECIALIZATION(__type, __access) \
    CREEK_KDTREE_DETAIL_ACCESS(decltype( __type (detail::kdtree_traits_dummy_type())), __access) \
    CREEK_KDTREE_DETAIL_COMPONENT_TYPE(decltype( __type (detail::kdtree_traits_dummy_type())), \
        decltype(kdtree_access_traits<decltype( __type (detail::kdtree_traits_dummy_type()))>::get(std::declval<decltype( __type (detail::kdtree_traits_dummy_type()))>(), std::size_t()))) \
    CREEK_KDTREE_DETAIL_SPECIALIZATION(decltype( __type (detail::kdtree_traits_dummy_type())))

namespace creek{
template<typename T, std::size_t N>
    struct kdtree_traits<std::array<T, N>>
    {
        typedef T component_type;
        
        inline static T access(std::array<T, N> const& _v, std::size_t i)
        {
            return _v[i];
        }
    };
}
#endif
