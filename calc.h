/// @file Calc.h
/// @brief Expression declaration and implementation
/// @author Dmitriy Kozhevnikov
/// @date Created on: 11-April-2011

#include <boost/function.hpp>

/// Interface class expression
class Expression
{
public:
   virtual int Evaluate() = 0;
   virtual ~Expression(){};
};

/// Implementation for number expression
template<class T>
class NumberExpression: public Expression
{
public:
   NumberExpression(T i_num): number(i_num) {}
   T Evaluate() {return number;}

private:
   T number;
};

/// Implementation for binary expression
/// Second template class determines operation
template<class T>
class BinaryExpression : public Expression
{
public:
   BinaryExpression(boost::shared_ptr<Expression> i_left,
           boost::shared_ptr<Expression> i_right, boost::function<T(T,T)> func)
   {
       i_left.get()
      right = i_right;
      op = func;
   }
   T Evaluate()
   {
      return op(left->Evaluate(), right->Evaluate());
   }
   virtual ~BinaryExpression()
   {
      delete left;
      delete right;
   }
private:
   boost::shared_ptr<Expression> left;
   boost::shared_ptr<Expression> right;
   boost::function<T(T,T)> op;
};


