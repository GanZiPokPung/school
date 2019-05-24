#pragma once

// AVL 트리 -> 균형이 갖추어진 이진 트리
// 원리 : 균형 인수
// -> 균형 인수가 2이상이거나 -2이하일 때 균형이 깨졌다고 말한다.

// 균형이 깨지는 경우

// 1. LL Rotation

/*
부모 노드의 왼쪽 자식 = 자식 노드의 오른쪽 자식
자식 노드의 오른쪽 자식 = 부모 노드
*/

// 2. LR Rotation
// 3. RR Rotation
// 4. RL Rotation

struct nodeAVL
{
	int data;
	struct nodeAVL *left;
	struct nodeAVL * right;
}*root;

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
};

