/* 
 * File:   Tree.h
 * Author: dima
 *
 * Created on 18 Апрель 2011 г., 0:48
 */

#ifndef TREE_H
#define	TREE_H
#include "TreeNode.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>

//using namespace boost;
//Char arrays for parsing
const char NUMS[] = "0123456789";
char OPS[] = "+-*/";

/// Auxiliary struct for operators
template<typename T>
struct Operator
{
    Operator(char tk, unsigned pr, treeFun fun):token(tk), priority(pr),functor(fun){}
    char token;
    unsigned priority;
    treeFun functor;
};

/// Main tree class
template<typename T>
class Tree
{
public:
    /// Bulding tree from string using operators
    void Parse(std::string);
    
    /// Evaluating tree using functors
    T Eval() {return root->Evaluate();}

    /// Expand set of binary operators
    void AddOperator(const Operator<T>& op) {operators.push_back(op);}

    /// Small helpful functions for work with operators
    unsigned GetPriority(char ch);
    treeFun GetOpFunctor(char ch);

private:
    treePtr root; ///< Pointer to root of tree
    std::vector<Operator<T> > operators;  ///< Set of operators
};

template<typename T>
unsigned Tree<T>::GetPriority(char ch)
{
    for(unsigned i = 0; i < operators.size(); ++i)
    {
        if (operators[i].token == ch)
        {
            return operators[i].priority;
        }
    }
    throw("Unknown operation");
}

template<typename T>
treeFun Tree<T>::GetOpFunctor(char ch)
{
    for(unsigned i = 0; i < operators.size(); ++i)
    {
        if (operators[i].token == ch)
        {
            return operators[i].functor;
        }
    }
    throw("Unknown operation");
}


template<typename T>
void Tree<T>::Parse(std::string str)
{
   std::string OPS;  ///<String with operators for search and substitude
   for (unsigned i = 0; i < operators.size(); ++i)
   {
       OPS.push_back(operators[i].token);
   }

   std::vector<char> ops;           ///< Stack with operators
   std::vector<treePtr> exprs;      ///< Stack with Nodes
   /// Variables for string parsing
   std::string::size_type numPos = str.find_first_of(NUMS, 0);
   std::string::size_type endPos = str.find_first_not_of(NUMS, numPos);
   std::string::size_type opPos;

   /// Current expression
   treePtr currentExpr;
   char currentOp;

   while (std::string::npos != endPos || std::string::npos != numPos)
   {
      /// Actually parsing only integers
      /// Doubles might be done by changing function atoi to appropriate
      Number<int> numb = {atoi(str.substr(numPos, endPos - numPos).c_str())};

      /// Left and right nodes are empty for number node
      treePtr numLink;
      currentExpr.reset(new TreeNode<T>(numLink, numLink, numb));
      exprs.push_back(currentExpr);

      // Find operator
      opPos = str.find_first_of(OPS, endPos);
      if (std::string::npos != opPos)
      {
         currentOp = str.substr(opPos, 1)[0];

         // Check priority of operators
         // While current op doesn't have higher priority than last in the stack - perform stack operation
         while (!ops.empty() && (GetPriority(ops.back()) >= GetPriority(currentOp)))
         {
            /// Find appropriate functor in operation set
            treeFun currFun(GetOpFunctor(ops.back()));

            /// Make operation from two last
            treePtr currBinExpr(new TreeNode<T>(exprs[exprs.size() -2], exprs.back(), currFun));
            /// Delete two last expressions and last operator from stack
            ops.pop_back();
            exprs.pop_back();
            exprs.pop_back();

            /// Push new expression to the stack
            exprs.push_back(currBinExpr);
         }

         // When perfomance complete - push current operation to the stack
         ops.push_back(currentOp);
      }
      numPos = str.find_first_of(NUMS, opPos);
      endPos = str.find_first_not_of(NUMS, numPos);
   }

   // If there are operators left in the stack - evalute them and push to expr stack
   while (!ops.empty() && !exprs.empty())
   {
        treeFun currFun(GetOpFunctor(ops.back()));
        treePtr currBinExpr(new TreeNode<T>(exprs[exprs.size() - 2], exprs.back(), currFun));
        ops.pop_back();
        exprs.pop_back();
        exprs.pop_back();
        exprs.push_back(currBinExpr);
   }

   // Last element in expression stack is root pointer
   root = exprs.back();
}


#endif	/* TREE_H */

