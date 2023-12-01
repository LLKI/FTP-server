#pragma once
#include <vector>

class XThread;
class XTask;
class XThreadPool
{
public:
	// 单例模式
	static XThreadPool *Get() {
		static XThreadPool p;
		return &p;
	}
	// 初始化所有线程
	void Init(int threadCount);

	// 分发任务到线程池中的某个线程
	void Dispatch(XTask*);
private:
	int threadCount;
	int lastThread = -1;
	std::vector<XThread *> threads;
	XThreadPool() {};
};

/*
负责管理线程池，采用了单例模式，确保整个应用程序只有一个线程池实例

程序通过 XThreadPool::Get() 获取线程池的实例。
调用 Init 函数初始化线程池，创建并启动指定数量的线程。
在需要执行的任务处调用 Dispatch 函数，将任务分发给线程池中的某个线程。
任务在被分发的线程中执行，通过 XTask 类定义的处理逻辑完成相应的工作。
*/

