#pragma once
#include "XFtpTask.h"
class XFtpRETR :
	public XFtpTask
{
	//RETR:�ӷ������������ļ�
	void Parse(std::string type, std::string msg);// ���� RETR �����ȡ�ļ��������ļ���׼������
	virtual void Event(bufferevent *, short);//�������ļ�������ص��¼��������ӳɹ������������
	virtual void Write(bufferevent *);//�����ļ������д�¼������ļ���ȡ���ݲ�ͨ������ͨ������

	bool Init() { return true; }


private:
	char buf[1024*1024] = {0};
};

