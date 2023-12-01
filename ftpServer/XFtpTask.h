#pragma once
#include <event2/bufferevent.h>

#include "XTask.h"

#include <string>
using namespace std;

struct bufferevent;

class XFtpTask :public XTask
{
public:
	string curDir = "/";
	string rootDir = "/";
	string ip = "";
	int port = 0;
	XFtpTask *cmdTask = 0;

	virtual void Parse(std::string, std::string) {}

	//用于向命令通道（cmdTask）发送响应消息。在 FTP 协议中，服务器通常需要向客户端发送一些响应消息
	void ResCMD(string msg);

	//用于建立数据连接，其中包括配置 bufferevent、设置超时时间、发起 socket 连接等操作
	//数据连接通常用于文件传输等需要大量数据交换的情况
	void ConnectoPORT();

	//用于关闭数据连接，释放相关资源
	void ClosePORT();

	//用于发送数据到对应的 bufferevent。在这里，通过 bufferevent_write 函数将数据写入到网络缓冲区
	void Send(const string& data);
	void Send(const char *data, size_t datasize);

	virtual void Event(bufferevent *, short) {}
	virtual void Read(bufferevent *) {}
	virtual void Write(bufferevent *) {}

	//用于设置 bufferevent 的回调函数，包括读、写和事件回调函数
	void Setcb(struct bufferevent*);
	bool Init() { return true; }

	//释放资源，关闭数据连接
	~XFtpTask();

protected:
	//这三个函数分别作为 bufferevent 的事件回调函数，用于处理事件、读取数据和写入数据
	static void EventCB(bufferevent *, short, void *);
	static void ReadCB(bufferevent *, void *);
	static void WriteCB(bufferevent *, void *);

	bufferevent *bev = 0;//libevent 中的 bufferevent 结构，用于处理网络 I/O 事件
	FILE *fp = 0;//是标准 C 库中的文件指针，用于文件 I/O 操作
};

/*
XFtpTask 类扩展了 XTask 类，添加了 FTP 协议相关的网络和文件操作函数。在 FTP 服务器中，XFtpTask 类的实例处理具体的客户端请求，而 XTask 类提供了一般性的任务
*/