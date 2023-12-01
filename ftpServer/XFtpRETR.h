#pragma once
#include "XFtpTask.h"
class XFtpRETR :
	public XFtpTask
{
	//RETR:从服务器上下载文件
	void Parse(std::string type, std::string msg);// 解析 RETR 命令，提取文件名，打开文件并准备传输
	virtual void Event(bufferevent *, short);//处理与文件传输相关的事件，如连接成功、传输结束等
	virtual void Write(bufferevent *);//处理文件传输的写事件，从文件读取数据并通过命令通道发送

	bool Init() { return true; }


private:
	char buf[1024*1024] = {0};
};

