/* 
 * File:   TreeNode.h
 * Author: dima
 *
 * Created on 18 Апрель 2011 г., 0:46
 */

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifndef TREENODE_H
#define	TREENODE_H

#define treePtr boost::shared_ptr<TreeNode<T> >
#define treeFun boost::function<T(const T&, const T&)>

/// Number functor
template <typename T>
struct Number
{
    T num;
    T operator()(const T& a, const T& b)
    {
        return num;
    }
};

/// Tree node class
template<class T>
class TreeNode
{
public:
   TreeNode(treePtr i_left, treePtr i_right, 
           treeFun op): left(i_left), right(i_right), operation(op){}
   T Evaluate()
   {
       if ((left.get() != 0) && (right.get() != 0)) ///> Check for number
       {
           return operation(left->Evaluate(), right->Evaluate());
       }
       else
       {
           return operation(0, 0);  ///> For number functor
       }
   }
private:
    treePtr left;
    treePtr right;
    boost::weak_ptr<TreeNode<T> > parent;
    treeFun operation;
};



#endif	/* TREENODE_H */

