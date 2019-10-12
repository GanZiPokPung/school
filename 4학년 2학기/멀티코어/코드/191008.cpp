#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
using namespace std;
using namespace chrono;

class NODE
{
public:
	int key;
	NODE *next;
	mutex n_lock;
	bool marked;

	NODE() { next = nullptr; marked = false; }
	NODE(int key_value)
	{
		next = nullptr;
		key = key_value;
		marked = false;
	}

	~NODE() {}
	void lock()
	{
		n_lock.lock();
	}
	void unlock()
	{
		n_lock.unlock();
	}
};

// 성긴 동기화
class CLIST 
{
	NODE head, tail;
	mutex glock;
public:
	CLIST()
	{
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.next = &tail;
	}
	~CLIST() {}

	void Init()
	{
		NODE *ptr;
		while (head.next != &tail)
		{
			ptr = head.next;
			head.next = head.next->next;
			delete ptr;
		}
	}

	bool Add(int key)
	{
		NODE *pred, *curr;

		pred = &head;
		glock.lock();
		curr = pred->next;
		while (curr->key < key)
		{
			pred = curr;
			curr = curr->next;
		}

		if(key == curr->key)
		{
			glock.unlock();
			return false;
		}
		else
		{
			NODE *node = new NODE(key);
			node->next = curr;
			pred->next = node;
			glock.unlock();
			return true;
		}
	}

	bool Remove(int key)
	{
		NODE *pred, *curr;

		pred = &head;
		glock.lock();
		curr = pred->next;
		while (curr->key < key)
		{
			pred = curr;
			curr = curr->next;
		}

		if (key == curr->key)
		{
			pred->next = curr->next;
			delete curr;
			glock.unlock();
			return true;
		}
		else
		{
			glock.unlock();
			return false;
		}
	}

	bool Contains(int key)
	{
		NODE *pred, *curr;

		pred = &head;
		glock.lock();
		curr = pred->next;
		while (curr->key < key)
		{
			pred = curr;
			curr = curr->next;
		}

		if (key == curr->key)
		{
			glock.unlock();
			return true;
		}
		else
		{
			glock.unlock();
			return false;
		}
	}

	void display20()
	{
		int c = 20;
		NODE *p = head.next;
		while (p != &tail)
		{
			cout << p->key << ", ";
			p = p->next;
			c--;
			if (c == 0) break;
		}
		cout << endl;
	}
};

// 세밀한 동기화
class FLIST
{
	NODE head, tail;
public:
	FLIST()
	{
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.next = &tail;
	}
	~FLIST() {}

	void Init()
	{
		NODE *ptr;
		while (head.next != &tail)
		{
			ptr = head.next;
			head.next = head.next->next;
			delete ptr;
		}
	}

	bool Add(int key)
	{
		NODE *pred, *curr;

		head.lock();
		pred = &head;
		curr = pred->next;
		curr->lock();
		while (curr->key < key)
		{
			pred->unlock();
			pred = curr;
			curr = curr->next;
			curr->lock();
		}

		if (key == curr->key)
		{
			curr->unlock();
			pred->unlock();
			return false;
		}
		else
		{
			NODE *node = new NODE(key);
			node->next = curr;
			pred->next = node;
			curr->unlock();
			pred->unlock();
			return true;
		}
	}

	bool Remove(int key)
	{
		NODE *pred, *curr;

		head.lock();
		pred = &head;
		curr = pred->next;
		curr->lock();
		while (curr->key < key)
		{
			pred->unlock();
			pred = curr;
			curr = curr->next;
			curr->lock();
		}

		if (key == curr->key)
		{
			pred->next = curr->next;
			curr->unlock();
			pred->unlock();
			delete curr;
			return true;
		}
		else
		{
			curr->unlock();
			pred->unlock();
			return false;
		}
	}

	bool Contains(int key)
	{
		NODE *pred, *curr;

		head.lock();
		pred = &head;
		curr = pred->next;
		curr->lock();
		while (curr->key < key)
		{
			pred->unlock();
			pred = curr;
			curr = curr->next;
			curr->lock();
		}

		if (key == curr->key)
		{
			curr->unlock();
			pred->unlock();
			return true;
		}
		else
		{
			curr->unlock();
			pred->unlock();
			return false;
		}
	}

	void display20()
	{
		int c = 20;
		NODE *p = head.next;
		while (p != &tail)
		{
			cout << p->key << ", ";
			p = p->next;
			c--;
			if (c == 0) break;
		}
		cout << endl;
	}
};

// 낙천적인 동기화
class OLIST
{
	NODE head, tail;
	NODE *freelist;
	NODE freetail;
	mutex fl_mutex;
public:
	OLIST()
	{
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.next = &tail;
		freetail.key = 0x7FFFFFFF;
		freelist = &freetail;
	}
	~OLIST() {}

	void Init()
	{
		NODE *ptr;
		while (head.next != &tail)
		{
			ptr = head.next;
			head.next = head.next->next;
			delete ptr;
		}
	}

	// 쓰지 않는 노드들의 모음을 메모리를 해제
	// 멀티 스레드에서 돌아가지 않을때 호출해야 한다.
	void recycle_freelist()
	{
		NODE *p = freelist;
		while (p != &freetail)
		{
			NODE *n = p->next;
			delete p;
			p = n;
		}
		freelist = &freetail;
	}

	bool Add(int key)
	{
		NODE *pred, *curr;

		while (true)
		{
			pred = &head;
			curr = pred->next;
			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				if (key == curr->key)
				{
					pred->unlock(); curr->unlock();
					return false;
				}
				else
				{
					NODE *node = new NODE(key);
					node->next = curr;
					pred->next = node;
					pred->unlock(); curr->unlock();
					return true;
				}
			}
			else
			{
				pred->unlock(); curr->unlock();
				continue;
			}
		}
	}

	bool Remove(int key)
	{
		NODE *pred, *curr;

		while (true)
		{
			pred = &head;
			curr = pred->next;
			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				if (curr->key == key)
				{
					pred->next = curr->next;
					fl_mutex.lock();
					curr->next = freelist;
					freelist = curr;
					fl_mutex.unlock();
					pred->unlock(); curr->unlock();
					return true;
				}
				else
				{
					pred->unlock(); curr->unlock();
					return false;
				}
			}
			else
			{
				pred->unlock(); curr->unlock();
				continue;
			}
		}
	}

	bool Contains(int key)
	{
		while (true)
		{
			NODE *pred = &head;
			NODE *curr = pred->next;

			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				pred->unlock(); curr->unlock();
				return (curr->key == key);
			}
			else
			{
				pred->unlock(); curr->unlock();
			}
		}
	}

	// 항상 유효성 검사를 해줘야한다.
	bool validate(NODE *pred, NODE *curr)
	{
		NODE *node = &head;
		while (node->key <= pred->key)
		{
			if (node == pred)
				return pred->next == curr;
			node = node->next;
		}
		return false;
	}

	void display20()
	{
		int c = 20;
		NODE *p = head.next;
		while (p != &tail)
		{
			cout << p->key << ", ";
			p = p->next;
			c--;
			if (c == 0) break;
		}
		cout << endl;
	}
};

// 게으른 동기화
class LLIST
{
	NODE head, tail;	
	
public:
	LLIST()
	{
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.next = &tail;
	}
	~LLIST() {}

	void Init()
	{
		NODE *ptr;
		while (head.next != &tail)
		{
			ptr = head.next;
			head.next = head.next->next;
			delete ptr;
		}
	}

	bool Add(int key)
	{
		NODE *pred, *curr;

		while (true)
		{
			pred = &head;
			curr = pred->next;
			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				if (key == curr->key)
				{
					pred->unlock(); curr->unlock();
					return false;
				}
				else
				{
					NODE *node = new NODE(key);
					node->next = curr;
					pred->next = node;
					pred->unlock(); curr->unlock();
					return true;
				}
			}
			else
			{
				pred->unlock(); curr->unlock();
				continue;
			}
		}
	}

	bool Remove(int key)
	{
		NODE *pred, *curr;

		while (true)
		{
			pred = &head;
			curr = pred->next;
			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				if (curr->key == key)
				{
					curr->marked = true;
					pred->next = curr->next;
					pred->unlock(); curr->unlock();
					return true;
				}
				else
				{
					pred->unlock(); curr->unlock();
					return false;
				}
			}
			else
			{
				pred->unlock(); curr->unlock();
				continue;
			}
		}
	}

	bool Contains(int key)
	{
		NODE *curr = &head;
		while (curr->key < key)
		{
			curr = curr->next;
		}

		return curr->key == key && !curr->marked;
	}

	// 유효성 검사
	bool validate(NODE *pred, NODE *curr)
	{
		return !pred->marked && !curr->marked && pred->next == curr;
	}

	void display20()
	{
		int c = 20;
		NODE *p = head.next;
		while (p != &tail)
		{
			cout << p->key << ", ";
			p = p->next;
			c--;
			if (c == 0) break;
		}
		cout << endl;
	}
};

class SPTR_NODE
{
public:
	int key;
	shared_ptr<SPTR_NODE> next;
	mutex n_lock;
	bool marked;

	SPTR_NODE() { next = nullptr; marked = false; }
	SPTR_NODE(int key_value)
	{
		next = nullptr;
		key = key_value;
		marked = false;
	}

	~SPTR_NODE()
	{

	}

	void lock()
	{
		n_lock.lock();
	}
	void unlock()
	{
		n_lock.unlock();
	}
};

// Shared Ptr를 사용한 게으른 동기화
class SPTR_LLIST
{
	shared_ptr<SPTR_NODE> head;
	shared_ptr<SPTR_NODE> tail;

public:
	SPTR_LLIST()
	{
		head = make_shared<SPTR_NODE>();
		tail = make_shared<SPTR_NODE>();
		head->key = 0x80000000;
		tail->key = 0x7FFFFFFF;
		head->next = tail;
	}
	~SPTR_LLIST() {}

	void Init()
	{
		head->next = tail;
	}

	bool Add(int key)
	{
		shared_ptr<SPTR_NODE> pred, curr;

		while (true)
		{
			pred = head;
			curr = pred->next;
			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				if (key == curr->key)
				{
					pred->unlock(); curr->unlock();
					return false;
				}
				else
				{
					shared_ptr<SPTR_NODE> node = make_shared<SPTR_NODE>(key);
					node->next = curr;
					pred->next = node;
					pred->unlock(); curr->unlock();
					return true;
				}
			}
			else
			{
				pred->unlock(); curr->unlock();
				continue;
			}
		}
	}

	bool Remove(int key)
	{
		shared_ptr<SPTR_NODE> pred, curr;

		while (true)
		{
			pred = head;
			curr = pred->next;
			while (curr->key < key)
			{
				pred = curr; curr = curr->next;
			}

			pred->lock(); curr->lock();
			if (validate(pred, curr))
			{
				if (curr->key == key)
				{
					curr->marked = true;
					pred->next = curr->next;
					pred->unlock(); curr->unlock();
					return true;
				}
				else
				{
					pred->unlock(); curr->unlock();
					return false;
				}
			}
			else
			{
				pred->unlock(); curr->unlock();
				continue;
			}
		}
	}

	bool Contains(int key)
	{
		shared_ptr<SPTR_NODE> curr = head;
		while (curr->key < key)
		{
			curr = curr->next;
		}

		return curr->key == key && !curr->marked;
	}

	// 유효성 검사
	bool validate(shared_ptr<SPTR_NODE> pred, shared_ptr<SPTR_NODE> curr)
	{
		return !pred->marked && !curr->marked && pred->next == curr;
	}

	void display20()
	{
		int c = 20;
		shared_ptr<SPTR_NODE> p = head->next;
		while (p != tail)
		{
			cout << p->key << ", ";
			p = p->next;
			c--;
			if (c == 0) break;
		}
		cout << endl;
	}
};

const auto NUM_TEST = 40000;
const auto KEY_RANGE = 1000;

SPTR_LLIST list;

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