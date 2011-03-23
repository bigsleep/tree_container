//-----------------------------------------------------------
//    tree.cpp
//-----------------------------------------------------------
#include <vector>
#include <algorithm>
#include "tree.h"

namespace creek
{
    static tree_node_base::iterator const empty = tree_node_base::iterator();
    //-----------------------------------------------------------
    //    tree_traverse_increment<preorder_tag>
    //-----------------------------------------------------------
    template<>
    tree_node_base::iterator tree_traverse_increment<preorder_tag>(tree_node_base::iterator node0)
    {
        assert((*node0));
        if(!((*node0)->m_children.empty())){
            return (*node0)->m_children.begin();
        }else if((*node0)->m_parent != empty || (*node0)->m_younger_sibling != empty){
            tree_node_base::iterator cur_node = node0;
            while(cur_node != empty){
                if((*cur_node)->m_younger_sibling != empty)
                    return (*cur_node)->m_younger_sibling;
                cur_node = (*cur_node)->m_parent;
            }
            return node0;
        }else{
            return node0;
        }
    }
    
    template<>
    tree_node_base::const_iterator tree_traverse_increment<preorder_tag>(tree_node_base::const_iterator node0)
    {
        assert((*node0));
        if(!(*node0)->m_children.empty()){
            return (*node0)->m_children.begin();
        }else if((*node0)->m_parent != empty || (*node0)->m_younger_sibling != empty){
            tree_node_base::const_iterator cur_node = node0;
            while(cur_node != empty){
                if((*cur_node)->m_younger_sibling != empty){
                    return (*cur_node)->m_younger_sibling;
                }
                cur_node = (*cur_node)->m_parent;
            }
            return node0;
        }else{
            return node0;
        }
    }
    
    //-----------------------------------------------------------
    //    tree_traverse_decrement<preorder_tag>
    //-----------------------------------------------------------
    template<>
    tree_node_base::iterator tree_traverse_decrement<preorder_tag>(tree_node_base::iterator node0)
    {
        assert((*node0));
        if((*node0)->m_older_sibling != empty){
            tree_node_base::iterator cur_node = (*node0)->m_older_sibling;
            while(!(*cur_node)->m_children.empty())
                cur_node = --((*cur_node)->m_children.end());
            return cur_node;
        }else if((*node0)->m_parent != empty){
            return (*node0)->m_parent;
        }else{
            return node0;
        }
    }
    
    template<>
    tree_node_base::const_iterator tree_traverse_decrement<preorder_tag>(tree_node_base::const_iterator node0)
    {
        assert((*node0));
        if((*node0)->m_older_sibling != empty){
            tree_node_base::const_iterator cur_node = (*node0)->m_older_sibling;
            while(!(*cur_node)->m_children.empty())
                cur_node = (--((*cur_node)->m_children.end()));
            return cur_node;
        }else if((*node0)->m_parent != empty){
            return (*node0)->m_parent;
        }else{
            return node0;
        }
    }
    
    
    //-----------------------------------------------------------
    //    tree_traverse_increment<postorder_tag>
    //-----------------------------------------------------------
    template<>
    tree_node_base::iterator tree_traverse_increment<postorder_tag>(tree_node_base::iterator node0)
    {
        assert((*node0));
        if((*node0)->m_younger_sibling != empty){
            tree_node_base::iterator cur_node = (*node0)->m_younger_sibling;
            while(!(*cur_node)->m_children.empty())
                cur_node = (*cur_node)->m_children.begin();
            return cur_node;
        }else if((*node0)->m_parent != empty){
            return (*node0)->m_parent;
        }else{
            return node0;
        }
    }
    
    template<>
    tree_node_base::const_iterator tree_traverse_increment<postorder_tag>(tree_node_base::const_iterator node0)
    {
        assert((*node0));
        if((*node0)->m_younger_sibling != empty){
            tree_node_base::const_iterator cur_node = (*node0)->m_younger_sibling;
            while(!(*cur_node)->m_children.empty())
                cur_node = (*cur_node)->m_children.begin();
            return cur_node;
        }else if((*node0)->m_parent != empty){
            return (*node0)->m_parent;
        }else{
            return node0;
        }
    }
    
    //-----------------------------------------------------------
    //    tree_traverse_decrement<postorder_tag>
    //-----------------------------------------------------------
    template<>
    tree_node_base::iterator tree_traverse_decrement<postorder_tag>(tree_node_base::iterator node0)
    {
        assert((*node0));
        if(!(*node0)->m_children.empty()){
            return (--((*node0)->m_children.end()));
        }else{
            tree_node_base::iterator cur_node = node0;
            while(cur_node != empty){
                if((*cur_node)->m_older_sibling != empty)
                    return (*cur_node)->m_older_sibling;
                cur_node = (*cur_node)->m_parent;
            }
            return node0;
        }
    }
    
    template<>
    tree_node_base::const_iterator tree_traverse_decrement<postorder_tag>(tree_node_base::const_iterator node0)
    {
        assert((*node0));
        if(!(*node0)->m_children.empty()){
            return (--((*node0)->m_children.end()));
        }else{
            tree_node_base::const_iterator cur_node = node0;
            while(cur_node != empty){
                if((*cur_node)->m_older_sibling != empty)
                    return (*cur_node)->m_older_sibling;
                cur_node = (*cur_node)->m_parent;
            }
            return node0;
        }
    }
    
    //-----------------------------------------------------------
    //    tree_traverse_increment<child_tag>
    //-----------------------------------------------------------
    template<>
    tree_node_base::iterator tree_traverse_increment<child_tag>(tree_node_base::iterator node0)
    {
        assert((*node0));
        return (++node0);
    }
    
    template<>
    tree_node_base::const_iterator tree_traverse_increment<child_tag>(tree_node_base::const_iterator node0)
    {
        assert((*node0));
        return (++node0);
        
    }
    
    //-----------------------------------------------------------
    //    tree_traverse_decrement<childe_tag>
    //-----------------------------------------------------------
    template<>
    tree_node_base::iterator tree_traverse_decrement<child_tag>(tree_node_base::iterator node0)
    {
        assert((*node0));
        return (--node0);
    }
    
    template<>
    tree_node_base::const_iterator tree_traverse_decrement<child_tag>(tree_node_base::const_iterator node0)
    {
        assert((*node0));
        return (--node0);
    }
}//---- namespace creek



