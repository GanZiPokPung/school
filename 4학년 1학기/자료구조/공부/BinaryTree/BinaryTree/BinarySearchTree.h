#pragma once

// 이진 탐색 트리
// 그냥 이진 트리와 다르게 아무렇게나 집어넣지 않는다.
// root 노드 값을 기준으로 작으면 왼쪽 크면 오른쪽으로 재귀를 타며
// leaf 노드가 나올때 까지 recursive하여 집어넣는다.

// 삭제할 경우 세가지로 나누어 처리한다.
// 1. 지울 노드이 자식 노드가 없을 경우
// -> 그냥 지워준다.
// 2. 지울 노드의 자식 노드가 한쪽에만 있을 경우
// -> 그 자식 노드를 지울 노드의 주소값 대입해주고 지워준다.
// 3. 지울 노드의 자식 노드가 두개일 경우
// -> 노드의 오른쪽 자식 노드에서 가장 최소값(오른쪽 자식 노드로 부터 맨 왼쪽 leaf노드)
//    을 가져와 값만 덮어써주고 그 leaf노드는 지운다.

class treeNode
{
	friend class BinarySearchTree;
private:
	int value;
	treeNode *left, *right;
	treeNode(int val) :
		value(val), left(nullptr), right(nullptr)
	{}
};

class BinarySearchTree
{
private:
	void recursiveInsert(treeNode *&node, int val);
	void recursiveRemove(treeNode *&node, int val);
	void preorderRecursivePrint(treeNode *node);
	void postorderRecursivePrint(treeNode *node);
	void inorderRecursivePrint(treeNode *node);

public:
	treeNode *root;
	BinarySearchTree() :
		root(nullptr)
	{}
	void insert(int val);
	void remove(int val);
	void preorderPrint();
	void postorderPrint();
	void inorderPrint();
};
