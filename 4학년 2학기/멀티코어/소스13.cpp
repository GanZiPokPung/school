#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

const int MAX = 50'000'000;

volatile int x, y;
volatile int trace_x[MAX], trace_y[MAX];

void thread_x()
{
	for(int i = 0; i<MAX; ++i)
	{
		x = i;
		//atomic_thread_fence(memory_order_seq_cst);
		trace_y[x] = y;
	}
}

void thread_y()
{
	for (int i = 0; i < MAX; ++i)
	{
		y = i;
		//atomic_thread_fence(memory_order_seq_cst);
		trace_x[y] = x;
	}
}

int main()
{
	int count = 0;
	thread t1{ thread_x };
	thread t2{ thread_y };
	t1.join();
	t2.join();
	for (int i = 0; i < (MAX - 1); ++i)
	{
		if (trace_x[i] != trace_x[i + 1]) continue;

		int x = trace_x[i];

		if (trace_y[x] != trace_y[x + 1]) continue;
		
		if (trace_y[x] == i)
			++count;
	}

	cout << "Number of Error = " << count << endl;
}