#pragma once
#include <vector>

class XThread;
class XTask;
class XThreadPool
{
public:
	// ����ģʽ
	static XThreadPool *Get() {
		static XThreadPool p;
		return &p;
	}
	// ��ʼ�������߳�
	void Init(int threadCount);

	// �ַ������̳߳��е�ĳ���߳�
	void Dispatch(XTask*);
private:
	int threadCount;
	int lastThread = -1;
	std::vector<XThread *> threads;
	XThreadPool() {};
};

/*
��������̳߳أ������˵���ģʽ��ȷ������Ӧ�ó���ֻ��һ���̳߳�ʵ��

����ͨ�� XThreadPool::Get() ��ȡ�̳߳ص�ʵ����
���� Init ������ʼ���̳߳أ�����������ָ���������̡߳�
����Ҫִ�е����񴦵��� Dispatch ������������ַ����̳߳��е�ĳ���̡߳�
�����ڱ��ַ����߳���ִ�У�ͨ�� XTask �ඨ��Ĵ����߼������Ӧ�Ĺ�����
*/

