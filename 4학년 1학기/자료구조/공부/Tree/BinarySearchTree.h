#pragma once

// ���� Ž�� Ʈ��
// �׳� ���� Ʈ���� �ٸ��� �ƹ����Գ� ������� �ʴ´�.
// rootNode ��� ���� �������� ������ ���� ũ�� ���������� ��͸� Ÿ��
// leaf ��尡 ���ö� ���� recursive�Ͽ� ����ִ´�.

// ������ ��� �������� ������ ó���Ѵ�.
// 1. ���� ����� �ڽ� ��尡 ���� ���
// -> �׳� �����ش�.
// 2. ���� ����� �ڽ� ��尡 ���ʿ��� ���� ���
// -> �� �ڽ� ��带 ���� ����� �ּҰ� �������ְ� �����ش�.
// 3. ���� ����� �ڽ� ��尡 �ΰ��� ���
// -> ����� ������ �ڽ� ��忡�� ���� �ּҰ�(������ �ڽ� ���� ���� �� ���� leaf���)
//    �� ������ ���� ������ְ� �� leaf���� �����.

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
	treeNode *rootNode;
	BinarySearchTree() :
		rootNode(nullptr)
	{}
	void insert(int val);
	void remove(int val);
	void preorderPrint();
	void postorderPrint();
	void inorderPrint();
};
