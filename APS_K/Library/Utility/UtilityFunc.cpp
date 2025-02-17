

#include "stdafx.h"
#include "UtilityFunc.h"

//! 'UtilityFunc.h' ���Ͽ��� ������ ���� �Լ����� �����Ѵ�. 


//! [���] ���� �׸���ġ õ���� ǥ�ؽð��� msec �������� �˾Ƴ��� ���ڿ��� �����Ͽ� ��ȯ�Ѵ�. 
//! [��ȯ��] �ð� ǥ�� ���ڿ�
CString GetString_CurrentSystemTime()
{
	SYSTEMTIME stCurrentSystem;
	::GetSystemTime(&stCurrentSystem);
	CString szRet = _T("");
	szRet.Format(_T("%04d-%02d-%02d %02d:%02d:%02d [%d]"), 
		stCurrentSystem.wYear, stCurrentSystem.wMonth, stCurrentSystem.wDay, 
		stCurrentSystem.wHour, stCurrentSystem.wMinute, stCurrentSystem.wSecond, 
		stCurrentSystem.wMilliseconds);
	return szRet;
}

//! [���] ���� ǥ�ؽð��� msec �������� �˾Ƴ��� ���ڿ��� �����Ͽ� ��ȯ�Ѵ�. 
//! [��ȯ��] �ð� ǥ�� ���ڿ�
CString GetString_CurrentLocalTime()
{
	SYSTEMTIME stCurrentLocal;
	::GetLocalTime(&stCurrentLocal);
	CString szRet = _T("");
	szRet.Format(_T("%04d-%02d-%02d %02d:%02d:%02d [%d]"), 
		stCurrentLocal.wYear, stCurrentLocal.wMonth, stCurrentLocal.wDay, 
		stCurrentLocal.wHour, stCurrentLocal.wMinute, stCurrentLocal.wSecond, 
		stCurrentLocal.wMilliseconds);
	return szRet;
}

//! [���] ���� ǥ�ؽð��� sec �������� �˾Ƴ��� ���ڿ��� �����Ͽ� ��ȯ�Ѵ�.
//! [��ȯ��] �ð� ǥ�� ���ڿ�
CString GetString_CurrentTime()
{
	COleDateTime currentTime;
	currentTime = COleDateTime::GetCurrentTime();
	CString szRet = _T("");
	szRet.Format(_T("%04d%02d%02d_%02d%02d%02d"), currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), 
		currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond());
	return szRet;
}

//! [���] ���� ǥ�ؽð��� sec �������� �˾Ƴ��� ���ڿ��� �����Ͽ� ��ȯ�Ѵ�. (�ð��� 0 ~ 24�� ǥ���Ѵ�)
//! [��ȯ��] �ð� ǥ�� ���ڿ�
CString GetString_CurrentTime_HMS()
{
	COleDateTime currentTime;
	currentTime = COleDateTime::GetCurrentTime();
	CString szRet = _T("");
	szRet = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	return szRet;
}

//! [���] ���� ǥ�ؽð��� sec �������� �˾Ƴ��� ���ڿ��� �����Ͽ� ��ȯ�Ѵ�. (�ð��� AM/PM���� �����Ѵ�)
//! [��ȯ��] �ð� ǥ�� ���ڿ�
CString GetString_CurrentTime_AP()
{
	COleDateTime currentTime;
	currentTime = COleDateTime::GetCurrentTime();
	CString szRet = _T("");
	szRet = currentTime.Format(_T("%Y-%m-%d %I:%M:%S %p"));
	return szRet;
}

//! [���] �־��� ���� ��ο� ����, ������ �����ϴ��� Ȯ���Ѵ�. 
//! [�Է°�] szInput : ���� ��θ�
//! [��ȯ��] ����, ������ �����ϸ� true�� ��ȯ�Ѵ�. 
bool IsExist_Path(CString szInput)
{
	DWORD dwAttribute;
	dwAttribute = ::GetFileAttributes(szInput);
	if ( dwAttribute == -1 )	//! �ش� ��ο� ������ �������� ���� ���
	{
		return false;
	}

	return true;
}

//! [���] �־��� ���� ��θ� �� �ڿ� "\\" ���ڰ� ������ �߰��Ѵ�. 
CString Append_Char_FolderPath(CString szFolderPath)
{
	CString szRetPath = _T("");
	szRetPath.Format(_T("%s"), szFolderPath);
	if ( szRetPath.Right(1) != _T("\\") )
	{
		szRetPath += _T("\\");
	}
	return szRetPath;
}

//! [���] ���ڿ��� ���������� �����ϸ� true
//! [�Է°�] ������ ���� ��ΰ� �ƴ϶�, ������ �̸��� �� ���ڿ�
//! [��ȯ��] true�̸� �Էµ� ���ڿ��� ���������� �����ϴ�. false�̸� �������ϴ�.
bool IsValid_FolderName(CString szInput)
{
	int iLenInput = (int)(szInput.GetLength());
	if ( iLenInput <= 1 )
	{
		return true;
	}

	bool bRet = true;

	int i = 0;
	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( szTemp.GetLength() + 1 );

	//! '\\, /, :, *, ?, \", <, >, |' ���� ���ڴ� ������ �̸����� ����� �� �����ϴ�.
	for ( i = 0; i < iLenInput; i++ )
	{
		if ( czTemp[i] == _T('/') || czTemp[i] == 0x5c || czTemp[i] == _T(':') || czTemp[i] == _T('|') || 
			czTemp[i] == _T('*') || czTemp[i] == _T('?') || czTemp[i] == _T('<') || czTemp[i] == _T('>') )
		{
			bRet = false;
			break;
		}
	}

	szTemp.ReleaseBuffer();

	return bRet;
}

//! [���] ���ڿ��� ��θ� �������� Ȯ���Ѵ�. 
//! [��ȯ��] ���ڿ��� ��θ����� �����ϸ� true, ���ڿ��� ���̰� 0�̸� true�� ��ȯ��
bool IsValid_Path(CString szInput)
{
	int iLen = (int)(szInput.GetLength());
	if ( iLen <= 0 )
	{
		return true;
	}

	bool bRet = true;

	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( iLen + 1 );

	if ( iLen == 1 )
	{
	}
	else
	{
		//! 0x5c : _T('\')

		for ( int i = 0; i < (iLen - 1); i++ )
		{
			if ( czTemp[i] == _T('/') && czTemp[i + 1] == _T('/') )	
			{
				bRet = false;
				break;
			}
			else if ( czTemp[i] == 0x5c && czTemp[i + 1] == 0x5c )
			{
				bRet = false;
				break;
			}
			else if ( czTemp[i] == 0x5c && czTemp[i + 1] == _T('/') )	
			{
				bRet = false;
				break;
			}
			else if ( czTemp[i] == _T('/') && czTemp[i + 1] == 0x5c )	
			{
				bRet = false;
				break;
			}
		}
	}

	szTemp.ReleaseBuffer();

	return bRet;
}

//! [���]   ���α׷��� ����� ������ ��θ� �˾Ƴ���. 
//! [��ȯ��] ���� ������ ��ġ�� ������ ���� ��θ�
CString GetProgramExeFolderPath()
{
	TCHAR szPath[_MAX_PATH  ];
	TCHAR drive [_MAX_DRIVE ];
    TCHAR dir   [_MAX_DIR   ];
    //TCHAR fname [_MAX_FNAME ];
    //TCHAR ext   [_MAX_EXT   ];
	GetModuleFileName(NULL, szPath, sizeof(szPath));
	_tsplitpath_s( szPath, drive, sizeof(drive), dir, sizeof(dir), NULL, 0, NULL, 0 );
	CString szRet = _T(""), szFolderPath = _T("");
	szFolderPath.Format(_T("%s%s"), drive, dir);
	szRet = Append_Char_FolderPath(szFolderPath);
	return szRet;
}

//! [���] ���� ����, ��������, ������ ������ ��� �����Ѵ�.
//! [�Է°�] ������ ���� ��θ�
//! [��ȯ��] ���������� �۵��ϸ� ���� 1�� ��ȯ�ϰ�, ������ �߻��ϸ� ������ ��ȯ�Ѵ�. 
int RemoveFolder(CString szFolderPath)
{
	//! ������ �Լ� ���ڰ� 'LPTSTR lpFolder'������ ����ϱ� �����ϹǷ�, 'CString szFolderPath'�� �����ϴ�. 
	int iLength = (int)(szFolderPath.GetLength());
	if ( iLength <= 0 )
	{
		return -1;
	}

	if ( ::GetFileAttributes(szFolderPath) == -1 )	//! ������ ���� ���
	{
		return -2;
	}

	LPTSTR lpFolder = NULL;
	lpFolder = szFolderPath.GetBuffer( iLength + 1 );

	WIN32_FIND_DATA		FindData;
    HANDLE				hFindHandle;
    TCHAR				czTemp[MAX_PATH];
    int					iLen_lpFolder = 0;

	//! �Էµ� ��θ��� ��ȿ���� �˻��Ѵ�.
    if ( lpFolder == NULL )
    {
		szFolderPath.ReleaseBuffer();
        return -3;
    }
    if ( *lpFolder == '\0' )
    {
		szFolderPath.ReleaseBuffer();
        return -4;
    }

	//! �Էµ� ������ ���� ����, ���� �˻��� ���� �˻� ���ڿ� ����
    iLen_lpFolder = (int)(_tcslen(lpFolder));
    if ( lpFolder[iLen_lpFolder - 1] == '\\' )
    {
        _stprintf_s(czTemp, _T("%s*.*"), lpFolder);
    }
    else
    {
        _stprintf_s(czTemp, _T("%s\\*.*"), lpFolder);
    }

	//! �Էµ� ������ ���� ����, ���� �˻� ����
    hFindHandle = ::FindFirstFile(czTemp, &FindData);

	//! �Էµ� ������ ������ ������ ������ ���� ���
    if ( INVALID_HANDLE_VALUE != hFindHandle )
    {
        while(1)
        {
			//! ã�Ƴ� ����, ������ ������ ���� ��θ� ����
            if ( lpFolder[iLen_lpFolder - 1] == '\\' )
            {
                _stprintf_s(czTemp, _T("%s%s"), lpFolder, FindData.cFileName);
            }
            else
            {
                _stprintf_s(czTemp, _T("%s\\%s"), lpFolder, FindData.cFileName);
            }

			//! ������ ���
            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ( _tcscmp(FindData.cFileName, _T(".")) == 0 )	//! �ڱ� �ڽ��� ������ �ǹ�
                {
                    //.    -->> skip
                }
                else if ( _tcscmp(FindData.cFileName, _T("..")) == 0 )	//! ���� ������ �ǹ�
                {
                    //..   -->> skip
                }
                else
                {
                    RemoveFolder(czTemp);	//! ��� ȣ��

					//! ������ �Ӽ��� ������ �����ϵ��� �����Ѵ�. 
                    if ( (FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || 
                        (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) )
                    {
                        ::SetFileAttributes(czTemp, FILE_ATTRIBUTE_NORMAL);
                    }

                    ::RemoveDirectory(czTemp);	//! ��� �ִ� ���� ���� ����
                }
            }
            else
            {
                //! ������ ���

				//! ������ �Ӽ��� ������ �����ϵ��� �����Ѵ�. 
                if ( (FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || 
                    (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) )
                {
                    ::SetFileAttributes(czTemp, FILE_ATTRIBUTE_NORMAL);
                }

                ::DeleteFile(czTemp);	//! ���� ����
            }

			//! �Էµ� ���� ���� �ٸ� ����, ������ �ִ��� ��� �˻縦 ����
            if ( !::FindNextFile(hFindHandle, &FindData) )
            {
                break;	//! �Էµ� ���� �Ʒ��� ���� ����, ������ ������ �˻� �ߴ�
            }
        }
    }
    ::FindClose(hFindHandle);

	//! ������ �Ӽ��� ������ �����ϰ� �� ���
    ::SetFileAttributes(lpFolder, FILE_ATTRIBUTE_NORMAL);
	//! �Էµ� ���� ����
    ::RemoveDirectory(lpFolder);

	szFolderPath.ReleaseBuffer();

	return 1;
}

//! [���] �Էµ� ����� ���� �Ʒ��� �ִ� ������ �̸��̳� ������ �̸��� CStringArray Ÿ������ �����´�. 
//! [�Է°�] szFolderPath : ������ ���� ���, bFile : true�̸� ���� �̸��� ����� �����´�, false�̸� ���� �̸��� ����� �����´�. 
//! [��ȯ��] ���������� �۵��ϸ� ���� 1�� ��ȯ�ϰ�, ������ �߻��ϸ� ������ ��ȯ�Ѵ�. 
//!			 arrInfo : ���� �̸��̳� ���� �̸��� ����Ǿ� ��ȯ�ȴ�. 
int GetList_Sub_FolderInfo(CString szFolderPath, CStringArray &arrInfo, bool bFile)
{
	int iLength = (int)(szFolderPath.GetLength());
	if ( iLength <= 0 )
	{
		return -1;
	}

	if ( ::GetFileAttributes(szFolderPath) == -1 )	//! ������ ���� ���
	{
		return -2;
	}

	LPTSTR lpFolder = NULL;
	lpFolder = szFolderPath.GetBuffer( iLength + 1 );

	WIN32_FIND_DATA		FindData;
    HANDLE				hFindHandle;
    TCHAR				czTemp[MAX_PATH];
	TCHAR				czFindTemp[MAX_PATH];
    int					iLen_lpFolder = 0;
	CString				szAdd = _T("");
	TCHAR				*pcFound;
	int					iTempBufSize = 0, iFoundindex = 0;

	//! �Էµ� ��θ��� ��ȿ���� �˻��Ѵ�.
    if ( lpFolder == NULL )
    {
		szFolderPath.ReleaseBuffer();
        return -3;
    }
    if ( *lpFolder == '\0' )
    {
		szFolderPath.ReleaseBuffer();
        return -4;
    }

	//! ���� ������ ����
	arrInfo.RemoveAll();
	arrInfo.FreeExtra();

	//! �Էµ� ������ ���� ����, ���� �˻��� ���� �˻� ���ڿ� ����
    iLen_lpFolder = (int)(_tcslen(lpFolder));
    if ( lpFolder[iLen_lpFolder - 1] == '\\' )
    {
        _stprintf_s(czTemp, _T("%s*.*"), lpFolder);
    }
    else
    {
        _stprintf_s(czTemp, _T("%s\\*.*"), lpFolder);
    }

	//! �Էµ� ������ ���� ����, ���� �˻� ����
    hFindHandle = ::FindFirstFile(czTemp, &FindData);

	//! �Էµ� ������ ������ ������ ������ ���� ���
    if ( INVALID_HANDLE_VALUE != hFindHandle )
    {
        while(1)
        {
			//! ������ ���
            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ( _tcscmp(FindData.cFileName, _T(".")) == 0 )	//! �ڱ� �ڽ��� ������ �ǹ�
                {
                    //.    -->> skip
                }
                else if ( _tcscmp(FindData.cFileName, _T("..")) == 0 )	//! ���� ������ �ǹ�
                {
                    //..   -->> skip
                }
                else
                {
					if ( FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )	//! � ü������ �ڵ� ������ ������ ���
					{
						//   -->> skip
					}
					else
					{
						if ( bFile == false )
						{
							szAdd.Format(_T("%s"), FindData.cFileName);
							arrInfo.Add(szAdd);
						}
					}
                }
            }
            else
            {
                //! ������ ���

				if ( FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )	//! � ü������ �ڵ� ������ ������ ���
				{
					//   -->> skip
				}
				else
				{
					if ( bFile == true )
					{
						//! Ȯ���� ���ڿ��� �߶󳻴� �۾��� �����Ѵ�. 
						_tcscpy_s(czFindTemp, FindData.cFileName);
						pcFound = _tcsrchr( czFindTemp, _T('.') );
						if ( pcFound != NULL )	//! Ȯ���� ���ڿ��� �����ϴ� '.'�� �߰ߵǴ�. 
						{
							iTempBufSize = (int)(sizeof(czFindTemp));
							iFoundindex = pcFound - czFindTemp;
							if ( iFoundindex > 0 && iFoundindex < iTempBufSize )
							{
								czFindTemp[iFoundindex] = NULL;
							}
						}
						szAdd.Format(_T("%s"), czFindTemp);
						arrInfo.Add(szAdd);
					}
				}
            }

			//! �Էµ� ���� ���� �ٸ� ����, ������ �ִ��� ��� �˻縦 ����
            if ( !::FindNextFile(hFindHandle, &FindData) )
            {
                break;	//! �Էµ� ���� �Ʒ��� ���� ����, ������ ������ �˻� �ߴ�
            }
        }
    }
    ::FindClose(hFindHandle);

	szFolderPath.ReleaseBuffer();

	return 1;
}

//! [���] ���� ������ ������ ��� ����
//! [�Է°�] szFolderPath_Source : ���� ������ �ִ� ������ ���� ���, 
//!	         szFolderPath_Dest : �����Ϸ��� ���� ������ ���� ���
//! [��ȯ��] ���������� �۵��ϸ� ���� 1�� ��ȯ�ϰ�, ������ �߻��ϸ� ������ ��ȯ�Ѵ�. 
int CopyFiles_In_Folder(CString szFolderPath_Source, CString szFolderPath_Dest)
{
	if ( szFolderPath_Source.GetLength() <= 0 || szFolderPath_Dest.GetLength() <= 0 )
	{
		return -1;
	}
	if ( ::GetFileAttributes(szFolderPath_Source) == -1 )	//! ������ ���� ���
	{
		return -2;
	}
	if ( ::GetFileAttributes(szFolderPath_Dest) == -1 )	//! ������ ���� ���
	{
		return -2;
	}

	//! ���� ��θ� �ڿ� "\\" ���ڰ� ������ �߰��Ѵ�. 
	if ( szFolderPath_Source.Right(1) != _T("\\") )
	{
		szFolderPath_Source += _T("\\");
	}
	if ( szFolderPath_Dest.Right(1) != _T("\\") )
	{
		szFolderPath_Dest += _T("\\");
	}
	//! ���� �ٷ� �Ʒ��� ��� ������ �����Ѵ�. 
	szFolderPath_Source += _T("*.*");

	CFileFind finder;
	CString szDestFilePath = _T("");

	BOOL bWorking = finder.FindFile(szFolderPath_Source);

	while(bWorking == TRUE)
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() )
		{
			continue;
		}
		if ( finder.IsDirectory() )
		{
			continue;
		}

		szDestFilePath = szFolderPath_Dest + finder.GetFileName();

		::CopyFile(finder.GetFilePath(), szDestFilePath, FALSE);
	}

	return 1;
}

//! [���] �Է��� ���� ������ ���ʴ�� ������ �ִ��� Ȯ���ϰ� ������ ������ �ڿ� ���� �������� ��ȯ
//! [�Է°�] szFolderPath : �θ� ������ ���� ��θ�, 
//!			 arrSubFolderName : �θ� ���� �Ʒ��� ���� ������ �̸� (index 0, index 1, ... �� ������ ����)
//! [��ȯ��] ���������� �۵��ϸ� ���� 1�� ��ȯ�ϰ�, ������ �߻��ϸ� ������ ��ȯ�Ѵ�. 
//!			 szLastFolderPath : �� �������� ���� ������ ���� ��θ�
int CreateSubFolders(CString szFolderPath, CStringArray &arrSubFolderName, CString &szLastFolderPath)
{
	szLastFolderPath = _T("");

	if ( szFolderPath.GetLength() <= 0 )
	{
		return -1;
	}
	if ( ::GetFileAttributes(szFolderPath) == -1 )	//! ������ ���� ���
	{
		return -2;
	}
	int iSize_SubFolder = (int)(arrSubFolderName.GetSize());
	if ( iSize_SubFolder <= 0 )
	{
		return -3;
	}

	//! ���� ��θ� �ڿ� "\\" ���ڰ� ������ �߰��Ѵ�. 
	if ( szFolderPath.Right(1) != _T("\\") )
	{
		szFolderPath += _T("\\");
	}

	BOOL bCreateDirectory = FALSE;
	CString szSubFolderName = _T("");

	CString szSubFolderPath = szFolderPath;

	int i = 0;
	int iIndex_Failed = -1;
	for ( i = 0; i < iSize_SubFolder; i++ )
	{
		szSubFolderName = arrSubFolderName.GetAt(i);
		szSubFolderPath += szSubFolderName;
		if ( szSubFolderPath.Right(1) != _T("\\") )
		{
			szSubFolderPath += _T("\\");
		}

		if ( ::GetFileAttributes(szSubFolderPath) == -1 )	//! ������ ���� ���
		{
			//! ���� ����
			bCreateDirectory = ::CreateDirectory(szSubFolderPath, NULL);
		}
		else
		{
			//! �̹� ������ ���� ���
			bCreateDirectory = TRUE;
		}

		if ( bCreateDirectory == FALSE )
		{
			iIndex_Failed = i;
			break;
		}
	}
	if ( iIndex_Failed >= 0 )
	{
		return -4;
	}

	szLastFolderPath = szSubFolderPath;

	return 1;
}

//! [���] ��,��,���� ���� ���������� �����Ѵ�. 
int CreateSubFolders(CString szFolderPath, int iYear, int iMonth, int iDay, CString &szLastFolderPath)
{
	if ( iMonth < 0 || iMonth > 12 )
	{
		return -100;
	}
	if ( iDay <= 0 || iDay > 31 )
	{
		return -101;
	}

	int iRet = 0;

	CString szInput = _T("");
	CStringArray arrInput;
	arrInput.RemoveAll();
	arrInput.FreeExtra();

	szInput.Format(_T("%04d"), iYear);
	arrInput.Add(szInput);
	szInput.Format(_T("%02d"), iMonth);
	arrInput.Add(szInput);
	szInput.Format(_T("%02d"), iDay);
	arrInput.Add(szInput);

	iRet = CreateSubFolders(szFolderPath, arrInput, szLastFolderPath);

	arrInput.RemoveAll();
	arrInput.FreeExtra();

	return iRet;
}

//! [���] ��,��,��,�ð��� ���� ���������� �����Ѵ�. 
int CreateSubFolders(CString szFolderPath, int iYear, int iMonth, int iDay, int iHour, CString &szLastFolderPath)
{
	if ( iMonth < 0 || iMonth > 12 )
	{
		return -100;
	}
	if ( iDay <= 0 || iDay > 31 )
	{
		return -101;
	}
	if ( iHour <= 0 || iHour > 24 )
	{
		return -102;
	}

	int iRet = 0;

	CString szInput = _T("");
	CStringArray arrInput;
	arrInput.RemoveAll();
	arrInput.FreeExtra();

	szInput.Format(_T("%04d"), iYear);
	arrInput.Add(szInput);
	szInput.Format(_T("%02d"), iMonth);
	arrInput.Add(szInput);
	szInput.Format(_T("%02d"), iDay);
	arrInput.Add(szInput);
	szInput.Format(_T("%02d"), iHour);
	arrInput.Add(szInput);

	iRet = CreateSubFolders(szFolderPath, arrInput, szLastFolderPath);

	arrInput.RemoveAll();
	arrInput.FreeExtra();

	return iRet;
}

//! [���] �Էµ� ���ڿ��� �������� Ȯ��
//! [�Է°�] bMinusChk : true�̸� ���ڿ� �� ���� '-' ���ڴ� ������ ó������ �ʴ´�. 
//! [��ȯ��] ���ڿ��� �����̸� true, ���ڿ��� ���̰� 0�̸� true�� ��ȯ
bool IsValid_Integer(CString szInput, bool bMinusChk)
{
	int iLen = (int)(szInput.GetLength());
	if ( iLen <= 0 )
	{
		return true;
	}

	bool bRet = true;

	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( iLen + 1 );

	for ( int i = 0; i < iLen; i++ )
	{
		if ( czTemp[i] < _T('0') || czTemp[i] > _T('9') )	
		{
			bRet = false;
			break;
		}
		else if ( czTemp[i] == _T('-') )
		{
			if ( bMinusChk == false || i != 0 )
			{
				bRet = false;
				break;
			}
		}
	}

	szTemp.ReleaseBuffer();

	return bRet;
}

//! [���] �Էµ� ���ڿ��� �Ǽ����� Ȯ��
//! [�Է°�] bMinusChk : true�̸� ���ڿ� �� ���� '-' ���ڴ� ������ ó������ �ʴ´�. 
//! [��ȯ��] ���ڿ��� �Ǽ��̸� true, ���ڿ��� ���̰� 0�̸� true�� ��ȯ
bool IsValid_Double(CString szInput, bool bMinusChk)
{
	int iLen = (int)(szInput.GetLength());
	if ( iLen <= 0 )
	{
		return true;
	}

	bool bRet = true;
	int iPointCnt = 0;

	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( iLen + 1 );

	for ( int i = 0; i < iLen; i++ )
	{
		if ( czTemp[i] == _T('.') )
		{
			iPointCnt++;
			continue;
		}
		else if ( czTemp[i] == _T('-') )
		{
			if ( bMinusChk == false || i != 0 )
			{
				bRet = false;
				break;
			}
		}
		else if ( czTemp[i] < _T('0') || czTemp[i] > _T('9') )	
		{			
			bRet = false;
			break;
		}
	}

	szTemp.ReleaseBuffer();

	if ( iPointCnt > 1 )
	{
		bRet = false;
	}

	return bRet;
}

//! [���] �Էµ� ���ڿ��� �������� Ȯ��
//! [��ȯ��] ���ڿ��� �����̸� true�� ��ȯ, ���ڿ��� ���̰� 0�̸� true�� ��ȯ
bool IsValid_Exponential(CString szInput)
{
	int iLen = (int)(szInput.GetLength());
	if ( iLen <= 0 )
	{
		return true;
	}

	bool bRet = true;
	int iPointCnt = 0, iExpIndex = 0, iExpCnt = 0;

	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( iLen + 1 );

	for ( int i = 0; i < iLen; i++ )
	{
		if ( czTemp[i] == _T('.') )
		{
			iPointCnt++;
			continue;
		}
		else if ( czTemp[i] == _T('-') )
		{
			if ( (i != 0) && (i != (iExpIndex + 1)) )
			{
				bRet = false;
				break;
			}
		}
		else if ( czTemp[i] < _T('0') || czTemp[i] > _T('9') )	
		{
			if ( czTemp[i] == _T('e') || czTemp[i] == _T('E') )
			{
				iExpIndex = i;
				iExpCnt++;
			}
			else
			{
				bRet = false;
				break;
			}
		}
	}

	szTemp.ReleaseBuffer();

	if ( iPointCnt > 1 )
	{
		bRet = false;
	}
	if ( iExpCnt > 1 )
	{
		bRet = false;
	}

	return bRet;
}

//! [���] �Էµ� ���ڿ��� �Ҽ��� �ڸ����� �˾� ����.
//! [��ȯ��] ���ڿ��� �Ǽ� �� ��, �Ҽ��� �ڸ����� ��ȯ
int GetDoublePointIndex(CString szInput)
{
	int iLen = (int)(szInput.GetLength());
	if ( iLen <= 0 )
	{
		return 0;
	}

	bool bFindPoint = false;
	int iCnt = 0;
	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( iLen + 1 );

	for ( int i = (iLen - 1); i >= 0; i-- )
	{
		if ( czTemp[i] == _T('.') )
		{
			bFindPoint = true;
			break;
		}
		else
		{
			iCnt++;
		}
	}

	szTemp.ReleaseBuffer();

	if ( bFindPoint == false )
	{
		iCnt = 0;
	}

	return iCnt;
}

//! [���] ���ڿ��� ���ĺ� ����, �������� Ȯ���Ѵ�. 
//! [��ȯ��] ���ڿ��� ����, �����̸� true, ���ڿ��� ���̰� 0�̸� true�� ��ȯ��
bool IsValid_AlphaNumeric(CString szInput)
{
	int iLen = (int)(szInput.GetLength());
	if ( iLen <= 0 )
	{
		return true;
	}

	bool bRet = true;

	CString szTemp = _T("");
	szTemp = szInput;
	TCHAR *czTemp = szTemp.GetBuffer( iLen + 1 );

	for ( int i = 0; i < iLen; i++ )
	{
		if ( czTemp[i] >= _T('0') || czTemp[i] <= _T('9') )	
		{
		}
		else if ( czTemp[i] >= _T('a') || czTemp[i] <= _T('z') )
		{
		}
		else if ( czTemp[i] >= _T('A') || czTemp[i] <= _T('Z') )
		{
		}
		else
		{
			bRet = false;
			break;
		}
	}

	szTemp.ReleaseBuffer();

	return bRet;
}

//! [���] ���ڿ��� ����(����, �Ǽ� ����, �������� ����)������ Ȯ��
//! [��ȯ��] ���ڿ��� �����̸� true, ���ڿ��� ���̰� 0�̸� true�� ��ȯ��
bool IsValid_Digit(CString szInput)
{
	CString szText;	
	TCHAR chTemp;
	bool bIsDigit = true;
	int  iIsPoint = 0;
	int  iIsMinus = 0;
	int  iStringLength = (int)(szInput.GetLength());
	
	for(int k = 0; k < iStringLength; k++)
	{
		chTemp = szInput.GetAt(k);

		if ( chTemp < _T('0') || chTemp > _T('9') )
		{
			if ( chTemp == _T('.') )
			{
				if ( k == 0  || k == (iStringLength - 1) || (iIsMinus > 0 && k == 1) )
				{
					//! '.'�� ������ ó���� ���� ���� �� ������, '-'�� ���� ��� �� �ٷ� �ڿ� ���� �� ����
					return false;	
				}
				else
				{
					++iIsPoint;
				}
			}
			else if ( chTemp == _T('-') )
			{
				if ( k != 0 )
				{
					return false;	//! '-'�� ������ �� ó������ �־�� ��
				}
				else
				{
					++iIsMinus;
				}
			}
			else
			{
				bIsDigit = false;
			}
		}
	}

	if ( iIsPoint > 1 || iIsMinus > 1 )
	{
		bIsDigit = false;
	}

	return bIsDigit;
}

//! [���] �Է��� ��ο��� Driver �̸����� �����Ѵ�. 
//! [�Է°�] szPath : ���� ��θ�
//! [��ȯ��] ����̹� �̸� ���ڿ�
CString GetDriveName(CString szPath)
{
	if ( szPath.GetLength() <= 0 )	//! �Է°� üũ
	{
		return szPath;
	}

	TCHAR czPath[_MAX_PATH], drive[_MAX_DRIVE];
	_tcscpy_s(czPath, szPath);
	_tsplitpath_s( czPath, drive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0 );

	CString szReturnPath = _T("");
	szReturnPath.Format(_T("%s"), drive);

	return szReturnPath;
}

//! [���] �Է��� ��ο��� ���� ���� ��θ� �����Ѵ�. 
//! [�Է°�] szFilePath : ���� ��θ�
//! [��ȯ��] "{����̹� ��}:\\{���� ���}\\" ���� ���� ���� ���� ��θ��� ��ȯ
CString GetFolderPath(CString szFilePath)
{
	if ( szFilePath.GetLength() <= 0 )	//! �Է°� üũ
	{
		return szFilePath;
	}

	TCHAR czPath[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR];
	_tcscpy_s(czPath, szFilePath);
	_tsplitpath_s( czPath, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0 );

	CString szReturnPath = _T("");
	szReturnPath.Format(_T("%s%s"), drive, dir);

	return szReturnPath;
}

//! [���] �־��� ��θ��� ���ϸ� (Ȯ���� ����)���� ����
//! [�Է°�] szFilePath : ���� ��θ�
CString GetFileNameExt(CString szFilePath)
{
	if ( szFilePath.GetLength() <= 0 )	//! �Է°� üũ
	{
		return szFilePath;
	}

	TCHAR czPath[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	_tcscpy_s(czPath, szFilePath);
	_tsplitpath_s( czPath, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );

	CString szReturnFileNameExt = _T("");
	szReturnFileNameExt.Format(_T("%s%s"), fname, ext);

	return szReturnFileNameExt;
}

//! �־��� ��θ��� ���� Ȯ���� ���� ����
//! [�Է°�] szFilePath : ���� ��θ�
CString GetFileExt(CString szFilePath)
{
	if ( szFilePath.GetLength() <= 0 )	//! �Է°� üũ
	{
		return szFilePath;
	}

	TCHAR czPath[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	_tcscpy_s(czPath, szFilePath);
	_tsplitpath_s( czPath, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );

	CString szReturnFileExt = _T("");
	szReturnFileExt.Format(_T("%s"), ext);

	return szReturnFileExt;
}

//! [���] �Էµ� ��ο� �Էµ� �����ͷ� Text ����(txt, csv ���� Ȯ���ڸ� ���� ����)�� �����Ѵ�. 
//! [�Է°�] szFilePath : ����Ǵ� ������ ���� ��� (Ȯ���ڱ��� �����ؾ� �Ѵ�), 
//!			 arrContents : �����ؾ� �� ���ڿ� ������, �� ���ڿ� ��Ұ� Text ���Ͽ����� 1���̴�, 
//!						   CStringArray�� �� ��� ���ڿ��� �� �ٲ� ���ڴ� ��� �ȴ�. 
//!			 bAppend : true�̸� ������ Text ������ ���� ���, ���� ������ �ڿ� ���ڿ� �����͸� �߰��Ѵ�. 
//!								������ ������ ���� ��쿡�� ���� �����Ѵ�, 
//!					   false�̸�, ������ ������ ������ �����ϰ�, ���ο� ������ �����Ѵ�. 
//! [��ȯ��] �۾��� �Ϸ��ϸ� ���� 1�� ��ȯ�Ѵ�. ������ �߻��ϸ� ������, ������ ��Ÿ���� �������� ��ȯ�Ѵ�. 
int SaveTextFile(CString szFilePath, CStringArray &arrContents, bool bAppend)
{
	if ( szFilePath.GetLength() <= 0 )
	{
		return -1;
	}
	int iArrSize = (int)(arrContents.GetSize());
	if ( iArrSize <= 0 )	//! �����ؾ� �� ���ڿ��� ���� ���
	{
		return -2;
	}
	
	CString szFolderPath = GetFolderPath(szFilePath);
	if ( ::GetFileAttributes(szFolderPath) == -1 )	//! ������ ����Ǿ�� �� ������ ���� ���
	{
		return -3;
	}

	UINT uFileOpenFlags;	//! ���� �Ӽ� ��

	if ( bAppend == false )
	{
		if ( ::GetFileAttributes(szFilePath) != -1 )	//! ���� ���� ����
		{
			::DeleteFile(szFilePath);
		}

		uFileOpenFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite;
	}
	else
	{
		//! ���� �Ӽ� ���� 'CFile::modeNoTruncate' �߰�
		uFileOpenFlags = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite;
	}

	try
	{
		CStdioFile SaveTextFile(szFilePath, uFileOpenFlags);	//! ���� ����

		if ( bAppend == true )
		{
			SaveTextFile.SeekToEnd();	//! �� ������ �ٷ� �̵��Ѵ�. 
		}

		int i = 0;
		CString szContent = _T(""), szWrite = _T("");

		for ( i = 0; i < iArrSize; i++ )
		{
			szContent = arrContents.GetAt(i);
			szWrite = szContent + _T("\n");		//! �� �ٲ� ���� �߰�
			SaveTextFile.WriteString(szWrite);	//! ���Ͽ� ���ڿ��� �����Ѵ�. 
		}

		SaveTextFile.Close();
	}
	catch(CFileException *e)
	{
		int iErrorValue = e->m_cause;
		e->Delete();
		return iErrorValue;
	}

	return 1;
}

//! [���] �Էµ� ��ο� �Էµ� �����ͷ� Text ����(txt, csv ���� Ȯ���ڸ� ���� ����)�� �����Ѵ�. 
//! [�Է°�] szFilePath : ����Ǵ� ������ ���� ��� (Ȯ���ڱ��� �����ؾ� �Ѵ�), 
//!			 szContent : �����ؾ� �� ���ڿ� ������, 1�ٿ� �ش��ϴ� �����͸� �Է��Ѵ�. ���� �� �ٲ� ���ڴ� ��� �ȴ�. 
//!			 bAppend : true�̸� ������ Text ������ ���� ���, ���� ������ �ڿ� ���ڿ� �����͸� �߰��Ѵ�. 
//!								������ ������ ���� ��쿡�� ���� �����Ѵ�, 
//!					   false�̸�, ������ ������ ������ �����ϰ�, ���ο� ������ �����Ѵ�. 
//! [��ȯ��] �۾��� �Ϸ��ϸ� ���� 1�� ��ȯ�Ѵ�. ������ �߻��ϸ� ������, ������ ��Ÿ���� �������� ��ȯ�Ѵ�.
int SaveTextFile(CString szFilePath, CString szContent, bool bAppend)
{
	if ( szFilePath.GetLength() <= 0 )
	{
		return -1;
	}

	CString szFolderPath = GetFolderPath(szFilePath);
	if ( ::GetFileAttributes(szFolderPath) == -1 )	//! ������ ����Ǿ�� �� ������ ���� ���
	{
		return -2;
	}

	UINT uFileOpenFlags;	//! ���� �Ӽ� ��

	if ( bAppend == false )
	{
		if ( ::GetFileAttributes(szFilePath) != -1 )	//! ���� ���� ����
		{
			::DeleteFile(szFilePath);
		}

		uFileOpenFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite;
	}
	else
	{
		//! ���� �Ӽ� ���� 'CFile::modeNoTruncate' �߰�
		uFileOpenFlags = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite;
	}

	try
	{
		CStdioFile SaveTextFile(szFilePath, uFileOpenFlags);	//! ���� ����

		if ( bAppend == true )
		{
			SaveTextFile.SeekToEnd();	//! �� ������ �ٷ� �̵��Ѵ�. 
		}

		int i = 0;
		CString szWrite = _T("");

		szWrite = szContent + _T("\n");		//! �� �ٲ� ���� �߰�
		SaveTextFile.WriteString(szWrite);	//! ���Ͽ� ���ڿ��� �����Ѵ�. 

		SaveTextFile.Close();
	}
	catch(CFileException *e)
	{
		int iErrorValue = e->m_cause;
		e->Delete();
		return iErrorValue;
	}

	return 1;
}

//! [���] �Էµ� ��ο� �Էµ� �����ͷ� Text ����(txt, csv ���� Ȯ���ڸ� ���� ����)�� �����Ѵ�. 
//! [�Է°�] szFilePath : ����Ǵ� ������ ���� ��� (Ȯ���ڱ��� �����ؾ� �Ѵ�), 
//!			 pczContents : �����ؾ� �� ���ڿ� ������, 2���� �迭�̸�, 1��° index�� �� ��ȣ�� �ǹ��Ѵ�. ���ڿ� ���۰� Queue ���·� ��ȴٰ� �����Ѵ�.
//!						   pczContents�� �� ���ڿ��� �� �ٲ� ���ڴ� �ʿ����.
//!			 iStartIndex : 1��° index������ ���� index, 
//!			 iSaveTextSize : ������ ���ڿ��� �� ����,  
//!			 iBufSize_1 : pczContents�� 1��° �迭 �������� ����
//!			 bAppend : true�̸� ������ Text ������ ���� ���, ���� ������ �ڿ� ���ڿ� �����͸� �߰��Ѵ�. 
//!								������ ������ ���� ��쿡�� ���� �����Ѵ�, 
//!					   false�̸�, ������ ������ ������ �����ϰ�, ���ο� ������ �����Ѵ�. 
//! [��ȯ��] �۾��� �Ϸ��ϸ� ���� 1�� ��ȯ�Ѵ�. ������ �߻��ϸ� ������, ������ ��Ÿ���� �������� ��ȯ�Ѵ�. 
int SaveTextFile(CString szFilePath, TCHAR **pczContents, int iStartIndex, int iSaveTextSize, int iBufSize_1, bool bAppend)
{
	if ( szFilePath.GetLength() <= 0 )
	{
		return -1;
	}

	//! ���� ��ȿ�� �˻�
	if ( pczContents == NULL || iBufSize_1 <= 0 )
	{
		return -2;
	}
	//! ���� index �˻�
	if ( iStartIndex < 0 || iStartIndex >= iBufSize_1 || iSaveTextSize <= 0 || iSaveTextSize > iBufSize_1 )
	{
		return -3;
	}
		
	CString szFolderPath = GetFolderPath(szFilePath);
	if ( ::GetFileAttributes(szFolderPath) == -1 )	//! ������ ����Ǿ�� �� ������ ���� ���
	{
		return -3;
	}

	UINT uFileOpenFlags;	//! ���� �Ӽ� ��

	if ( bAppend == false )
	{
		if ( ::GetFileAttributes(szFilePath) != -1 )	//! ���� ���� ����
		{
			::DeleteFile(szFilePath);
		}

		uFileOpenFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite;
	}
	else
	{
		//! ���� �Ӽ� ���� 'CFile::modeNoTruncate' �߰�
		uFileOpenFlags = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite;
	}

	try
	{
		CStdioFile SaveTextFile(szFilePath, uFileOpenFlags);	//! ���� ����

		if ( bAppend == true )
		{
			SaveTextFile.SeekToEnd();	//! �� ������ �ٷ� �̵��Ѵ�. 
		}

		int i = 0, iIndex = iStartIndex;
		CString szContent = _T(""), szWrite = _T("");

		for ( i = 0; i < iSaveTextSize; i++ )
		{
			if ( iIndex >= iBufSize_1 )	//! Queue������ ���ڿ� ���۰� ���� �ٴٶ��� ��
			{
				iIndex = 0;
			}

			szContent.Format(_T("%s"), pczContents[iIndex++]);
			szWrite = szContent + _T("\n");		//! �� �ٲ� ���� �߰�
			SaveTextFile.WriteString(szWrite);	//! ���Ͽ� ���ڿ��� �����Ѵ�. 
		}

		SaveTextFile.Close();
	}
	catch(CFileException *e)
	{
		int iErrorValue = e->m_cause;
		e->Delete();
		return iErrorValue;
	}

	return 1;
}

//! [���] �Էµ� ����� Text ����(txt, csv ���� Ȯ���ڸ� ���� ����)�� �о arrContents�� �����Ѵ�. 
//! [�Է°�] szFilePath : ����Ǵ� ������ ���� ���
//! [��ȯ��] �۾��� �Ϸ��ϸ� ���� 1�� ��ȯ�Ѵ�. ������ �߻��ϸ� ������, ������ ��Ÿ���� �������� ��ȯ�Ѵ�. 
//!			 arrContents : Text ������ ���ڿ��� �����Ͽ� ��ȯ�Ѵ�. CStringArray ��� 1���� Text ���Ͽ����� 1���̴�. 
int ReadTextFile(CString szFilePath, CStringArray &arrContents)
{
	if ( szFilePath.GetLength() <= 0 )
	{
		return -1;
	}

	if ( ::GetFileAttributes(szFilePath) == -1 )	//! ������ ���� ���
	{
		return -2;
	}

	//! ���� ������ ����
	arrContents.RemoveAll();
	arrContents.FreeExtra();

	try
	{
		CStdioFile ReadTextFile(szFilePath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone);	//! ���� ����

		CString szRead = _T("");

		while ( ReadTextFile.ReadString(szRead) )
		{
			arrContents.Add(szRead);
		}		

		ReadTextFile.Close();
	}
	catch(CFileException *e)
	{
		int iErrorValue = e->m_cause;
		e->Delete();
		return iErrorValue;
	}

	return 1;
}

//! [���] �ش� int ���� ���� ���� ���ڿ��� ��ȯ
//! [�Է°�] iErrorValue : Error�� ��Ÿ���� CFileException�� m_cause ��
//! [��ȯ��] ���� ���ڿ�
CString GetErrorMsg_FileException(int iErrorValue)
{
	CString szReturn = _T("");

	switch(iErrorValue)
	{
	case CFileException::none:
		{
			szReturn = _T("No error ");
		}
		break;
	case CFileException::genericException:
		{
			szReturn = _T("An unspecified error occurred. ");
		}
		break;
	case CFileException::fileNotFound:
		{
			szReturn = _T("The file could not be located. ");
		}
		break;
	case CFileException::badPath:
		{
			szReturn = _T("All or part of the path is invalid. ");
		}
		break;
	case CFileException::tooManyOpenFiles:
		{
			szReturn = _T("The permitted number of open files was exceeded. ");
		}
		break;
	case CFileException::accessDenied:
		{
			szReturn = _T("The file could not be accessed. ");
		}
		break;
	case CFileException::invalidFile:
		{
			szReturn = _T("There was an attempt to use an invalid file handle. ");
		}
		break;
	case CFileException::removeCurrentDir:
		{
			szReturn = _T("The current working directory cannot be removed. ");
		}
		break;
	case CFileException::directoryFull:
		{
			szReturn = _T("There are no more directory entries. ");
		}
		break;
	case CFileException::badSeek:
		{
			szReturn = _T("There was an error trying to set the file pointer. ");
		}
		break;
	case CFileException::hardIO:
		{
			szReturn = _T("There was a hardware error. ");
		}
		break;
	case CFileException::sharingViolation:
		{
			szReturn = _T("A shared region was locked. ");
		}
		break;
	case CFileException::lockViolation:
		{
			szReturn = _T("There was an attempt to lock a region that was already locked. ");
		}
		break;
	case CFileException::diskFull:
		{
			szReturn = _T("The disk is full. ");
		}
		break;
	case CFileException::endOfFile:
		{
			szReturn = _T("The end of file was reached. ");
		}
		break;
	}

	return szReturn;
}

//! [���] ���ڿ� �Ľ� �Լ�
//! [�Լ� ����] Token : �Ľ̵� ���ڿ��� ������� ����ȴ�. 
//!				strOriginal : �Էµ� ���ڿ�
//!				cpDelimiter : �Ľ��� ���� ���� (��. UMAC���� ���� ���� �ּҿ��� ���� �о���̸� 0x0D�� ���еǾ� �ִ�.)
void ParseString(CStringArray& Token, CString& szOriginal, TCHAR* cpDelimiter)
{
	int i = 0;
	int iOldPos = 0;
    Token.RemoveAll();
	Token.FreeExtra();

	int iOriginalLen = (int)(szOriginal.GetLength());

    for (i = 0; i < iOriginalLen; i++)
    {
        if (szOriginal.GetAt(i) == *cpDelimiter)
        {            
			Token.Add(szOriginal.Mid(iOldPos, i - iOldPos));
            iOldPos = i + 1;
        }
    }

	CString szLast = szOriginal.Mid(iOldPos, i - iOldPos);
	if ( szLast.GetLength() > 0 )
	{
		Token.Add(szLast);
	}
}

//! [���] ���ڿ� �Ľ� �Լ�
//! [�Լ� ����] Token : �Ľ̵� ���ڿ��� ������� ����ȴ�. 
//!				strOriginal : �Էµ� ���ڿ�
//!				cDelimiter : �Ľ��� ���� ���� (��. UMAC���� ���� ���� �ּҿ��� ���� �о���̸� 0x0D�� ���еǾ� �ִ�.)
void ParseString(CStringArray& Token, CString& szOriginal, TCHAR cDelimiter)
{
	int i = 0;
	int iOldPos = 0;
    Token.RemoveAll();
	Token.FreeExtra();

	int iOriginalLen = (int)(szOriginal.GetLength());

    for (i = 0; i < iOriginalLen; i++)
    {
        if (szOriginal.GetAt(i) == cDelimiter)
        {            
			Token.Add(szOriginal.Mid(iOldPos, i - iOldPos));
            iOldPos = i + 1;
        }
    }

	CString szLast = szOriginal.Mid(iOldPos, i - iOldPos);
	if ( szLast.GetLength() > 0 )
	{
		Token.Add(szLast);
	}
}

//! [���] ���ڿ� �и� �Լ�, ���ڿ��� 2���� ���ڷ� �и��Ѵ�.
//! [�Է°�] szInput : �Էµ� ���ڿ�, 
//!			 cpDelimiter : �Ľ��� ���� ����
//! [��ȯ��] �۾��� �Ϸ�Ǹ� true�� ��ȯ�Ѵ�. ������ �߻��ϸ� false�� ��ȯ�Ѵ�. 
//!			 iFirst : ���� ���� ���� ����, iSecond : ���� ���� ���� ����
bool GetSeparateNumber(CString szInput, int &iFirst, int &iSecond, TCHAR* cpDelimiter)
{
	int iLen = szInput.GetLength();
	if ( iLen <= 0 )
	{
		return false;
	}

	int iPos = szInput.Find(cpDelimiter);
	if ( iPos < 0 )	//! ���� ���ڰ� ���� ��
	{
		return false;
	}

	if ( iPos == 0 )			//! ���� ���ڰ� �� �տ� ���� ��
	{
		return false;
	}
	if ( iPos == (iLen - 1) )	//! ���� ���ڰ� �� �ڿ� ���� ��
	{
		return false;
	}

	CString szLeft = szInput.Left(iPos);
	CString szRight = szInput.Right(iLen - 1 - iPos);

	iFirst = _ttoi(szLeft);
	iSecond = _ttoi(szRight);

	return true;
}

//! [���] ���ڿ� �и� �Լ�, ���ڿ��� 2���� ���ڷ� �и��Ѵ�.
//! [�Է°�] szInput : �Էµ� ���ڿ�, 
//!			 cpDelimiter : �Ľ��� ���� ����
//! [��ȯ��] �۾��� �Ϸ�Ǹ� true�� ��ȯ�Ѵ�. ������ �߻��ϸ� false�� ��ȯ�Ѵ�. 
//!			 iFirst : ���� ���� ���� ����, dbSecond : ���� ���� ���� ����
bool GetSeparateNumber(CString szInput, int &iFirst, double &dbSecond, TCHAR* cpDelimiter)
{
	int iLen = szInput.GetLength();
	if ( iLen <= 0 )
	{
		return false;
	}

	int iPos = szInput.Find(cpDelimiter);
	if ( iPos < 0 )	//! ���� ���ڰ� ���� ��
	{
		return false;
	}

	if ( iPos == 0 )			//! ���� ���ڰ� �� �տ� ���� ��
	{
		return false;
	}
	if ( iPos == (iLen - 1) )	//! ���� ���ڰ� �� �ڿ� ���� ��
	{
		return false;
	}

	CString szLeft = szInput.Left(iPos);
	CString szRight = szInput.Right(iLen - 1 - iPos);

	iFirst = _ttoi(szLeft);
	dbSecond = Convert_String_Double(szRight);

	return true;
}

//! [���] ���ڿ� �и� �Լ�, ���ڿ��� 2���� ���ڿ��� �и��Ѵ�.
//! [�Է°�] szInput : �Էµ� ���ڿ�, 
//!			 cpDelimiter : �Ľ��� ���� ����
//! [��ȯ��] �۾��� �Ϸ�Ǹ� true�� ��ȯ�Ѵ�. ������ �߻��ϸ� false�� ��ȯ�Ѵ�. 
//!			 szFirst : ���� ���� ���� ���ڿ�, szSecond : ���� ���� ���� ���ڿ�
bool GetSeparateString(CString szInput, CString &szFirst, CString &szSecond, TCHAR* cpDelimiter)
{
	int iLen = szInput.GetLength();
	if ( iLen <= 0 )
	{
		return false;
	}

	int iPos = szInput.Find(cpDelimiter);
	if ( iPos < 0 )	//! ���� ���ڰ� ���� ��
	{
		szFirst = szInput;
		szSecond = _T("");
		return false;
	}

	if ( iPos == 0 )			//! ���� ���ڰ� �� �տ� ���� ��
	{
		return false;
	}
	if ( iPos == (iLen - 1) )	//! ���� ���ڰ� �� �ڿ� ���� ��
	{
		return false;
	}

	szFirst = szInput.Left(iPos);
	szSecond = szInput.Right(iLen - 1 - iPos);

	return true;
}

#include <math.h>	//! pow, floor �Լ� ������ �ʿ��ϴ�. 

//! [���] �Ҽ��� �ݿø� �Լ�
//! [�Լ� ����] dbInput : �Է��� �Ǽ���
//!				iResultMode : 0 �̻��� ���� ����
//!							  (��. 1 : �Ҽ��� 2° �ڸ����� �ݿø�, 2 : �Ҽ��� 3° �ڸ����� �ݿø�)
//!	[��ȯ��] �ݿø��� �Ǽ���
double GetFloor(double dbInput, int iResultMode)
{
	double dbResult = 0.;

	double dbBase = pow(10., iResultMode);
	dbResult = floor(dbBase * (dbInput + (0.5 / dbBase))) / dbBase;

	return dbResult;
}

//! ���ڿ��� �����ο� �Ҽ��� �ڸ� �κ��� 2���� ������ ��ȯ
bool GetInteger_From_Double(CString szInput, int &iInterger, int &iDecimalPoint)
{
	int iLen = szInput.GetLength();
	if ( iLen <= 0 )
	{
		return false;
	}

	CString szInteger = _T(""), szDecimalPoint = _T("");

	GetSeparateString(szInput, szInteger, szDecimalPoint, _T("."));

	szInteger.TrimLeft();
	szInteger.TrimRight();
	szDecimalPoint.TrimLeft();
	szDecimalPoint.TrimRight();

	if ( szInteger.GetLength() <= 0 )
	{
		iInterger = 0;
	}
	else
	{
		iInterger = _ttoi(szInteger);
	}

	if ( szDecimalPoint.GetLength() <= 0 )
	{
		iDecimalPoint = 0;
	}
	else
	{
		iDecimalPoint = _ttoi(szDecimalPoint);
	}

	return true;
}

//! ���ڿ��� �����ο� �Ҽ��� �ڸ� �κ��� �Ǽ��� ��ȯ
bool GetInteger_From_Double(CString szInput, int &iInterger, double &dbDecimalPoint)
{
	int iLen = szInput.GetLength();
	if ( iLen <= 0 )
	{
		return false;
	}

	CString szInteger = _T(""), szDecimalPoint = _T("");

	GetSeparateString(szInput, szInteger, szDecimalPoint, _T("."));

	szInteger.TrimLeft();
	szInteger.TrimRight();
	szDecimalPoint.TrimLeft();
	szDecimalPoint.TrimRight();

	if ( szInteger.GetLength() <= 0 )
	{
		iInterger = 0;
	}
	else
	{
		iInterger = _ttoi(szInteger);
	}

	if ( szDecimalPoint.GetLength() <= 0 )
	{
		dbDecimalPoint = 0.;
	}
	else
	{
		CString szTemp = _T("");
		szTemp.Format(_T("0.%s"), szDecimalPoint);
		dbDecimalPoint = Convert_String_Double(szTemp);
	}

	return true;
}

//! [���] �Ǽ��� �Էµ� �Ҽ��� �ڸ����� ���� ���ڿ��� ��ȯ
//! [�Լ� ����] dbInput : �Է��� �Ǽ���, iPrecision : �Ҽ��� �ڸ���
CString GetStringFromDouble(double dbInput, int iPrecision)
{
	CString szReturn = _T("");
	if ( iPrecision < 0 )
	{
		iPrecision = 0;
	}
	CString szPrecision = _T("");
	szPrecision.Format(_T("%%.%dlf"), iPrecision);
	szReturn.Format(szPrecision, GetFloor(dbInput, iPrecision));
	return szReturn;
}

//! �Էµ� szInput1 ���ڿ����� szInput2 ���ڿ��� ���Ե� ������ �˾Ƴ���. 
//! [�Լ� ����] szInput1 : ���� ���ڿ�, szInput2 : ���� ���ڿ����� ã�Ƴ����� �ϴ� ���ڿ�
//! [��ȯ��] szInput1 ���ڿ� �ȿ��� szInput2 ���ڿ��� ����
//!			 ���ڿ��� ���̰� '0'�̰ų�, �ش� ���ڿ��� ���� ���ڿ��� ������ '0'�� ��ȯ
int GetCount_String_In_String(CString szInput1, CString szInput2)
{
	int iRet = 0;

	int iLength_Input = szInput1.GetLength();

	if ( iLength_Input <= 0 )
	{
		return 0;
	}
	if ( szInput2.GetLength() <= 0 )
	{
		return 0;
	}

	int iIndex_Start = 0;
	int iIndex_Ret_Start = 0;

	while(1)
	{
		if ( iIndex_Start > iLength_Input )
		{
			break;
		}

		iIndex_Ret_Start = szInput1.Find(szInput2, iIndex_Start);
		if ( iIndex_Ret_Start < 0 )
		{
			break;
		}

		iIndex_Start = iIndex_Ret_Start + 1;

		iRet++;
	}//! while(1)

	return iRet;
}

//! �Է��� �������� 2���� �и��Ͽ�, 2���� 8 bit 16������ ��ȯ
void Separate_Hex_From_Integer_8bit(int iInput, int &iHigh, int &iLow)
{
	iLow  = iInput & 0xff;
	iHigh = (iInput >> 8) & 0xff;
}

//! �Է��� 2���� 8 bit 16�������� ���ļ�, 1���� �������� ��ȯ
void Merge_Integer_From_Hex_8bit(int iHigh, int iLow, int &iOutput)
{
	iOutput =  iLow & 0xff;
	iOutput += (iHigh & 0xff) << 8;
}

//! �Էµ� 16���� ���ڿ� �ش��ϴ� ���ڸ� ��ȯ
TCHAR GetHexChar(int iInput)
{
	TCHAR cRet = '-';

	int iValue = iInput % 16;

	switch(iValue)
	{
	case 0:
		{
			cRet = '0';
		}
		break;
	case 1:
		{
			cRet = '1';
		}
		break;
	case 2:
		{
			cRet = '2';
		}
		break;
	case 3:
		{
			cRet = '3';
		}
		break;
	case 4:
		{
			cRet = '4';
		}
		break;
	case 5:
		{
			cRet = '5';
		}
		break;
	case 6:
		{
			cRet = '6';
		}
		break;
	case 7:
		{
			cRet = '7';
		}
		break;
	case 8:
		{
			cRet = '8';
		}
		break;
	case 9:
		{
			cRet = '9';
		}
		break;
	case 10:
		{
			cRet = 'A';
		}
		break;
	case 11:
		{
			cRet = 'B';
		}
		break;
	case 12:
		{
			cRet = 'C';
		}
		break;
	case 13:
		{
			cRet = 'D';
		}
		break;
	case 14:
		{
			cRet = 'E';
		}
		break;
	case 15:
		{
			cRet = 'F';
		}
		break;
	}

	return cRet;
}

//! 10������ 16���� ���ڷ� ����
long Convert_Decimal_To_Hex(int iInput_Decimal)
{
	//! 10������ 16���� ���ڿ��� ��ȯ
	TCHAR czChar[100];
		
	int iCnt = 0;
	int iQuotient = 0;	//! ��
	int iRemain = 0;	//! ������

	int iValue = iInput_Decimal;

	while(1)
	{
		iQuotient = iValue / 16;
		iRemain   = iValue % 16;

		czChar[iCnt++] = GetHexChar(iRemain);

		if ( iQuotient < 16 )
		{
			czChar[iCnt++] = GetHexChar(iQuotient);
			break;
		}

		iValue = iQuotient;
	}//! while(1)

	//! �Է��� ���ڿ��� �������� �����Ѵ�. 
	int i = 0;
	TCHAR szNum[100];
	memset(szNum, 0, sizeof(szNum) * sizeof(TCHAR));
	for ( i = 0; i < iCnt; i++ )
	{
		_stprintf_s(szNum, _T("%s%c"), szNum, czChar[iCnt - i - 1]);
	}

	long lHex = HexString_to_Num(szNum);

	return lHex;
}

//! [���] �Էµ� ���ڿ��� 16���� ���ڷ� ��ȯ�Ѵ�. (��. ���ڿ� "0x16" -> ���� 0x16)
//! [�Լ� ����] czHex : ���ڿ�
//! [��ȯ��] 16���� ������
//! [���� ����] �Էµ� ���ڿ��� �������� NULL ���ڸ� ���� ���ڿ��̾�� �Ѵ�. 
long HexString_to_Num(LPTSTR czHex)
{
	TCHAR szBuf[20];
	TCHAR *cpEnd;
	_stprintf_s(szBuf, _T("%s"), czHex);
	long lRet = _tcstol(szBuf, &cpEnd, 16);	//! 16���� ���ڷ� ��ȯ
	return lRet;
}

long HexString_to_Num(CString szHex)
{
	int iLen = (int)(szHex.GetLength());
	if ( iLen <= 0 )
	{
		return -1;
	}
	TCHAR *czTemp = szHex.GetBuffer( iLen + 1 );
	long lRet = HexString_to_Num(czTemp);
	szHex.ReleaseBuffer();

	return lRet;
}

//! [���] Hex -> Dec
//!        �Էµ� 16���� ���ڸ� 10���� ���ڷ� ����
int ConvertHexToDec(TCHAR *pszHex)
{
	int iDec = 0;
	int iTemp = 0;
    int iLen = 0;
    int i = 0;           

    iLen = _tcslen(pszHex);

    for (i = 0;  i < iLen ; i++)
    {
        iTemp = 0;                     

        if (*(pszHex+i) >= 'A' && *(pszHex+i) <='F')
            iTemp = (*(pszHex+i) - (TCHAR)'A' + 10);
        else
            iTemp = (*(pszHex+i) - (TCHAR)'0');

        iDec = iDec * 16 + iTemp;
    }

    return iDec;
}

//! [���] �簢 ������ �������� �߽����� ��ǥ�� ��, ������ ���̸� ������, �簢 ������ ��ȯ
//! [�Է°�] iCenterX : �簢 ������ �������� �߽����� ��ǥ X,
//!			 iCenterY : �簢 ������ �������� �߽����� ��ǥ Y, 
//!			 iWidth : �簢 ������ ��, iHeight : �簢 ������ ����
//! [��ȯ��] �簢 ����
CRect makeP2R(int iCenterX, int iCenterY, int iWidth, int iHeight)
{
	CRect rcRet;

	rcRet.left = iCenterX - (iWidth >> 1);
	rcRet.top = iCenterY - (iHeight >> 1);
	rcRet.right = rcRet.left + iWidth;
	rcRet.bottom = rcRet.top + iHeight;

	return rcRet;
}

//! [���] pWndofSub�� pWndToCenterOn�� �߽ɿ� ��ġ ��Ų��. 
//! [�Է°�] pWndToCenterOn : ���� Window, pWndofSub : ��ġ�� �����Ϸ��� Window
void CenterWindowOnOwner(CWnd* pWndToCenterOn, CWnd* pWndofSub, bool bFlag_Top)
{
	//! Window�� ��ȿ�� �˻�
	if ( pWndToCenterOn == NULL || pWndofSub == NULL )
	{
		return;
	}

	//! �� Window�� �簢 ������ ���Ѵ�. 
	CRect rectToCenterOn;
	pWndToCenterOn->GetWindowRect(&rectToCenterOn);

	CRect rectSubWindow;
	pWndofSub->GetWindowRect(&rectSubWindow);

	//! �߽����� �̵��ϱ� ���� ���� ���� ��ǥ�� ���Ѵ�. 
	int iXLeft = (int)((double)(rectToCenterOn.left + rectToCenterOn.right) / 2. - (double)(rectSubWindow.Width()) / 2.);
	int iYTop = (int)((double)(rectToCenterOn.top + rectToCenterOn.bottom) / 2. - (double)(rectSubWindow.Height()) / 2.);

	//! Window ��ġ �̵� (ũ��� �������� �ʴ´�.)
	if ( bFlag_Top == true )
	{
		SetWindowPos(pWndofSub->GetSafeHwnd(), HWND_TOPMOST, iXLeft, iYTop, -1, -1, SWP_NOSIZE | SWP_NOACTIVATE);
	}
	else
	{
		SetWindowPos(pWndofSub->GetSafeHwnd(), NULL,		 iXLeft, iYTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	//pWndofSub->SetWindowPos(NULL, iXLeft, iYTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

//! [���] �ش� ��ü�� ȭ�� �� ���� �����Ѵ�.
void Set_TopWindow(CWnd* pWnd)
{
	//! Window�� ��ȿ�� �˻�
	if ( pWnd == NULL )
	{
		return;
	}

	SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

//! [���] HDD�� ����� �����´�.
//! [��ȯ��] ������ ���� ��쿡�� ������ ��ȯ�Ѵ�. arrDriveList���� HDD�� ����� ��� �ִ�. 
//!			 HDD�� ������ '0'�� ��ȯ�Ѵ�. HDD�� �߰ߵǸ� HDD�� ������ ��ȯ�Ѵ�. 
int GetHDDList(CStringArray &arrDriveList)
{
	arrDriveList.RemoveAll();
	arrDriveList.FreeExtra();

	//! ���� PC�� ����̹� ����� �����´�. 
	DWORD dwDrives = ::GetLogicalDrives();
	if ( dwDrives <= 0 )
	{
		return -1;
	}

	CString szDriverTemp = _T("");
	DWORD dwMask = 0x01;
	int i = 0;
	for ( i = 0; i < 32; i++ )
	{
		if ( dwDrives & (dwMask << i) )
		{
			szDriverTemp.Format(_T("%c:\\"), _T('A') + i);		//! 'A:\\ ~ Z:\\' ������ ����̹��� �ִ��� Ȯ��

			if ( GetDriveType(szDriverTemp) == DRIVE_FIXED )	//! HDD�� ���
			{
				arrDriveList.Add(szDriverTemp);
			}
			//! DRIVE_REMOVABLE : USB, DRIVE_CDROM : ODD
		}
	}

	int iSize = (int)(arrDriveList.GetSize());
	
	return iSize;
}

//! [���] HDD�� ���� �뷮�� �˾Ƴ���. 
//! [�Է°�] ����̹� �̸� ���ڿ� ("C:\\", "D:\\", ...)
//! [��ȯ��] HDD�� ���� �뷮, ���� MByte, ������ ���� ��쿡�� 0�� ��ȯ�Ѵ�. 
//!			 pdbTotalSize�� ������ �����Ǿ� ������, HDD�� �� ũ�⸦ ��ȯ�Ѵ�. ���� MByte
double GetRemainSize_InHDD(CString szDriver, double *pdbTotalSize)
{
	if ( szDriver.GetLength() <= 0 )	//! ����̹� �̸� �˻� 
	{
		return 0.;
	}

	CString szDriverName = szDriver.Left(1);	//! ù��° ���ڸ� �����´�. 

	//! ���� PC�� ����̹� ����� �����´�. 
	DWORD dwDrives = ::GetLogicalDrives();
	if ( dwDrives <= 0 )
	{
		return 0.;
	}

	bool bFind = false;
	CString szDriverTemp = _T("");
	DWORD dwMask = 0x01;
	int i = 0;
	for ( i = 0; i < 32; i++ )
	{
		if ( dwDrives & (dwMask << i) )
		{
			szDriverTemp.Format(_T("%c"), _T('A') + i);
			if ( szDriverName.CompareNoCase(szDriverTemp) == 0 )
			{
				bFind = true;
				break;
			}
		}
	}

	if ( bFind == false )	//! �Է��� ����̹��� ���� PC�� ���� ��
	{
		return 0.;
	}

	//! HDD�� ������ �����´�. 
	ULARGE_INTEGER i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
	if ( ::GetDiskFreeSpaceEx(
			szDriverName + _T(":\\"), 
			(PULARGE_INTEGER)&i64FreeBytesToCaller, 
			(PULARGE_INTEGER)&i64TotalBytes, 
			(PULARGE_INTEGER)&i64FreeBytes) == FALSE )
	{
		return 0.;
	}

	if ( pdbTotalSize != NULL && i64TotalBytes.QuadPart != 0 )
	{
		*pdbTotalSize = (double)((DWORD)(i64TotalBytes.QuadPart / (1024 * 1024)));
	}

	if ( i64FreeBytesToCaller.QuadPart != 0 )
	{
		double dbMegaSize = (double)((DWORD)(i64FreeBytesToCaller.QuadPart / (1024 * 1024)));

		return dbMegaSize;
	}
	
	return 0.;
}

//! [���] �Է��� ���ҽ����� ���ڿ��� �����´�. 
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� Edit ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//! [��ȯ��] ������ ������ ������ ��ȯ, ������ ������ 1�� ��ȯ�Ѵ�. 
//!				-1 : �θ� ������ �����Ͱ� NULL�̴�. 
//!				-2 : ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ����.
//!				-3 : ���ڿ��� ���̰� 0�̴�.
//!			 szRet : ��ȯ�Ǵ� ���ڿ�
int GetString_FromUI(int iUID, CWnd* pParentWnd, CString &szRet)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	//! ���ڿ��� �����´�. 
	pWnd->GetWindowText(szRet);

	//! ���ڿ��� ���̸� �˻�
	szRet.TrimLeft();
	szRet.TrimRight();
	int iStringSize = (int)(szRet.GetLength());
	if ( iStringSize <= 0 )
	{
		return -3;
	}

	return 1;
}

//! [���] �Է��� ���ҽ����� ���� ������ ���� �����´�. 
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� Edit ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//! [��ȯ��] ���ҽ��� ǥ�õ� ������ ��, ������ ������ ������ ��ȯ
//!				-1 : �θ� ������ �����Ͱ� NULL�̴�. 
//!				-2 : ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ����.
//!				-3 : ���ڿ��� ���̰� 0�̴�.
//!				-4 : ���ڿ��� ������ �ƴϴ�. 
int GetIntegerValue_FromUI(int iUID, CWnd* pParentWnd)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	//! ���ڿ��� �����´�. 
	CString szGetTemp = _T("");
	pWnd->GetWindowText(szGetTemp);

	//! ���ڿ��� ���̸� �˻�
	szGetTemp.TrimLeft();
	szGetTemp.TrimRight();
	int iStringSize = (int)(szGetTemp.GetLength());
	if ( iStringSize <= 0 )
	{
		return -3;
	}

	//! �Է°��� �������� Ȯ��
	bool bDigit = true;
	int i = 0;
	for (i = 0; i < iStringSize; i++ )
	{
		if ( _istdigit( szGetTemp.GetAt(i) ) == 0 )
		{
			bDigit = false;
			break;
		}
	}
	if ( bDigit == false )
	{
		return -4;
	}

	//! ���ڿ� -> ����
	int iRet = _ttoi(szGetTemp);

	return iRet;
}

//! [���] �Է��� ���ҽ����� �Ǽ��� ���� �����´�. 
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� Edit ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//! [��ȯ��] ������ ������ ������ ��ȯ, ������ ������ 1�� ��ȯ�Ѵ�. 
//!				-1 : �θ� ������ �����Ͱ� NULL�̴�. 
//!				-2 : ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ����.
//!				-3 : ���ڿ��� ���̰� 0�̴�.
//!				-4 : ���ڿ��� �Ǽ����� �ƴϴ�. 
//!			 dbRet : ��ȯ�Ǵ� �Ǽ���
int GetDoubleValue_FromUI(int iUID, CWnd* pParentWnd, double &dbRet)
{
	dbRet = -1.;

	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	//! ���ڿ��� �����´�. 
	CString szGetTemp = _T("");
	pWnd->GetWindowText(szGetTemp);

	//! ���ڿ��� ���̸� �˻�
	szGetTemp.TrimLeft();
	szGetTemp.TrimRight();
	int iStringSize = (int)(szGetTemp.GetLength());
	if ( iStringSize <= 0 )
	{
		return -3;
	}

	//! �Է°��� �Ǽ�(�������� �������� ����)���� Ȯ��
	bool bDigit = true;
	TCHAR tempCharacter;
	int i = 0, iDotCnt = 0;
	for (i = 0; i < iStringSize; i++ )
	{
		tempCharacter = szGetTemp.GetAt(i);

		if ( i == 0 )
		{
			if ( tempCharacter == _T('-') || tempCharacter == _T('+') )
			{
				continue;
			}
		}

		if ( tempCharacter == _T('.') )
		{
			iDotCnt++;
			continue;
		}

		if ( bDigit == true )
		{
			if ( _istdigit(tempCharacter) == 0 )
			{
				bDigit = false;			
			}
		}

		if ( bDigit == false )
		{
			break;
		}
	}
	if ( iDotCnt > 1 )
	{
		return -4;
	}
	if ( bDigit == false )
	{
		return -4;
	}

	//! ���ڿ� -> ����
	TCHAR *pEndString = _T("");
	double dbReturn = _tcstod(szGetTemp, &pEndString);

	dbRet = dbReturn;

	return 1;
}

//! [���] �Է��� ���ҽ��� ���ڿ��� �����Ѵ�.
//! [�Է°�] iUID : ���ҽ� ID, pParentWnd : �θ� ������ ������, szInput : ������ ���ڿ�
//! [��ȯ��] ������ ������ ������ ��ȯ, ������ ������ 1�� ��ȯ�Ѵ�. 
int SetString_to_UI(int iUID, CWnd* pParentWnd, CString szInput)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	//! ���ڿ� ����
	pWnd->SetWindowText(szInput);

	return 1;
}

//! �Է��� ���ҽ��� �簢 ������ �����´�.
int GetWindowRect_FromUI(int iUID, CWnd* pParentWnd, CRect &rcRet)
{
	rcRet.SetRectEmpty();

	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	pWnd->GetWindowRect(&rcRet);

	return 1;
}

//! �Է��� ���ҽ��� �簢 ������ �����´�.
int GetClientRect_FromUI(int iUID, CWnd* pParentWnd, CRect &rcRet)
{
	rcRet.SetRectEmpty();

	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	pWnd->GetClientRect(&rcRet);

	return 1;
}

//! �Է��� ���ҽ��� �簢 ������ �����Ѵ�. 
int SetRect_InCtrl(int iUID, CWnd* pParentWnd, CRect rcInput)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	pWnd->MoveWindow(rcInput);

	return 1;
}

//! �Է��� ���ҽ��� �ڵ鰪�� �����´�.
HWND GetHandle_FromUI(int iUID, CWnd* pParentWnd)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return NULL;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return NULL;
	}

	return pWnd->GetSafeHwnd();
}

//! [���] �Էµ� ���ڿ��� �Ǽ��� ���ڷ� ��ȯ
//! [�Է°�] szInput : ���ڿ�
//! [��ȯ��] �Ǽ��� ����
double Convert_String_Double(CString szInput)
{
	TCHAR *pEndString = _T("");
	double dbReturn = _tcstod(szInput, &pEndString);
	return dbReturn;
}

//! [���] �Ǽ��� �Ǽ��� ���� �������� ��ȯ
//! [�Է°�] dbInput : �Է°�, dbDivide : �Է°��� ������ �ִ� ��
//! [��ȯ��] �Ǽ��� ����, piMultiply : �Է°��� ������ �ִ� ���� ��(�����踦 ��ȯ)
double GetDoubleModulus(double dbInput, double dbDivide, int *piMultiply)
{
	//! ������ �ִ� ���ڰ� �ʹ� ������ '0'�� ��ȯ
	if ( fabs(dbDivide) < 0.000001 )
	{
		return 0.;
	}

	double dbPortion = dbInput / dbDivide;
	int iPortion = (int)(dbPortion);
	double dbNewPortion = (double)iPortion;

	if ( piMultiply != NULL )
	{
		*piMultiply = iPortion;
	}

	double dbRet = dbInput - (dbNewPortion * dbDivide);
	return dbRet;
}

//! [���] �ش� ��Ʈ�ѿ� ��Ŀ�� ����
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
int SetFous_InCtrl(int iUID, CWnd* pParentWnd)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	pWnd->SetFocus();

	return 1;
}

//! [���] �ش� ��Ʈ���� ���� ���̰� �ϰų� �����
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//!			 bVisible : true�̸� �ش� ��Ʈ���� ���� ���̰� �ϰ�, false�̸� ���� ������ �ʰ� �Ѵ�. 
int SetVisibleState_InCtrl(int iUID, CWnd* pParentWnd, bool bVisible)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	if ( bVisible == true )
	{
		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd->ShowWindow(SW_HIDE);
	}

	return 1;
}

//! [���] �ش� ��Ʈ���� ���� ���̴� ����
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//!			 bVisible : true�̸� �ش� ��Ʈ���� ���� ���̰�, false�̸� ���� ������ �ʴ´�. 
int GetVisibleState_InCtrl(int iUID, CWnd* pParentWnd, bool &bGetVisible)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	if ( pWnd->IsWindowVisible() == TRUE )
	{
		bGetVisible = true;
	}
	else
	{
		bGetVisible = false;
	}

	return 1;
}

//! [���] �ش� ��Ʈ���� Ȱ��ȭ/��Ȱ��ȭ
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//!			 bEnable : true�̸� Ȱ��ȭ, false�̸� ��Ȱ��ȭ
int SetEnableState_InCtrl(int iUID, CWnd* pParentWnd, bool bEnable)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	if ( bEnable == true )
	{
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		pWnd->EnableWindow(FALSE);
	}

	return 1;
}

//! [���] �ش� ��Ʈ���� Ȱ��ȭ ����
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������
//!			 bGetEnable : true�̸� Ȱ��ȭ, false�̸� ��Ȱ��ȭ
int GetEnableState_InCtrl(int iUID, CWnd* pParentWnd, bool &bGetEnable)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	if ( pWnd->IsWindowEnabled() == TRUE )
	{
		bGetEnable = true;
	}
	else
	{
		bGetEnable = false;
	}

	return 1;
}

//! [���] �ش� Button(�Ǵ� Check, Radio) ��Ʈ���� ���� ���¸� �˾� ����. 
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��
//!			 pParentWnd : �θ� ������ ������
//! [��ȯ��] bGetCheck : ���õǾ� ������ true, �׷��� ������ false
int GetButtonCheckState_InCtrl(int iUID, CWnd* pParentWnd, bool &bGetCheck)
{
	bGetCheck = false;

	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	UINT nChecked = pParentWnd->IsDlgButtonChecked(iUID);
	if ( nChecked != 0 )
	{
		bGetCheck = true;
	}
	else
	{
		bGetCheck = false;
	}

	return 1;
}

//! [���] �ش� Button(�Ǵ� Check, Radio) ��Ʈ���� ���� ���¸� ����
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� ��Ʈ�ѵ��� ID��
//!			 pParentWnd : �θ� ������ ������
//!			 bCheck : true�̸� ����, false�̸� ���� ����
int SetButtonCheckState_InCtrl(int iUID, CWnd* pParentWnd, bool bCheck)
{
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	if ( bCheck == true )
	{
		pParentWnd->SendDlgItemMessage(iUID, BM_SETCHECK, BST_CHECKED, 0);
	}
	else
	{
		pParentWnd->SendDlgItemMessage(iUID, BM_SETCHECK, BST_UNCHECKED, 0);
	}

	return 1;
}

//! [���] �Էµ� ���ڿ� szText�� �Է��� ���ҽ� iUID�� ���� Edit ��Ʈ�ѿ� ǥ���Ѵ�. 
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� Edit ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������, szText : �߰� ���ڿ�
//! [��ȯ��] �۾��� �Ϸ��ϸ� 1�� ��ȯ, ������ ������ ������ ��ȯ
//!				-1 : �θ� ������ �����Ͱ� NULL�̴�. 
//!				-2 : ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ����.
//!				-3 : CEdit �� ��ȯ ����
int SetString_InEditCtrl(int iUID, CWnd* pParentWnd, CString szText)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	CEdit *pEdit = (CEdit*)pWnd;
	if ( pEdit == NULL )
	{
		return -3;
	}

	pEdit->SetWindowText(szText);

	return 1;
}

//! [���] �Էµ� ���ڿ� szAdd�� �Է��� ���ҽ� iUID�� ���� Edit ��Ʈ�ѿ� �߰��Ѵ�. 
//! [�Է°�] iUID : ���ҽ� ID, ȭ�� ���� Edit ��Ʈ�ѵ��� ID��, pParentWnd : �θ� ������ ������, szAdd : �߰� ���ڿ�
//!			 bNextLine : true�̸� �Է� ���ڿ� �ڷ� �� �ٲ� ���ڸ� �߰��Ѵ�. 
//!			 bReset : true�̸� ������ ���ڿ��� �����ϰ� ���� �Է� �����Ѵ�. 
//! [��ȯ��] �۾��� �Ϸ��ϸ� 1�� ��ȯ, ������ ������ ������ ��ȯ
//!				-1 : �θ� ������ �����Ͱ� NULL�̴�. 
//!				-2 : ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ����.
//!				-3 : CEdit �� ��ȯ ����
int AppendString_InEditCtrl(int iUID, CWnd* pParentWnd, CString szAdd, bool bNextLine, bool bReset)
{
	//! �Է°��� ��ȿ�� �˻�
	if ( pParentWnd == NULL )
	{
		return -1;
	}

	//! ���ҽ� ID�� �ش��ϴ� ��Ʈ���� ��ȿ�� �˻�
	CWnd* pWnd = NULL;
	pWnd = pParentWnd->GetDlgItem(iUID);
	if ( pWnd == NULL )
	{
		return -2;
	}

	CEdit *pEdit = (CEdit*)pWnd;
	if ( pEdit == NULL )
	{
		return -3;
	}

	//! ������ ���ڿ� ����, ���� �ʱ�ȭ
	if ( bReset == true )
	{
		pEdit->SetWindowText(_T(""));
	}

	CString szNew = szAdd;
	if ( bNextLine == true )
	{
		szNew += _T("\r\n");	//! �� �ٲ� ���� �߰� 
	}
	
	//! �� �������� ���ڿ� �߰�
	pEdit->SetSel(-1, -1);
	pEdit->ReplaceSel(szNew);

	//! �� ������ �ٷ� �̵��Ѵ�. 
	int iLineSize = pEdit->GetLineCount();
	pEdit->LineScroll(iLineSize);

	return 1;
}

//! [���] 'SetThreadAffinityMask' �Լ��� Mask ���ڰ��� ��ȯ�޴´�. 
//! [�Է°�] piCPU_Use : int �迭, CPU�� ����ϸ� 1, ������� ������ 0, 
//!			             0 �� index�� 1�� CPU�� ����Ų��. 1�� index�� 2�� CPU, ...
//!			 iSize : piCPU_Use �迭���� ����� �������� ����
//! [��ȯ��] 'SetThreadAffinityMask' �Լ��� Mask ���ڰ�, ������ �߻��ϸ� 0�� ��ȯ�Ѵ�. 
DWORD GetMask_CPU_Affinity(int *piCPU_Use, int iSize)
{
	if ( piCPU_Use == NULL )
	{
		return 0;
	}

	//! �ý����� ����� �˾Ƴ���. 
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	//! CPU ������ ���� ũ�⸦ Ȯ���Ѵ�. 
	int iCPU_Size = (int)(sysInfo.dwNumberOfProcessors);
	if ( iSize <= 0 || iSize > iCPU_Size )
	{
		return 0;
	}

	int i = 0;
	DWORD dwRet = 0;

	iSize--;
	for ( i = iSize; i >= 0; i-- )
	{
		if ( piCPU_Use[i] != 0 )
		{
			dwRet = dwRet | (0x01 << (iSize - i));
		}
	}

	return dwRet;
}

//! Process (���α׷� ��ü)���� ����ϴ� CPU�� �����Ѵ�.
//! [���� ����] 
//!		1) Process���� ����� CPU�� ������ ��� Thread�� ����� CPU�� �����ؾ� �Ѵ�. 
//!		2) Thread Affinity(ģȭ��)�� �׻� Process Affinity(ģȭ��)�� ������̾�� �Ѵ�.
//! [�Է°�]
//!     bFlag_All_CPU : true�̸�,  PC�� ��� CPU Core�� ����ϴ� ������ �����Ѵ�. piCPU_Use, iSize ���� ������� �ʴ´�.
//!                     false�̸�, iSize ũ���� piCPU_Use ������ �����͸� �̿��Ѵ�.
//!     piCPU_Use : 0 �� index�� 1�� CPU�� ����Ų��. 1�� index�� 2�� CPU, ...
int SetProcessAffinity(bool bFlag_All_CPU, int *piCPU_Use, int iSize)
{
	if ( bFlag_All_CPU == false )
	{
		if ( piCPU_Use == NULL || iSize <= 0 )
		{
			return -1;
		}
	}

	//! �ý����� ����� �˾Ƴ���. 
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);	
	
	//! CPU ������ ���� ����� CPU Core�� �����Ѵ�. 
	int iCPU_Size = (int)(sysInfo.dwNumberOfProcessors);
	if ( iCPU_Size <= 0 )
	{
		return -2;
	}

	int i = 0;	
	const int iBufSize_Process_CPU_Core = 100;

	//! GetMask_CPU_Affinity �Լ��� �Է��� ����
	int aiCPU[iBufSize_Process_CPU_Core];
	memset(aiCPU, 0, sizeof(aiCPU));

	//! ��� ������ CPU�� ������ �˾Ƴ���.
	int iSize_Possible_CPU_Core = 0;
	if ( iCPU_Size >= iBufSize_Process_CPU_Core )
	{
		iSize_Possible_CPU_Core = iBufSize_Process_CPU_Core;
	}
	else
	{
		iSize_Possible_CPU_Core = iCPU_Size;
	}	

	if ( bFlag_All_CPU == false )
	{
		//! GetMask_CPU_Affinity �Լ��� �Է��� ���� ����
		//! ���ÿ� ���ۿ��� ������ ��� CPU ������ Ȯ��	
		int iCnt_Use_CPU = 0;
		for ( i = 0; i < iSize_Possible_CPU_Core; i++ )
		{
			if ( i >= iSize )
			{
				break;
			}

			if ( piCPU_Use[i] != 0 )
			{
				aiCPU[i] = 1;
				iCnt_Use_CPU++;
			}
		}

		if ( iCnt_Use_CPU <= 0 )
		{
			return -3;
		}
	}
	else
	{
		for ( i = 0; i < iSize_Possible_CPU_Core; i++ )
		{
			if ( i >= iBufSize_Process_CPU_Core )
			{
				break;
			}

			aiCPU[i] = 1;			
		}
	}

	DWORD dwMaskValue = GetMask_CPU_Affinity(aiCPU, iSize_Possible_CPU_Core);

	HANDLE hProcess = GetCurrentProcess();

	//! Process�� �۵���ų CPU�� �����Ѵ�.  
	DWORD dwRet = ::SetProcessAffinityMask(hProcess, dwMaskValue);
	if ( dwRet <= 0 )
	{
		DWORD dwError = GetLastError();

		//! The handle is invalid		
		if ( dwError == ERROR_INVALID_HANDLE )
		{
			return -4;
		}

		//! 'dwMaskValue' �Է°��� ������ ����
		if ( dwError == ERROR_INVALID_PARAMETER )
		{
			return -5;
		}

		return -6;
	}

	return 1;
}

//! ������ ���� bool�� �迭(ũ�Ⱑ 16 �̻�)�� ��ȯ
bool Convert_Integer_To_BooleanBuffer(int iInput, bool *pabOutput, int iBufSize_Output)
{
	if ( pabOutput == NULL || iBufSize_Output < 16 )
	{
		return false;
	}

	int aiChk[16];
	aiChk[0]  = 0x0001;
	aiChk[1]  = 0x0002;
	aiChk[2]  = 0x0004;
	aiChk[3]  = 0x0008;
	aiChk[4]  = 0x0010;
	aiChk[5]  = 0x0020;
	aiChk[6]  = 0x0040;
	aiChk[7]  = 0x0080;
	aiChk[8]  = 0x0100;
	aiChk[9]  = 0x0200;
	aiChk[10] = 0x0400;
	aiChk[11] = 0x0800;
	aiChk[12] = 0x1000;
	aiChk[13] = 0x2000;
	aiChk[14] = 0x4000;
	aiChk[15] = 0x8000;

	int i = 0;
	for ( i = 0; i < 16; i++ )
	{
		if ( iInput & aiChk[i] )
		{
			pabOutput[i] = true;
		}
		else
		{
			pabOutput[i] = false;
		}
	}	

	return true;
}

//! bool�� �迭(ũ�Ⱑ 16 �̻�)�� ������ ������ ��ȯ
bool Convert_BooleanBuffer_To_Integer(bool *pabInput, int iBufSize_Input, int *piOutput)
{
	if ( pabInput == NULL || iBufSize_Input < 16 || piOutput == NULL )
	{
		return false;
	}

	int iRet = *piOutput;

	int aiChk[16];
	aiChk[0]  = 0x0001;
	aiChk[1]  = 0x0002;
	aiChk[2]  = 0x0004;
	aiChk[3]  = 0x0008;
	aiChk[4]  = 0x0010;
	aiChk[5]  = 0x0020;
	aiChk[6]  = 0x0040;
	aiChk[7]  = 0x0080;
	aiChk[8]  = 0x0100;
	aiChk[9]  = 0x0200;
	aiChk[10] = 0x0400;
	aiChk[11] = 0x0800;
	aiChk[12] = 0x1000;
	aiChk[13] = 0x2000;
	aiChk[14] = 0x4000;
	aiChk[15] = 0x8000;

	int i = 0;
	for ( i = 0; i < 16; i++ )
	{
		if ( pabInput[i] == true )
		{
			iRet |= aiChk[i];
		}
		else
		{
			iRet &= (~aiChk[i]);
		}
	}

	*piOutput = iRet;

	return true;
}

//! 16 bit ���� 2���� ���ļ�, 32 bit ���� 1���� ��ȯ
//! [�Է°�] iInput1_Low_16bit, iInput2_High_16bit : 16 bit ���� 2��
//!			 iInput1_Low_16bit : ���� �� bit, iInput2_High_16bit : ���� �� bit
//! [��ȯ��] 32 bit ������
int Convert_Integer_From_16bit_To_32bit(int iInput1_Low_16bit, int iInput2_High_16bit)
{
	int iRet = 0;

	iRet = iInput1_Low_16bit & 0xffff;
	iRet += (iInput2_High_16bit & 0xffff) << 16;

	return iRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
int CalcMultiply(double dbInput1, double dbInput2, bool bFlag_Round_Off)
{
	int iRet = 0;
	double dbRet = 0.;
	dbRet = dbInput1 * dbInput2;
	if ( bFlag_Round_Off == true )
	{
		iRet = (int)(dbRet + 0.5);
	}
	else
	{
		iRet = (int)dbRet;
	}
	return iRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
int CalcMultiply(double dbInput1, int iInput2, bool bFlag_Round_Off)
{
	int iRet = 0;
	double dbRet = 0.;
	dbRet = dbInput1 * ((double)(iInput2));
	if ( bFlag_Round_Off == true )
	{
		iRet = (int)(dbRet + 0.5);
	}
	else
	{
		iRet = (int)dbRet;
	}
	return iRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
CRect CalcMultiply(double dbInput, CRect rcInput, bool bFlag_Round_Off)
{
	CRect rcRet;
	rcRet.left = CalcMultiply(dbInput, rcInput.left, bFlag_Round_Off);
	rcRet.top = CalcMultiply(dbInput, rcInput.top, bFlag_Round_Off);
	int iWidth = rcInput.Width();
	int iHeight = rcInput.Height();
	int iNewWidth = CalcMultiply(dbInput, iWidth, bFlag_Round_Off);
	int iNewHeight = CalcMultiply(dbInput, iHeight, bFlag_Round_Off);
	rcRet.right = rcRet.left + iNewWidth;
	rcRet.bottom = rcRet.top + iNewHeight;
	return rcRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
CRect CalcMultiply(double dbInputX, double dbInputY, CRect rcInput, bool bFlag_Round_Off)
{
	CRect rcRet;
	rcRet.left = CalcMultiply(dbInputX, rcInput.left, bFlag_Round_Off);
	rcRet.top = CalcMultiply(dbInputY, rcInput.top, bFlag_Round_Off);
	int iWidth = rcInput.Width();
	int iHeight = rcInput.Height();
	int iNewWidth = CalcMultiply(dbInputX, iWidth, bFlag_Round_Off);
	int iNewHeight = CalcMultiply(dbInputY, iHeight, bFlag_Round_Off);
	rcRet.right = rcRet.left + iNewWidth;
	rcRet.bottom = rcRet.top + iNewHeight;
	return rcRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
int CalcDivide(double dbInput1, double dbInput2, bool bFlag_Round_Off)
{
	if ( fabs(dbInput2) < 0.00001 )
	{
		return 0;
	}

	int iRet = 0;
	double dbRet = 0.;
	dbRet = dbInput1 / dbInput2;
	if ( bFlag_Round_Off == true )
	{
		iRet = (int)(dbRet + 0.5);
	}
	else
	{
		iRet = (int)dbRet;
	}
	return iRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
int CalcDivide(double dbInput1, int iInput2, bool bFlag_Round_Off)
{
	if ( iInput2 == 0 )
	{
		return 0;
	}

	int iRet = 0;
	double dbRet = 0.;
	dbRet = dbInput1 / ((double)(iInput2));
	if ( bFlag_Round_Off == true )
	{
		iRet = (int)(dbRet + 0.5);
	}
	else
	{
		iRet = (int)dbRet;
	}
	return iRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
int CalcDivide(int iInput1, double dbInput2, bool bFlag_Round_Off)
{
	if ( fabs(dbInput2) < 0.00001 )
	{
		return 0;
	}

	int iRet = 0;
	double dbRet = 0.;
	dbRet = ((double)(iInput1)) / dbInput2;
	if ( bFlag_Round_Off == true )
	{
		iRet = (int)(dbRet + 0.5);
	}
	else
	{
		iRet = (int)dbRet;
	}
	return iRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
CRect CalcDivide(double dbInput, CRect rcInput, bool bFlag_Round_Off)
{
	CRect rcRet;
	rcRet.left = CalcDivide(rcInput.left, dbInput, bFlag_Round_Off);
	rcRet.top = CalcDivide(rcInput.top, dbInput, bFlag_Round_Off);
	int iWidth = rcInput.Width();
	int iHeight = rcInput.Height();
	int iNewWidth = CalcDivide(iWidth, dbInput, bFlag_Round_Off);
	int iNewHeight = CalcDivide(iHeight, dbInput, bFlag_Round_Off);
	rcRet.right = rcRet.left + iNewWidth;
	rcRet.bottom = rcRet.top + iNewHeight;
	return rcRet;
}

//! [�Է°�] bFlag_Round_Off : true�̸� �ݿø��� �Ѵ�.
CRect CalcDivide(double dbInputX, double dbInputY, CRect rcInput, bool bFlag_Round_Off)
{
	CRect rcRet;
	rcRet.left = CalcDivide(rcInput.left, dbInputX, bFlag_Round_Off);
	rcRet.top = CalcDivide(rcInput.top, dbInputY, bFlag_Round_Off);
	int iWidth = rcInput.Width();
	int iHeight = rcInput.Height();
	int iNewWidth = CalcDivide(iWidth, dbInputX, bFlag_Round_Off);
	int iNewHeight = CalcDivide(iHeight, dbInputY, bFlag_Round_Off);
	rcRet.right = rcRet.left + iNewWidth;
	rcRet.bottom = rcRet.top + iNewHeight;
	return rcRet;
}

void Get_Valid_Point(CRect rcEntire, CPoint &ptInput)
{
	if ( ptInput.x < rcEntire.left )
	{
		ptInput.x = rcEntire.left;
	}
	if ( ptInput.x > rcEntire.right )
	{
		ptInput.x = rcEntire.right;
	}
	if ( ptInput.y < rcEntire.top )
	{
		ptInput.y = rcEntire.top;
	}
	if ( ptInput.y > rcEntire.bottom )
	{
		ptInput.y = rcEntire.bottom;
	}
}

void Get_Valid_Line(CRect rcEntire, CPoint &ptStart, CPoint &ptEnd)
{
	if ( ptStart.x < rcEntire.left )
	{
		ptStart.x = rcEntire.left;
	}
	if ( ptStart.x > rcEntire.right )
	{
		ptStart.x = rcEntire.right;
	}
	if ( ptStart.y < rcEntire.top )
	{
		ptStart.y = rcEntire.top;
	}
	if ( ptStart.y > rcEntire.bottom )
	{
		ptStart.y = rcEntire.bottom;
	}

	if ( ptEnd.x < rcEntire.left )
	{
		ptEnd.x = rcEntire.left;
	}
	if ( ptEnd.x > rcEntire.right )
	{
		ptEnd.x = rcEntire.right;
	}
	if ( ptEnd.y < rcEntire.top )
	{
		ptEnd.y = rcEntire.top;
	}
	if ( ptEnd.y > rcEntire.bottom )
	{
		ptEnd.y = rcEntire.bottom;
	}
}

//! �Էµ� �簢 ������ ��ü �簢 ������ ���� �ʵ��� �����Ͽ� ��ȯ 
//! [�Է°�]
//!		rcEntire : ȭ���� ��ü �簢 ����
//!		rcInputChk : ������ �簢 ����
//!		bFlag_Base_Left : ������ �簢 �������� ���� ���� �����̸� true, ������ ���� �����̸� false
//!		bFlag_Base_Top : ������ �簢 �������� ���� ���� �����̸� true, �Ʒ��� ���� �����̸� false
CRect Get_Valid_Rect(CRect rcEntire, CRect rcInputChk, bool bFlag_Base_Left, bool bFlag_Base_Top)
{
	//CRect rcClient;
	//GetClientRect(rcClient);

	int iTempWidth = rcInputChk.Width();
	int iTempHeight = rcInputChk.Height();

	//! �簢 ������ ũ�� ����
	if ( iTempWidth > rcEntire.Width() )
	{
		iTempWidth = rcEntire.Width();
		rcInputChk.right = rcInputChk.left + iTempWidth;
	}
	if ( iTempHeight > rcEntire.Height() )
	{
		iTempHeight = rcEntire.Height();
		rcInputChk.bottom = rcInputChk.top + iTempHeight;
	}
	if ( iTempWidth < 0 )
	{
		if ( bFlag_Base_Left == true )
		{
			rcInputChk.right = rcInputChk.left;
		}
		else
		{
			rcInputChk.left = rcInputChk.right;
		}
		
		iTempWidth = 0;
	}
	if ( iTempHeight < 0 )
	{
		if ( bFlag_Base_Top == true )
		{
			rcInputChk.bottom = rcInputChk.top;
		}
		else
		{
			rcInputChk.top = rcInputChk.bottom;
		}
		
		iTempHeight = 0;
	}

	//! �簢 ������ ��ġ ����
	if ( rcInputChk.left < rcEntire.left )
	{
		rcInputChk.left = rcEntire.left;
		rcInputChk.right = rcInputChk.left + iTempWidth;
	}
	if ( rcInputChk.right > rcEntire.right )
	{
		rcInputChk.right = rcEntire.right;
		rcInputChk.left = rcInputChk.right - iTempWidth;
	}
	if ( rcInputChk.top < rcEntire.top )
	{
		rcInputChk.top = rcEntire.top;
		rcInputChk.bottom = rcInputChk.top + iTempHeight;
	}
	if ( rcInputChk.bottom > rcEntire.bottom )
	{
		rcInputChk.bottom = rcEntire.bottom;
		rcInputChk.top = rcInputChk.bottom - iTempHeight;
	}

	//! �簢 ������ ��, ���̰� (-)���� ���� �ʵ��� �Ѵ�.

	return rcInputChk;
}

//! �Էµ� �ð��� �����Ѵ�. (strTime_Dest = strTime_Source), 'strTime_Source'�� �ð��� 'strTime_Dest'�� �Է��Ѵ�. 
void CopySystemTime(SYSTEMTIME strTime_Source, SYSTEMTIME &strTime_Dest)
{
	strTime_Dest.wYear         = strTime_Source.wYear;
	strTime_Dest.wMonth        = strTime_Source.wMonth;
	strTime_Dest.wDay          = strTime_Source.wDay;
	strTime_Dest.wDayOfWeek    = strTime_Source.wDayOfWeek;
	strTime_Dest.wHour         = strTime_Source.wHour;
	strTime_Dest.wMinute       = strTime_Source.wMinute;
	strTime_Dest.wSecond       = strTime_Source.wSecond;
	strTime_Dest.wMilliseconds = strTime_Source.wMilliseconds;
}

//! [���] �Էµ� �ð��� ���Ͽ�, �ð����� ��ȯ�Ѵ�.
//! [��ȯ��] 
//!		1) strTime1�� strTime2���� ������, �ð����� ������ ��ȯ�Ѵ�. 
//!		2) strTime2�� strTime1���� ������, �ð����� ����� ��ȯ�Ѵ�. 
//!		3) ��ȯ���� ������ msec�̴�.
//!		4) ��ȯ���� ���밪�� '100000' �̸�, ��� ���� �������� �Ǻ�������, �ð����� ��Ȯ���� ���� ����̴�.
//!		5) �ð��� �����ϸ�, '0'�� ��ȯ�Ѵ�. 
int CompareSystemTime(SYSTEMTIME strTime1, SYSTEMTIME strTime2)
{
	int iRet = 0;

	FILETIME astrTempTime[2];
	LARGE_INTEGER astrTempInteger[2];

	_int64 iResult64 = 0;
	int iTimeDiff = 0;
	long lRet = 0;
	int iChk = 0;

	SystemTimeToFileTime( &strTime1, &(astrTempTime[0]) );
	SystemTimeToFileTime( &strTime2, &(astrTempTime[1]) );	

	lRet = CompareFileTime( &(astrTempTime[0]), &(astrTempTime[1]) );

	if ( lRet == 0 )
	{
		//! 2���� �ð��� �����ϴ�. 
		iRet = 0;
	}
	else
	{
		memcpy( &(astrTempInteger[0]), &(astrTempTime[0]), sizeof(FILETIME) );
		memcpy( &(astrTempInteger[1]), &(astrTempTime[1]), sizeof(FILETIME) );

		_int64 iResult64 = (int)(astrTempInteger[0].QuadPart - astrTempInteger[1].QuadPart);

		iTimeDiff = (int)(iResult64 / 10000);

		iChk = iTimeDiff * lRet;

		if ( (iTimeDiff == 0) || (iChk < 0) )
		{
			//! 1) �ð����� '0'�� ���� �� �����Ƿ�, ������ �߻��� ������
			//! 2) �ð����� ��ȣ�� �ݴ밡 �� �� �����Ƿ�, ������ �߻��� ������

			if ( lRet < 0 )
			{
				iRet = -100000;
			}
			else
			{
				iRet = 100000;
			}
		}
		else
		{
			iRet = iTimeDiff;
		}
	}

	return iRet;
}

void checkMessage() 
{
	//MSG msg;
	//PeekMessage(&msg,::GetActiveWindow(),NULL,NULL,PM_REMOVE);
	//	//	PreTranslateMessage(&msg);
	//TranslateMessage(&msg);
	//DispatchMessage(&msg);
}

//! Idle Loop Processing (From MSDN 2010)
bool PeekAndPump()
{
	MSG msg;

    while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
	{
          if (!AfxGetApp()->PumpMessage()) 
		  {
               ::PostQuitMessage(0);
               return false;
          }
    }

	// let MFC do its idle processing
    LONG lIdle = 0;
    while (AfxGetApp()->OnIdle(lIdle++));

	// Perform some background processing here // using another call to OnIdle

    return true;
}

