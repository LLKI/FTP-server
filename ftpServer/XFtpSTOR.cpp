#include "XFtpSTOR.h"
#include "testUtil.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>
using namespace std;

//处理数据连接中的读事件，用于接收客户端上传的文件数据。
//通过 bufferevent_read 从数据连接读取数据，将数据写入文件流 fp 中。
//使用了一个无限循环，不断读取数据，直到读取到的数据长度小于等于 0，表示数据读取完成。
void XFtpSTOR::Read(bufferevent *bev) {
	testout("At XFtpSTOR::Read");
	if (!fp) return;
	while (1) {
		int len = bufferevent_read(bev, buf, sizeof(buf));
		if (len <= 0) {
			return;
		}
		fwrite(buf, 1, len, fp);
	}
}

//处理数据连接中的事件，如连接关闭、错误等。
//如果事件包括 EOF（文件结束）、ERROR（错误）或 TIMEOUT（超时），则关闭数据连接、发送完成信息，并触发相应操作。
//如果事件包括 CONNECTED（连接成功），则输出相应信息。
void XFtpSTOR::Event(bufferevent *bev, short events) {
	testout("At XFtpSTOR::Event");
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
		ClosePORT();
		ResCMD("226 Transfer complete");
	}
	else if (events & BEV_EVENT_CONNECTED)
		cout << "XFtpSTOR BEV_EVENT_CONNECTED" << endl;
}

/*
该函数用于解析客户端发送过来的STOR 命令
解析命令参数中的文件名，并拼接成完整的文件路径。
尝试以二进制写入模式打开文件，若成功则触发数据连接，并发送文件 OK 的响应，同时触发数据连接读事件。
如果文件打开失败，发送文件打开失败的响应。
*/
void XFtpSTOR::Parse(std::string type, std::string msg) {
	testout("At XFtpSTOR::Parse");
	int pos = msg.rfind(" ") + 1;
	string filename = msg.substr(pos, msg.size() - pos - 2);
	string path = cmdTask->rootDir + cmdTask->curDir + filename;
	testout("filepath:[" << path << "]");
	fp = fopen(path.c_str(), "wb");
	if (fp) {
		ConnectoPORT();
		ResCMD("125 File OK");
		bufferevent_trigger(bev, EV_READ, 0);
	}
	else {
		ResCMD("450 file open failed!");
	}

}

