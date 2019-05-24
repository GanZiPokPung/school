#include "HeapTree.h"

#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

CHeapTree::CHeapTree()
{
}


CHeapTree::~CHeapTree()
{
}

CMinHeap::CMinHeap(int cap)
	:CHeapTree()
{
	heap_size = 0;
	capacity = cap;
	harr = new int[cap];
}

CMinHeap::~CMinHeap()
{
	delete harr;
	harr = nullptr;
}

void CMinHeap::MinHeapify(int i)
{
	int l = Left(i);
	int r = Right(i);
	int smallest = i;

	if (l < heap_size && harr[l] < harr[i])
		smallest = l;
	if (r < heap_size && harr[r] < harr[smallest])
		smallest = r;

	if (smallest != i)
	{
		swap(harr[i], harr[smallest]);
		MinHeapify(smallest);
	}
}

int CMinHeap::ExtractMin()
{
	if (heap_size <= 0)
		return INT_MAX;
	if (heap_size == 1)
	{
		heap_size--;
		return harr[0];
	}

	int root = harr[0];

	//힙에 가장 마지막에 있는 놈을 루트로 가져와
	harr[0] = harr[heap_size - 1];
	heap_size--;
	// 정렬 작업을 해준다.
	MinHeapify(0);

	return root;
}

void CMinHeap::DecreaseKey(int i, int new_val)
{
	harr[i] = new_val;
	while (i != 0 && harr[Parent(i)] > harr[i])
	{
		swap(harr[i], harr[Parent(i)]);
		i = Parent(i);
	}
}

void CMinHeap::deleteKey(int i)
{
	DecreaseKey(i, INT_MIN);
	ExtractMin();
}

void CMinHeap::insertKey(int k)
{
	if (heap_size == capacity)
	{
		cout << "\nOverflow : Could not insertKey\n";
		return;
	}

	heap_size++;
	int i = heap_size - 1;
	harr[i] = k;

	// 자신의 위치를 검색
	while (i != 0 && harr[Parent(i)] > harr[i])
	{
		swap(harr[i], harr[Parent(i)]);
		i = Parent(i);
	}
}
