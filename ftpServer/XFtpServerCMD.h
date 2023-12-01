#pragma once
#include "XFtpTask.h"

#include <map>

class XFtpServerCMD : public XFtpTask
{
public:
	// 初始化任务
	virtual bool Init();

	virtual void Event(bufferevent *be, short events);

	virtual void Read(bufferevent *be);//处理命令通道上的读取事件，将收到的命令分发到相应的处理对象进行解析

	// 注册命令处理对象，不需要考虑线程安全，调用时未分发到线程
	void Reg(std::string, XFtpTask *call);


	XFtpServerCMD();
	~XFtpServerCMD();
private:
	std::map<std::string, XFtpTask*>calls;// 存储不同类型命令对应的处理对象
	std::map<XFtpTask*, int>calls_del;// 存储待删除的处理对象
};

