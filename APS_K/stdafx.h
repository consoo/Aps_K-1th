// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#pragma warning(disable: 4819)	//! MS���� ����, Ư�� ��� (warning C4819) ǥ�� ����
#pragma warning(disable: 4996)	//Ư�� ��� (warning C4996) ǥ�� ����
//

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����
#include <afxsock.h>            // MFC ���� Ȯ��
#include "resource.h"
////////////////////////////////////////////////////////////////////////////////////////
// opencv 2.4.3 include
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>



#pragma comment (lib, "mil.lib")
#pragma comment (lib, "milpat.lib")
#pragma comment (lib, "milmod.lib")
#pragma comment (lib, "milim.lib")
#pragma comment (lib, "milblob.lib")
#pragma comment(lib, "uxtheme.lib")








//////////////////////////////////////////////////////////////////////////////////////

#ifndef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#pragma warning(push)
#pragma warning(disable:4146)
//msado60.tlb
#import "C:\Program Files\Common files\System\ado\msado15.dll" rename ("EOF","adoEOF") no_namespace
//
//#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")
//#import "C:\\Program Files (x86)\\Common files\\System\\ado\\msado15.dll" rename ("EOF","adoEOF") no_namespace
//#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF")//EndOfFile
#pragma warning(pop) 

#include "GlobalDefine.h"
#include "GlobalDeclare.h"


#include <iostream>

//
//const float imageWidth = 1280;
//const float imageHeight = 1080;
//

