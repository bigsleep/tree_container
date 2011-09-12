//

#include <iostream>
#include <string>
#include "tree.h"
#include <fstream>
#include <cstddef>

int main()
{
	using namespace creek;
	tree<std::string> t1;
	tree<std::string>::pre_order_iterator ite1 = t1.pre_order_begin();
	tree<std::string>::pre_order_iterator a, b, c, d, e, f, g, h, i, j;
	f = t1.insert(ite1, "F");
	b = t1.append_child(f, "B");
	g = t1.append_child(f, "G");
	a = t1.append_child(b, "A");
	d = t1.append_child(b, "D");
	c = t1.append_child(d, "C");
	e = t1.append_child(d, "E");
	i = t1.append_child(g, "I");
	h = t1.append_child(i, "H");
	j = t1.insert(g, "J");
	
	std::cout << t1.size() << std::endl;

	std::cout << "pre order traversal" << std::endl;
	tree<std::string>::const_pre_order_iterator pre = t1.pre_order_begin(),
		end_pre = t1.pre_order_end();
	
	while(pre != end_pre){
		std::cout << *pre << std::endl;
		
		std::cout << "child: ";
		auto ch = child_order_begin(pre);
		auto chend = child_order_end(pre);
		while(ch != chend)
		{
		    std::cout << *ch << " ";
		    ++ch;
		}
		std::cout << std::endl;
		auto rch = child_order_rbegin(pre);
		auto rchend = child_order_rend(pre);
		std::cout << "r child: ";
		while(rch != rchend)
		{
		    std::cout << *rch << " ";
		    ++rch;
		}
		std::cout << std::endl;
		++pre;
	}
	std::ofstream out1("tree_example.dot");
	creek::output_dot(t1, out1);
}

