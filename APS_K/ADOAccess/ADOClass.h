  // ADOClass.h: interface for the CADOClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOCLASS_H__3672C5FF_18F6_487D_B1D3_CBE7C810D401__INCLUDED_)
#define AFX_ADOCLASS_H__3672C5FF_18F6_487D_B1D3_CBE7C810D401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CADOClass  
{
public:
	
	CADOClass();
	virtual ~CADOClass();

	strdefADO	adoRegist;

	CCriticalSection	m_csAdoProcPtr;	  //! DataBase ����

	bool	bADOConnect;
	BOOL func_AA_DBConnect();			//DB ����
	void func_AA_DBDisConnect();		//DB ���� ����

	bool func_AATaskToRecordID(CString sUseModel, CString sBarcodeID);		//���ڵ� �߰�
	bool func_AATaskToRecordLaser(CString sBarcodeID, double dOffsetX, double dOffsetY, double dValue[8]);						//���ڵ� ���� - Laser ���� ����	
	bool func_AATaskToRecordAlign(CString sBarcodeID,  double dOffsetX, double dOffsetY, double dOffsetT);						//���ڵ� ���� - Align (Lans, Holder�� Align ������)
	bool func_AATaskToAAPcbMoter(CString sBarcodeID, double dPCBX, double dPCBY, double dPCBTh, double dPCBTx, double dPCBTy, double dPCBZ);

	bool func_Get_RecordData(CString sBarcodeID);							//���ڵ� �б� -> ���ڵ������.

	bool func_Set_DeleteRecord(int nDeleteDay);								//���ڵ� ���� -> ����.

	// -------------------------------------------------------------
	// RS���� �ʿ��� ������ Ÿ���� VARIANTARG�� �����Ͽ� �߰��ϼ���
	// -------------------------------------------------------------

	// Record Getdata(int)
	BOOL GetRsDataInt(LPCTSTR lpszColumn, INT& nValue);

	// Record Getdata(CString)
	BOOL GetRsDataChar(LPCTSTR lpszColumn, CString& csValue);

	// Record Getdata(char*)
	BOOL GetRsDataChar(LPCTSTR lpszColumn, char* pszValue);

	// Record Getdata(long)
	BOOL GetRsDataLong(LPCTSTR lpszColumn, LONG& lValue);

	// Record GetdateTime(CTime)
	BOOL GetRsDataTime(LPCTSTR lpszColumn, CTime &cTime);

	// Record GetdateTime(BOOL)
	BOOL GetRsDataBool(LPCTSTR lpszColumn, BOOL &bValue);

	// Record Getdata(float)
	BOOL GetRsDataFloat(LPCTSTR lpszColumn, float& fValue);

	// Record Getdata(double)
	BOOL GetRsDataDouble(LPCTSTR lpszColumn, double& dValue);

	// -------------------------------------------------------------


	// RS EOF���� ����
	BOOL IsEOF();
	
	// RS(Recordset) ���������� �̵�
	void LastRS();

	// RS �������� �̵�
	void PrevRS();

	// RS �������� �̵�
	void NextRS();

	// RS ó������ �̵�
	void FirstRS();

	// Close ���ڵ� ��
	BOOL CloseRS();

	// ������ �����Ͽ� ���ڵ���� �̾Ƴ���.(Command ��ü �̿�) - �ӵ� ����, �ߺ� ���� ó�� �ȵ�
	bool ExecuteQryCmd(LPCSTR lpszQry, long lop=adCmdUnknown);

	// ������ �����Ͽ� ���ڵ���� �̾Ƴ���.(RecordSet ��ü �̿�) - �ӵ� ����, �ߺ� ���� ó�� ����
	bool ExecuteQryRs(CString lpszQry, CursorTypeEnum emCurType=adOpenStatic, LockTypeEnum emLockType=adLockReadOnly, long lop=adCmdUnknown);

	// ������ �����Ѵ�.(Insert, Update, Delete�� : Command��ü �̿�)
	bool InUpDelQryCmd(LPCSTR lpszQry);

	// ������ �����Ѵ�.(Insert, Update, Delete�� : Connection ��ü �̿�)
	bool InUpDelQryConn(LPCSTR lpszQry);

	// DB�� �����Ѵ�.(Open����)
	bool Connect(LPCSTR lpszProvider, ExecuteOptionEnum emOption = adOptionUnspecified);

	// DB�� �ݾ��ش�(m_pConnection, m_pCommand)
	bool DisConnect();

	// Ʈ����� ó��
	LONG BeginTrans() const { return m_pConnection->BeginTrans();}
	HRESULT RollbackTrans() const { return m_pConnection->RollbackTrans();}
	HRESULT CommitTrans() const { return m_pConnection->CommitTrans();}

	// ���� ���ڵ� ����� �ҷ����� �Լ�
	int NextRecord(void);

	// �ش� Į������ ���͸��ϴ� �Լ�
	int setFilter(_bstr_t strField, _bstr_t strFilter);

	// ���ڵ���� �����ϴ� �Լ�
	long GetRecordCount(void);

private:
	_RecordsetPtr m_pRs;
	_CommandPtr m_pCommand;
	_ConnectionPtr m_pConnection;

	// ���� ���� �Լ�(����ó����)
	void DumpError(const _com_error &e) const;




};

#endif // !defined(AFX_ADOCLASS_H__3672C5FF_18F6_487D_B1D3_CBE7C810D401__INCLUDED_)
