#pragma once

#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

// #define TEST_TREE

typedef int Type;

class TreeNode
{
public:
	TreeNode();
	TreeNode(Type key);
	~TreeNode();

	Type key;            
	int height;
	TreeNode *left;    
	TreeNode *right;
};

class AVLTree
{
public:
	AVLTree();
	~AVLTree();

	bool Add(Type key);
	bool Delete(Type key);
	TreeNode* Search(Type);

	void PreOrder(vector<Type> &vec);
	void PostOrder(vector<Type> &vec);
	void MidOrder(vector<Type> &vec);

protected:
	TreeNode *m_pRoot;

protected:
	TreeNode* left_left_rotation(TreeNode *k2);
	TreeNode* right_right_rotation(TreeNode *k1);
	TreeNode* left_right_rotation(TreeNode *k3);
	TreeNode* right_left_rotation(TreeNode *k1);
	TreeNode* insert(TreeNode *p, Type key);
	TreeNode* delete_node(TreeNode* tree, TreeNode *p);
	void Free(TreeNode *node);

	// 查找最小结点：返回tree为根结点的AVL树的最小结点。
	TreeNode* avltree_minimum(TreeNode *p);
	// 查找最大结点：返回tree为根结点的AVL树的最大结点。
	TreeNode* avltree_maximum(TreeNode *p);
};


#ifdef TEST_TREE
void Show(vector<Type> &va);
void TestTree();
#endif // TEST_TREE

#endif