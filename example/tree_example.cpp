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
	tree<std::string>::preorder_iterator ite1 = t1.preorder_begin();
	tree<std::string>::preorder_iterator a, b, c, d, e, f, g, h, i, j;
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

	std::cout << "preorder traversal" << std::endl;
	tree<std::string>::const_preorder_iterator pre = t1.preorder_begin(),
		end_pre = t1.preorder_end();
	
	while(pre != end_pre){
		std::cout << *pre << std::endl;
		++pre;
	}
	
	std::cout << "postorder traversal" << std::endl;
	tree<std::string>::const_postorder_iterator post = t1.postorder_begin(),
		end_post = t1.postorder_end();
	
	while(post != end_post){
		std::cout << *post << std::endl;
		++post;
	}
#if 0	
	std::ofstream out1("tree_example.dot");
	creek::output_dot(t1, out1);
#endif
}

