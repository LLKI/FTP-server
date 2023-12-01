#include <string.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include <string>
using namespace std;

#include "XFtpServerCMD.h"
#include "testUtil.h"

#define BUFS 4096

void XFtpServerCMD::Reg(std::string cmd, XFtpTask *call) {
	testout("At XFtpServerCMD::Reg");
	if (!call) {
		cout << "XFtpServerCMD::Reg call is null " << endl;
		return;
	}
	if (cmd.empty()) {
		cout << "XFtpServerCMD::Reg cmd is null " << endl;
		return;
	}
	// 已经注册的是否覆盖，不覆盖，提示错误
	if (calls.find(cmd) != calls.end()) {
		cout << cmd << " is alredy register" << endl;
		return;
	}
	testout(cmd << " Reg success");
	call->base = base;
	call->cmdTask = this;// 指向的是调用该函数的 XFtpServerCMD 类的实例  this 是一个指向当前对象的指针，它指向调用成员函数的对象
	calls[cmd] = call;
	calls_del[call] = 0;
}

void XFtpServerCMD::Event(bufferevent *bev, short events) {
	testout("At XFtpServerCMD::Event");
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))//文件结束事件(即连接的一方关闭了连接),错误事件,超时事件
	{
		delete this;
	}
}

/*
1.通过 bufferevent_read 从缓冲区读取数据，存储在 buf 中
2.循环处理读取到的数据，逐个字符处理,提取命令类型，以空格或回车为分隔符，存储在 type 中
3.检查 calls 映射表，看是否有注册过对应类型的命令处理对象
	如果找到注册的处理对象，调用其 Parse 函数进行命令解析
	如果未找到注册的处理对象，返回一个 "200 OK" 的响应
*/
void XFtpServerCMD::Read(bufferevent *bev) {
	cout << endl;
	testout("At XFtpServerCMD::Read");
	char buf[BUFS] = { 0 };
	while (1) {
		int len = bufferevent_read(bev, buf, BUFS);
		if (len <= 0) break;
		cout << "Recv CMD(" << len << "):" << buf;
		// 分发到处理对象
		// 分析出类型
		string type = "";
		for (int i = 0; i < len; i++) {
			if (buf[i] == ' ' || buf[i] == '\r')
				break;
			type += buf[i];
		}

		// 解析命令类型并调用命令对象进行解析
		cout << "type is [" << type << "]" << endl;
		if (calls.find(type) != calls.end()) {
			testout("begin to parse");
			XFtpTask *t = calls[type];
			t->Parse(type, buf);
			testout("curDir: [" << curDir << "]");
		}
		else {
			cout << "parse object not found" << endl;
			ResCMD("200 OK\r\n");
		}
	}
}

bool XFtpServerCMD::Init() {
	testout("At XFtpServerCMD::Init");

	// 在线程的base里添加一个缓冲区对sock的缓冲事件，这就是命令通道
	bufferevent *bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
	if (!bev) {
		delete this;
		return false;
	}

	// 添加超时
	timeval t = {300, 0};
	bufferevent_set_timeouts(bev, &t, 0);

	string msg = "220 Welcome to XFtpServer\r\n";
	bufferevent_write(bev, msg.c_str(), msg.size());

	this->cmdTask = this;
	this->bev = bev;
	// 注册本对象实现的回调函数
	Setcb(bev);

	return true;
}

XFtpServerCMD::XFtpServerCMD() {
}

XFtpServerCMD::~XFtpServerCMD() {
	ClosePORT();
	for (auto i : calls_del) {
		delete i.first;
	}
}
