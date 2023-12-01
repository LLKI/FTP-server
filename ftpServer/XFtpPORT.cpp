#include "XFtpPORT.h"
#include "testUtil.h"

#include <iostream>
#include <vector>
using namespace std;


/*
解析 FTP 客户端发来的 PORT 命令。PORT 命令用于告知服务器客户端的数据传输地址和端口，以主动模式进行数据连接

解析 msg 字符串： msg 字符串中包含了客户端发送的 PORT 命令信息。通过遍历字符串，将其中的数字和逗号分隔开，并保存在 vals 中

检查参数数量： 检查 vals 中保存的参数数量是否为6，如果不是，表示参数错误，返回 501 Syntax error in parameters or arguments

解析 IP 和端口： 通过拼接 vals 中的数字，得到 IP 地址，同时计算端口号 port

设置 IP 和端口： 将解析得到的 IP 和端口设置在 cmdTask 对象下，以备后续使用

回应客户端： 发送 200 PORT command success. 给客户端，表示 PORT 命令解析成功
*/
void XFtpPORT::Parse(string type, string msg) {
	testout("XFtpPORT::Parse");
	// PORT 127,0,0,1,70,96\r\n
	// PORT n1,n2,n3,n4,n5,n6\r\n
	// port = n5 * 256 + n6
	
	vector<string>vals;
	string tmp = "";
	for (int i = 5; i < msg.size(); i++) {
		if (msg[i] == ',' || msg[i] == '\r') {
			vals.push_back(tmp);
			tmp = "";
			continue;
		}
		tmp += msg[i];
	}
	if (vals.size() != 6) {
		ResCMD("501 Syntax error in parameters or arguments.");
		return;
	}

	// 解析出ip和port，并设置在主要流程cmdTask下
	ip = vals[0] + "." + vals[1] + "." + vals[2] + "." + vals[3];
	port = atoi(vals[4].c_str()) * 256 + atoi(vals[5].c_str());
	cmdTask->ip = ip;
	cmdTask->port = port;
	testout("ip: " << ip);
	testout("port: " << port);
	ResCMD("200 PORT command success.");
}
