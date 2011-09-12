#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include "tree.h"


std::string GetToken(std::string::const_iterator& _it, std::string::const_iterator _end);
bool CheckTree(creek::tree<std::string> const& _tree);

bool Parse(std::string const& _strexpr,
    creek::tree<std::string>& _tree)
{
    if(_strexpr == "") return false;
    
    typedef creek::tree<std::string>::pre_order_iterator pre_order_iterator;
    creek::tree<std::string> treetmp;
    pre_order_iterator curr = treetmp.pre_order_begin();
    
    std::string::const_iterator it_str = _strexpr.begin(), it_end = _strexpr.end();
    int rbracket_count = 0, lbracket_count = 0;
    
    while(1){
        std::string token = GetToken(it_str, it_end);
        if(token == "") break;
        
        if(token == "("){
            ++rbracket_count;
            token = GetToken(it_str, it_end);
            
            if(token == "(" || token == ")" || token == ""){
                return false;
            }
            
            if(treetmp.empty())
                curr = treetmp.insert(curr, token);
            else
                curr = treetmp.append_child(curr, token);
        }else if(token == ")"){
            if(treetmp.empty()){
                return false;
            }
            ++lbracket_count;
            curr = creek::parent(curr);
        }else{
            if(treetmp.empty()){
                return false;
            }
            treetmp.append_child(curr, token);
        }
        
    }
    
    if(rbracket_count != lbracket_count){
        return false;
    }
    
    if(!CheckTree(treetmp)){
        return false;
    }
    _tree = treetmp;
    return true;
}

std::string GetToken(std::string::const_iterator& _it, std::string::const_iterator _end)
{
    while(1){
        char c = *_it;
        if(_it == _end) return std::string();
        if(c != ' ' && c != '\t' && c != '\r' && c != '\n')
            break;
        else
            ++_it;
    }
    
    if((*_it) == '('){
        ++_it;
        return std::string("(");
    }
    if((*_it) == ')'){
        ++_it;
        return std::string(")");
    }
    
    std::string tmp;
    tmp.push_back(*_it);
    ++_it;
    while(1){
        char c = *_it;
        if(_it == _end) return tmp;
        if(c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != '(' && c != ')'){
            tmp.push_back(c);
            ++_it;
        }else return tmp;
    }
}


bool is_operator(std::string const& _token)
{
    if(_token == "+" || _token == "-" || _token == "*" || _token == "/")
        return true;
    else return false;
}

bool is_number(std::string const& _token)
{
    try{
        auto num = std::stod(_token);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool CheckTree(creek::tree<std::string> const& _tree)
{
    typedef creek::tree<std::string>::const_pre_order_iterator const_pre_order_iterator;
    const_pre_order_iterator it = _tree.pre_order_begin();
    const_pre_order_iterator end = _tree.pre_order_end();
    
    while(it != end){
        if((creek::child_order_begin(it) != creek::child_order_end(it)) && !is_operator(*it)) return false;
        if((creek::child_order_begin(it) == creek::child_order_end(it)) && !is_number(*it)) return false;
        ++it;
    }
    return true;
}

double Calculate(creek::tree<std::string> const& _tree)
{
    typedef creek::tree<std::string>::const_pre_order_iterator const_pre_order_iterator;
    typedef creek::tree<std::string>::const_child_order_iterator const_child_order_iterator;
    typedef creek::tree<double>::pre_order_iterator pre_order_iterator;
    typedef creek::tree<std::string>::const_reverse_pre_order_iterator const_reverse_pre_order_iterator;
    typedef creek::tree<double>::reverse_pre_order_iterator reverse_pre_order_iterator;
        
    creek::tree<double> numtree;
    {
        const_pre_order_iterator it = _tree.pre_order_begin();
        const_pre_order_iterator end = _tree.pre_order_end();
        pre_order_iterator jt = numtree.insert(numtree.pre_order_begin(), 0.0);
        while(it != end){
            const_child_order_iterator cit = creek::child_order_begin(it);
            const_child_order_iterator cend = creek::child_order_end(it);
            while(cit != cend){
                numtree.append_child(jt, 0.0);
                ++cit;
            }
            ++it;
            ++jt;
        }
    }
    
    {
        const_reverse_pre_order_iterator it = _tree.pre_order_rbegin();
        const_reverse_pre_order_iterator end = _tree.pre_order_rend();
        reverse_pre_order_iterator jt = numtree.pre_order_rbegin();
        while(it != end){
            if(creek::child_order_begin(it) == creek::child_order_end(it)){
                *jt = std::stod(*it);
            }else{
                char op = (*it)[0];
                switch(op){
                    case '+':
                    {
                        double tmp = std::accumulate(creek::child_order_begin(jt), creek::child_order_end(jt), 0.0, std::plus<double>());
                        *jt = tmp;
                        break;
                    }
                    case '-':
                    {
                        std::size_t size = std::distance(creek::child_order_begin(jt), creek::child_order_end(jt));
                        if(size == 1){
                            *jt = - (*(creek::child_order_begin(jt)));
                        }else{
                            double tmp = std::accumulate(++(creek::child_order_begin(jt)), creek::child_order_end(jt), *(creek::child_order_begin(jt)), std::minus<double>());
                            *jt = tmp;
                        }
                        break;
                    }
                    case '*':
                    {
                        double tmp = std::accumulate(creek::child_order_begin(jt), creek::child_order_end(jt), 1.0, std::multiplies<double>());;
                        *jt = tmp;
                        break;
                    }
                    case '/':
                    {
                        std::size_t size = std::distance(creek::child_order_begin(jt), creek::child_order_end(jt));
                        if(size == 1){
                            *jt = 1.0 / (*(creek::child_order_begin(jt)));
                        }else{
                            double tmp = std::accumulate(++(creek::child_order_begin(jt)), creek::child_order_end(jt), *(creek::child_order_begin(jt)), std::divides<double>());
                            *jt = tmp;
                        }
                        break;
                    }
                }
            }
            
            ++it;
            ++jt;
        }
        
        return *(numtree.pre_order_begin());
    }
}

int main()
{
    creek::tree<std::string> tree;
    while(1){
        std::string expr;
        
        std::cout << "> ";
        std::cout.flush();
        
        std::getline(std::cin, expr);
        if(expr == "(quit)" || expr == "(exit)" || expr == "quit" || expr == "exit") break;
        
        if(Parse(expr, tree)){
            std::cout << "> = " << Calculate(tree) << std::endl;
        }else{
            std::cout << "> " << "parse error" << std::endl;
        }
    }
}


