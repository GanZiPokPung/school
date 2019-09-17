#include <vector> // �ҽ�����
#include <iostream> // �����
#include <chrono> // �ð��� ����
#include <thread> // ������
#include <mutex> // ��ȣ ����

using namespace std;
using namespace std::chrono;

const int MAX_NUM = 1500000;
vector<int> g_primes;
mutex g_pl;
int g_counter;
mutex g_cl;
bool is_prime(int num)
{
	if (1 == num)
	{
		return false;
	}
	if (2 == num)
	{
		return true;
	}
	if (0 == (num % 2))
	{
		return false;
	}

	for (int i = 3; i < num - 1; i += 2)
	{
		if (0 == num % i)
		{
			return false;
		}
	}

	return true;
}

void thread_func() // call by reference
{
	while (true)
	{
		int num;
		{
			if (MAX_NUM < g_counter)
			{
				break;
			}

			lock_guard<mutex> ll(g_cl);
			num = g_counter;
			++g_counter;
		}

		if (true == is_prime(num))
		{
			g_pl.lock();
			g_primes.push_back(num);
			g_pl.unlock();
		}
	}
}

int main()
{
	vector<thread> threads;
	// ���� �ð�
	auto start_time = system_clock::now();

	for (int i = 0; i < 11; ++i)
	{
		threads.emplace_back(thread_func);
	}

	for (auto &th : threads)
	{
		th.join();
	}

	// ���� ���� �ð�
	auto end_time = system_clock::now();
	auto exec_time = end_time - start_time;
	auto exec_ms = duration_cast<milliseconds>(exec_time).count();

	cout << "Number of Prime less than " << MAX_NUM << " is "
		<< g_primes.size() << ".\n";
	cout << "Execution time is " << exec_ms << "ms.\n";
	system("pause");
}