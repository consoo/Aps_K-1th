// EEPROMTest.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EEPROMTest.h"

// CEEPROMTest ��ȭ �����Դϴ�.
#define MAX_LENGTH 1025
IMPLEMENT_DYNAMIC(CEEPROMTest, CDialogEx)

CEEPROMTest::CEEPROMTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_EEPROM, pParent)
	, m_Str_SlaveAddr(_T("0x50"))
	, m_Str_Addr(_T("0x0000"))
	, m_Str_AddrLength(2)
	, m_Str_DataLength(0)
{

}

CEEPROMTest::~CEEPROMTest()
{
}

void CEEPROMTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SLAVE_ADDR, m_Str_SlaveAddr);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_Str_Addr);
	DDX_Text(pDX, IDC_EDIT_ADDR_LENGTH, m_Str_AddrLength);
	DDX_Text(pDX, IDC_EDIT_DATA_LENGTH, m_Str_DataLength);
	DDX_Text(pDX, IDC_EDIT_DATA, m_Str_Data_Hex);
	DDX_Text(pDX, IDC_EDIT_DATA2, m_Str_Data_Ch);

	DDX_Control(pDX, IDC_BTN_OUTPUT_ASCII, m_Button_Ascii);
	DDX_Control(pDX, IDC_BTN_OUTPUT_FLOAT, m_Button_Float);
	DDX_Control(pDX, IDC_BTN_OUTPUT_DEC, m_Button_Dec);
	DDX_Control(pDX, IDC_BTN_OUTPUT_HEX, m_Button_Hex);
	
}


BEGIN_MESSAGE_MAP(CEEPROMTest, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_READ, &CEEPROMTest::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_MIU_OPEN, &CEEPROMTest::OnBnClickedBtnMiuOpen)
	ON_BN_CLICKED(IDC_BTN_MIU_STOP, &CEEPROMTest::OnBnClickedBtnMiuStop)
	ON_BN_CLICKED(IDC_BTN_MIU_CLOSE, &CEEPROMTest::OnBnClickedBtnMiuClose)
	ON_BN_CLICKED(IDC_BTN_WRITE, &CEEPROMTest::OnBnClickedBtnWrite)
	ON_BN_CLICKED(IDCANCEL, &CEEPROMTest::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_ASCII, &CEEPROMTest::OnBnClickedBtnOutputAscii)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_FLOAT, &CEEPROMTest::OnBnClickedBtnOutputFloat)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_DEC, &CEEPROMTest::OnBnClickedBtnOutputDec)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_HEX, &CEEPROMTest::OnBnClickedBtnOutputHex)
END_MESSAGE_MAP()


// CEEPROMTest �޽��� ó�����Դϴ�.


BOOL CEEPROMTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CEEPROMTest::SetInterface()
{
	FuncModeChange(theApp.MainDlg->output_Mode);
}
///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
//EEPROM READ
//
///-------------------------------------------------------------------------------------
void CEEPROMTest::OnBnClickedBtnRead()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!MIU.m_pBoard->IsConnected())
	{
		theApp.MainDlg->putListLog("[CCD] Grabber Board ���� ����");
		return;
	}

	unsigned int nSlaveAddress;
	unsigned int nAddress;
	unsigned char pData[MAX_LENGTH];
	int errorCode = 0;

	UpdateData(TRUE);
	sscanf_s(m_Str_SlaveAddr.GetBuffer(0), "%x", &nSlaveAddress);
	sscanf_s(m_Str_Addr.GetBuffer(0), "%x", &nAddress);

	m_Str_Data_Hex.Empty();
	m_Str_Data_Ch.Empty();

	m_Str_Data_Ch = eepromData.EEPROM_Read(m_Str_Addr.GetBuffer(0), m_Str_DataLength, theApp.MainDlg->output_Mode, false);
	m_Str_Data_Hex = eepromData.CovertToHex(m_Str_Data_Ch, theApp.MainDlg->output_Mode);
	UpdateData(FALSE);
	return;




	UpdateData(TRUE);

	sscanf_s(m_Str_SlaveAddr.GetBuffer(0), "%x", &nSlaveAddress);
	sscanf_s(m_Str_Addr.GetBuffer(0), "%x", &nAddress);

	errorCode = MIU.m_pBoard->ReadI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, m_Str_AddrLength, pData, (unsigned short)m_Str_DataLength);
	CString tempData;
	CString tenData;
	int ten;
	char szHex[16];
	if (errorCode)
	{
		m_Str_Data_Hex.Format("Error : %d", errorCode);
		m_Str_Data_Hex.AppendFormat(" %s", GetErrorStr(errorCode));
	}
	else
	{
		m_Str_Data_Hex = "";
		m_Str_Data_Ch = "";
		for (int i = 0; i < (int)m_Str_DataLength; i++)
		{
			//m_Str_Data_Hex.AppendFormat("%02X ", (char*)pData[i]);
			if (theApp.MainDlg->output_Mode == ASCII_MODE)
			{
				m_Str_Data_Hex.AppendFormat("%02x", (char*)pData[i]);
			}
			else if (theApp.MainDlg->output_Mode == FLOAT_MODE)
			{
				m_Str_Data_Hex.AppendFormat("%x", (char*)pData[i]);
			}
			else if (theApp.MainDlg->output_Mode == DEC_MODE)
			{
				m_Str_Data_Hex.AppendFormat("%x", (char*)pData[i]);
			}

		}
		m_Str_Data_Ch = CovertToChar(m_Str_Data_Hex, theApp.MainDlg->output_Mode);
		//m_Str_Data_Ch.Format("%s", tenData);
	}
	theApp.MainDlg->putListLog(m_Str_Data_Ch);
	UpdateData(FALSE);
}

///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
//EEPROM WRITE
//
///-------------------------------------------------------------------------------------
void CEEPROMTest::OnBnClickedBtnWrite()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	unsigned int nSlaveAddress;
	unsigned int nAddress;
	unsigned char pData[MAX_LENGTH];
	unsigned char charStr[MAX_PATH];
	byte rtnData[MAX_PATH];
	int errorCode = 0;
	UpdateData(TRUE);

	sscanf_s(m_Str_SlaveAddr.GetBuffer(0), "%x", &nSlaveAddress);
	sscanf_s(m_Str_Addr.GetBuffer(0), "%x", &nAddress);

	if (!MIU.m_pBoard->SetWPDisable()) {
		theApp.MainDlg->putListLog("Write Disable fail!!!!!");
	}
	else {
		theApp.MainDlg->putListLog("Write Disable Success");
	}

	eepromData.eeprom_Data_Sum(m_Str_Data_Hex.GetBuffer(0), m_Str_DataLength, theApp.MainDlg->output_Mode, rtnData, true);
	errorCode = MIU.m_pBoard->WriteI2CBurst((unsigned short)0x50, (unsigned short)nAddress, 2, rtnData, (unsigned short)m_Str_DataLength);
	if (errorCode)
	{
		m_Str_Data_Hex.Format("Error : %d", errorCode);
		m_Str_Data_Hex.AppendFormat(" %s", eepromData.GetErrorStr(errorCode));
	}
	else
	{
		theApp.MainDlg->putListLog("wright ok!");
	}
	if (!MIU.m_pBoard->SetWPEnable()) {
		theApp.MainDlg->putListLog("Write Enable fail!!!!!");
	}
	else {
		theApp.MainDlg->putListLog("Write Enable Success");
	}





	return;

	//if (!MIU.m_pBoard->SetWPDisable())
	//	theApp.MainDlg->putListLog("Write Disable fail!!!!!");
	//else
	//	theApp.MainDlg->putListLog("Write Disable Success");
	//UpdateData(TRUE);

	//sscanf_s(m_Str_SlaveAddr.GetBuffer(0), "%x", &nSlaveAddress);
	//sscanf_s(m_Str_Addr.GetBuffer(0), "%x", &nAddress);

	//
	////charStr[0] = (unsigned char)_tcstol(temp.Mid(0, 2), NULL, 16);
	//
	//if (theApp.MainDlg->output_Mode == DEC_MODE)
	//{
	//	temp = m_Str_Data_Hex;
	//}
	//else
	//{
	//	temp = CovertToHex(m_Str_Data_Hex, theApp.MainDlg->output_Mode);
	//}
	//
	//
	//temp.Replace(" ", "");
	//
	//for (int i = 0; i < (int)m_Str_DataLength; i++)
	//{
	//	//pData[i] = (unsigned char)_tcstol(CovertToHex(temp).Mid(i * 2, 2), NULL, 16);//hex�ڵ�� �Է�������
	//	pData[i] = (unsigned char)_tcstol(temp.Mid(i * 2, 2), NULL, 16);//hex�ڵ�� �Է�����������ڵ�

	//}

	//errorCode = MIU.m_pBoard->WriteI2CBurst((unsigned short)nSlaveAddress, (unsigned short)nAddress, m_Str_AddrLength, pData, (unsigned short)m_Str_DataLength);

	//if (errorCode)
	//{
	//	m_Str_Data_Hex.Format("Error : %d", errorCode);
	//	m_Str_Data_Hex.AppendFormat(" %s", GetErrorStr(errorCode));
	//}
	//else
	//{
	//	theApp.MainDlg->putListLog("wright ok!");
	//}
	//if (!MIU.m_pBoard->SetWPEnable())
	//	theApp.MainDlg->putListLog("Write Enable fail!!!!!");
	//else
	//	theApp.MainDlg->putListLog("Write Enable Success");
	//UpdateData(FALSE);
}
///-------------------------------------------------------------------------------------
///-------------------------------------------------------------------------------------
//
//
//
///-------------------------------------------------------------------------------------
//CString CEEPROMTest::CovertToHex(CString data)
//{
//	CString rtnValue;
//	CString temporary;
//	for (int x = 0; x < data.GetLength(); x++)
//	{
//		int value = (int)(data[x]);
//		temporary.Format("%02X", value);
//		rtnValue += temporary;
//	}
//	return rtnValue;
//}
//CString CEEPROMTest::CovertToChar(CString data)
//{
//	CString rtnValue;
//	CString temporary;
//	data.Replace(" ", "");
//	int len = (int)data.GetLength();
//	for (int x = 0; x < len; x+=2)
//	{
//		byte value = (unsigned char)_tcstol(data.Mid(x, 2), NULL, 16); //(int)(data[x]);
//		temporary.Format("%c", value);
//		rtnValue += temporary;
//	}
//	return rtnValue;
//}
/*tempData.Format("%02X ", pData[i]);
ten = (int)strtol((char*)tempData.GetBuffer(0), NULL, 16);
tempData.Format("%c ", ten);
tempData.Trim();

tenData += tempData;*/
CString CEEPROMTest::GetErrorStr(int errorCode)
{
	CString retStr;
	switch (errorCode)
	{
	case ERR_BOARD_GET_LIST:
		retStr = "ERR_BOARD_GET_LIST";
		break;
	case ERR_BOARD_NO_DEVICE:
		retStr = "ERR_BOARD_NO_DEVICE";
		break;
	case ERR_BOARD_OPEN:
		retStr = "ERR_BOARD_OPEN";
		break;
	case ERR_BOARD_INIT:
		retStr = "ERR_BOARD_INIT";
		break;
	case ERR_BOARD_SET_POWER_5_12:
		retStr = "ERR_BOARD_SET_POWER_5_12";
		break;
	case ERR_BOARD_GRAB_START:
		retStr = "ERR_BOARD_GRAB_START";
		break;
	case ERR_BOARD_I2C_WRITE:
		retStr = "ERR_BOARD_I2C_WRITE";
		break;
	case ERR_BOARD_I2C_READ:
		retStr = "ERR_BOARD_I2C_READ";
		break;
	case ERR_BOARD_BOOT_CAMERA:
		retStr = "ERR_BOARD_BOOT_CAMERA";
		break;
	case ERR_BOARD_CLOSE:
		retStr = "ERR_BOARD_CLOSE";
		break;
	case ERR_BOARD_LOAD_MODEL:
		retStr = "ERR_BOARD_LOAD_MODEL";
		break;
	default:
		break;
	}
	return retStr;
}

void CEEPROMTest::OnBnClickedBtnMiuOpen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int errorCode;
	CString strErrCount;
	CString selectedIniFile;

	MIU.m_pBoard->SetINIFile(selectedIniFile);
	errorCode = MIU.m_pBoard->OpenBoard(MIU.m_cBoardIndex);

	if (errorCode)
	{
		strErrCount.Format("OpenBoard Error: %d", errorCode);
		strErrCount.AppendFormat(" %s", GetErrorStr(errorCode));
	}
	else
	{
		strErrCount.Format("OpenBoard Success");
	}
	theApp.MainDlg->putListLog(strErrCount);
}


void CEEPROMTest::OnBnClickedBtnMiuStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.MainDlg->putListLog("		MIU Stop ����.");
	MIU.Stop();
	theApp.MainDlg->putListLog("MIU Stop �Ϸ�.");
}


void CEEPROMTest::OnBnClickedBtnMiuClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.MainDlg->putListLog("MIU Close ����.");
	if (MIU.Close())
	{
		theApp.MainDlg->putListLog("MIU Close �Ϸ�.");
	}
	else
	{
		theApp.MainDlg->putListLog("MIU Close ����.");
	}

}




void CEEPROMTest::OnBnClickedCancel()
{
	/*if (!MIU.m_pBoard->SetWPDisable())
		AfxMessageBox("Write fail!!!!!");
	else
		AfxMessageBox("Write Success");*/

	//return;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}


void CEEPROMTest::OnBnClickedBtnOutputAscii()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.MainDlg->output_Mode = ASCII_MODE;
	theApp.MainDlg->putListLog("ascii Output");
	FuncModeChange(theApp.MainDlg->output_Mode);
}


void CEEPROMTest::OnBnClickedBtnOutputFloat()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.MainDlg->output_Mode = FLOAT_MODE;//hex
	theApp.MainDlg->putListLog("float Output");
	FuncModeChange(theApp.MainDlg->output_Mode);
}


void CEEPROMTest::OnBnClickedBtnOutputDec()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.MainDlg->output_Mode = DEC_MODE;
	theApp.MainDlg->putListLog("dec Output");
	FuncModeChange(theApp.MainDlg->output_Mode);
}
void CEEPROMTest::FuncModeChange(int type)
{
	m_Button_Ascii.m_iStateBtn = 0;
	m_Button_Float.m_iStateBtn = 0;
	m_Button_Dec.m_iStateBtn = 0;
	m_Button_Hex.m_iStateBtn = 0;
	switch (type)
	{
	case ASCII_MODE:
		m_Button_Ascii.m_iStateBtn = 1;
		break;
	case FLOAT_MODE:
		m_Button_Float.m_iStateBtn = 1;
		break;
	case DEC_MODE:
		m_Button_Dec.m_iStateBtn = 1;
		break;
	case HEX_MODE:
		m_Button_Hex.m_iStateBtn = 1;
		break;
	}
	m_Button_Ascii.Invalidate();
	m_Button_Float.Invalidate();
	m_Button_Dec.Invalidate();
	m_Button_Hex.Invalidate();

}

void CEEPROMTest::OnBnClickedBtnOutputHex()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.MainDlg->output_Mode = HEX_MODE;
	theApp.MainDlg->putListLog("hex Output");
	FuncModeChange(theApp.MainDlg->output_Mode);
}
