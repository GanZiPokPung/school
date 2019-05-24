#include "pch.h"
#include "Tree_AVL.h"

CTreeAVL::CTreeAVL()
{
}

CTreeAVL::~CTreeAVL()
{
}

// AVL Ʈ���� ���̸� ���
int CTreeAVL::height(nodeAVL * node)
{
	int h = 0;

	if (nullptr != node)
	{
		//��������� ���� Ȥ�� ���������� �˻��մϴ�.
		int left = height(node->left);
		int right = height(node->right);
		int maxHeight = max(left, right);
		h = maxHeight + 1;
	}

	return h;
}

// �����μ�(������ ����)�� ��ȯ
int CTreeAVL::difference(nodeAVL * node)
{
	// ���� �ڽ��� ���̿� ������ �ڽ��� ���� ���̸� ��ȯ�մϴ�.
	int left = height(node->left);
	int right = height(node->right);
	int factor = left - right;
	return factor;
}

// LL ȸ��
nodeAVL * CTreeAVL::ll(nodeAVL * node)
{
	nodeAVL *temp;
	temp = node->left;
	node->left = temp->right;
	node->right = node;
	return temp;
}

// RR ȸ��
nodeAVL * CTreeAVL::rr(nodeAVL * node)
{
	// ���� ȸ������ �׳� �θ� ����� ������ �ڽ� ���� �����͸� ��ȯ
	nodeAVL *temp;
	temp = node->right;
	node->right = temp->left;
	temp->left = node;
	return temp;
}

// LR ȸ��
nodeAVL * CTreeAVL::lr(nodeAVL * node)
{
	// LR ȸ���� ���� �ڽ��� �������� RR, ������ �������� LL
	nodeAVL *temp;
	temp = node->left;
	node->left = rr(temp);
	return ll(node);
}

// RL ȸ��
nodeAVL * CTreeAVL::rl(nodeAVL * node)
{
	nodeAVL *temp;
	temp = node->right;
	node->right = ll(temp);
	return rr(node);
}

// AVL Ʈ���� ������ ���ߴ� �Լ�
nodeAVL * CTreeAVL::balance(nodeAVL * node)
{
	int factor = difference(node);

	// ���� ����Ʈ�������� ������ �Ǿ� ������ ���� ���
	if (factor > 1)
	{
		// ���� �ڽ� ��忡 ���� �߻�
		if (difference(node->left) > 0)
			node = ll(node);

		// ������ �ڽ� ��忡 ���� �߻�
		else
			node = lr(node);
	}
	else if (factor < -1)
	{
		if (difference(node->right) > 0)
			node = rl(node);
		else
			node = rr(node);
	}

	return node;
}

nodeAVL * CTreeAVL::insert(nodeAVL * node, int val)
{
	if (nullptr == node)
	{
		node = new nodeAVL;
		node->data = val;
		node->left = nullptr;
		node->right = nullptr;
		return node;
	}
	else if (val < node->data)
	{
		node->left = insert(node->left, val);
		node = balance(node);
	}
	else if (val >= node->data)
	{
		node->right = insert(node->right, val);
		node = balance(node);
	}

	return node;
}

void CTreeAVL::display(nodeAVL * node, int level)
{
	int i;

	// ���� Ʈ���� ������� ���� ���
	if (nullptr != node)
	{
		display(node->right, level + 1);
		printf("\n");


		// ���� ��Ʈ ����� ���
		if (root == node)
		{
			cout << "Root->";
		}

		// i�� ���������� ���� ��Ʈ�� �ƴ� ����� ��
		for (i = 0; i < level && node != root; ++i)
		{
			cout << "     ";
		}

		// �ڽ��� ��ġ�� �����͸� ���
		cout << node->data;

		// ���� ��嵵 ���
		display(node->left, level + 1);
	}
}

void CTreeAVL::inorder(nodeAVL * node)
{
	if (nullptr == node)
		return;

	inorder(node->left);
	cout << node->data << " ";
	inorder(node->right);
}

void CTreeAVL::preorder(nodeAVL * node)
{
	if (nullptr == node)
		return;

	cout << node->data << " ";
	preorder(node->left);
	preorder(node->right);
}

void CTreeAVL::postorder(nodeAVL * node)
{
	if (nullptr == node)
		return;

	postorder(node->left);
	postorder(node->right);
	cout << node->data << " ";
}
