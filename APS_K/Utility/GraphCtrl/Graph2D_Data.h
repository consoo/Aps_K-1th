//! 2D ������ǥ �׷��� ��Ʈ���� �׷��� ������ Ŭ����
//! Maked by LHW (2008/3/14 ~ )
//! ���� �۾��� [2009/6/4]

#pragma once

#include <vector>
#include <algorithm>	//! Added by LHW [2008/4/10]

//! Added by LHW [2008/4/10]
struct strDataForSort
{
	int iDataIndex;	//! m_vPoint2D������ ������ index
	double dbData;	//! ������ �� ����� �Ǵ� ��

	strDataForSort()
	{
		InitInfo();
	}

	void InitInfo()
	{
		iDataIndex = -1;
		dbData = 0.;
	}

	strDataForSort& operator=(const strDataForSort &info)  //! = ������ ������
	{
		iDataIndex = info.iDataIndex;
		dbData = info.dbData;

		return *this;
	}	
};

class CGraph2D_Data
{
	//! Modify by LHW [2008/5/23] : iCustomData, crPoint �߰�
	struct strPoint2D
	{
		double dbX;	//! X ������
		double dbY;	//! Y ������

		int iCustomData;
		CRect crPoint;
	};

public:
	CGraph2D_Data(void);
	~CGraph2D_Data(void);

	int m_iElementIndex;	//! ������ ������� index, Modify by LHW [2008/4/11]

	//! ������ �Է� �Լ�
	bool SetPoint2D(double *pdbInputY, int iInputSize);
	bool SetPoint2D(double dbStartX, double dbIncrementX, double *pdbInputY, int iInputSize);
	bool SetPoint2D(double *pdbInputX, double *pdbInputY, int iInputSize);
	bool AddPoint2D(double dbInputX, double dbInputY);

	//! Added by LHW [2008/5/23] : ������ �Է� �Լ�, Custom ������ �Է� ��� �߰�
	bool SetPoint2D(double *pdbInputY, int *piCustomData, int iInputSize);
	bool SetPoint2D(double dbStartX, double dbIncrementX, double *pdbInputY, int *piCustomData, int iInputSize);
	bool SetPoint2D(double *pdbInputX, double *pdbInputY, int *piCustomData, int iInputSize);
	bool AddPoint2D(double dbInputX, double dbInputY, int iCustomData);

	bool ClearBuffer2D();	//! ������ ��� ����
	bool DecideMinMax();	//! �Էµ� �����Ϳ��� m_dbMinX, m_dbMaxX, m_dbMinY, m_dbMaxY�� ã�Ƴ�

	int m_iMaxIndex;	//! m_dbMaxY���� �ش��ϴ� X���� index, Added by LHW [2008/4/15]

	double m_dbMinX;	//! �Էµ� �������� X �ּҰ�
	double m_dbMaxX;	//! �Էµ� �������� X �ִ밪
	double m_dbMinY;	//! �Էµ� �������� Y �ּҰ�
	double m_dbMaxY;	//! �Էµ� �������� Y �ִ밪

	int m_iVisualDataSize;		//! ���� Range �������� ���� ���̴� �������� ����, X�� Range�� ���, Modify by LHW [2009/6/4]
	int m_iVisualDataStartIndex;//! ���� Range �������� ���� ���̴� �������� ���� index (������ ����)
	int m_iVisualDataEndIndex;	//! ���� Range �������� ���� ���̴� �������� ��ħ index (������ ����)
	int	m_iVisualMaxIndex;		//! ���� Range �������� ���� ���̴� ������ �ִ밪�� index

	int m_iVisualRealDataStartIndex;//! ���� Range �������� ���� ���̴� �������� ���� index, Added by LHW [2008/11/18]
	int m_iVisualRealDataEndIndex;	//! ���� Range �������� ���� ���̴� �������� ��ħ index, Added by LHW [2008/11/18]

	int GetPointSize() const;				//! ������ ���� ��ȯ
	double GetXPoint2D(int iIndex) const;	//! �־��� ������ index�� X ������ ��ȯ
	double GetYPoint2D(int iIndex) const;	//! �־��� ������ index�� Y ������ ��ȯ

	//! Added by LHW [2008/5/23] : �׷����� ���� �簢 ������ �����ϰų� ��ȯ
	bool SetDataPointRect(int iIndex, CRect crPoint);
	CRect GetDataPointRect(int iIndex);
	int GetCustomData(int iIndex);

	bool ClearBufferSort();						//! ���Ŀ� ������ ������ ��� ����
	bool SetDataSort(strDataForSort *pstrInputData, int iInputSize);	//! �����ϱ� ���� ������ �Է�
	bool AddDataSort(strDataForSort strInputData);						//! �����ϱ� ���� ������ �߰�
	bool SortProc(bool bAscend = true);			//! �Էµ� �����͸� ����, bAscend�� true�̸� �������� ����
	int GetSortDataSize() const;				//! ���ĵ� ������ ������ ũ�� ��ȯ
	bool SortXDirection(bool bAscend = true);	//! X �����ͷ� ����, bAscend�� true�̸� �������� ����, SetDataSort�� AddDataSort ȣ������ �ʾƵ� ��
	bool SortYDirection(bool bAscend = true);	//! Y �����ͷ� ����, bAscend�� true�̸� �������� ����, SetDataSort�� AddDataSort ȣ������ �ʾƵ� ��
	int GetIndexPoint2D_Sort(int iIndex) const;	//! ���ĵ� ������� �־��� index�� ������ index�� ��ȯ

private:
	std::vector<strPoint2D> m_vPoint2D;		//! �׷����� �׸��� �����͸� �����ϴ� ����
	std::vector<strDataForSort> m_vSortRes;	//! ���� ����� �����ϴ� ����	
};
