#define BOOST_TEST_MODULE tree
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <iostream>
#include <cstddef>
#include "nary_tree.h"

BOOST_AUTO_TEST_CASE( construction )
{
    using namespace creek;
    nary_tree<4, int> tree1(3, 10);
    
    BOOST_CHECK_EQUAL( tree1.size(), ((4 * 4 * 4 - 1) / (4 - 1)) );
    {
        nary_tree<4, int>::iterator it = tree1.begin(), end = tree1.end();
        
        BOOST_CHECK(it != end);
        BOOST_CHECK_EQUAL(std::size_t(end - it), tree1.size());
        
        for(std::size_t i = 0; i < tree1.size(); ++i){
            (*it) = i;
            ++it;
        }
    }
    {
        nary_tree<4, int>::iterator it = tree1.begin(), end = tree1.end();
        
        for(std::size_t i = 0; i < tree1.size(); ++i){
            std::cout << (*it) << ": ";
            auto chit = creek::begin(it), chend = creek::end(it);
            std::cout << "children(";
            while(chit != chend){
                std::cout << *chit << " ";
                ++chit;
            }
            std::cout << ") parent(";
            if(parent(it) != nary_tree<4, int>::iterator())
                std::cout << (*(parent(it))) << ")" << std::endl;
            else
                std::cout << "root)" << std::endl;
            ++it;
        }
    }
    {
        typedef nary_tree<4, int>::iterator iterator;
        iterator it = tree1.begin(), end = tree1.end();
        
        for(std::size_t i = 0; i < tree1.size(); ++i){
            iterator chit = creek::begin(it), chend = creek::end(it);
            while(chit != chend){
                BOOST_CHECK(it == parent(chit));
                BOOST_CHECK((*it) == (*(parent(chit))));
                ++chit;
            }
            ++it;
        }
    }
}



BOOST_AUTO_TEST_CASE( copy_construction )
{
    using namespace creek;
    nary_tree<6, int> tree1(4, 0);
    {
        nary_tree<6, int>::iterator it = tree1.begin(), end = tree1.end();
        for(std::size_t i = 0; i < tree1.size(); ++i){
            (*it) = i;
            ++it;
        }
    }
    
    nary_tree<6, int> tcopy(tree1);
    BOOST_CHECK_EQUAL(tree1.size(), tcopy.size());
    BOOST_CHECK_EQUAL(tree1.level(), tcopy.level());
    {
        typedef nary_tree<6, int>::iterator iterator;
        iterator it = tcopy.begin(), end = tcopy.end();
        
        for(std::size_t i = 0; i < tree1.size(); ++i){
            iterator chit = creek::begin(it), chend = creek::end(it);
            while(chit != chend){
                BOOST_CHECK(it == parent(chit));
                BOOST_CHECK((*it) == (*(parent(chit))));
                ++chit;
            }
            ++it;
        }
    }
    {
        typedef nary_tree<6, int>::iterator iterator;
        iterator it = tcopy.begin(), end = tcopy.end(), itorg = tree1.begin();
        
        while(it != end){
            BOOST_CHECK_EQUAL(*it, *itorg);
            BOOST_CHECK_EQUAL(level(it), level(itorg));
            ++it;
            ++itorg;
        }
    }
}

BOOST_AUTO_TEST_CASE( copy )
{
    using namespace creek;
    nary_tree<8, double> tree1(6, 0.8);
    {
        nary_tree<8, double>::iterator it = tree1.begin(), end = tree1.end();
        for(std::size_t i = 0; i < tree1.size(); ++i){
            (*it) = i * 0.1;
            ++it;
        }
    }
    
    nary_tree<8, double> tcopy;
    
    tcopy = tree1;
    BOOST_CHECK_EQUAL(tree1.size(), tcopy.size());
    BOOST_CHECK_EQUAL(tree1.level(), tcopy.level());
    {
        typedef nary_tree<8, double>::iterator iterator;
        iterator it = tcopy.begin(), end = tcopy.end();
        
        for(std::size_t i = 0; i < tree1.size(); ++i){
            iterator chit = creek::begin(it), chend = creek::end(it);
            while(chit != chend){
                BOOST_CHECK(it == parent(chit));
                BOOST_CHECK((*it) == (*(parent(chit))));
                ++chit;
            }
            ++it;
        }
    }
    {
        typedef nary_tree<8, double>::iterator iterator;
        iterator it = tcopy.begin(), end = tcopy.end(), itorg = tree1.begin();
        
        while(it != end){
            BOOST_CHECK_EQUAL(*it, *itorg);
            BOOST_CHECK_EQUAL(level(it), level(itorg));
            ++it;
            ++itorg;
        }
    }
    {
        std::size_t size = tree1.size(), out_of_range = size + 10;
        BOOST_CHECK_THROW(tree1.at(size), std::out_of_range);
        BOOST_CHECK_THROW(tree1.at(out_of_range), std::out_of_range);
        BOOST_CHECK_THROW((tree1 = nary_tree<8, double>(1000000, 0)), std::length_error);
        BOOST_CHECK_THROW((nary_tree<100, int>(1000000, 0)), std::length_error);
    }
}

BOOST_AUTO_TEST_CASE( element_access )
{
    using namespace creek;
    nary_tree<2, std::size_t> tree1(3, 0);
    
    {
        nary_tree<2, std::size_t>::iterator it = tree1.begin(), end = tree1.end();
        
        for(std::size_t i = 0; i < tree1.size(); ++i){
            (*it) = i;
            ++it;
        }
    }
    {
        nary_tree<2, std::size_t>::iterator it = tree1.begin(), end = tree1.end();
        for(std::size_t i = 0; i < tree1.size(); ++i){
            BOOST_CHECK_EQUAL(tree1[i], i);
            ++it;
        }
    }
    {
        std::size_t size = tree1.size(), out_of_range = size + 10;
        BOOST_CHECK_THROW(tree1.at(size), std::out_of_range);
        BOOST_CHECK_THROW(tree1.at(out_of_range), std::out_of_range);
    }
    
}

BOOST_AUTO_TEST_CASE( iterator )
{
    using namespace creek;
    nary_tree<3, std::string> tree1(3, "hello");
    typedef nary_tree<3, std::string>::iterator iterator;
    typedef nary_tree<3, std::string>::const_iterator const_iterator;
    {
        iterator it = tree1.begin();
        for(std::size_t i = 0; i < tree1.size(); ++i){
            (*it) = i;
            ++it;
        }
    }
    
    // copy
    {
        iterator it1 = tree1.begin();
        const_iterator it2 = it1;
        iterator it3(it1);
        // iterator it4 = it2; cannot convert const iterator to iterator
    }
    
    // comparison
    {
        iterator it1 = tree1.begin();
        const_iterator it2 = it1;
        iterator it3(it1), it4 = tree1.end();
        const_iterator it5 = it4;
        
        // ==
        BOOST_CHECK(it1 == it3); // unconst  unconst
        BOOST_CHECK(it1 == it2); // unconst  const
        BOOST_CHECK(it2 == it1); // const    unconst
        // !=
        BOOST_CHECK(it1 != it4); // unconst  unconst
        BOOST_CHECK(it1 != it5); // unconst  const
        BOOST_CHECK(it5 != it1); // const    unconst
        // <
        BOOST_CHECK(it1 < it4);    // unconst  unconst
        BOOST_CHECK(it1 < it5);    // unconst  const
        BOOST_CHECK(!(it5 < it1)); // const    unconst
        // <=
        BOOST_CHECK(it1 <= it4);    // unconst  unconst
        BOOST_CHECK(it1 <= it5);    // unconst  const
        BOOST_CHECK(!(it5 <= it1)); // const    unconst
        // >
        BOOST_CHECK(!(it1 > it4)); // unconst  unconst
        BOOST_CHECK(!(it1 > it5)); // unconst  const
        BOOST_CHECK(it5 > it1);    // const    unconst
        // >=
        BOOST_CHECK(!(it1 >= it4)); // unconst  unconst
        BOOST_CHECK(!(it1 >= it5)); // unconst  const
        BOOST_CHECK(it5 >= it1);    // const    unconst
        
    }
    
    // operators
    {
        iterator it1 = tree1.begin();
        const_iterator it2 = it1;
        iterator it3(it1), it4 = tree1.end();
        const_iterator it5 = it4;
        
        // iterator defference
        BOOST_CHECK_EQUAL(std::size_t(it4 - it1), tree1.size());
        BOOST_CHECK_EQUAL(std::size_t(it4 - it2), tree1.size());
        BOOST_CHECK_EQUAL(std::size_t(it5 - it1), tree1.size());
        BOOST_CHECK_EQUAL(std::size_t(it5 - it2), tree1.size());
        
        // iterator + size_t
        it1 = it1 + 3;
        ++it2; ++it2; ++it2;
        BOOST_CHECK(it1 == it2);
        
        // ++it, it++
        
        // +=
        
        // --it, it--
        
        // -=
        
        // operator[]
        
        // operator*
        
        // operator->
    }
}

