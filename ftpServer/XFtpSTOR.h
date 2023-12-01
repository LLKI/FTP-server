#pragma once
#include "XFtpTask.h"
class XFtpSTOR :public XFtpTask
{

public:
	void Read(bufferevent *);//重写了基类的虚函数，用于处理数据连接中的读事件。在 FTP 的 STOR 命令中，服务器接收数据，所以需要实现对应的读取逻辑

	void Event(bufferevent *, short );//重写了基类的虚函数，用于处理数据连接中的事件。这个函数可能需要根据具体的情况进行处理，比如处理连接关闭等事件

	void Parse(std::string , std::string );//重写了基类的虚函数，用于解析 FTP 命令

private:
	char buf[1024*1024] = {0};//存储数据的缓冲区，大小为 1 MB
};

