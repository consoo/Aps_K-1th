#pragma once
#include <afxmt.h>


class CFlowProcess : public CWnd
{
	DECLARE_DYNAMIC(CFlowProcess)

public:
	CFlowProcess();
	virtual ~CFlowProcess();

protected:
	DECLARE_MESSAGE_MAP()

public:
	double	dProcessTime;

	int RunProc_ProductLoading(int iUseStep);		//! �۾��� ��ǰ ���� �ڵ� Step(10000 ~ )
	int RunProc_PCBOutsideAlign(int iUseStep);		//! PCB �ܺ� Align Step
	int RunProc_EpoxyResing(int iUseStep);			//! Epoxy ���� �� ���� �˻� Step
	int	RunProc_LensOutSideAlign(int iUseStep);		//! Lens �ܺ� Align Step
	int RunProc_LaserMeasure(int iUseStep);			//! ����/�ܺ� Laser ���� Step

	int RunProc_OisAf(int iUseStep);				//! OIS (50000 ~
	int RunProc_AutoFocus(int iUseStep);			//! Active Align ����(60000 ~
	int RunProc_UV_Process(int iUseStep);

	int RunProc_FinalInsp(int iUseStep);			//! �˻� ����(�̹�, ����)
	int RunPorc_emission(int iUseStep);				//! ����ǰ ���� Step
};
