#pragma once

// AVL Ʈ�� -> ������ ���߾��� ���� Ʈ��
// ���� : ���� �μ�
// -> ���� �μ��� 2�̻��̰ų� -2������ �� ������ �����ٰ� ���Ѵ�.

// ������ ������ ���

// 1. LL Rotation

/*
�θ� ����� ���� �ڽ� = �ڽ� ����� ������ �ڽ�
�ڽ� ����� ������ �ڽ� = �θ� ���
*/

// 2. LR Rotation
// 3. RR Rotation
// 4. RL Rotation

typedef struct nodeAVL
{
	int data;
	struct nodeAVL *left;
	struct nodeAVL * right;
}NODE_AVL;

class CTreeAVL
{
public:
	CTreeAVL();
	~CTreeAVL();
public:
	int height(nodeAVL *node);
	int difference(nodeAVL *node);
	
	nodeAVL* ll(nodeAVL *node);
	nodeAVL* rr(nodeAVL *node);
	nodeAVL* lr(nodeAVL *node);
	nodeAVL* rl(nodeAVL *node);
	nodeAVL* balance(nodeAVL *node);
	
	nodeAVL* insert(nodeAVL *node, int val);

	void display(nodeAVL *node, int level);
	void inorder(nodeAVL *node);
	void preorder(nodeAVL *node);
	void postorder(nodeAVL *node);
public:
	NODE_AVL *root = nullptr;
};

