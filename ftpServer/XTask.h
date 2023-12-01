#pragma once
class XTask
{
public:
	// 一客户端一个base
	struct event_base *base = 0;

	// 连接的sock
	int sock = 0;

	// 线程池id
	int thread_id = 0;

	// 初始化任务
	virtual bool Init() = 0;
};

/*
存储事件库的信息,每个客户端（或任务）都有一个独立的事件库
*/

