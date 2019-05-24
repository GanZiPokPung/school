#include "pch.h"
#include "Tree_AVL.h"

CTreeAVL::CTreeAVL()
{
}

CTreeAVL::~CTreeAVL()
{
}

// AVL 트리의 높이를 출력
int CTreeAVL::height(nodeAVL * node)
{
	int h = 0;

	if (nullptr != node)
	{
		//재귀적으로 왼쪽 혹은 오른쪽으로 검색합니다.
		int left = height(node->left);
		int right = height(node->right);
		int maxHeight = max(left, right);
		h = maxHeight + 1;
	}

	return h;
}

// 균형인수(높이의 차이)를 반환
int CTreeAVL::difference(nodeAVL * node)
{
	// 왼쪽 자식의 높이와 오른쪽 자식의 높이 차이를 반환합니다.
	int left = height(node->left);
	int right = height(node->right);
	int factor = left - right;
	return factor;
}

// LL 회전
nodeAVL * CTreeAVL::ll(nodeAVL * node)
{
	nodeAVL *temp;
	temp = node->left;
	node->left = temp->right;
	node->right = node;
	return temp;
}

// RR 회전
nodeAVL * CTreeAVL::rr(nodeAVL * node)
{
	// 말이 회전이지 그냥 부모 노드의 오른쪽 자식 노드와 데이터를 교환
	nodeAVL *temp;
	temp = node->right;
	node->right = temp->left;
	temp->left = node;
	return temp;
}

// LR 회전
nodeAVL * CTreeAVL::lr(nodeAVL * node)
{
	// LR 회전은 왼쪽 자식을 기준으로 RR, 본인을 기준으로 LL
	nodeAVL *temp;
	temp = node->left;
	node->left = rr(temp);
	return ll(node);
}

// RL 회전
nodeAVL * CTreeAVL::rl(nodeAVL * node)
{
	nodeAVL *temp;
	temp = node->right;
	node->right = ll(temp);
	return rr(node);
}

// AVL 트리의 균형을 맞추는 함수
nodeAVL * CTreeAVL::balance(nodeAVL * node)
{
	int factor = difference(node);

	// 왼쪽 서브트리쪽으로 삽입이 되어 균형이 깨진 경우
	if (factor > 1)
	{
		// 왼쪽 자식 노드에 문제 발생
		if (difference(node->left) > 0)
			node = ll(node);

		// 오른쪽 자식 노드에 문제 발생
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

	// 현재 트리가 비어있지 않은 경우
	if (nullptr != node)
	{
		display(node->right, level + 1);
		printf("\n");


		// 현재 루트 노드인 경우
		if (root == node)
		{
			cout << "Root->";
		}

		// i가 현레벨보다 낮고 루트가 아닌 경우일 때
		for (i = 0; i < level && node != root; ++i)
		{
			cout << "     ";
		}

		// 자신의 위치에 데이터를 출력
		cout << node->data;

		// 왼쪽 노드도 출력
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
