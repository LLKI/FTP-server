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

	//����������ͨ����cmdTask��������Ӧ��Ϣ���� FTP Э���У�������ͨ����Ҫ��ͻ��˷���һЩ��Ӧ��Ϣ
	void ResCMD(string msg);

	//���ڽ����������ӣ����а������� bufferevent�����ó�ʱʱ�䡢���� socket ���ӵȲ���
	//��������ͨ�������ļ��������Ҫ�������ݽ��������
	void ConnectoPORT();

	//���ڹر��������ӣ��ͷ������Դ
	void ClosePORT();

	//���ڷ������ݵ���Ӧ�� bufferevent�������ͨ�� bufferevent_write ����������д�뵽���绺����
	void Send(const string& data);
	void Send(const char *data, size_t datasize);

	virtual void Event(bufferevent *, short) {}
	virtual void Read(bufferevent *) {}
	virtual void Write(bufferevent *) {}

	//�������� bufferevent �Ļص���������������д���¼��ص�����
	void Setcb(struct bufferevent*);
	bool Init() { return true; }

	//�ͷ���Դ���ر���������
	~XFtpTask();

protected:
	//�����������ֱ���Ϊ bufferevent ���¼��ص����������ڴ����¼�����ȡ���ݺ�д������
	static void EventCB(bufferevent *, short, void *);
	static void ReadCB(bufferevent *, void *);
	static void WriteCB(bufferevent *, void *);

	bufferevent *bev = 0;//libevent �е� bufferevent �ṹ�����ڴ������� I/O �¼�
	FILE *fp = 0;//�Ǳ�׼ C ���е��ļ�ָ�룬�����ļ� I/O ����
};

/*
XFtpTask ����չ�� XTask �࣬����� FTP Э����ص�������ļ������������� FTP �������У�XFtpTask ���ʵ���������Ŀͻ������󣬶� XTask ���ṩ��һ���Ե�����
*/