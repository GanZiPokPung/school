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

	NODE() { next = nullptr; }
	NODE(int key_value)
	{
		next = nullptr;
		key = key_value;
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

const auto NUM_TEST = 4000000;
const auto KEY_RANGE = 1000;

const int num_thread = 16;

FLIST clist;

void ThreadFunc(void *lp)
{
	int key = 0;

	srand(*((unsigned int*)lp));

	for (int i = 0; i < NUM_TEST / num_thread; ++i)
	{
		switch (rand() % 3)
		{
		case 0:
			key = rand() % KEY_RANGE;
			clist.Add(key);
			break;
		case 1:
			key = rand() % KEY_RANGE;
			clist.Remove(key);
			break;
		case 2: 
			key = rand() % KEY_RANGE;
			clist.Contains(key);
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
		clist.Init();
		
		for (int i = 0; i < n; ++i)
			threads.emplace_back(ThreadFunc, &n);

		for (auto& thread : threads)
			thread.join();
		
		threads.clear();

		auto end_time = high_resolution_clock::now();

		auto exec_time = end_time - start_time;

		int exec_ms = (int)duration_cast<milliseconds>(exec_time).count();

		cout << "Threads [" << n << "]\n";
		clist.display20(); 
		cout << "\n Exec_time = Time = " << exec_ms << "ms\n";
	}

	system("pause");
}