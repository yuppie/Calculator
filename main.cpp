/// @file calc.cpp
/// @brief parsing implementation & main
/// @author Dmitriy Kozhevnikov
/// @date Created on: 22-March-2011


#include "Tree.h"
#include <iostream>
#include <stdlib.h>

int Pow(const int& base, const int& exp)
{
    int result(1);
    for(int i = 0; i<exp; ++i)
    {
        result *= base;
    }
    return result;
}


int main (int argc, char* argv[])
{
//   if (argc > 1)
//   {
//      std::string i_str(argv[1]);
      
      ///--Expression tree--
      Tree<int> expr;
      
      ///--Operators: sign, priority, functor
      Operator<int> plus('+', 1, std::plus<int>());
      Operator<int> minus('-', 1, std::minus<int>());
      Operator<int> multiply('*', 2, std::multiplies<int>());
      Operator<int> divide('/', 2, std::divides<int>());
      Operator<int> power('^', 3, &Pow);
      
      expr.AddOperator(plus);
      expr.Parse("3+4");
      std::cout<<"3+4="<<expr.Eval()<<'\n';

      expr.AddOperator(multiply);
      expr.Parse("3*2+1*10");
      std::cout<<"3*2+1*10="<<expr.Eval()<<'\n';

      expr.AddOperator(divide);
      expr.Parse("3*2+1*10+2/1");
      std::cout<<"3*2+1*10+2/1="<<expr.Eval()<<'\n';

      expr.AddOperator(power);
      expr.Parse("3*2^2");
      std::cout<<"3*2^2="<<expr.Eval()<<'\n';

      Tree<double> exprDb;
      Operator<double> dbplus('+', 1, std::plus<double>());
      Operator<double> dbdivide ('/', 2, std::divides<double>());

      exprDb.AddOperator(dbplus);
      exprDb.AddOperator(dbdivide);

      exprDb.Parse("3+2/3");
      std::cout<<"3+2/3="<<exprDb.Eval()<<'\n';
//      try
//      {
//         //Evaluating expression tree
//         std::cout<<expr.Eval()<<'\n';
//      }
//      catch (std::exception& err_str)
//      {
//         std::cerr<<err_str.what()<<'\n';
//      }
//   }
//   else
//   {
//      std::cout<<"No expression entered"<<'\n';
//   }

   return 0;
}