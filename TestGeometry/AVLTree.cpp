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
 * LL�������Ӧ�����(�ҵ���ת)��
 *
 * ����ֵ����ת��ĸ��ڵ�
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
 * RR�����Ҷ�Ӧ�����(����ת)��
 *
 * ����ֵ����ת��ĸ��ڵ�
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
 * LR�����Ҷ�Ӧ�����(��˫��ת)��
 *
 * ����ֵ����ת��ĸ��ڵ�
 */
TreeNode* AVLTree::left_right_rotation(TreeNode *k3)
{
    k3->left = right_right_rotation(k3->left);
    return left_left_rotation(k3);
}

/*
 * RL�������Ӧ�����(��˫��ת)��
 *
 * ����ֵ����ת��ĸ��ڵ�
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
	// ��Ϊ�� ���� û��Ҫɾ���Ľڵ㣬ֱ�ӷ���NULL��
	if (tree == NULL || z == NULL)
		return NULL;

	if (z->key < tree->key)        // ��ɾ���Ľڵ���"tree��������"��
	{
		tree->left = delete_node(tree->left, z);
		// ɾ���ڵ����AVL��ʧȥƽ�⣬�������Ӧ�ĵ��ڡ�
		if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
		{
			TreeNode *r =  tree->right;
			if (HEIGHT(r->left) > HEIGHT(r->right))
				tree = right_left_rotation(tree);
			else
				tree = right_right_rotation(tree);
		}
	}
	else if (z->key > tree->key)// ��ɾ���Ľڵ���"tree��������"��
	{
		tree->right = delete_node(tree->right, z);
		// ɾ���ڵ����AVL��ʧȥƽ�⣬�������Ӧ�ĵ��ڡ�
		if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
		{
			TreeNode *l =  tree->left;
			if (HEIGHT(l->right) > HEIGHT(l->left))
				tree = left_right_rotation(tree);
			else
				tree = left_left_rotation(tree);
		}
	}
	else    // tree�Ƕ�ӦҪɾ���Ľڵ㡣
	{
		// tree�����Һ��Ӷ��ǿ�
		if ((tree->left) && (tree->right))
		{
			if (HEIGHT(tree->left) > HEIGHT(tree->right))
			{
				// ���tree�����������������ߣ�
				// ��(01)�ҳ�tree���������е����ڵ�
				//   (02)�������ڵ��ֵ��ֵ��tree��
				//   (03)ɾ�������ڵ㡣
				// ����������"tree�������������ڵ�"��"tree"������
				// �������ַ�ʽ�ĺô��ǣ�ɾ��"tree�������������ڵ�"֮��AVL����Ȼ��ƽ��ġ�
				TreeNode *max = avltree_maximum(tree->left);
				tree->key = max->key;
				tree->left = delete_node(tree->left, max);
			}
			else
			{
				// ���tree��������������������(��������ȣ�������������������1)
				// ��(01)�ҳ�tree���������е���С�ڵ�
				//   (02)������С�ڵ��ֵ��ֵ��tree��
				//   (03)ɾ������С�ڵ㡣
				// ����������"tree������������С�ڵ�"��"tree"������
				// �������ַ�ʽ�ĺô��ǣ�ɾ��"tree������������С�ڵ�"֮��AVL����Ȼ��ƽ��ġ�
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

