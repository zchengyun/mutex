#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string& url)
{
	// 模拟长页面读取
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::string result = "hello world";

	//传入的mutex对象(即它所管理的mutex对象)会被当前线程锁住。在lock_guard对象被析构时，它所管理的mutex对象会自动解锁
	std::lock_guard<std::mutex> guard(g_pages_mutex);
	g_pages[url] = result;
}

int main()
{
	std::thread t1(save_page, "http://foo");
	std::thread t2(save_page, "http://bar");
	t1.join();
	t2.join();

	// 访问g_pages是安全的，因为线程t1/t2生命周期已结束
	for (const auto& pair : g_pages) {
		std::cout << pair.first << " => " << pair.second << '\n';
	}
}
