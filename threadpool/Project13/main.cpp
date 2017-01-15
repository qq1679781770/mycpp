#include"ThreadPool.h"
int main()
{
	ThreadPool pool;
	std::thread th1([&pool]
	{
		for (int i = 0; i < 10; ++i)
		{
			auto thdId = std::this_thread::get_id();
			pool.AddTask([thdId] {std::cout << "同步线程1的ID：" << thdId << std::endl; });
		}
	});
	std::thread th2([&pool]
	{
		for (int i = 0; i < 10; ++i)
		{
			auto thdId = std::this_thread::get_id();
			pool.AddTask([thdId] {std::cout << "同步线程1的ID：" << thdId << std::endl; });
		}
	});
	std::this_thread::sleep_for(std::chrono::seconds(2));
	getchar();
	pool.Stop();
	th1.join();
	th2.join();
	getchar();
	return 0;
}