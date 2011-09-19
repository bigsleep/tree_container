#define BOOST_TEST_MODULE tree
#include <boost/test/included/unit_test.hpp>
#include <boost/random.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <cstddef>
#include <functional>

#include "rbtree.h"

using namespace creek;

BOOST_AUTO_TEST_CASE( construct_copy )
{
    rbtree<int> t1;
    t1.insert(-120);
    t1.insert(2);
    t1.insert(54);
    t1.insert(52);
    t1.insert(-333);
    t1.insert(92);
    t1.insert(4);
    t1.insert(12);
    t1.insert(70);
    t1.insert(-102);
    t1.insert(54);
    t1.insert(6);
    
    BOOST_CHECK(t1.size() == 12);
    BOOST_CHECK(std::is_sorted(t1.in_order_begin(), t1.in_order_end()));
    
    rbtree<int> t2 = t1;
    
    BOOST_CHECK(t1 == t2);
    BOOST_CHECK(t1.size() == t2.size());
    BOOST_CHECK(std::equal(t1.pre_order_begin(), t1.pre_order_end(), t2.pre_order_begin()));
    BOOST_CHECK(std::equal(t1.in_order_begin(), t1.in_order_end(), t2.in_order_begin()));
    BOOST_CHECK(std::equal(t1.post_order_begin(), t1.post_order_end(), t2.post_order_begin()));
}

BOOST_AUTO_TEST_CASE( insert )
{
    using namespace creek;
    creek::rbtree<int> t1;
    std::size_t size = 1000;
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-100000, 100000), dst2(0, size);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst), rand2(engine, dst2);
    
    for(std::size_t i = 0; i < size; ++i){
        t1.insert(rand());
        BOOST_CHECK(t1.check_invariant());
        BOOST_CHECK(std::is_sorted(t1.in_order_begin(), t1.in_order_end()));
    }
}

BOOST_AUTO_TEST_CASE( erase )
{
    using namespace creek;
    creek::rbtree<int> t1;
    std::size_t size = 10000;
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-1000000, 1000000);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst);
    
    for(std::size_t i = 0; i < size; ++i){
        t1.insert(rand());
    }
    
    bool result = true;
    for(std::size_t i = 0; i < 100; ++i){
        auto it = t1.in_order_begin();
        boost::uniform_int<> dst2(0, t1.size() - 1);
        boost::variate_generator<
            boost::mt19937&, boost::uniform_int<>
        > rand2(engine, dst2);
        for(std::size_t j = 0, sz = rand2(); j < sz; ++j){
            ++it;
        }
        t1.erase(it);
        
        result &= t1.check_invariant();
    }
    BOOST_CHECK(result);
}


BOOST_AUTO_TEST_CASE( predicate )
{
    using namespace creek;
    auto t1 = creek::rbtree<int, std::function<bool(int,int)>>(std::greater<int>());
    std::size_t size = 1000;
    boost::mt19937 engine(0);
    boost::uniform_int<> dst(-100000, 100000), dst2(0, size);
    boost::variate_generator<
        boost::mt19937&, boost::uniform_int<>
    > rand(engine, dst), rand2(engine, dst2);
    
    for(std::size_t i = 0; i < size; ++i){
        t1.insert(rand());
        BOOST_CHECK(t1.check_invariant());
        BOOST_CHECK(std::is_sorted(t1.in_order_rbegin(), t1.in_order_rend()));
    }
}


