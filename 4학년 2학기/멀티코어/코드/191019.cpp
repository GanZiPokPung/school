#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
using namespace std;
using namespace chrono;

// Non-Blocking
// 성능(1)과 이상현상으로 인한 속성으로 느려지는 문제(2)를 막아줌
// 충돌이 많은 상황에서도 안정적으로 성능을 뽑아준다.(3)

// 잘만든 blocking > lock free (충돌이 많을 경우)

class LFNODE;
class MPTR
{
	int value;
public:
	void set(LFNODE *node, bool removed)
	{
		value = reinterpret_cast<int>(node);
		if (true == value)
		{
			value = value | 0x01;
		}
		else
		{
			value = value & 0xFFFFFFFE;
		}
	}
	LFNODE *getptr()
	{
		return reinterpret_cast<LFNODE *>(value & 0xFFFFFFFE);
	}
	LFNODE *getptr(bool *removed)
	{
		int temp = value;
		if (0 == (temp & 0x1)) *removed = false;
		else *removed = true;

		return reinterpret_cast<LFNODE *>(temp & 0xFFFFFFFE);
	}
	bool CAS(LFNODE *old_node, LFNODE *new_node, bool old_removed, bool new_removed)
	{
		int old_value, new_value;
		old_value = reinterpret_cast<int>(old_node);
		
		if (true == old_removed) old_value = old_value | 0x01;
		else old_value = old_value & 0xFFFFFFFE;
		
		new_value = reinterpret_cast<int>(new_node);
		
		if (true == new_removed) new_value = new_value | 0x01;
		else new_value = new_value & 0xFFFFFFFE;

		return atomic_compare_exchange_strong(
			reinterpret_cast<atomic_int *>(&value), &old_value, new_value);
	}
	bool TryMarking(LFNODE *old_node, bool new_removed)
	{
		int old_value, new_value;
		old_value = reinterpret_cast<int>(old_node);
		old_value = old_value & 0xFFFFFFFE;
		new_value = old_value;
		if (true == new_removed) new_value = new_value | 0x01;
		
		return atomic_compare_exchange_strong(
			reinterpret_cast<atomic_int *>(&value), &old_value, new_value);
	}
};

class LFNODE
{
public:
	int key;
	MPTR next;

	LFNODE() { next.set(nullptr, false); }
	LFNODE(int key_value)
	{
		next.set(nullptr, false);
		key = key_value;
	}
	~LFNODE() {}
};

class LFLIST
{
	LFNODE head, tail;
	LFNODE *freelist;
	LFNODE freetail;
	mutex  fl_mutex;

public:
	LFLIST()
	{
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.next.set(&tail, false);
		freetail.key = 0x7FFFFFFF;
		freelist = &freetail;
	}
	~LFLIST() {}

	void Init()
	{
		LFNODE *ptr;
		while (head.next.getptr() != &tail)
		{
			ptr = head.next.getptr();
			head.next = head.next.getptr()->next;
			delete ptr;
		}
	}

	/*void recycle_freelist()
	{
		LFNODE *p = freelist;
		while (p != &freelist)
		{

		}
	}*/

	// 포인터 자체의 값을 변경할 것이므로 레퍼런스로 받는다.
	void find(int key, LFNODE *(&pred), LFNODE *(&curr))
	{
		retry:
		pred = &head;
		curr = pred->next.getptr();
		while (true)
		{
			bool removed;
			LFNODE *succ = curr->next.getptr(&removed);
			while (true == removed) 
			{
				if (false == pred->next.CAS(curr, succ, false, false))
					goto retry;
				curr = succ;
				succ = curr->next.getptr(&removed);
			}
			if (curr->key >= key) return;
			pred = curr;
			curr = curr->next.getptr();
		}
	}

	bool Add(int key)
	{
		LFNODE *pred, *curr;

		while (true)
		{
			find(key, pred, curr);

			if (key == curr->key)
			{
				return false;
			}
			else
			{
				LFNODE *node = new LFNODE(key);
				node->next.set(curr, false);
				
				//node->next= new LFNODE(key);
				/* 초기화를 잘했다면 굳이 할 필요가 없음. */
				node->next.TryMarking(curr, false);
			
				if(pred->next.CAS(curr, node, false, false))
					return true;
			}
		}
	}

	bool Remove(int key)
	{
		LFNODE *pred, *curr;

		while (true)
		{
			find(key, pred, curr);

			if (curr->key != key)
			{
				return false;
			}
			else
			{
				LFNODE *succ = curr->next.getptr();
				
				if (false == curr->next.TryMarking(succ, true))
					continue;

				pred->next.CAS(curr, succ, false, false);
				return true;
			}
		}
	}

	bool Contains(int key)
	{
		bool marked = false;

		LFNODE *curr = &head;
		while (curr->key < key)
		{
			curr = curr->next.getptr();
			LFNODE *succ = curr->next.getptr(&marked);
		}
		return curr->key == key && !marked;
	}

	void display20()
	{
		int c = 20;
		LFNODE *p = head.next.getptr();
		while (p != &tail)
		{
			cout << p->key << ", ";
			p = p->next.getptr();
			c--;
			if (c == 0) break;
		}
		cout << endl;
	}
};

const auto NUM_TEST = 4000000;
const auto KEY_RANGE = 100;

LFLIST list;
void ThreadFunc(void *lp)
{
	
	int key;

	int num_thread = *((int*)lp);

	for (int i = 0; i < NUM_TEST / num_thread; ++i)
	{
		switch (rand() % 3)
		{
		case 0:
			key = rand() % KEY_RANGE;
			list.Add(key);
			break;
		case 1:
			key = rand() % KEY_RANGE;
			list.Remove(key);
			break;
		case 2: 
			key = rand() % KEY_RANGE;
			list.Contains(key);
			break;
		default:
			cout << "Errror\n";
			exit(-1);
		}
	}
}

int main()
{
	for (auto n = 1; n <= 16; n *= 2)
	{
		auto start_time = high_resolution_clock::now();
		
		vector<thread> threads;
		list.Init();
		
		for (int i = 0; i < n; ++i)
			threads.emplace_back(ThreadFunc, &n);

		for (auto& thread : threads)
			thread.join();
		
		threads.clear();

		auto end_time = high_resolution_clock::now();

		auto exec_time = end_time - start_time;

		int exec_ms = (int)duration_cast<milliseconds>(exec_time).count();

		cout << "Threads [" << n << "]\n";
		list.display20();
		// 낙천적인 동기화 메모리 해제 함수
		/*list.recycle_freelist();*/
		cout << "Exec_time = " << exec_ms << "ms\n\n";
	}

	system("pause");
}