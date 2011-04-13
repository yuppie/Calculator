/// @file calc.cpp
/// @brief parsing implementation & main
/// @author Dmitriy Kozhevnikov
/// @date Created on: 22-March-2011

#include "calc.h"
#include "tree.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
//#include <boost/function.hpp>

#include <boost/interprocess/smart_ptr/weak_ptr.hpp>
#include <boost/any.hpp>

//using namespace boost;
//Char arrays for parsing
char NUMS[] = "0123456789";
char OPS[] = "+-*/";

boost::function<int(int,int)> functor;





//Determing priority if ops before pushing to stack
bool Priority(char first, char second)
{
   if ((first == '+' || first == '-') && (second == '*' || second == '/'))
   {
      return true;
   }
   return false;
}

//Perform pushing to expression stack binary operation with appropriate template
void PerformOperation(std::vector<char>& i_ops, std::vector<Expression*>& i_exprs)
{
   Expression* currentExpr;
   switch (i_ops.back())
   {
   case '+':
      currentExpr = new BinaryExpression<int> (i_exprs[i_exprs.size() -2], i_exprs.back(), std::plus<int>());
      break;
   case '-':
      currentExpr = new BinaryExpression<int> (i_exprs[i_exprs.size() -2], i_exprs.back(), std::minus<int>());
      break;
   case '*':
      currentExpr = new BinaryExpression<int> (i_exprs[i_exprs.size() -2], i_exprs.back(), std::multiplies<int>());
      break;
   case '/':
      currentExpr = new BinaryExpression<int> (i_exprs[i_exprs.size() -2], i_exprs.back(), std::divides<int>());
      break;
   default:
      currentExpr = 0;
      break;
   }
   i_ops.pop_back();
   i_exprs.pop_back();
   i_exprs.pop_back();
   i_exprs.push_back(currentExpr);
}

//Parsing function, returns a pointer on top expression of expression tree
Expression* parser (std::string str)
{
   Expression* currentExpr;
   char currentOp;
   std::vector<char> ops;           //< Stack with operators
   std::vector<Expression*> exprs;  //< Stack with expressions
   std::string::size_type numPos = str.find_first_of(NUMS, 0);
   std::string::size_type endPos = str.find_first_not_of(NUMS, numPos);
   std::string::size_type opPos;

   while (std::string::npos != endPos || std::string::npos != numPos)
   {
      // Push numberExpression to expressions stack
      currentExpr = new NumberExpression<int>(atoi(str.substr(numPos, endPos - numPos).c_str()));
      exprs.push_back(currentExpr);

      // Find operator
      opPos = str.find_first_of(OPS, endPos);
      if (std::string::npos != opPos)
      {
         currentOp = str.substr(opPos, 1)[0];

         // Check priority of operators
         // While current op has higher priority than last in the stack - perform stack operation
         while (!ops.empty() && !Priority(ops.back(), currentOp))
         {
            PerformOperation(ops, exprs);
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
      PerformOperation(ops, exprs);
   }
   // Last element in expression stack is desired pointer
   return exprs.back();
}

int main (int argc, char* argv[])
{
   if (argc > 1)
   {
      std::string i_str(argv[1]);

      // Building expression tree
      Expression* expr = parser(i_str);
      try
      {
         //Evaluating expression tree
         std::cout<<expr->Evaluate()<<'\n';
      }
      catch (std::exception& err_str)
      {
         std::cerr<<err_str.what()<<'\n';
      }
   }
   else
   {
      std::cout<<"No expression entered"<<'\n';
   }
   return 0;
}