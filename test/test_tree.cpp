#define BOOST_TEST_MODULE tree
#include <boost/test/included/unit_test.hpp>
#include "tree.h"
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
using namespace creek;

BOOST_AUTO_TEST_CASE( append_child_and_insert )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    BOOST_CHECK_EQUAL( t1.size(), 9 );
    std::ofstream file1("tree.dot");
    output_dot(t1, file1);
    //FBADCEGIH
    tree<std::string>::pre_order_iterator ite2 = t1.pre_order_begin();
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    ++ite2;
    BOOST_CHECK( ite2 == t1.pre_order_end() );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "H" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "I" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "G" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "E" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "C" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "D" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "A" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "B" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "F" );
    
    
    tree<std::string>::pre_order_iterator ite3, j, k, l;
    ite3 = std::find(t1.pre_order_begin(), t1.pre_order_end(), "B");
    j = t1.insert(ite3, "J");
    ite3 = std::find(t1.pre_order_begin(), t1.pre_order_end(), "D");
    k = t1.insert(ite3, "K");
    ite3 = std::find(t1.pre_order_begin(), t1.pre_order_end(), "I");
    k = t1.insert(ite3, "L");
    
    BOOST_CHECK_EQUAL( t1.size(), 12 );
    
    //FJBAKDCEGLIH
    ite2 = t1.pre_order_begin();
    BOOST_CHECK_EQUAL( *ite2, "F" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "J" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "B" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "A" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "K" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "D" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "C" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "E" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "G" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "L" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "I" );
    ++ite2;
    BOOST_CHECK_EQUAL( *ite2, "H" );
    ++ite2;
    BOOST_CHECK( ite2 == t1.pre_order_end() );
    
    std::cout << "1" << std::endl;
}

BOOST_AUTO_TEST_CASE( construct_and_copy )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    BOOST_CHECK_EQUAL( t1.size(), 9 );
    
    tree<std::string> t2(t1), t3;
    BOOST_CHECK_EQUAL( t1.size(), t2.size() );
    tree<std::string>::pre_order_iterator itea = t1.pre_order_begin(), iteb = t2.pre_order_begin();
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    
    t3 = t1;
    BOOST_CHECK_EQUAL( t3.size(), t1.size() );
    itea = t1.pre_order_begin();
    iteb = t3.pre_order_begin();
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    BOOST_CHECK_EQUAL( *itea, *iteb );
    ++itea; ++iteb;
    
    tree<int> t4, t5;
    t4.insert(t4.pre_order_begin(), 999);
    t5 = t4;
    
    tree<int> t6, t7;
    //BOOST_CHECK(t6.post_order_begin() == t6.pre_order_begin());
    t6.insert(t6.post_order_begin(), 99999);
    t7 = t6;
    t7 = t7 = t7 = t6;
    #if 0
    BOOST_CHECK_THROW(
        ({
            for(std::size_t i = 0; i < 1000000; ++i)
            for(std::size_t j = 0; j < 1000000; ++j)
                t1.append_child(t1.pre_order_begin(), "X");
        }),
        std::exception
    )
    #endif
    std::cout << "2" << std::endl;
}


BOOST_AUTO_TEST_CASE( size )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    
    BOOST_CHECK_EQUAL( t1.size(), 0 );
    f = t1.insert(ite1, "F");
    BOOST_CHECK_EQUAL( t1.size(), 1 );
    b = t1.append_child(f, "B");
    BOOST_CHECK_EQUAL( t1.size(), 2 );
    g = t1.append_child(f, "G");
    BOOST_CHECK_EQUAL( t1.size(), 3 );
    a = t1.append_child(b, "A");
    BOOST_CHECK_EQUAL( t1.size(), 4 );
    d = t1.append_child(b, "D");
    BOOST_CHECK_EQUAL( t1.size(), 5 );
    c = t1.append_child(d, "C");
    BOOST_CHECK_EQUAL( t1.size(), 6 );
    e = t1.append_child(d, "E");
    BOOST_CHECK_EQUAL( t1.size(), 7 );
    i = t1.append_child(g, "I");
    BOOST_CHECK_EQUAL( t1.size(), 8 );
    h = t1.append_child(i, "H");
    
    BOOST_CHECK_EQUAL( t1.size(), 9 );
    
    t1.insert(t1.pre_order_begin(), "J");
    BOOST_CHECK_EQUAL( (*(t1.pre_order_begin())), "J" );
    
    BOOST_CHECK_EQUAL( t1.size(), 10 );
    
    t1.erase(c);
    BOOST_CHECK_EQUAL( t1.size(), 9 );
    t1.erase(e);
    BOOST_CHECK_EQUAL( t1.size(), 8 );
    
    tree<std::string> t2(t1), t3;
    BOOST_CHECK_EQUAL( t1.size(), t2.size() );
    std::cout << "3" << std::endl;
}

BOOST_AUTO_TEST_CASE( post_order_iterator )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    //ACEDBHIGF
    tree<std::string>::post_order_iterator ite2 = t1.post_order_begin();
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ite2++;
    BOOST_CHECK( ite2 == t1.post_order_end() );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "F" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "G" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "I" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "H" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "B" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "D" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "E" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "C" );
    ite2--;
    BOOST_CHECK_EQUAL( *ite2, "A" );
    BOOST_CHECK_EQUAL( (*ite2).size(), 1 );
    BOOST_CHECK_EQUAL( ite2->size(), 1 );
    BOOST_CHECK( child_order_begin(t1.pre_order_begin()) != child_order_end(t1.pre_order_begin()) );
    
    tree<std::string>::pre_order_iterator itmp = t1.pre_order_begin();
    ++itmp;
    BOOST_CHECK( parent(itmp) == t1.pre_order_begin() );
    
    tree<std::string>::reverse_child_order_iterator ite3 = child_order_rbegin(t1.pre_order_begin());
    tree<std::string>::reverse_child_order_iterator ite4 = child_order_rend(t1.pre_order_begin());
    
    BOOST_CHECK( &(*ite3) == &(*(--(  child_order_end(t1.pre_order_begin()) ))) );
    BOOST_CHECK( &(*(--ite4)) == &(*(  child_order_begin(t1.pre_order_begin())  )) );
    
    std::cout << "4" << std::endl;
}

BOOST_AUTO_TEST_CASE( child_order_iterator )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    ite1 = t1.pre_order_begin();
    tree<std::string>::child_order_iterator ch1 = child_order_begin(ite1);
    BOOST_CHECK_EQUAL( *ch1, "B");
    ++ch1;
    BOOST_CHECK_EQUAL( *ch1, "G");
    
    tree<std::string>::const_child_order_iterator ch2 = child_order_begin(ite1);
    ch2++;
    BOOST_CHECK(ch1 == ch2);
    BOOST_CHECK(ch2 == ch1);
    
    tree<std::string>::const_child_order_iterator ch3 = child_order_begin(t1.pre_order_begin()),
        end3 = child_order_end(t1.pre_order_begin());
    
    BOOST_CHECK_EQUAL( *ch3, "B" );
    ++ch3;
    BOOST_CHECK_EQUAL( *ch3, "G" );
    --ch3;
    BOOST_CHECK_EQUAL( *ch3, "B" );
    ++ch3;
    ch3--;
    BOOST_CHECK_EQUAL( *ch3, "B" );
    
    tree<std::string>::const_child_order_iterator ch4 = t1.append_child(ch3, "X");
    tree<std::string>::const_child_order_iterator ch5;
    BOOST_CHECK_EQUAL( *ch4, "X" );
    BOOST_CHECK_EQUAL( (*ch4).size(), 1 );
    BOOST_CHECK_EQUAL( ch4->size(), 1 );
    
    
    std::cout << "5" << std::endl;
}


struct Element
{
    static int construction_count;
    static int destruction_count;
    
    Element(){++construction_count;}
    Element(Element const& _other){++construction_count;}
    ~Element(){++destruction_count;}
};
int Element::construction_count = 0;
int Element::destruction_count = 0;

BOOST_AUTO_TEST_CASE( construction_and_destruction_count )
{
    {
        tree<Element> t1;
        tree<Element>::pre_order_iterator ite1 = t1.pre_order_begin();
        tree<Element>::pre_order_iterator a, b, c, d, e, f, g, h, i;
        f = t1.insert(ite1, Element());
        b = t1.append_child(f, Element());
        g = t1.append_child(f, Element());
        a = t1.append_child(b, Element());
        d = t1.append_child(b, Element());
        c = t1.append_child(d, Element());
        e = t1.append_child(d, Element());
        i = t1.append_child(g, Element());
        h = t1.append_child(i, Element());
    
        tree<Element> t2(t1), t3;
    
        t3 = t1;
        t2 = t3;
        t3 = t2;
        tree<Element>::pre_order_iterator ite2 = t3.pre_order_begin();
        ++ite2;
        ++ite2;
        ++ite2;
        t3.erase(ite2);
    }
    BOOST_CHECK_EQUAL( Element::construction_count, Element::destruction_count );
    
    
    std::cout << "6" << std::endl;
}

BOOST_AUTO_TEST_CASE( append_child_tree )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    tree<std::string> t2(t1);
    t2.append_child_tree(t2.post_order_begin(), t1);
    BOOST_CHECK_EQUAL( t2.size(), t1.size() * 2 );
    t2.append_child_tree(t2.post_order_begin(), t2);
    
    t1.append_child_tree(e, t1);
    tree<std::string>::const_pre_order_iterator ite2 = t1.pre_order_begin();
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    
    std::cout << "7" << std::endl;
}

BOOST_AUTO_TEST_CASE( post_order_reverse_iterator )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    //ACEDBHIGF
    tree<std::string>::reverse_post_order_iterator ite2 = t1.post_order_rbegin();
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ite2++;
    
    BOOST_CHECK( ite2 == t1.post_order_rend() );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "A" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "C" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "E" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "D" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "B" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "H" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "I" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "G" );
    ite2--;
    BOOST_CHECK_EQUAL( *ite2, "F" );
    BOOST_CHECK( child_order_begin(ite2) != child_order_end(ite2));
    
    tree<std::string> t2 = t1;
    
    t1.erase(ite2);
    BOOST_CHECK_EQUAL( t1.size(), 0 );
    
    t1.insert(t1.pre_order_begin(), "J");
    t1.append_child_tree(t1.post_order_rbegin(), t2);
    BOOST_CHECK_EQUAL( t1.size(), t2.size() + 1 );
    
    tree<std::string>::reverse_post_order_iterator hoge = t1.post_order_rbegin();
    tree<std::string>::child_order_iterator hige = child_order_begin(hoge);
    hige = child_order_end(hoge);
    hoge = t1.post_order_rbegin();
    tree<std::string>::child_order_iterator huga = child_order_begin(t1.post_order_rend());
    huga = child_order_end(t1.post_order_rbegin());
    
    
    std::cout << "8" << std::endl;
}

BOOST_AUTO_TEST_CASE( pre_order_reverse_iterator )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    //ACEDBHIGF
    tree<std::string>::reverse_pre_order_iterator ite2 = t1.pre_order_rbegin();
    BOOST_CHECK_EQUAL( "H", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    
    BOOST_CHECK( ite2 == t1.pre_order_rend() );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "F" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "B" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "A" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "D" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "C" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "E" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "G" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "I" );
    --ite2;
    BOOST_CHECK_EQUAL( *ite2, "H" );
    
    tree<std::string> t2 = t1;
    
    while(*ite2 != "F") ++ite2;
    
    t1.erase(ite2);
    BOOST_CHECK_EQUAL( t1.size(), 0 );
    
    t1.insert(t1.pre_order_begin(), "J");
    t1.append_child_tree(t1.pre_order_rbegin(), t2);
    BOOST_CHECK_EQUAL( t1.size(), t2.size() + 1 );
    
    
    std::cout << "9" << std::endl;
}

BOOST_AUTO_TEST_CASE( insert_child )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    t1.insert_child(f, b, "J");
    tree<std::string>::const_pre_order_iterator ite2 = t1.pre_order_begin();
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "J", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    
    std::cout << "10" << std::endl;
}

BOOST_AUTO_TEST_CASE( insert_child_tree )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    t1.insert_child_tree(f, g, t1.get_subtree(d));
    tree<std::string>::const_pre_order_iterator ite2 = t1.pre_order_begin();
    BOOST_CHECK_EQUAL( "F", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "G", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "I", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "H", *ite2 );
    std::cout << "11" << std::endl;
}

BOOST_AUTO_TEST_CASE( get_sub_tree )
{
    tree<std::string> t1;
    tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
    tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i;
    f = t1.insert(ite1, "F");
    b = t1.append_child(f, "B");
    g = t1.append_child(f, "G");
    a = t1.append_child(b, "A");
    d = t1.append_child(b, "D");
    c = t1.append_child(d, "C");
    e = t1.append_child(d, "E");
    i = t1.append_child(g, "I");
    h = t1.append_child(i, "H");
    
    tree<std::string> subtree = t1.get_subtree(b);
    BOOST_CHECK_EQUAL( subtree.size(), 5 );
    tree<std::string>::const_pre_order_iterator ite2 = subtree.pre_order_begin();
    BOOST_CHECK_EQUAL( "B", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "A", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "D", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "C", *ite2 );
    ++ite2;
    BOOST_CHECK_EQUAL( "E", *ite2 );
    std::cout << "12" << std::endl;
    
}

