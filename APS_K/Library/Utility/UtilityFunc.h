#pragma once

//! ����(Global) Utility �Լ��� �����Ѵ�. 
//! ����(Global) Utility �Լ��� ������ 'UtilityFunc.cpp' ���Ͽ��� �Ѵ�. 
//! Program�ȿ��� �ݺ������� ���Ǵ� ��ɵ��� ���� �Լ��� �����Ͽ� ����Ѵ�. 
//! Maked by LHW, 2010/6/1
//! Version : 2013/3/17

#include "../../stdafx.h"

//! ���� �ð� ��ȯ
CString GetString_CurrentSystemTime();											
//! ���� �ð� ��ȯ
CString GetString_CurrentLocalTime();
//! ���� �ð� ��ȯ
CString GetString_CurrentTime();									
//! ���� �ð� ��ȯ
CString GetString_CurrentTime_HMS();												
//! ���� �ð� ��ȯ
CString GetString_CurrentTime_AP();												

//! �Էµ� �ð��� ���Ͽ�, �ð����� ��ȯ�Ѵ�.
int CompareSystemTime(SYSTEMTIME strTime1, SYSTEMTIME strTime2);
//! �Էµ� �ð��� �����Ѵ�. (strTime1 = strTime2), strTime2�� �ð��� strTime1�� �Է��Ѵ�. 
void CopySystemTime(SYSTEMTIME strTime_Source, SYSTEMTIME &strTime_Dest);

//! ���ڿ� �Ľ� �Լ�
void ParseString(CStringArray& Token, CString& szOriginal, TCHAR* cpDelimiter);			
//! ���ڿ� �Ľ� �Լ�
void ParseString(CStringArray& Token, CString& szOriginal, TCHAR cDelimiter);			
//! ���ڿ� �и� �Լ�
bool GetSeparateNumber(CString szInput, int &iFirst, int &iSecond, TCHAR* cpDelimiter);				
//! ���ڿ� �и� �Լ�
bool GetSeparateNumber(CString szInput, int &iFirst, double &dbSecond, TCHAR* cpDelimiter);			
//! ���ڿ� �и� �Լ�
bool GetSeparateString(CString szInput, CString &szFirst, CString &szSecond, TCHAR* cpDelimiter);	

//! Text ����(txt, csv ��) �б� �Լ�
int ReadTextFile(CString szFilePath, CStringArray &arrContents);						
//! Text ����(txt, csv ��) ���� �Լ�
int SaveTextFile(CString szFilePath, CString szContent, bool bAppend = false);			
//! Text ����(txt, csv ��) ���� �Լ�
int SaveTextFile(CString szFilePath, CStringArray &arrContents, bool bAppend = false);	
//! Text ����(txt, csv ��) ���� �Լ�
int SaveTextFile(CString szFilePath, TCHAR **pczContents, int iStartIndex, int iSaveTextSize, int iBufSize_1, bool bAppend = false);
//! CFileException�� ���� ���ڿ� ��ȯ
CString GetErrorMsg_FileException(int iErrorValue);										

//! ���α׷��� ����� ������ ��θ� �˾Ƴ���. 
CString GetProgramExeFolderPath();
//! �־��� ��θ��� Driver �̸��� �����Ѵ�. 
CString GetDriveName(CString szPath);														
//! �־��� ��θ��� ���� ���� ��θ� ����
CString GetFolderPath(CString szFilePath);
//! �־��� ��θ��� ���ϸ� (Ȯ���� ����)���� ����
CString GetFileNameExt(CString szFilePath);	
//! �־��� ��θ��� ���� Ȯ���� ���� ����
CString GetFileExt(CString szFilePath);										
//! �־��� ��θ� ����, ������ �����ϸ� true
bool IsExist_Path(CString szInput);		
//! �־��� ���� ��θ� �� �ڿ� "\\" ���ڰ� ������ �߰��Ѵ�. 
CString Append_Char_FolderPath(CString szFolderPath);
//! ���ڿ��� ���� �̸����� �����ϸ� true
bool IsValid_FolderName(CString szInput);													
//! ���ڿ��� ��θ����� �����ϸ� true
bool IsValid_Path(CString szInput);												
//! ������ ���� ����, ���ϱ��� ��� ����
int RemoveFolder(CString szFolderPath);														
//! ���� ������ ����, ���� ����� �����´�. 
int GetList_Sub_FolderInfo(CString szFolderPath, CStringArray &arrInfo, bool bFile = false);
//! ���� ������ ������ ��� ����
int CopyFiles_In_Folder(CString szFolderPath_Source, CString szFolderPath_Dest);
//! �Է��� ���� ������ ���ʴ�� ������ �ִ��� Ȯ���ϰ� ������ ������ �ڿ� ���� �������� ��ȯ
int CreateSubFolders(CString szFolderPath, CStringArray &arrSubFolderName, CString &szLastFolderPath);
//! ��,��,���� ���� ���������� �����Ѵ�. 
int CreateSubFolders(CString szFolderPath, int iYear, int iMonth, int iDay, CString &szLastFolderPath);
//! ��,��,��,�ð��� ���� ���������� �����Ѵ�. 
int CreateSubFolders(CString szFolderPath, int iYear, int iMonth, int iDay, int iHour, CString &szLastFolderPath);

//! ���ڿ��� �Ǽ� �� ��, �Ҽ��� �ڸ����� ��ȯ
int GetDoublePointIndex(CString szInput);											
//! �Ҽ��� �ݿø� �Լ�	
double GetFloor(double dbInput, int iResultMode);									
//! �Ǽ��� �Էµ� �Ҽ��� �ڸ����� ���� ���ڿ��� ��ȯ
CString GetStringFromDouble(double dbInput, int iPrecision);						
//! �Ǽ��� �Ǽ��� ���� �������� ��ȯ
double GetDoubleModulus(double dbInput, double dbDivide, int *piMultiply = NULL);	
//! ���ڿ��� �����ο� �Ҽ��� �ڸ� �κ��� 2���� ������ ��ȯ
bool GetInteger_From_Double(CString szInput, int &iInterger, int &iDecimalPoint);
//! ���ڿ��� �����ο� �Ҽ��� �ڸ� �κ��� �Ǽ��� ��ȯ
bool GetInteger_From_Double(CString szInput, int &iInterger, double &dbDecimalPoint);

//! ���ڿ��� ����(����, �Ǽ� ����)������ Ȯ��
bool IsValid_Digit(CString szInput);											
//! ���ڿ��� �����̸� true
bool IsValid_Integer(CString szInput, bool bMinusChk = false);											
//! ���ڿ��� �Ǽ��̸� true
bool IsValid_Double(CString szInput, bool bMinusChk = false);					
//! ���ڿ��� �����̸� true�� ��ȯ
bool IsValid_Exponential(CString szInput);										
//! ���ڿ��� ����, �����̸� true
bool IsValid_AlphaNumeric(CString szInput);

//! �Էµ� szInput1 ���ڿ����� szInput2 ���ڿ��� ���Ե� ������ �˾Ƴ���. 
int GetCount_String_In_String(CString szInput1, CString szInput2);					

//! �Է��� �������� 2���� �и��Ͽ�, 2���� 8 bit 16������ ��ȯ
void Separate_Hex_From_Integer_8bit(int iInput, int &iHigh, int &iLow);
//! �Է��� 2���� 8 bit 16�������� ���ļ�, 1���� �������� ��ȯ
void Merge_Integer_From_Hex_8bit(int iHigh, int iLow, int &iOutput);
//! �Էµ� 16���� ���ڿ� �ش��ϴ� ���ڸ� ��ȯ
TCHAR GetHexChar(int iInput);
//! 10������ 16���� ���ڷ� ����
long Convert_Decimal_To_Hex(int iInput_Decimal);
//! �Էµ� 16���� ���� ���ڿ��� 16���� ���ڷ� ����
long HexString_to_Num(CString szHex);
long HexString_to_Num(LPTSTR czHex);
//! �Էµ� 16���� ���ڸ� 10���� ���ڷ� ����, Hex -> Dec
int ConvertHexToDec(TCHAR *pszHex);										
//! �Էµ� ���ڿ��� �Ǽ��� ���ڷ� ��ȯ
double Convert_String_Double(CString szInput);	

//! �߽��� iCenterX, iCenterY�� ��ǥ�� �� iWidth, ���� iHeight�� ���� �簢 ������ ��ȯ�Ѵ�. 
CRect makeP2R(int iCenterX, int iCenterY, int iWidth, int iHeight);								

//! pWndofSub�� pWndToCenterOn�� �߽ɿ� ��ġ��Ų��. 
void CenterWindowOnOwner(CWnd* pWndToCenterOn, CWnd* pWndofSub, bool bFlag_Top = false);

//! �ش� ��ü�� ȭ�� �� ���� �����Ѵ�.
void Set_TopWindow(CWnd* pWnd);

//! HDD ����̹��� ����� �����´�.
int GetHDDList(CStringArray &arrDriveList);									
//! ����̹��� ���� �뷮�� MByte ������ �޴´�. 
double GetRemainSize_InHDD(CString szDriver, double *pdbTotalSize = NULL);		

//! �Է��� ���ҽ��� �簢 ������ �����´�.
int GetWindowRect_FromUI(int iUID, CWnd* pParentWnd, CRect &rcRet);
//! �Է��� ���ҽ��� �簢 ������ �����´�.
int GetClientRect_FromUI(int iUID, CWnd* pParentWnd, CRect &rcRet);
//! �Է��� ���ҽ��� �簢 ������ �����Ѵ�. 
int SetRect_InCtrl(int iUID, CWnd* pParentWnd, CRect rcInput);

//! �Է��� ���ҽ��� �ڵ鰪�� �����´�.
HWND GetHandle_FromUI(int iUID, CWnd* pParentWnd);

//! �ش� ��Ʈ���� Ȱ��ȭ/��Ȱ��ȭ
int SetEnableState_InCtrl(int iUID, CWnd* pParentWnd, bool bEnable);
//! �ش� ��Ʈ���� Ȱ��ȭ ����
int GetEnableState_InCtrl(int iUID, CWnd* pParentWnd, bool &bGetEnable);			
//! �ش� ��Ʈ�ѿ� ��Ŀ�� ����
int SetFous_InCtrl(int iUID, CWnd* pParentWnd);

//! �ش� ��Ʈ���� ���� ���̰� �ϰų� �����
int SetVisibleState_InCtrl(int iUID, CWnd* pParentWnd, bool bVisible);
//! �ش� ��Ʈ���� ���� ���̴� ����
int GetVisibleState_InCtrl(int iUID, CWnd* pParentWnd, bool &bGetVisible);

//! �ش� Button(�Ǵ� Check, Radio) ��Ʈ���� ���� ���¸� �˾� ����. 
int GetButtonCheckState_InCtrl(int iUID, CWnd* pParentWnd, bool &bGetCheck);
//! �ش� Button(�Ǵ� Check, Radio) ��Ʈ���� ���� ���¸� ����
int SetButtonCheckState_InCtrl(int iUID, CWnd* pParentWnd, bool bCheck);

//! �Է��� ���ҽ����� ���ڿ��� �����´�. 
int GetString_FromUI(int iUID, CWnd* pParentWnd, CString &szRet);			
//! �Է��� ���ҽ����� ���� ������ ���� �����´�. 
int GetIntegerValue_FromUI(int iUID, CWnd* pParentWnd);							
//! �Է��� ���ҽ����� �Ǽ��� ���� �����´�. 
int GetDoubleValue_FromUI(int iUID, CWnd* pParentWnd, double &dbRet);

//! �Է��� ���ҽ��� ���ڿ��� �����Ѵ�.
int SetString_to_UI(int iUID, CWnd* pParentWnd, CString szInput);

//! �Էµ� ���ڿ��� �Է��� ���ҽ��� ���� Edit ��Ʈ�ѿ� ǥ���Ѵ�. 
int SetString_InEditCtrl(int iUID, CWnd* pParentWnd, CString szText);			
//! �Էµ� ���ڿ��� �Է��� ���ҽ��� ���� Edit ��Ʈ�ѿ� �߰��Ѵ�. �� ��, Edit ��Ʈ���� 'Multi Line' �Ӽ��� ������ �Ѵ�. 
int AppendString_InEditCtrl(int iUID, CWnd* pParentWnd, CString szAdd, bool bNextLine, bool bReset = false);

//! Ư�� CPU���� Thread�� ����� ��, ��� CPU�� �����ϱ� ���� Mask ó�� ����	
//! �Էµ� �����Ϳ��� SetProcessAffinityMask, SetThreadAffinityMask �Լ����� ��밡���� Mask ���� ���Ѵ�. 
DWORD GetMask_CPU_Affinity(int *piCPU_Use, int iSize);

//! Process (���α׷� ��ü)���� ����ϴ� CPU�� �����Ѵ�.
//! [���� ����] 
//!		1) Process���� ����� CPU�� ������ ��� Thread�� ����� CPU�� �����ؾ� �Ѵ�. 
//!		2) Thread Affinity(ģȭ��)�� �׻� Process Affinity(ģȭ��)�� ������̾�� �Ѵ�.
//! [�Է°�]
//!     bFlag_All_CPU : true�̸�,  PC�� ��� CPU Core�� ����ϴ� ������ �����Ѵ�. piCPU_Use, iSize ���� ������� �ʴ´�.
//!                     false�̸�, iSize ũ���� piCPU_Use ������ �����͸� �̿��Ѵ�.
//!     piCPU_Use : 0 �� index�� 1�� CPU�� ����Ų��. 1�� index�� 2�� CPU, ...
int SetProcessAffinity(bool bFlag_All_CPU, int *piCPU_Use = NULL, int iSize = 0);

//! bool abTest[16]�� ��, �迭�� ũ�� : sizeof(abTest) / sizeof(bool)
//! ������ ���� bool�� �迭(ũ�Ⱑ 16 �̻�)�� ��ȯ
bool Convert_Integer_To_BooleanBuffer(int iInput, bool *pabOutput, int iBufSize_Output);
//! bool�� �迭(ũ�Ⱑ 16 �̻�)�� ������ ������ ��ȯ
bool Convert_BooleanBuffer_To_Integer(bool *pabInput, int iBufSize_Input, int *piOutput);

//! 16 bit ���� 2���� ���ļ�, 32 bit ���� 1���� ��ȯ
int Convert_Integer_From_16bit_To_32bit(int iInput1_Low_16bit, int iInput2_High_16bit);

//! [���� ����] double -> int �ڷ��� ��ȯ���� �ݿø��� ���� ������ �ɰ��� ������ �߻��Ѵ�. �ݿø��� �ص� ������ '0'�� ������ �ʴ´�.
//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�. 
//! �ڷ��� ��ȯ���� ���� �ڵ尡 ������������ �߰��� �Լ�, �ܼ��� �Է°��� �����ְ� �� ��ȯ�� �ؼ� ��ȯ�Ѵ�. 
int CalcMultiply(double dbInput1, double dbInput2, bool bFlag_Round_Off = true);
int CalcMultiply(double dbInput1, int iInput2, bool bFlag_Round_Off = true);
CRect CalcMultiply(double dbInput, CRect rcInput, bool bFlag_Round_Off = true);
CRect CalcMultiply(double dbInputX, double dbInputY, CRect rcInput, bool bFlag_Round_Off = true);
//! �ڷ��� ��ȯ���� ���� �ڵ尡 ������������ �߰��� �Լ�, �ܼ��� ù��° �Է°��� 2��° �Է°����� �������ְ� �� ��ȯ�� �ؼ� ��ȯ�Ѵ�. 
//! ��, 2��° �Է°��� '0'�̸� '0'�� ��ȯ�Ѵ�. 
int CalcDivide(double dbInput1, double dbInput2, bool bFlag_Round_Off = true);
int CalcDivide(double dbInput1, int iInput2, bool bFlag_Round_Off = true);
int CalcDivide(int iInput1, double dbInput2, bool bFlag_Round_Off = true);
CRect CalcDivide(double dbInput, CRect rcInput, bool bFlag_Round_Off = true);
CRect CalcDivide(double dbInputX, double dbInputY, CRect rcInput, bool bFlag_Round_Off = true);

//! �Էµ� �簢 ������ ��ü �簢 ������ ���� �ʵ��� �����Ͽ� ��ȯ 
void Get_Valid_Point(CRect rcEntire, CPoint &ptInput);
void Get_Valid_Line(CRect rcEntire, CPoint &ptStart, CPoint &ptEnd);
CRect Get_Valid_Rect(CRect rcEntire, CRect rcInputChk, bool bFlag_Base_Left, bool bFlag_Base_Top);

//! ���콺, Ű���� �Է� ���� �̺�Ʈ �߻��� �ִ��� üũ.. 
void checkMessage();
//! Idle Loop Processing
bool PeekAndPump();
