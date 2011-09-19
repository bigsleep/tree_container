#define BOOST_TEST_MODULE kdtree
#include <boost/test/included/unit_test.hpp>
#include <boost/random.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <cstddef>

#include "kdtree.h"

BOOST_AUTO_TEST_CASE( test_1d )
{
    std::cout << "1" << std::endl;
    typedef std::array<double, 1> v1d;
    std::vector<v1d> elem = {
        v1d{{96.0}},
        v1d{{47.0}},
        v1d{{11.0}},
        v1d{{32.0}},
        v1d{{56.0}},
        v1d{{8.0}},
        v1d{{13.0}},
        v1d{{23.0}},
        v1d{{61.0}},
        v1d{{72.0}}};
    
    typedef creek::kdtree<std::array<double, 1>, 1> tree;
    tree t1(elem.begin(), elem.end());
    
    {
        auto it = t1.pre_order_begin();
        auto end = t1.pre_order_end();
        std::vector<v1d> expect = {
            v1d{{47.0}},
            v1d{{13.0}},
            v1d{{11.0}},
            v1d{{8.0}},
            v1d{{32.0}},
            v1d{{23.0}},
            v1d{{72.0}},
            v1d{{61.0}},
            v1d{{56.0}},
            v1d{{96.0}}};
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.post_order_begin();
        auto end = t1.post_order_end();
        std::vector<v1d> expect = {
            v1d{{8.0}},
            v1d{{11.0}},
            v1d{{23.0}},
            v1d{{32.0}},
            v1d{{13.0}},
            v1d{{56.0}},
            v1d{{61.0}},
            v1d{{96.0}},
            v1d{{72.0}},
            v1d{{47.0}}};
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.in_order_begin();
        auto end = t1.in_order_end();
        std::vector<v1d> expect = {
            v1d{{8.0}},
            v1d{{11.0}},
            v1d{{13.0}},
            v1d{{23.0}},
            v1d{{32.0}},
            v1d{{47.0}},
            v1d{{56.0}},
            v1d{{61.0}},
            v1d{{72.0}},
            v1d{{96.0}}};
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    // reverse
    {
        auto it = t1.pre_order_rbegin();
        auto end = t1.pre_order_rend();
        auto jt = t1.pre_order_begin();
        auto jend = t1.pre_order_end();
        std::vector<v1d> expect(jt, jend);
        std::reverse(expect.begin(), expect.end());
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.post_order_rbegin();
        auto end = t1.post_order_rend();
        auto jt = t1.post_order_begin();
        auto jend = t1.post_order_end();
        std::vector<v1d> expect(jt, jend);
        std::reverse(expect.begin(), expect.end());
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.in_order_rbegin();
        auto end = t1.in_order_rend();
        auto jt = t1.in_order_begin();
        auto jend = t1.in_order_end();
        std::vector<v1d> expect(jt, jend);
        std::reverse(expect.begin(), expect.end());
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
}

// traits
CREEK_KDTREE_SPECIALIZATION(double, );

BOOST_AUTO_TEST_CASE( test_1d_with_traits )
{
    std::cout << "2" << std::endl;
    std::vector<double> elem =
        {96.0, 47.0, 11.0, 32.0, 56.0, 8.0, 13.0, 23.0, 61.0, 72.0};
    
    typedef creek::kdtree<double, 1> tree;
    tree const t1(elem.begin(), elem.end());
    
    {
        auto it = t1.pre_order_begin();
        auto end = t1.pre_order_end();
        std::vector<double> expect =
            {47.0, 13.0, 11.0, 8.0, 32.0, 23.0, 72.0, 61.0, 56.0, 96.0};
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.post_order_begin();
        auto end = t1.post_order_end();
        std::vector<double> expect =
            {8.0, 11.0, 23.0, 32.0, 13.0, 56.0, 61.0, 96.0, 72.0, 47.0};
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.in_order_begin();
        auto end = t1.in_order_end();
        std::vector<double> expect = 
            {8.0, 11.0, 13.0, 23.0, 32.0, 47.0, 56.0, 61.0, 72.0, 96.0};
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    // reverse
    {
        auto it = t1.pre_order_rbegin();
        auto end = t1.pre_order_rend();
        auto jt = t1.pre_order_begin();
        auto jend = t1.pre_order_end();
        std::vector<double> expect(jt, jend);
        std::reverse(expect.begin(), expect.end());
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.post_order_rbegin();
        auto end = t1.post_order_rend();
        auto jt = t1.post_order_begin();
        auto jend = t1.post_order_end();
        std::vector<double> expect(jt, jend);
        std::reverse(expect.begin(), expect.end());
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
    {
        auto it = t1.in_order_rbegin();
        auto end = t1.in_order_rend();
        auto jt = t1.in_order_begin();
        auto jend = t1.in_order_end();
        std::vector<double> expect(jt, jend);
        std::reverse(expect.begin(), expect.end());
        
        BOOST_CHECK(std::equal(it, end, expect.begin()));
    }
}

BOOST_AUTO_TEST_CASE( test_1d_random )
{
    std::cout << "3" << std::endl;
    typedef std::array<double, 1> element;
    typedef creek::kdtree<element, 1> tree;
    std::size_t test_count = 100;
    
    boost::mt19937 engine(0);
    boost::uniform_int<> dst_size(0, 10000);
    boost::uniform_real<> dst_value(-1.0E+20, 1.0E+20);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand_size(engine, dst_size);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_real<>
    > rand_value(engine, dst_value);
    
    std::vector<element> storage;
    storage.reserve(100000);
    for(std::size_t i = 0; i < test_count; ++i){
        storage.clear();
        std::size_t size = rand_size();
        for(std::size_t j = 0; j < size; ++j){
            double value = rand_value();
            storage.push_back(element{{value}});
        }
        
        tree t1(storage.begin(), storage.end());
        std::sort(storage.begin(), storage.end());
        
        {
            auto it = t1.in_order_begin();
            auto end = t1.in_order_end();
            
            std::vector<element> v;
            v.reserve(size);
            while(it != end){
                v.push_back((*it));
                ++it;
            }
            BOOST_CHECK(v.size() == t1.size());
            BOOST_CHECK(v == storage);
        }
    }
}

BOOST_AUTO_TEST_CASE( copy_test )
{
    std::cout << "5" << std::endl;
    std::vector<double> elem =
        {96.0, 47.0, 11.0, 32.0, 56.0, 8.0, 13.0, 23.0, 61.0, 72.0};
    
    typedef creek::kdtree<double, 1> tree;
    tree t1(elem.begin(), elem.end());
    
    tree t2 = t1;
    
    BOOST_CHECK(std::equal(t1.pre_order_begin(), t1.pre_order_end(), t2.pre_order_begin()));
}

struct my_point
{
    int a[2];
    static std::size_t count;
    
    my_point()
        : a()
    {
        ++count;
    }
    my_point(my_point const& _other)
        : a()
    {
        a[0] = _other.a[0];
        a[1] = _other.a[1];
        ++count;
    }
    
    ~my_point()
    {
        --count;
    }
    
    bool operator==(my_point const& _b) const
    {
        return (a[0] == _b.a[0] && a[1] == _b.a[1]);
    }
    
    bool operator!=(my_point const& _b) const
    {
        return !(*this == _b);
    }
};

std::size_t my_point::count = 0;

CREEK_KDTREE_SPECIALIZATION(my_point, .a[i]);

BOOST_AUTO_TEST_CASE( leak )
{
    std::cout << "6" << std::endl;
    {
        std::vector<my_point> elem;
        
        boost::mt19937 engine(0);
        boost::uniform_int<> dst_size(0, 10000);
        boost::uniform_real<> dst_value(-1.0E+20, 1.0E+20);
        boost::variate_generator<
            boost::mt19937&, boost::uniform_int<>
        > rand_size(engine, dst_size);
        boost::variate_generator<
            boost::mt19937&, boost::uniform_real<>
        > rand_value(engine, dst_value);
        
        for(std::size_t i = 0; i < 1000; ++i){
            my_point p;
            p.a[0] = rand();
            p.a[1] = rand();
            elem.push_back(p);
        }
        BOOST_CHECK(my_point::count == elem.size());
        
        creek::kdtree<my_point, 2> t1(elem.begin(), elem.end());
        creek::kdtree<my_point, 2> t2 = t1;
        BOOST_CHECK(std::equal(t1.pre_order_begin(), t1.pre_order_end(), t2.pre_order_begin()));
    }
    BOOST_CHECK(my_point::count == 0);
}

bool compare(std::array<int, 2> const& _a, std::array<int, 2> const& _b)
{
    if(_a[0] < _b[0]) return true;
    else if(_a[0] == _b[0]) return (_a[1] < _b[1]);
    else return false;
}

BOOST_AUTO_TEST_CASE( apply_within_range )
{
    std::cout << "7" << std::endl;
    std::vector<std::array<int, 2>> elem;
    
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-1000, 1000);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst);
    
    elem.reserve(10000);
    for(std::size_t i = 0; i < 10000; ++i){
        int x = rand(), y = rand();
        elem.push_back(std::array<int, 2>{{x, y}});
    }
    
    std::array<int, 2> low_bound = {{100, -200}};
    std::array<int, 2> up_bound = {{300, -100}};
    
    namespace ph = std::placeholders;
    std::vector<std::array<int, 2>> v;
    typedef void (std::vector<std::array<int, 2>>::*push_back_type)(std::array<int, 2> const&);
    push_back_type pb = &std::vector<std::array<int, 2>>::push_back;
    std::function<void(std::array<int, 2>const&)>
        pick = std::bind(pb, std::ref(v), ph::_1);
    
    creek::kdtree<std::array<int, 2>, 2> t1(elem.begin(), elem.end());
    
    t1.apply_within_range(low_bound, up_bound, pick);
    
    std::vector<std::array<int, 2>> expected;
    for(std::size_t i = 0, sz = elem.size(); i < sz; ++i){
        if(low_bound[0] <=  elem[i][0] &&
           elem[i][0]   <   up_bound[0] &&
           low_bound[1] <=  elem[i][1] &&
           elem[i][1]   <   up_bound[1]) expected.push_back(elem[i]);
    }
    
    std::cout << v.size() << " " << expected.size() << std::endl;
    std::sort(v.begin(), v.end(), &compare);
    std::sort(expected.begin(), expected.end(), &compare);
    BOOST_CHECK(v == expected);
}

BOOST_AUTO_TEST_CASE( optimise )
{
    std::cout << "8" << std::endl;
    std::vector<std::array<int, 2>> elem;
    
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-1000, 1000);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst);
    
    creek::kdtree<std::array<int, 2>, 2> t1;
    elem.reserve(100);
    for(std::size_t i = 0; i < 100; ++i){
        int x = rand(), y = rand();
        auto a = std::array<int, 2>{{x, y}};
        elem.push_back(a);
        t1.insert(a);
    }
    
    creek::kdtree<std::array<int, 2>, 2> t2(elem.begin(), elem.end());
    t1.optimise();
    
    BOOST_CHECK(t1 == t2);
}

struct myarray
{
    int a[2];
};

int myaccess(myarray const& a, std::size_t i)
{
    return a.a[i];
}

BOOST_AUTO_TEST_CASE( without_traits )
{
    std::cout << "9" << std::endl;
    std::vector<myarray> elem;
    std::vector<std::array<int, 2>> elem2;
    
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-1000, 1000);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst);
    
    elem.reserve(100);    
    elem2.reserve(100);
    for(std::size_t i = 0; i < 100; ++i){
        int x = rand(), y = rand();
        auto a = myarray{{x, y}};
        auto b = std::array<int, 2>{{x, y}};
        elem.push_back(a);
        elem2.push_back(b);
    }
    
    typedef creek::kdtree<myarray, 2, std::function<int(myarray const&, std::size_t)>,
        creek::kdtree_default, int> tree_type;
    tree_type t1(elem.begin(), elem.end(), &myaccess);
    creek::kdtree<std::array<int, 2>, 2> t2(elem2.begin(), elem2.end());
    
    BOOST_CHECK(t1.size() == t2.size());
    
    auto it = t1.pre_order_begin();
    auto end = t1.pre_order_end();
    auto jt = t2.pre_order_begin();
    bool check = true;
    while(it != end){
        check &= (*it).a[0] == (*jt)[0];
        check &= (*it).a[1] == (*jt)[1];
        ++it;
        ++jt;
    }
    BOOST_CHECK(check);
}


BOOST_AUTO_TEST_CASE( with_predicate )
{
    std::vector<std::array<int, 2>> elem;
    
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-1000, 1000);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst);
    
    elem.reserve(100);
    for(std::size_t i = 0; i < 100; ++i){
        int x = rand(), y = rand();
        auto a = std::array<int, 2>{{x, y}};
        elem.push_back(a);
    }
    auto cp = elem;
    
    typedef creek::kdtree<std::array<int, 2>, 2,
        creek::kdtree_default, std::function<bool(int, int)>, int> tree_type;
    tree_type t1(elem.begin(), elem.end(), creek::kdtree_default(), std::greater<int>());    
    creek::kdtree<std::array<int, 2>, 2> t2(cp.begin(), cp.end());
}


struct val
{
    int a;
};

bool operator==(val _a, int _b)
{
    return _a.a == _b;
}

bool operator==(int _b, val _a)
{
    return _a.a == _b;
}

CREEK_KDTREE_SPECIALIZATION(int, );

BOOST_AUTO_TEST_CASE( with_out_traits_with_predicate )
{
    std::cout << "10" << std::endl;
    std::vector<val> elem;
    std::vector<int> elem2;
    
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-1000, 1000);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst);
    
    elem.reserve(100);
    elem2.reserve(100);
    for(std::size_t i = 0; i < 100; ++i){
        int x = rand();
        elem.push_back(val{x});
        elem2.push_back(x);
    }
    
    namespace ph = std::placeholders;
    typedef creek::kdtree<val, 1, std::function<int(val const&, std::size_t)>, std::function<bool(int const&,int const&)>, int> tree_type;
    tree_type t1(elem.begin(), elem.end(), std::bind(&val::a, ph::_1), std::greater<int>());
    creek::kdtree<int, 1> t2(elem2.begin(), elem2.end());
    
    BOOST_CHECK(t1.size() == t2.size());
    
    auto it = t1.in_order_begin();
    auto end = t1.in_order_end();
    auto jt = t2.in_order_rbegin();
    BOOST_CHECK(std::equal(it, end, jt));
}


