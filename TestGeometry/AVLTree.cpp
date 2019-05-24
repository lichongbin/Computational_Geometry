#include "StdAfx.h"
#include "Const.h"
#include "AVLTree.h"

#define HEIGHT(p)    ( (p==NULL) ? -1 : (((TreeNode *)(p))->height) )

TreeNode::TreeNode()
: left(NULL)
, right(NULL)
{
}

TreeNode::TreeNode(Type k)
: key(k)
, left(NULL)
, right(NULL)
{
}

TreeNode::~TreeNode()
{
}

AVLTree::AVLTree()
: m_pRoot(NULL)
{
}

AVLTree::~AVLTree()
{
	Free(m_pRoot);
}

bool AVLTree::Add(Type key)
{
	m_pRoot = insert(m_pRoot, key);
	return true;
}

bool AVLTree::Delete(Type key)
{
	TreeNode *p = Search(key);
	delete_node(m_pRoot, p);
	return true;
}

TreeNode* AVLTree::Search(Type key)
{
	TreeNode *x = m_pRoot;
	while ( x != NULL && x->key != key)
	{
		if (key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

void AVLTree::PreOrder(vector<Type> &vec)
{
	vec.clear();

	TreeNode *p = m_pRoot;
	vector<TreeNode*> s;
	while (true) 
	{
		while (p != NULL)
		{
			vec.push_back(p->key);
			s.push_back(p);
			p = p->left;
		}

		if (s.size() <= 0)
		{
			break;
		}

		p = *(s.rbegin());
		s.pop_back();
		p = p->right;
	}
}

void AVLTree::PostOrder(vector<Type> &vec)
{
	vec.clear();
	TreeNode *p = m_pRoot,
		     *t = NULL;

	vector<TreeNode*> s;
	while (true) 
	{
		while (p != NULL)
		{
			s.push_back(p);
			p = p->left;
		}

		if (s.size() <= 0)
		{
			break;
		}

		p = *(s.rbegin());
		if (NULL == p->right || t == p->right)
		{
			vec.push_back(p->key);
			s.pop_back();
			t = p;
			p = NULL;
		}
		else
		{
			p = p->right;
		}
	}
}

void AVLTree::MidOrder(vector<Type> &vec)
{
	vec.clear();

	TreeNode *p = m_pRoot;
	vector<TreeNode*> s;
	while (true) 
	{
		while (p != NULL)
		{
			s.push_back(p);
			p = p->left;
		}

		if (s.size() <= 0)
		{
			break;
		}

		p = *(s.rbegin());
		vec.push_back(p->key);
		s.pop_back();
		p = p->right;
	}
}

/*
 * LL：左左对应的情况(右单旋转)。
 *
 * 返回值：旋转后的根节点
 */
TreeNode* AVLTree::left_left_rotation(TreeNode *k2)
{
    TreeNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = MAX( HEIGHT(k2->left), HEIGHT(k2->right)) + 1;
    k1->height = MAX( HEIGHT(k1->left), k2->height) + 1;

    return k1;
}

/*
 * RR：右右对应的情况(左单旋转)。
 *
 * 返回值：旋转后的根节点
 */
TreeNode* AVLTree::right_right_rotation(TreeNode *k1)
{
    TreeNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;

    k1->height = MAX( HEIGHT(k1->left), HEIGHT(k1->right)) + 1;
    k2->height = MAX( HEIGHT(k2->right), k1->height) + 1;

    return k2;
}

/*
 * LR：左右对应的情况(左双旋转)。
 *
 * 返回值：旋转后的根节点
 */
TreeNode* AVLTree::left_right_rotation(TreeNode *k3)
{
    k3->left = right_right_rotation(k3->left);
    return left_left_rotation(k3);
}

/*
 * RL：右左对应的情况(右双旋转)。
 *
 * 返回值：旋转后的根节点
 */
TreeNode* AVLTree::right_left_rotation(TreeNode *k1)
{
    k1->right = left_left_rotation(k1->right);
    return right_right_rotation(k1);
}

TreeNode* AVLTree::insert(TreeNode *p, Type key)
{
	if (NULL == p)
	{
		p = new TreeNode(key);
	}
	else if (key < p->key)
	{
		p->left = insert(p->left, key);
		if (HEIGHT(p->left) - HEIGHT(p->right) == 2)
		{
			if (key < p->left->key)
				p = left_left_rotation(p);
			else
				p = left_right_rotation(p);
		}
	}
	else 
	{
		p->right = insert(p->right, key);
		if (HEIGHT(p->right) - HEIGHT(p->left) == 2)
		{
			if (key > p->right->key)
				p = right_right_rotation(p);
			else
				p = right_left_rotation(p);
		}
	}
	p->height = MAX( HEIGHT(p->left), HEIGHT(p->right)) + 1;
	return p;
}

TreeNode* AVLTree::delete_node(TreeNode* tree, TreeNode *z)
{
	// 根为空 或者 没有要删除的节点，直接返回NULL。
	if (tree == NULL || z == NULL)
		return NULL;

	if (z->key < tree->key)        // 待删除的节点在"tree的左子树"中
	{
		tree->left = delete_node(tree->left, z);
		// 删除节点后，若AVL树失去平衡，则进行相应的调节。
		if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
		{
			TreeNode *r =  tree->right;
			if (HEIGHT(r->left) > HEIGHT(r->right))
				tree = right_left_rotation(tree);
			else
				tree = right_right_rotation(tree);
		}
	}
	else if (z->key > tree->key)// 待删除的节点在"tree的右子树"中
	{
		tree->right = delete_node(tree->right, z);
		// 删除节点后，若AVL树失去平衡，则进行相应的调节。
		if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
		{
			TreeNode *l =  tree->left;
			if (HEIGHT(l->right) > HEIGHT(l->left))
				tree = left_right_rotation(tree);
			else
				tree = left_left_rotation(tree);
		}
	}
	else    // tree是对应要删除的节点。
	{
		// tree的左右孩子都非空
		if ((tree->left) && (tree->right))
		{
			if (HEIGHT(tree->left) > HEIGHT(tree->right))
			{
				// 如果tree的左子树比右子树高；
				// 则(01)找出tree的左子树中的最大节点
				//   (02)将该最大节点的值赋值给tree。
				//   (03)删除该最大节点。
				// 这类似于用"tree的左子树中最大节点"做"tree"的替身；
				// 采用这种方式的好处是：删除"tree的左子树中最大节点"之后，AVL树仍然是平衡的。
				TreeNode *max = avltree_maximum(tree->left);
				tree->key = max->key;
				tree->left = delete_node(tree->left, max);
			}
			else
			{
				// 如果tree的左子树不比右子树高(即它们相等，或右子树比左子树高1)
				// 则(01)找出tree的右子树中的最小节点
				//   (02)将该最小节点的值赋值给tree。
				//   (03)删除该最小节点。
				// 这类似于用"tree的右子树中最小节点"做"tree"的替身；
				// 采用这种方式的好处是：删除"tree的右子树中最小节点"之后，AVL树仍然是平衡的。
				TreeNode *min = avltree_maximum(tree->right);
				tree->key = min->key;
				tree->right = delete_node(tree->right, min);
			}
		}
		else
		{
			TreeNode *tmp = tree;
			tree = tree->left ? tree->left : tree->right;
			free(tmp);
		}
	}

	return tree;
}

void AVLTree::Free(TreeNode *node)
{
	if (NULL == node)
	{
		return;
	}

	if (node->left != NULL)
	{
		Free(node->left);
	}

	if (node->right != NULL)
	{
		Free(node->right);
	}
}

TreeNode* AVLTree::avltree_minimum(TreeNode *tree)
{
	if (tree == NULL)
		return NULL;

	while(tree->left != NULL)
		tree = tree->left;
	return tree;
}

TreeNode* AVLTree::avltree_maximum(TreeNode *tree)
{
	if (tree == NULL)
		return NULL;

	while(tree->right != NULL)
		tree = tree->right;
	return tree;
}

#ifdef TEST_TREE

void Show(vector<Type> &va)
{
	TRACE(_T("\n"));
	for (vector<int>::iterator iter = va.begin(); iter != va.end(); iter++)
	{
		TRACE(_T("%4d"), *iter);
	}
}

void Test()
{
	TRACE(_T("Sequence:\n"));

	AVLTree tree;
	for (int i = 0; i < 10; i++)
	{
		int n = rand() % 100;
		tree.Add(n);
		TRACE(_T("%4d"), n);
	}

	vector<int> va;
	tree.PreOrder(va);
	Show(va);

	tree.PostOrder(va);
	Show(va);

	tree.MidOrder(va);
	Show(va);
}

#endif // TEST_TREE

