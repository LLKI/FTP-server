#pragma once
#include "XFtpTask.h"
class XFtpSTOR :public XFtpTask
{

public:
	void Read(bufferevent *);//��д�˻�����麯�������ڴ������������еĶ��¼����� FTP �� STOR �����У��������������ݣ�������Ҫʵ�ֶ�Ӧ�Ķ�ȡ�߼�

	void Event(bufferevent *, short );//��д�˻�����麯�������ڴ������������е��¼����������������Ҫ���ݾ����������д������紦�����ӹرյ��¼�

	void Parse(std::string , std::string );//��д�˻�����麯�������ڽ��� FTP ����

private:
	char buf[1024*1024] = {0};//�洢���ݵĻ���������СΪ 1 MB
};

