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

	// 부모 노드 반환
	int Parent(int i) { return (i - 1) / 2; }
	// 좌측 자식 노드 반환
	int Left(int i) { return (2 * i + 1); }
	// 우측 자식 노드 반환
	int Right(int i) { return (2 * i + 2); }

	void DecreaseKey(int i, int new_val);

	// 가장 작은 값(루트노드) 반환
	int GetMin() { return harr[0]; }

	void deleteKey(int i);
	void insertKey(int k);

private:
	// 힙 요소 포인터
	int  *harr;
	// 최소 힙의 최대 크기
	int capacity;
	// 최소 힙의 현재 크기
	int heap_size;
};

