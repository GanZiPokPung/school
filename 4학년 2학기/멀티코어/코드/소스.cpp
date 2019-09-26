#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
using namespace std;
using namespace chrono;

volatile int sum;
mutex ml;

// 
atomic<int> victim{ 0 };
atomic<bool> flag[2]{ false, false };

// 피터슨
void plock(int myid)
{
	int other = 1 - myid;
	flag[myid] = true;
	victim = myid;
	//_asm mfence;
	//atomic_thread_fence(memory_order_seq_cst);
	while ((true == flag[other]) && (victim == myid));
}
void punlock(int myid)
{
	flag[myid] = false;
}

// CAS
volatile int x;		// 0 -> Lock이 free이다. 
					// 1 -> 누군가 Lock을 얻어서 CS을 실행중
bool CAS(volatile int *addr, int exp, int up)
{
	// addr이 exp와 같으면 up을 쓰고 true 아니면 안쓰고 false
	return atomic_compare_exchange_strong(
		reinterpret_cast<volatile atomic_int*>(addr), &exp, up);
}

void CASLock()
{
	// 
	while (false == CAS(&x, 0, 1));
}

void CASUnlock()
{
	// fence
	// _asm mfence;
	// atomic_thread_fence(memory_order_seq_cst);
	// 0이 
	x = 0;
}

void do_work(int num_threads)
{
	for (int i = 0; i < 50000000 / num_threads; ++i)
	{
		sum += 2;
	}
}

void do_work2(int num_threads, int myid)
{
	for (int i = 0; i < 50000000 / num_threads; ++i)
	{
		plock(myid);
		sum += 2;
		punlock(myid);
	}
}

void do_work3(int num_threads)
{
	for (int i = 0; i < 50000000 / num_threads; ++i)
	{
		CASLock();
		sum += 2;
		CASUnlock();
	}
}


int main()
{
	for (auto n = 1; n <= 16; n *= 2)
	{
		sum = 0;
		vector<thread> threads;

		auto start_time = high_resolution_clock::now();

		for (int i = 0; i < n; ++i)
			threads.emplace_back(do_work3, n);

		for (auto& thread : threads)
			thread.join();

		threads.clear();

		auto end_time = high_resolution_clock::now();

		auto exec_time = end_time - start_time;

		int exec_ms = (int)duration_cast<milliseconds>(exec_time).count();

		cout << "Threads [" << n << "] " << "Sum = " << sum << " Exec_time = Time = " << exec_ms << "ms\n";
	}

	system("pause");
}