// DisplacementDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DisplacementDlg.h"
// CDisplacementDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDisplacementDlg, CDialogEx)

CDisplacementDlg::CDisplacementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplacementDlg::IDD, pParent)
	, m_Edit_ResultVal(0)
{

}

CDisplacementDlg::~CDisplacementDlg()
{
}

void CDisplacementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_ResultVal);
}


BEGIN_MESSAGE_MAP(CDisplacementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_COMM_OPEN, &CDisplacementDlg::OnBnClickedBtnCommOpen)
	ON_BN_CLICKED(IDC_BTN_COMM_CLOSE, &CDisplacementDlg::OnBnClickedBtnCommClose)
	ON_BN_CLICKED(IDC_BTN_SCAN, &CDisplacementDlg::OnBnClickedBtnScan)
	ON_BN_CLICKED(IDC_BTN_DLG_CLOSE, &CDisplacementDlg::OnBnClickedBtnDlgClose)
	ON_BN_CLICKED(IDC_BTN_ZERO_ON, &CDisplacementDlg::OnBnClickedBtnZeroOn)
	ON_BN_CLICKED(IDC_BTN_ZERO_OFF, &CDisplacementDlg::OnBnClickedBtnZeroOff)
	ON_BN_CLICKED(IDC_BTN_KEY_LOCK, &CDisplacementDlg::OnBnClickedBtnKeyLock)
	ON_BN_CLICKED(IDC_BTN_KEY_UNLOCK, &CDisplacementDlg::OnBnClickedBtnKeyUnlock)
END_MESSAGE_MAP()


// CDisplacementDlg �޽��� ó�����Դϴ�.

BOOL CDisplacementDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	//SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	//SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDisplacementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		//CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.


// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.

void CDisplacementDlg::OnBnClickedBtnCommOpen()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	if( !Keyence.func_RS232_CommOpen(sysData.iCommPort[COMM_LASER], sysData.iBaudRate[COMM_LASER]) )
	{
		sLangChange.LoadStringA(IDS_STRING275);
		sMsg.Format(sLangChange);

		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
		return;

	}
	sLangChange.LoadStringA(IDS_STRING274);
	sMsg.Format(sLangChange);
	pFrame->putListLog(sMsg);

}


void CDisplacementDlg::OnBnClickedBtnCommClose()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	Keyence.func_RS232_CommClose();
	sLangChange.LoadStringA(IDS_STRING277);
	sMsg.Format(sLangChange);
	delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
	pFrame->putListLog(sMsg);
}


void CDisplacementDlg::OnBnClickedBtnScan()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg, sRecv;
	CString sMsgBuff = "";
	CString sResult = "";
	CString Err = "";
	double	dLaserVal;

	if( !Keyence.func_LT9030_Scan(dLaserVal) )
	{
		sLangChange.LoadStringA(IDS_STRING273);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
		return ;
	}

	m_Edit_ResultVal = dLaserVal;
	UpdateData(FALSE);
}


void CDisplacementDlg::OnBnClickedBtnDlgClose()
{
	CDialog::OnOK();
}


void CDisplacementDlg::OnBnClickedBtnZeroOn()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	if( !Keyence.func_LT9030_AutoZeroSet(0, true) )
	{
		sLangChange.LoadStringA(IDS_STRING272);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
	}

		sLangChange.LoadStringA(IDS_STRING270);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
}


void CDisplacementDlg::OnBnClickedBtnZeroOff()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	if( !Keyence.func_LT9030_AutoZeroSet(0, false) )
	{
		sLangChange.LoadStringA(IDS_STRING271);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
	}
		sLangChange.LoadStringA(IDS_STRING269);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
}


void CDisplacementDlg::OnBnClickedBtnKeyLock()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	if( !Keyence.func_LT9030_KeyLock(true) )
	{
		sLangChange.LoadStringA(IDS_STRING268);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);

		return;
	}
		sLangChange.LoadStringA(IDS_STRING266);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
}


void CDisplacementDlg::OnBnClickedBtnKeyUnlock()
{
	CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
	CString sMsg;
	if( !Keyence.func_LT9030_KeyLock(false) )
	{
		sLangChange.LoadStringA(IDS_STRING267);
		sMsg.Format(sLangChange);

		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
	}

		sLangChange.LoadStringA(IDS_STRING265);
		sMsg.Format(sLangChange);
		delayMsg(sMsg.GetBuffer(99), 50000, M_COLOR_RED);
		pFrame->putListLog(sMsg);
}

