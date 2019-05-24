#pragma once

#include <iostream>
using namespace std;

class CHeapTree
{
public:
	CHeapTree();
	~CHeapTree();
};

class CMinHeap :
	public CHeapTree
{
public:
	CMinHeap(int cap);
	~CMinHeap();

public:
	void MinHeapify(int);
	int ExtractMin();

	// �θ� ��� ��ȯ
	int Parent(int i) { return (i - 1) / 2; }
	// ���� �ڽ� ��� ��ȯ
	int Left(int i) { return (2 * i + 1); }
	// ���� �ڽ� ��� ��ȯ
	int Right(int i) { return (2 * i + 2); }

	void DecreaseKey(int i, int new_val);

	// ���� ���� ��(��Ʈ���) ��ȯ
	int GetMin() { return harr[0]; }

	void deleteKey(int i);
	void insertKey(int k);

private:
	// �� ��� ������
	int  *harr;
	// �ּ� ���� �ִ� ũ��
	int capacity;
	// �ּ� ���� ���� ũ��
	int heap_size;
};

