#pragma once
#include <event2/util.h>
#include <list>
#include <mutex>
class XTask;
struct event_base;
class XThread
{
public:
	// 启动线程
	void Start();

	// 线程入口函数
	void Main();

	// 安装线程，初始化evevnt_base和管道监听事件用于激活
	bool Setup();

	// 收到主线程发出的激活消息（线程池任务分发）
	void Notify(evutil_socket_t, short);

	// 线程激活
	void Activate();

	// 添加任务, 一个线程可以同时处理多个任务，共用一个event_base
	void AddTack(XTask *);

	XThread();
	~XThread();

	// 线程编号
	int id = 0;

private:
	int notify_send_fd = 0;
	event_base *base = 0;
	std::list<XTask*> tasks;
	std::mutex tasks_mutex;
};

/*
实现了一个简单的线程池,每个线程（XThread）都有一个独立的事件库（event_base).以及一个用于激活线程的管道。
主线程通过向某个线程的管道写入数据来激活线程。
激活的线程从任务队列中取出任务进行处理。
每个线程都有独立的 event_base，因此能够并行处理多个任务。
*/
