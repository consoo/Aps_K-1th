#pragma once


// **************************************************************************************
// 
// ���� ���Ǵ� Util �Լ����� ��� ��������.
//
//	History
//		2012.10.22	V 1.0.1		parkchas	Revision
//		2012.11.05	V 1.1.0		parkchas	������. Logfile ���(Define ����). �Լ� �߰�.
//
// **************************************************************************************


//////////////////////////////////////////////////////////////////////////
// ���� ����
//

/* LOG FILE ��� ��� ���� ���� ( DIR , FILE NAME ) **********************************/
#ifndef LOGFILE
#define LOGFILE

#define LOGFILE_DIR_DAY			1	// LOGS/DAY		folder ����
#define LOGFILE_DIR_MONTH		2	// LOGS/MONTH	folder ����
#define LOGFILE_DIR_YEAR		4	// LOGS/YEAR	folder ����
#define LOGFILE_DIR_FULLDAY		7	// LOGS/YEAR/MONTH/DAY folder ����

#define LOGFILE_FILE_HOUR		64	// FileName(           hh) Filename ����
#define LOGFILE_FILE_DATE		128	// FileName(yyyy-mm-dd   ) Filename ����
#define LOGFILE_FILE_FULLDAY	192	// FileName(yyyy-mm-dd hh) Filename ����
#endif


#define INVALID_ARGUMENT				20001


/*****************************************************************************/
/*	THREAD ���� ����                                                          */
/*****************************************************************************/
#define THREAD_CREATE_MAXCNT_OVER		30001


/*****************************************************************************/
/*	FTP ���� ����                                                          */
/*****************************************************************************/
#define FTP_DOWNLOAD_FAIL				40001


//////////////////////////////////////////////////////////////////////////
// Macro ����
//
#ifndef UTCHAR
#define UTCHAR(var,len) TCHAR var[len]; ZeroMemory(var,sizeof(var));
#endif


//////////////////////////////////////////////////////////////////////////
// Process Function
//
void DoEvent(int iMiliSecond=10);


//////////////////////////////////////////////////////////////////////////
// Directory / File Function
//
BOOL WINAPI MakeFullDir(LPCTSTR lpszDirPath);

BOOL ReturnParameter(CString szOrigin, char cDelimeter, int nOrder, CString &szReturn);
void ReturnSeperatorFileName(LPCTSTR lpszFullPathName, LPCTSTR lpszPath, LPCTSTR lpszFileName);
void ReturnSeperatorFileName(CString szFullPathName, CString &szPath, CString &szFileName);

void GetCurTimeString(PTCHAR lpszCurTime, LPCTSTR alpszFlag);

BOOL WINAPI MakeSureDirectoryPathExists(LPCTSTR lpszDirPath);
// V 1.1.0 Revision
void _WriteLogFile(LPCTSTR apszLogMsg, LPCTSTR apszLogDir, LPCTSTR apszLogFile, UINT apuiOption=LOGFILE_DIR_FULLDAY | LOGFILE_FILE_FULLDAY);


BOOL SetRegistyStartProgram(BOOL bAutoExec, LPCSTR lpValueName, LPCSTR lpExeFileName);


// =======================================================================
//	Binary Data Operation
//
long arr2long (byte* arr, long start);


// =======================================================================
//	Hexa Data Operation
//
int CStringToHexaBytes(CString szInput, PBYTE pOutput);
int CharStringToHexaBytes(char* pszInput, PBYTE pOutput);
BYTE	byCode2AsciiValue( char cData );
int AsciiToHex(char *pSrc, char* pDst, int iSrcLength, char cSeperator);// ������ HEX ��Ʈ������...


// =======================================================================
//	���� ó�� ���
//	Bitmap Image
//
int BuildImageSize(int, int, int);						// �̹��� ũ��� ��Ʈ�� �׸��� ���̰��� ���� ���� �̹��� ũ�� �˾Ƴ���
int DetermineDummyValue(int, int);						// ��Ʈ�� ���α��� 4��� ������ �� �˾Ƴ���
bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile );
bool LoadBMPIntoDC ( HDC hDC, CRect rc, LPCTSTR bmpfile );
BYTE* LoadBMP24bit ( int* width, int* height, long* size, LPCTSTR bmpfile );
BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height );
bool SaveBMP24bit ( BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile );


// =======================================================================
//	File �Լ�
BOOL WINAPI FileExists(LPCTSTR szFileName);
BOOL FileFolderCopyMove(CString strSrcPath, CString strDestPath, UINT wFunc, HWND hWnd);
long GetFileSize(char* szFile);



// =======================================================================
//	Character ���� �Լ�
//
char *commify(double val, char *buf, int round=0);
//CString CommaString(double rVal, int round=0);
char* CommaString(double rVal, int round=0);
char* TrimRight( char * szAscii );
char* TrimLeft ( char * szAscii );
char* Trim	   ( char * szAscii );
char* TrimCopy ( char * szAscii );

int CharCount(CString szOrigin, char cDelimeter);
void CoordinateTrans( double sx, double sy, double *tx, double *ty, int type );
double ST2Second(SYSTEMTIME st);


// ����ð� ������ : mili-seconds ������ Ȯ�� �����ϴ�.
// ���� : Instance �� �����ϰ�,
//          STARTTIMER() �Լ��� ȣ���ϰ� Ÿ�̸Ӹ� �����Ѵ�.
//          GetElapsedTime() �Լ��� ����ð��� ���� �޴´�.

class CElapsedTimer
{
public:
	CElapsedTimer( BOOL bInitStart = FALSE )
	{
		sTime.HighPart = 0;					// ����
		sTime.LowPart  = 0;					// ����
		eTime.HighPart = 0;					// ����
		eTime.LowPart  = 0;					// ����
		freq.HighPart = 0;					// ����
		freq.LowPart  = 0;					// ����

		if( bInitStart ) STARTTIMER();
	};
	~CElapsedTimer() {};

private:
	LARGE_INTEGER sTime;					// ����
	LARGE_INTEGER eTime;					// ����
	LARGE_INTEGER freq;						// Ÿ�̸� �ֱ�

public:
	void STARTTIMER( void )
	{
		QueryPerformanceFrequency( &freq );	// Ÿ�̸��� �ֱ⸦ �� freq�� ����
		QueryPerformanceCounter( &sTime );	// ���۽ð� ����
	};

	__int64 GetElapsedTime()
	{
		QueryPerformanceCounter( &eTime );	// ���� �ֱ�� ���� �ð��� �����´�.
		return ( eTime.QuadPart - sTime.QuadPart ) / ( freq.QuadPart / 1000 );
	};
};