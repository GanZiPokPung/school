#include "BinarySearchTree.h"

#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

void BinarySearchTree::recursiveInsert(treeNode *& node, int val)
{
	if (nullptr == node)
		node = new treeNode(val);
	// 탐색하고 있는 노드가 NULL이면 새로 만든다.(삽입)

	else if (val > node->value)
		recursiveInsert(node->right, val);
	// 삽입할 값이 노드의 값보다 크면 오른쪽 Child를 탐색

	else if (val < node->value)
		recursiveInsert(node->left, val);
	// 삽입할 값이 노드의 값보다 작으면 왼쪽 Child를 탐색
}

void BinarySearchTree::recursiveRemove(treeNode *& node, int val)
{
	treeNode *removal = new treeNode(0);

	if (node == nullptr)
		return;
	// 찾지 못했을 경우 리턴해서 함수 종료

	else if (val < node->value)
		recursiveRemove(node->right, val);
	// 삭제할 값이 노드의 값보다 크면 오른쪽 Child를 탐색

	else if (val > node->value)
		recursiveRemove(node->left, val);
	// 삭제할 값이 노드의 값보다 작으면 왼쪽 Child를 탐색

	else
	// 삭제할 노드를 찾았음
	{
		if (nullptr == node->left && nullptr == node->right)
		// 경우 1 : 노드의 Child가 없을때
		{
			delete node;
			node = nullptr;
		}

		else if (nullptr == node->left)
		// 경우 2 : 노드의 Child가 오른쪽 하나일 때
		{
			removal = node;
			node = node->right;
			delete removal;
		}

		else if (nullptr == node->right)
			// 경우 2 : 노드의 Child가 왼쪽 하나일 때
		{
			removal = node;
			node = node->left;
			delete removal;
		}

		else
			// 경우 3 : 노드의 Child가 두개일 때
		{
			removal = node->right;

			while (nullptr != removal->left)
			// 노드의 오른쪽 Subtree에서 최소값을 가진 노드를 찾는다.
				removal = removal->left;

			int minVal = removal->value;
			// 최소값 노드의 값을 백업해둔다.
			recursiveRemove(rootNode, minVal);
			// 최소값 노드를 삭제한다.
			node->value = minVal;
			// 백업한 최소값을 "실제 삭제된 값을 가진 노드"의 값에 덮어쓴다.
		}
	}
}

void BinarySearchTree::preorderRecursivePrint(treeNode * node)
{
	if (nullptr != node)
	{
		cout << node->value << endl;
		preorderRecursivePrint(node->left);
		preorderRecursivePrint(node->right);
	}
}

void BinarySearchTree::postorderRecursivePrint(treeNode * node)
{
	if (nullptr != node)
	{
		postorderRecursivePrint(node->left);
		postorderRecursivePrint(node->right);
		cout << node->value << endl;
	}
}

void BinarySearchTree::inorderRecursivePrint(treeNode * node)
{
	if (nullptr != node)
	{
		inorderRecursivePrint(node->left);
		cout << node->value << endl;
		inorderRecursivePrint(node->right);
	}
}

void BinarySearchTree::insert(int val)
{
	recursiveInsert(rootNode, val);
}

void BinarySearchTree::remove(int val)
{
	recursiveRemove(rootNode, val);
}

void BinarySearchTree::preorderPrint()
{
	cout << "PREORDER" << endl;
	preorderRecursivePrint(rootNode);
}

void BinarySearchTree::postorderPrint()
{
	cout << "POSTORDER" << endl;
	postorderRecursivePrint(rootNode);
}

void BinarySearchTree::inorderPrint()
{
	cout << "INORDER" << endl;
	inorderRecursivePrint(rootNode);
}