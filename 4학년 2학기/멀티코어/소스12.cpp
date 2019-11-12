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

volatile int victim{ 0 };
volatile bool flag[2]{ false, false };

void plock(int myid)
{
	int other = 1 - myid;
	flag[myid] = true;
	//_asm mfence;
	atomic_thread_fence(memory_order_seq_cst);
	victim = myid;
	while ((true == flag[other]) && (victim == myid));
}
void punlock(int myid)
{
	flag[myid] = false;
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

int main()
{

	for (int num_threads = 1; num_threads <= 2; num_threads *= 2)
	{
		sum = 0;
		vector<thread> threads;

		auto start_time = high_resolution_clock::now();

		for (int i = 0; i < num_threads; ++i)
			threads.emplace_back(do_work2, num_threads, i);

		for (auto& thread : threads)
			thread.join();

		threads.clear();

		auto end_time = high_resolution_clock::now();

		auto exec_time = end_time - start_time;

		int exec_ms = duration_cast<milliseconds>(exec_time).count();

		cout << "Threads [" << num_threads << "] " << "Sum = " << sum << "	Exec_time = Time = " << exec_ms << "ms\n";
	}

}