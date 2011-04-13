/* 
 * File:   tree.h
 * Author: dima
 *
 * Created on 12 Апрель 2011 г., 14:13
 */
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>

template <typename T>
class TreeNode
{
public:
    boost::shared_ptr<TreeNode> _left, _right;
    boost::weak_ptr<TreeNode> _parent;
    T value;
    void Output(std::ostream& o_rStr);
};

template <typename T>
class Tree
{
public:
    Tree (boost::shared_ptr<TreeNode<T> >){}
    bool AddNode(T i_value, boost::shared_ptr<TreeNode <T> > i_where);
private:
    boost::shared_ptr<TreeNode<T> > m_root;
};

bool Tree::AddNode(T i_value, boost::shared_ptr<TreeNode<T> > i_where)
{
    boost::shared_ptr<TreeNode<T> > node = new TreeNode();
    node->value = i_value;
    node->_parent = i_where;
    if(i_where->_left.get() == 0)
    {
        i_where->_left = node;
    }
    else
    {
        if(i_where->_right.get() == 0)
        {
            i_where->_right = node;
        }
        else
            return false;

    }
    return true;
}

TreeNode::Output(std::ostream& o_rStr)
{
    o_rStr<<value<<'\n';
    if(_left.get() != 0)
    {
        _left->Output(o_rStr);
    }
    if(_right.get() != 0)
    {
        _right->Output(o_rStr);
    }
}







