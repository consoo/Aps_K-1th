#pragma once

#include <afxmt.h>

template<class T>
class CQueue
{
private:
	struct NODE
	{
		T pPacket;
		NODE* pNext;
	};

	NODE* m_pHeadNode;	// �Ӹ� ��� ������
	NODE* m_pTailNode;	// ���� ��� ������

	int m_nNodeCount;	// ť�� �ִ� ��Ʈ�� ����
	CCriticalSection m_clCritSect;

public:
	CQueue()
	{
		m_pHeadNode = NULL;
		m_pTailNode = NULL;

		m_nNodeCount = 0;
	};

	~CQueue()
	{
		this->RemoveAll();
	};

	bool Push(T pPacket)
	{
		m_clCritSect.Lock();

		// ��带 �����.
		NODE* pNode = new NODE;
		if (pNode == NULL)
		{
			m_clCritSect.Unlock();
			return false;
		}

		// �� ��带 �����Ѵ�.
		pNode->pPacket = pPacket;
		pNode->pNext = NULL;

		// �ֺ� ť���� Ȯ���Ѵ�.
		if (m_pHeadNode == NULL)
		{
			// �Ӹ����� ������尡 ��� �� ��带 ����Ų��.
			m_pHeadNode = pNode;
			m_pTailNode = pNode;
		}
		else
		{
			// �������� �� ��带 �����Ѵ�.
			m_pTailNode->pNext = pNode;
			// �� ��尡 ������尡 �ȴ�.
			m_pTailNode = pNode;
		}

		// ť�� ��� ������ �����Ѵ�.
		m_nNodeCount++;

		m_clCritSect.Unlock();

		return true;
	};

	T Pop(void)
	{
		m_clCritSect.Lock();

		// �ֺ� ť���� Ȯ���Ѵ�.
		if (m_pHeadNode == NULL)
		{
			m_clCritSect.Unlock();
			return NULL;
		}

		T pPacket;

		// �Ӹ���� �����͸� ���� �����Ѵ�.
		NODE* pTempHead = m_pHeadNode;

		// �Ӹ������ ��Ŷ �����͸� �Ѱ��ش�.
		pPacket = m_pHeadNode->pPacket;

		// �Ӹ���尡 ����Ű�� ��尡 �� �Ӹ���尡 �ȴ�.
		m_pHeadNode = m_pHeadNode->pNext;
		// �� �Ӹ���尡 NULL�̸� ������嵵 NULL�̴�.
		if (m_pHeadNode == NULL)
		{
			m_pTailNode = NULL;
		}

		// ���� �Ӹ���带 �����Ѵ�.
		delete pTempHead;

		// ť�� ��� ������ �����Ѵ�.
		m_nNodeCount--;

		m_clCritSect.Unlock();

		return pPacket;
	};

	void RemoveAll(void)
	{
		T pPacket;

		// ��� ��带 POP�ϰ� ��Ŷ �޸𸮵� �����Ѵ�.
		while (true)
		{
			pPacket = this->Pop();

			if (pPacket == NULL)
			{
				break;
			}
			else
			{
				delete pPacket;
			}
		}
	};

	int GetCount(void)
	{
		int nRetVal;

		m_clCritSect.Lock();
		nRetVal = m_nNodeCount;
		m_clCritSect.Unlock();

		return nRetVal;
	};
};