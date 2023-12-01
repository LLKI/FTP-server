#include "XFtpSTOR.h"
#include "testUtil.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>
using namespace std;

//�������������еĶ��¼������ڽ��տͻ����ϴ����ļ����ݡ�
//ͨ�� bufferevent_read ���������Ӷ�ȡ���ݣ�������д���ļ��� fp �С�
//ʹ����һ������ѭ�������϶�ȡ���ݣ�ֱ����ȡ�������ݳ���С�ڵ��� 0����ʾ���ݶ�ȡ��ɡ�
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

//�������������е��¼��������ӹرա�����ȡ�
//����¼����� EOF���ļ���������ERROR�����󣩻� TIMEOUT����ʱ������ر��������ӡ����������Ϣ����������Ӧ������
//����¼����� CONNECTED�����ӳɹ������������Ӧ��Ϣ��
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
�ú������ڽ����ͻ��˷��͹�����STOR ����
������������е��ļ�������ƴ�ӳ��������ļ�·����
�����Զ�����д��ģʽ���ļ������ɹ��򴥷��������ӣ��������ļ� OK ����Ӧ��ͬʱ�����������Ӷ��¼���
����ļ���ʧ�ܣ������ļ���ʧ�ܵ���Ӧ��
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

