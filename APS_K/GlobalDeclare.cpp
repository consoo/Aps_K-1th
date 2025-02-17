#include "stdafx.h"
#include "GlobalDeclare.h"

CDioControl		Dio;
CModelData		model;
CAxlMotorSet	motor;
CPasswordData	password;
CTask	Task;

CSystemData	sysData;
CVision vision;
CModelList		modelList;
CMIU       MIU;
CBarcodeRead		barcode;
CMandoSfrSpec		MandoSfrSpec;
CADOClass		g_ADOData;
CList_RS232C		SerialPortList;
CLightControl LightControl;
CLightControl LightControlSecond;
CLightControl LightControlthird;
CIrvLedControl		IrvAlignLed;
CUVCommand UVCommand;
CUVCommand UVCommand2;
CKeyenceSensor		Keyence;
MIU_DEVICE gMIUDevice;
char gDeviceCount;
char gDeviceIndex;
int	g_iCCDCamView;	//! Added by LHW [2013/3/27)
bool g_bMovingflag = false;
CDelayMsgDlg*	delayDlg;
CWork			work;

CAlarmDialog*	alarmDlg = NULL;

bool		g_AlarmFlag;
int			g_AlarmCnt;
CeepromData	eepromData;
CMandoInspLog		MandoInspLog;
CMESCommunication	MESCommunication;
double	dSFR_N_4_PositionX;
double	dSFR_N_8_PositionX;
CPRIFunc_Insp g_clPriInsp;
bool	g_bReady_UV;
//! DIO ���� ���� �ӽ� ���� ����
int curInDio[5] = { 0,1 };
int curOutDioByte[5][4];
double	g_dFindCenterX[3];
double	g_dFindCenterY[3];
double	g_dFindAngle[3];
bool JogPlusflag;
bool JogMinusflag;
int iLaser_Pos = 0;
CDPoint		LaserPos[4];
double		LaserValue[4];
CPoint cpFiducialPos[4];
CRect crFiducialRoi[4];
LARGE_INTEGER	m_liStartCounter;
LARGE_INTEGER	m_liFreq;
//////////////////////////////////////////////////////////////////////////


double myTimer(bool flag)
{
	int elapsedTime = 0;
	LARGE_INTEGER	m_aliCounter;

	if (!flag)
	{
		QueryPerformanceFrequency(&m_liFreq);
		QueryPerformanceCounter(&m_liStartCounter);
	}
	else
	{
		double dRet = -1.;


		if (QueryPerformanceCounter(&(m_aliCounter)) == TRUE)
		{
			dRet = ((double)(m_aliCounter.QuadPart - m_liStartCounter.QuadPart)) / ((double)m_liFreq.QuadPart);
			dRet *= 1000.;
		}

		return dRet;
	}

	return 0;
}
void Jpg_ImageSave(IplImage* jpgImg, int index)
{
	SYSTEMTIME time;
	::GetLocalTime(&time);

	CFileFind finder;
	BOOL IsFind;
	CString FolderName;
	CString jpgName;
	FolderName.Format("%s\\%s\\%04d%02d\\Image", BASE_LOG_DIR,model.mCurModelName, time.wYear, time.wMonth);

	IsFind = finder.FindFile(FolderName);
	if (!IsFind)
	{
		FolderName.Format("%s", BASE_DIR);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}
		FolderName.Format("%s", BASE_LOG_DIR);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}

		FolderName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}

		FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}

		FolderName.Format("%s\\%s\\%04d%02d\\Image", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
		CreateDirectory(FolderName, NULL);
	}
	//
	CString imgName;
	if (index == BLACKSPOT_JPG)
	{
		imgName.Format("BlackSpot");
	}
	else if (index == LCBJPG)
	{
		imgName.Format("Lcb");
	}
	else if (index == YMEAN_JPG)
	{
		imgName.Format("Ymean");
	}
	else if (index == DEFECT_MID_JPG)
	{
		imgName.Format("DefectMid");
	}
	else if (index == DEFECT_LOW_JPG)
	{
		imgName.Format("DefectLow");
	}
	else if (index == MTF_JPG)
	{
		imgName.Format("Mtf");
	}

	

	SYSTEMTIME SysTime;
	::GetLocalTime(&SysTime);
	finder.Close();

	CString		fileName = _T("");
	jpgName.Format("%s\\%s_%s_%04d%02d%02d%02d%02d%02d.jpg", FolderName, Task.ChipID, imgName, SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);

	cvSaveImage(jpgName, jpgImg);
}

bool g_FinalInspLog()
{
    
    CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
    SYSTEMTIME stSysTime;
    ::GetLocalTime(&stSysTime);

    FILE *out = NULL;

    CFileFind finder;
    BOOL IsFind = FALSE;
    CString FolderName = "";
    int i = 0;
    int j = 0;
    int indexX = 0;
    int indexY = 0;

    CString sFileMode = _T("");
    int lotLength = strlen(Task.LotNo);
    int IDLength = strlen(Task.ChipID);

    if (lotLength<1)
        sprintf_s(Task.LotNo, "(NULL)");

    if (IDLength<1)
        sprintf_s(Task.ChipID, "(NULL)");



    FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

        FolderName.Format("%s", BASE_LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

		FolderName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}


		FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}
    }
    int iStepNo = Task.m_iCnt_Step_AA_Total;

    char fileName[5000];

    CString sName = "";
    CString sTemp = ""; 
    sTemp.Format("%s", Task.ChipID);

	wsprintf(fileName, "%s\\%s_FinalLog_%04d%02d%02d.csv", FolderName, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay);

    CFile file;

    if (finder.FindFile(fileName) == TRUE)
    {
        BOOL bFlag_File_Open = file.Open(fileName, CFile::modeRead);
        if (bFlag_File_Open == FALSE)
        {
            sLangChange.LoadStringA(IDS_STRING1437);	//"������ ��� �� �Դϴ�."
            errMsg2(Task.AutoFlag, sLangChange);
            finder.Close();

            return false;
        }
		else 
		{
			file.Close();
		}
    }

    finder.Close();

    if (fopen_s(&out, fileName, "r") != NULL)
    {
        if (fopen_s(&out, fileName, "w") == NULL)
        { 
            _ftprintf_s(out, _T("Time,"));
			_ftprintf_s(out, _T("Model,				SW Version,"));
			_ftprintf_s(out, _T("Lot ID,"));

			for (i = 0; i < model.mGlobalSmallChartCount; i++)//for (i = 0; i < MTF_INSP_CNT; i++)
			{
				_ftprintf_s(out, _T("[SFR] Roi-%d,"), i);
			}
            _ftprintf_s(out, _T("[OC] OC_X ,	[OC] OC_Y,"));
            _ftprintf_s(out, _T("[OC] Delta_X ,[OC] Delat_Y,"));
			_ftprintf_s(out, _T("Rotation,Current,"));
			_ftprintf_s(out, _T("[NG LIST],[JUDGE],"));

            _ftprintf_s(out, _T("\n"));
            fclose(out);
        }
        if (out)		fclose(out);
    }
    else
    {
        if (out)		fclose(out);
    }


#ifdef _UNICODE
    sFileMode = _T("a,ccs=UTF-8");
#else
    sFileMode = _T("a");
#endif

    if (!fopen_s(&out, fileName, "a"))
    {
        _ftprintf_s(out, _T("'%04d-%02d-%02d %02d:%02d:%02d,"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
		_ftprintf_s(out, _T("%s,%s,"), model.mCurModelName, VER_STR);
		_ftprintf_s(out, _T("%s,"),Task.ChipID); 
		for (i = 0; i < model.mGlobalSmallChartCount; i++)//for (i = 0; i < MTF_INSP_CNT; i++)
		{
			_ftprintf_s(out, _T("%lf,"), model.m_LogSfr[i]);
		}
		
        _ftprintf_s(out, _T("%0.3lf, %0.3lf,%.03lf,%.03lf,"), model.m_LogOC_X, model.m_LogOC_Y, model.m_LogOC_DelatX, model.m_LogOC_DelatY);
		_ftprintf_s(out, _T("%.3lf,%.3lf,"), Task.dLensRotation, model.dGen2Current);
		if (MandoInspLog.bInspRes == true)		fprintf_s(out, "%s,OK", (LPTSTR)(LPCTSTR)MandoInspLog.sNGList);
		else									fprintf_s(out, "%s,NG", (LPTSTR)(LPCTSTR)MandoInspLog.sNGList);
		//
        _ftprintf_s(out, _T("\n"));

        fclose(out);
    }

    return true;
}


bool saveSfrLog(int iMode)					//AA1 �� step�� �α� Lot������: �α״��������(4/5)
{
    //����ǰ�϶��� ���ϸ� ���̵�� ����
    CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;
    SYSTEMTIME time;
    ::GetLocalTime(&time);
	int i = 0;
    FILE *out = NULL;

    CFileFind finder;
    BOOL IsFind = FALSE;
    CString FolderName = "";


    int lotLength = (int)strlen(Task.LotNo);
    int IDLength = (int)strlen(Task.ChipID);

    if (lotLength<1)
        sprintf_s(Task.LotNo, "(NULL)");

    if (IDLength<1)
        sprintf_s(Task.ChipID, "(NULL)");



    FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

        FolderName.Format("%s", BASE_LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

		FolderName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}


        FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
        CreateDirectory(FolderName, NULL);
    }
    int iStepNo = Task.m_iCnt_Step_AA_Total;

    char fileName[5000];

    CString sName = "";
    CString sTemp = "";
    sTemp.Format("%s", Task.ChipID);

    wsprintf(fileName, "%s\\%04d%02d%02d_AAStepLog.csv", FolderName, time.wYear, time.wMonth, time.wDay);

    CFile file;

    if (finder.FindFile(fileName) == TRUE)
    {
        BOOL bFlag_File_Open = file.Open(fileName, CFile::modeRead);
        if (bFlag_File_Open == FALSE)
        {
            sLangChange.LoadStringA(IDS_STRING1437);	//"������ ��� �� �Դϴ�."
            errMsg2(Task.AutoFlag, sLangChange);
            finder.Close();

            return false;
        }
        else
            file.Close();
    }

    finder.Close();

    if (fopen_s(&out, fileName, "r") != NULL)
    {
        if (fopen_s(&out, fileName, "w") == NULL)
        {
            fprintf_s(out, ",,,,,Resolution,,,LensMotor,,,,,PCBMotor,,,,,,,,,Average SFR,,,,,each");
            fprintf_s(out, "\n");
            //
            fprintf_s(out, "Date,Time,");
            fprintf_s(out, "SW Version,Model,");
            fprintf_s(out, "BarcodeID,");
            fprintf_s(out, "Width,Height,ocx, ocy,");
            fprintf_s(out, "Step,");
            fprintf_s(out, "Tx,Ty,X,Y,Z,");//LensMotor
            fprintf_s(out, "Tx,Ty,X,Y,TH,Z,");//PCBMotor

			//
			//==============================================================================step �α�
			fprintf_s(out, ",0.0F, AA_TL, AA_TR, AA_BL, AA_BR");
			for (i = 0; i < model.mGlobalSmallChartCount; i++)//for (i = 0; i < MTF_INSP_CNT; i++)
			{
				_ftprintf_s(out, _T(",[%d] ROI"), i);
			}												
            fprintf_s(out, "\n");
        }

        if (out)		fclose(out);
    }
    else
    {
        if (out)		fclose(out);
    }

    if (!fopen_s(&out, fileName, "a"))
    {
        if (iMode == SFR_OFFSET_DATA)	// UV ��ȭ �� PCB X, Y, Tx, Ty, Th Offset �� ����.
        {
            Task.m_iCnt_Best_Step = iStepNo - 1;
            fprintf_s(out, ", , , , , , , , , , , , , , ,UV Offset,X,Y,Z,Tx,Ty,Theta\n");
            fprintf_s(out, ", , , , , , , , , ,  ,, , , , ,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf\n"
                , sysData.m_dOffset_Prev_UV_X, sysData.m_dOffset_Prev_UV_Y, sysData.m_dOffset_Prev_UV_Z, sysData.m_dOffset_Prev_UV_Tx, sysData.m_dOffset_Prev_UV_Ty, sysData.m_dOffset_Prev_UV_Th);//Task.dUvOffset_Aver);
        }
        else
        {
            fprintf_s(out, "%04d-%02d-%02d,", time.wYear, time.wMonth, time.wDay);
            fprintf_s(out, "%02d:%02d:%02d,", time.wHour, time.wMinute, time.wSecond);
            fprintf_s(out, "VER.%s,%s,", VER_STR, model.mCurModelName);
            fprintf_s(out, "%s,", Task.ChipID);

            double dTemp = 0.0;
            dTemp = sysData.m_dOffset_Prev_UV_X + sysData.m_dOffset_Prev_UV_Y + sysData.m_dOffset_Prev_UV_Z + sysData.m_dOffset_Prev_UV_Tx + sysData.m_dOffset_Prev_UV_Ty + sysData.m_dOffset_Prev_UV_Th;
            if (iMode == 1)
            {
                fprintf_s(out, ",,,,,,,Tilt After,,TiltX,%lf,TiltY,%lf\n", Task.SFR.dTilt_X, Task.SFR.dTilt_Y);
            }
            else if (iMode == OC_INSP)
            {
                fprintf_s(out, ", , , ,,,%0.3lf,%0.3lf,%d,%d,%d,%d,%d\n", (gMIUDevice.nWidth / 2) - Task.dOpticalPosX, (gMIUDevice.nHeight / 2) - Task.dOpticalPosY, Task.iOpticalThVal, Task.rOpticalROI.left, Task.rOpticalROI.right, Task.rOpticalROI.top, Task.rOpticalROI.bottom);
            }
            else
            {
				fprintf_s(out, "%d,%d,", gMIUDevice.nWidth, gMIUDevice.nHeight);
				fprintf_s(out, "%.2lf,%.2lf,", MandoInspLog.dOCResult[0], MandoInspLog.dOCResult[1]);
                if (iMode == SFR_FIRST || iMode == SFR_SECOND || iMode == SFR_FINAL || iMode == SFR_MANUAL)
                    fprintf_s(out, "%d,", iStepNo);
                else if (iMode == SFR_BEFORE_UV)
                    fprintf_s(out, "Before UV_SFR,");
                else if (iMode == SFR_AFTER_UV)
                    fprintf_s(out, "After UV_SFR,");
                else if (iMode == SFR_AFTER_OFFSET_MOVE)
                    fprintf_s(out, "After Offset, ");
                fprintf_s(out, "%f,%f,%f,%f,%f,"
                    , motor.GetCommandPos(Motor_Lens_Xt), motor.GetCommandPos(Motor_Lens_Yt), motor.GetEncoderPos(Motor_Lens_X)
                    , motor.GetEncoderPos(Motor_Lens_Y), motor.GetCommandPos(Motor_Lens_Z));
                fprintf_s(out, "%f,%f,%f,%f,%f,%f,,"
                    , motor.GetCommandPos(Motor_PCB_Xt), motor.GetCommandPos(Motor_PCB_Yt), motor.GetEncoderPos(Motor_PCB_X)
                    , motor.GetCommandPos(Motor_PCB_Y), motor.GetCommandPos(Motor_PCB_TH), motor.GetCommandPos(Motor_PCB_Z));

                fprintf_s(out, "%f,%f,%f,%f,%f,"
					, Task.SFR.fSfrN4[iStepNo][0], Task.SFR.fSfrN4[iStepNo][1], Task.SFR.fSfrN4[iStepNo][2], Task.SFR.fSfrN4[iStepNo][3], Task.SFR.fSfrN4[iStepNo][4]);

				for (int i = 0; i<model.mGlobalSmallChartCount; i++)//for (int i = 0; i<MTF_INSP_CNT; i++)
                {
					fprintf_s(out, "%f,", Task.SFR.dMaxSfr_StepLog[i]);	//saveSfrLog

                }
                fprintf_s(out, "\n");
                if (iMode == SFR_AFTER_OFFSET_MOVE)
                    fprintf_s(out, "\n"); 
                if (iMode == SFR_AFTER_UV)
                {
                    fprintf_s(out, "\n\n\n\n\n");

                }

            }
        }
        fclose(out);
    }
    return true;
}

//-----------------------------------------------------------------------------
//
//	SFR �α� ����
//
//-----------------------------------------------------------------------------
bool g_SaveLGITLog(int nUnit, CString name, CString title, CString data)
{
	TCHAR szPath[SIZE_OF_1K];
	TCHAR szFilePath[SIZE_OF_1K];
	SYSTEMTIME stSysTime;
	CFileFind clFinder;
	CString sFileMode = _T("");
	CFile clFile;
	FILE* fp;
	//int nStepNo;

	::GetLocalTime(&stSysTime);

	if (_tcslen(Task.LotNo) <= 0)
		_tcscpy_s(Task.LotNo, SIZE_OF_100BYTE, _T("EMPTY"));

	if (_tcslen(Task.ChipID) <= 0)
		_tcscpy_s(Task.ChipID, SIZE_OF_100BYTE, _T("EMPTY"));

	if (clFinder.FindFile(BASE_LOG_DIR) == FALSE)
		CreateDirectory(BASE_LOG_DIR, NULL);
	//
	_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s\\%04d%02d\\ACMIS"), BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
	if (clFinder.FindFile(szPath) == FALSE)
	{
		_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s"), BASE_LOG_DIR, model.mCurModelName);
		if (clFinder.FindFile(szPath) == FALSE)
			CreateDirectory(szPath, NULL);

		_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s\\%04d%02d"), BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
		if (clFinder.FindFile(szPath) == FALSE)
			CreateDirectory(szPath, NULL);

		_stprintf_s(szPath, SIZE_OF_1K, _T("%s\\%s\\%04d%02d\\ACMIS"), BASE_LOG_DIR, model.mCurModelName, stSysTime.wYear, stSysTime.wMonth);
		if (clFinder.FindFile(szPath) == FALSE)
			CreateDirectory(szPath, NULL);
	}




	

	//nStepNo = g_clTaskWork[nUnit].m_nCountOfTotalStepAA;

	_stprintf_s(szFilePath, SIZE_OF_1K, _T("%s\\%s_AA%d_%s.csv"), szPath, Task.ChipID, nUnit + 1, name);
	if (clFinder.FindFile(szFilePath) == TRUE)
	{
		if (clFile.Open(szFilePath, CFile::modeRead) == FALSE)
		{
			theApp.MainDlg->putListLog(_T("������ ��� �� �Դϴ�."));
			return false;
		}
		else
		{
			clFile.Close();
		}
	}
#ifdef _UNICODE
	sFileMode = _T("w,ccs=UTF-8");
#else
	sFileMode = _T("w");//_T("w");
#endif

						// ������ ������ ����� �����.
	if (clFinder.FindFile(szFilePath) == FALSE)
	{
		_tfopen_s(&fp, szFilePath, sFileMode);
		if (fp != NULL)
		{
			_ftprintf_s(fp, _T("�׸�,%s,"), (LPTSTR)(LPCTSTR)title);
			_ftprintf_s(fp, "\n");
		}
		fclose(fp);
	}

#ifdef _UNICODE
	sFileMode = _T("a,ccs=UTF-8");
#else
	sFileMode = _T("a");
#endif

	_tfopen_s(&fp, szFilePath, sFileMode);
	if (fp != NULL)
	{
		_ftprintf_s(fp, _T("%s,%s"), (LPTSTR)(LPCTSTR)name, (LPTSTR)(LPCTSTR)data);
		_ftprintf_s(fp, "\n");
		fclose(fp);
	}
	return true;
}
double calcChiSquareRow(cv::Size boardSize, cv::vector<cv::Point2f> pointBuf)
{
    double sum_xSquare;
    double sum_x;
    double sum_xy;
    double sum_ySquare;
    double sum_y;

    double a, b, c, alpha, beta, sin_theta, cos_theta;
    double chi_square;
    double sum_chi_square = 0;

    for (int row = 0; row < boardSize.height; row += 2)
    {
        sum_xSquare = 0;
        sum_x = 0;
        sum_xy = 0;
        sum_ySquare = 0;
        sum_y = 0;

        for (int col = 0; col < boardSize.width; col++)
        {
            sum_xSquare += std::pow(pointBuf.at(col + row * boardSize.width).x, 2);
            sum_x += pointBuf.at(col + row * boardSize.width).x;
            sum_xy += pointBuf.at(col + row * boardSize.width).x * pointBuf.at(col + row * boardSize.width).y;
            sum_ySquare += std::pow(pointBuf.at(col + row * boardSize.width).y, 2);
            sum_y += pointBuf.at(col + row * boardSize.width).y;
        }

        a = sum_xSquare - (std::pow(sum_x, 2) / boardSize.width);
        b = sum_xy - ((sum_x * sum_y) / boardSize.width);
        c = sum_ySquare - (std::pow(sum_y, 2) / boardSize.width);
        alpha = a - c;
        beta = alpha / (2 * sqrt(std::pow(alpha, 2) + (4 * std::pow(b, 2))));
        sin_theta = sqrt(0.5 - beta);
        cos_theta = sqrt(0.5 + beta);

        chi_square = (a * std::pow(sin_theta, 2)) - (2 * std::abs(b) * std::abs(sin_theta) * cos_theta) + (c * std::pow(cos_theta, 2));
        sum_chi_square += chi_square;
    }

    return sum_chi_square;
}

double calcChiSquareCol(cv::Size boardSize, cv::vector<cv::Point2f> pointBuf)
{
    double sum_xSquare;
    double sum_x;
    double sum_xy;
    double sum_ySquare;
    double sum_y;

    double a, b, c, alpha, beta, sin_theta, cos_theta;
    double chi_square;
    double sum_chi_square = 0;

    for (int col = 0; col < boardSize.width; col += 2)
    {
        sum_xSquare = 0;
        sum_x = 0;
        sum_xy = 0;
        sum_ySquare = 0;
        sum_y = 0;

        for (int row = 0; row < boardSize.height; row++)
        {
            sum_xSquare += std::pow(pointBuf.at(col + row * boardSize.width).x, 2);
            sum_x += pointBuf.at(col + row * boardSize.width).x;
            sum_xy += pointBuf.at(col + row * boardSize.width).x * pointBuf.at(col + row * boardSize.width).y;
            sum_ySquare += std::pow(pointBuf.at(col + row * boardSize.width).y, 2);
            sum_y += pointBuf.at(col + row * boardSize.width).y;
        }

        a = sum_xSquare - (std::pow(sum_x, 2) / boardSize.height);
        b = sum_xy - ((sum_x * sum_y) / boardSize.height);
        c = sum_ySquare - (std::pow(sum_y, 2) / boardSize.height);
        alpha = a - c;
        beta = alpha / (2 * sqrt(std::pow(alpha, 2) + (4 * std::pow(b, 2))));
        sin_theta = sqrt(0.5 - beta);
        cos_theta = sqrt(0.5 + beta);

        chi_square = (a * std::pow(sin_theta, 2)) - (2 * std::abs(b) * std::abs(sin_theta) * cos_theta) + (c * std::pow(cos_theta, 2));
        sum_chi_square += chi_square;
    }

    return sum_chi_square;
}



// DistortionCenter ����
cv::vector<cv::Point2f> undistort(cv::Size boardSize, cv::vector<cv::Point2f> pointBuf, cv::Point2f distortionCenter, double w)
{
    cv::vector<cv::Point2f> undistortedCoordinates;

    for (int i = 0; i < boardSize.width * boardSize.height; i++)
    {
        cv::Point2f cartesianCoordinates = pointBuf.at(i) - distortionCenter;

        double rd = sqrt(std::pow(cartesianCoordinates.x, 2) + std::pow(cartesianCoordinates.y, 2));
        double ru = tan(rd * w) / (2 * tan(w / 2.0));

        cartesianCoordinates *= (float)(ru / rd);

        undistortedCoordinates.push_back(cartesianCoordinates + distortionCenter);
    }

    return undistortedCoordinates;
}

cv::Point2f evaluateDistortionCenter(cv::Size boardSize, cv::vector<cv::Point2f> pointBuf, const double alpha, const double beta, const double gamma)
{
    cv::Point2f returnDC;

    double resultCol, resultRow;

    int ilo, ihi, inhi;
    float fac, fac1, fac2;

    cv::Point2f simplexPc, simplexPsum, simplexPs, simplexPss;
    double simplexYs, simplexYsave;

    cv::vector<cv::Point2f> simplexP;
    cv::vector<double> simplexY;

    cv::vector<cv::Point2f> undistortedPointBuf;

    const int offset = 30;
    simplexP.push_back(cv::Point2f(gMIUDevice.nWidth / 2, gMIUDevice.nHeight / 2 - offset));
    simplexP.push_back(cv::Point2f((gMIUDevice.nWidth / 2) - (0.85 * offset), (gMIUDevice.nHeight / 2) + (0.5 * offset)));
    simplexP.push_back(cv::Point2f((gMIUDevice.nWidth / 2) + (0.85 * offset), (gMIUDevice.nHeight / 2) + (0.5 * offset)));

    undistortedPointBuf = undistort(boardSize, pointBuf, simplexP.at(0), w);
    resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
    resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
    simplexY.push_back(resultCol + resultRow);

    undistortedPointBuf = undistort(boardSize, pointBuf, simplexP.at(1), w);
    resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
    resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
    simplexY.push_back(resultCol + resultRow);

    undistortedPointBuf = undistort(boardSize, pointBuf, simplexP.at(2), w);
    resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
    resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
    simplexY.push_back(resultCol + resultRow);


    for (int ii = 0; ii < 500; ii++)
    {
        ilo = 0;

        ihi = simplexY.at(0) > simplexY.at(1) ? (inhi = 1, 0) : (inhi = 0, 1);
        for (int i = 0; i < 3; i++)
        {
            if (simplexY.at(i) <= simplexY.at(ilo))
            {
                ilo = i;
            }

            if (simplexY.at(i) > simplexY.at(ihi))
            {
                inhi = ihi;
                ihi = i;
            }
            else if (simplexY.at(i) > simplexY.at(inhi) && i != ihi)
            {
                inhi = i;
            }
        }
        //
        returnDC = simplexP.at(ilo);

        simplexPsum = simplexP.at(0) + simplexP.at(1) + simplexP.at(2);
        simplexPc = (1 / 3) * simplexPsum;

        fac = alpha;//-1.0;
        fac1 = (1.0 - fac) / 2;
        fac2 = fac1 - fac;
        simplexPs = fac1 * simplexPsum - fac2 * simplexP.at(ihi);
        undistortedPointBuf = undistort(boardSize, pointBuf, simplexPs, w);
        resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
        resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
        simplexYs = resultCol + resultRow;

        if (simplexYs < simplexY.at(ihi))
        {
            simplexY.at(ihi) = simplexYs;
            simplexP.at(ihi) = simplexPs;
            simplexPsum = simplexP.at(0) + simplexP.at(1) + simplexP.at(2);
        }
        if (simplexYs <= simplexY.at(ilo))
        {
            fac = beta;//2.0;
            fac1 = (1.0 - fac) / 2;
            fac2 = fac1 - fac;
            simplexPs = fac1 * simplexPsum - fac2 * simplexP.at(ihi);
            undistortedPointBuf = undistort(boardSize, pointBuf, simplexPs, w);
            resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
            resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
            simplexYs = resultCol + resultRow;

            if (simplexYs < simplexY.at(ihi))
            {
                simplexY.at(ihi) = simplexYs;
                simplexP.at(ihi) = simplexPs;
                simplexPsum = simplexP.at(0) + simplexP.at(1) + simplexP.at(2);
            }
        }
        else if (simplexYs >= simplexY.at(inhi))
        {
            simplexYsave = simplexY.at(ihi);
            fac = gamma;//0.5;
            fac1 = (1.0 - fac) / 2;
            fac2 = fac1 - fac;
            simplexPs = fac1 * simplexPsum - fac2 * simplexP.at(ihi);
            undistortedPointBuf = undistort(boardSize, pointBuf, simplexPs, w);
            resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
            resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
            simplexYs = resultCol + resultRow;

            if (simplexYs < simplexY.at(ihi))
            {
                simplexY.at(ihi) = simplexYs;
                simplexP.at(ihi) = simplexPs;
                simplexPsum = simplexP.at(0) + simplexP.at(1) + simplexP.at(2);
            }
            if (simplexYs >= simplexYsave)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (i != ilo)
                    {
                        simplexP.at(i) = 0.5 * (simplexP.at(i) + simplexP.at(ilo));
                        undistortedPointBuf = undistort(boardSize, pointBuf, simplexP.at(i), w);
                        resultCol = calcChiSquareCol(boardSize, undistortedPointBuf);
                        resultRow = calcChiSquareRow(boardSize, undistortedPointBuf);
                        simplexY.at(i) = resultCol + resultRow;
                    }
                }
                simplexPsum = simplexP.at(0) + simplexP.at(1) + simplexP.at(2);
            }
        }

    }

    return returnDC;
}




void saveInspImage(int iMode, int iStep)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    int length = strlen(Task.LotNo);
    if (length<1)
        sprintf_s(Task.LotNo, "(NULL)");

    CFileFind finder;
    BOOL IsFind;
    CString FolderName, tmpName;

    if (iMode == LENS_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Lens", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
    else if (iMode == PCB_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Pcb", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth,  Task.LotNo);
    else if (iMode == CHIP_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Chip", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
    else if (iMode == AA_FINAL_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Active Align", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
    else if (iMode == AA_NG_IMAGE_SAVE || iMode == AA_CIRCLE_NG_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Active Align NG", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
    else if (iMode == OPTIC_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Optic", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
    else if (iMode == DEFECT_NG_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Defect", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
    else if (iMode == EPOXY_IMAGE_SAVE)
        FolderName.Format("%s\\%s\\%04d%02d\\%s\\Epoxy", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);

    IsFind = finder.FindFile(FolderName);


    if (!IsFind)
    {
        tmpName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(tmpName);
        if (!IsFind)
            CreateDirectory(tmpName, NULL);

        tmpName.Format("%s", BASE_LOG_DIR);
        IsFind = finder.FindFile(tmpName);
        if (!IsFind)
            CreateDirectory(tmpName, NULL);

		tmpName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName);
		IsFind = finder.FindFile(tmpName);
		if (!IsFind)
			CreateDirectory(tmpName, NULL);


        tmpName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
        CreateDirectory(tmpName, NULL);

        tmpName.Format("%s\\%s\\%04d%02d\\%s", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth, Task.LotNo);
        CreateDirectory(tmpName, NULL);


        CreateDirectory(FolderName, NULL);
    }


    finder.Close();


    char fileName[1000];

    if (iMode == LENS_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\Lens - %d.bmp", FolderName, iStep);
        MbufExport(fileName, M_BMP, vision.MilProcImageChild[1]);
    }
    else if (iMode == PCB_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\PCB - %d.bmp", FolderName, iStep);
        MbufExport(fileName, M_BMP, vision.MilProcImageChild[1]);
    }
    else if (iMode == CHIP_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\Chip - %d.bmp", FolderName, iStep);
        MbufExport(fileName, M_BMP, vision.MilProcImageChild[0]);
    }
    else if (iMode == AA_FINAL_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\[�Ϸ�] %d.bmp", FolderName, work.m_iCnt_Output % 2);
        MbufExport(fileName, M_BMP, vision.MilProcImage[1]);
    }
    else if (iMode == AA_NG_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\[�Ϸ�] %s-%d.bmp", FolderName, Task.ChipID, iStep);
        MbufExport(fileName, M_BMP, vision.MilProcImage[1]);
    }
    else if (iMode == AA_CIRCLE_NG_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\[������ũ] %s-%d.bmp", FolderName, Task.ChipID, iStep);
        MbufExport(fileName, M_BMP, vision.MilProcImage[1]);
    }
    else if (iMode == OPTIC_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\[OC] %s.bmp", FolderName, Task.ChipID);
        MbufExport(fileName, M_BMP, vision.MilProcImage[1]);
    }
    else if (iMode == DEFECT_NG_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\%s - %02d%02d%02d.bmp", FolderName, Task.ChipID, time.wHour, time.wMinute, time.wSecond);
        MbufExport(fileName, M_BMP, vision.MilProcImage[1]);
    }
    else if (iMode == EPOXY_IMAGE_SAVE)
    {
        wsprintf(fileName, "%s\\Epoxy Insp - %d.bmp", FolderName, iStep);
        MbufExport(fileName, M_BMP, vision.MilGrabImageChild[CAM2]);
    }
}


void LogSave(CString logStr)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    FILE *out;

    CFileFind finder;
    BOOL IsFind;
    CString FolderName;
    
    FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName,  time.wYear, time.wMonth);
    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

        FolderName.Format("%s", BASE_LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
        {
            CreateDirectory(FolderName, NULL);
        }

		FolderName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName );
		IsFind = finder.FindFile(FolderName);
		if (!IsFind)
		{
			CreateDirectory(FolderName, NULL);
		}

        FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
        CreateDirectory(FolderName, NULL);
    }
    finder.Close();

    char fileName[1000];
    wsprintf(fileName, "%s\\%04d%02d%02d_LogDataAA.txt", FolderName, time.wYear, time.wMonth, time.wDay);

    if (!fopen_s(&out, fileName, "a"))
    {
        fprintf_s(out, "%s\n", (LPTSTR)(LPCTSTR)logStr);
        fclose(out);
    }
}


void SocketDataSave(int flag, CString logStr)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    FILE *out;

    CFileFind finder;
    BOOL IsFind;

    CString FolderName;
    FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);

    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
            CreateDirectory(FolderName, NULL);

        FolderName.Format("%s", LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
            CreateDirectory(FolderName, NULL);

        FolderName.Format("%s\\%04d%02d", LOG_DIR, time.wYear, time.wMonth);
        CreateDirectory(FolderName, NULL);

        FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);
        CreateDirectory(FolderName, NULL);
    }

    finder.Close();

    char fileName[1000];
    wsprintf(fileName, "%s\\SocketData-AA%d-%d-%02d.txt", FolderName, sysData.m_iSystemNo, sysData.m_iUnitNo, time.wDay);

    if (!fopen_s(&out, fileName, "a"))
    {
        if (flag == SEND_DATA)
            fprintf_s(out, "%02d:%02d:%02d:%03d [SND] %s [P %d, L %d]\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, (LPTSTR)(LPCTSTR)logStr, Task.PCBTask, Task.LensTask);
        else
            fprintf_s(out, "%02d:%02d:%02d:%03d [RCV] %s [P %d, L %d]\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, (LPTSTR)(LPCTSTR)logStr, Task.PCBTask, Task.LensTask);
        fclose(out);
    }
}


void MiuLogSave(CString logStr)
{
    return;


    SYSTEMTIME time;
    ::GetLocalTime(&time);

    FILE *out;

    CFileFind finder;
    BOOL IsFind;
    CString FolderName;

    FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);

    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
            CreateDirectory(FolderName, NULL);

        FolderName.Format("%s", LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
            CreateDirectory(FolderName, NULL);

        FolderName.Format("%s\\%04d%02d", LOG_DIR, time.wYear, time.wMonth);
        CreateDirectory(FolderName, NULL);

        FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);
        CreateDirectory(FolderName, NULL);
    }

    finder.Close();

    char fileName[1000];
    wsprintf(fileName, "%s\\MiuData-AA%d-%d-%02d.txt", FolderName, sysData.m_iSystemNo, sysData.m_iUnitNo, time.wDay);

    if (!fopen_s(&out, fileName, "a"))
    {
        fprintf_s(out, "%02d:%02d:%02d:%03d %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, (LPTSTR)(LPCTSTR)logStr);
        fclose(out);
    }
}



void AlignLogSave(CString logStr)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    FILE *out;

    CFileFind finder;
    BOOL IsFind;
    CString FolderName;

    FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);

    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
            CreateDirectory(FolderName, NULL);

        FolderName.Format("%s", LOG_DIR);
        IsFind = finder.FindFile(FolderName);
        if (!IsFind)
            CreateDirectory(FolderName, NULL);

        FolderName.Format("%s\\%04d%02d", LOG_DIR, time.wYear, time.wMonth);
        CreateDirectory(FolderName, NULL);

        FolderName.Format("%s\\%04d%02d\\%02d", LOG_DIR, time.wYear, time.wMonth, time.wDay);
        CreateDirectory(FolderName, NULL);
    }

    finder.Close();

    char fileName[1000];
    wsprintf(fileName, "%s\\AlignData-AA%d-%d-%02d.txt", FolderName, sysData.m_iSystemNo, sysData.m_iUnitNo, time.wDay);

    if (!fopen_s(&out, fileName, "a"))
    {
        fprintf_s(out, "%02d:%02d:%02d:%03d %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, (LPTSTR)(LPCTSTR)logStr);
        fclose(out);
    }
}



void AlignResultSave(CString logStr)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    FILE *out;

    CFileFind finder;
    BOOL IsFind;
    CString FolderName;

	FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);

    IsFind = finder.FindFile(FolderName);
    if (!IsFind)
    {
        FolderName.Format("%s", BASE_DIR);
        IsFind = finder.FindFile(FolderName);
		if (!IsFind) {
			CreateDirectory(FolderName, NULL);
		}
        FolderName.Format("%s", BASE_LOG_DIR);
        IsFind = finder.FindFile(FolderName);
		if (!IsFind) {
			CreateDirectory(FolderName, NULL);
		}
		FolderName.Format("%s\\%s", BASE_LOG_DIR, model.mCurModelName);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind) {
			CreateDirectory(FolderName, NULL);
		}
        FolderName.Format("%s\\%s\\%04d%02d", BASE_LOG_DIR, model.mCurModelName, time.wYear, time.wMonth);
		IsFind = finder.FindFile(FolderName);
		if (!IsFind) {
			CreateDirectory(FolderName, NULL);
		}
    }

    finder.Close();

    char fileName[1000];
    wsprintf(fileName, "%s\\AlignResult_%04d%02d%02d.csv", FolderName, time.wYear, time.wMonth, time.wDay);

    if (!fopen_s(&out, fileName, "a"))
    {
        fprintf_s(out, "%02d:%02d:%02d:%03d, %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, (LPTSTR)(LPCTSTR)logStr);
        fclose(out);
    }
}




void DoEvents()
{
    MSG message;
    if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        ::TranslateMessage(&message);
        ::DispatchMessage(&message);
    }
}

bool PcbOsTest()
{
    return true;
}


void deletedir(CString FolderName)
{
    CString str;

    CFileFind finder;

    BOOL bP = finder.FindFile(FolderName + "\\*.*");

    try
    {
        while (bP)
        {
            bP = finder.FindNextFile();
            CString strName = finder.GetFileName();
            if (finder.IsDirectory() && !finder.IsDots())
                deletedir(FolderName + "\\" + strName);
            else
            {
                if (!finder.IsDots())
                    ::DeleteFile(FolderName + "\\" + strName);
            }
        }
    }
    catch (CException* e)
    {
        str.Format(_T("Exception of Deleting the folder"));
        e->Delete();
    }

    finder.Close();
    ::RemoveDirectory(FolderName);
}


void delayMsg(CString msg, int delay, COLORREF color)
{
    if (delayDlg == NULL)
    {
        delayDlg = new CDelayMsgDlg;
        delayDlg->Create(IDD_DIALOG_DELAY);
        delayDlg->ShowWindow(SW_HIDE);
    }

    delayDlg->ShowWindow(SW_HIDE);

    CString dispStr;
    dispStr.Format("  %s", msg);
    delayDlg->m_labelDelayMsg.SetText(dispStr);

    delayDlg->delay = delay + 500;
    delayDlg->color = color;

    delayDlg->showTime = CTime::GetCurrentTime();
    delayDlg->ShowWindow(SW_SHOW);


    for (int i = 0; i<5; i++)
        checkMessage();
}



void delayMsg(bool showFlag)
{
    if (delayDlg == NULL)
    {
        delayDlg = new CDelayMsgDlg;
        delayDlg->Create(IDD_DIALOG_DELAY);
    }

    delayDlg->ShowWindow(SW_HIDE);
    delayDlg->delay = 0;
}



bool getMotorPos(int axis, int index, double &pos, bool flagDisp)		// flagDisp :1-> ȭ�� ���� ��  0-> ���ġ��
{
    bool rtnVal = true;

    if (axis >= Motor_Lens_X && axis <= Motor_Lens_Z)			// 5�� ����
    {
        if (index >= Wait_Pos && index <= Bonding_Pos)			// 0:���, 1:����, 2:Align, 3:����
        {
            if (flagDisp)
                pos = (model.axis[axis].pos[index] - sysData.dDataset[axis]);
            else
                pos = model.axis[axis].pos[index];
        }
        else
        {
            rtnVal = false;
            if (flagDisp)
                pos = (model.axis[axis].pos[index] - sysData.dDataset[axis]);
            else
                pos = model.axis[axis].pos[index];
        }
    }
    else if (axis >= Motor_PCB_X && axis <= Motor_PCB_Xt)		//5�� ����
    {
        if (flagDisp)
            pos = (model.axis[axis].pos[index] - sysData.dDataset[axis]);
        else
            pos = model.axis[axis].pos[index];
    }
    else
    {
        CString tmpStr;
        sLangChange.LoadStringA(IDS_STRING1238);	//���� ��ġ ���� ������ [getMotorPos (%d, %d)]
        tmpStr.Format(sLangChange, axis, index);

        delayMsg(tmpStr.GetBuffer(999), 3000, M_COLOR_RED);
        rtnVal = false;
        pos = 0;
    }

    return rtnVal;
}

bool askMsg(CString MSG)
{
    CAskMsgDlg dlg;

    if (MSG.Find("\n") >= 0)
    {
        dlg.m_sMsg = "\n" + MSG;
    }
    else
    {
        dlg.m_sMsg = "\n\n" + MSG;
    }

    if (dlg.DoModal() == IDOK)
        return true;
    else
        return false;

    if (Task.AutoFlag == 1)
        Dio.setAlarm(ALARM_OFF);	// Lens  ������ ���߸� Yellow
}

int askMsg2(CString MSG)
{
    CAskMsgDlg2 dlg;

    if (MSG.Find("\n") >= 0)
    {
        dlg.m_sMsg = "\n" + MSG;
    }
    else
    {
        dlg.m_sMsg = "\n\n" + MSG;
    }

    if (Task.AutoFlag == 1)
        Dio.setAlarm(ALARM_ON);

    dlg.DoModal();

    if (Task.AutoFlag == 1)
        Dio.setAlarm(ALARM_OFF);

    return dlg.m_iSelectNo;
}


void errMsg2(int Autoflag, CString MSG)
{
    CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

    if (pFrame != NULL)
    {
        CString tmpMsg;
        if (Autoflag == 1)
        {
            g_AlarmFlag = false;

            tmpMsg.Format("%s  [Lens Cent %d. Grip %d. PCB %d.]", MSG, Task.m_iLensPickupNoCentering, Task.m_iLensPickupNoGrip, Task.m_iPcbPickupNo);

            if (alarmDlg != NULL)
                alarmDlg->AlarmAdd(tmpMsg);

            Dio.setAlarm(ALARM_ON);

            if (sysData.m_FreeRun == 1)
            {
                Task.PauseLensStep = 0;
                Task.PausePCBStep = 0;

                sysData.m_FreeRun = 0;
                Task.AutoFlag = 0;		// ����
            }
            else
            {
                //Task.PauseLensStep = abs(Task.LensTask);
                Task.PausePCBStep = abs(Task.PCBTask);

                Task.AutoFlag = 2;		// �Ͻ�����
            }

            pFrame->AutoRunView(Task.AutoFlag);
        }

        pFrame->putListLog(tmpMsg);

        CInformDlg InformDlg;						// JJH..
        InformDlg.m_sMsg = MSG;
        InformDlg.color = M_COLOR_RED;

        InformDlg.DoModal();
    }
    else
    {
        AfxMessageBox(MSG);
    }
}

//! Added by LHW (2013/4/16)
//! GUI ȭ�鿡�� ������ ��ư ���ڿ��� ����, �� ��ư�� ������ ����� �ӵ� Gain�� ��ȯ�Ѵ�. 
//! GUI ȭ�鿡�� ��ư�� ���ڿ��� �����Ѵ�. 
bool SetJogSpeed(int iID_GUI, CWnd* pParentWnd, double *pOutSpeedGain)
{
    if (pParentWnd == NULL)
    {
        return false;
    }
    if (pOutSpeedGain == NULL)
    {
        return false;
    }

    CString sGetTemp = _T("");
    GetString_FromUI(iID_GUI, pParentWnd, sGetTemp);
    sGetTemp.TrimLeft();
    sGetTemp.TrimRight();

    if (sGetTemp.CompareNoCase(_T("LOW")) == 0)
    {
        *pOutSpeedGain = MOTOR_JOG_MID;
        SetString_to_UI(iID_GUI, pParentWnd, _T("MEDIUM"));
    }
    else if (sGetTemp.CompareNoCase(_T("MEDIUM")) == 0)
    {
        *pOutSpeedGain = MOTOR_JOG_HIGH;
        SetString_to_UI(iID_GUI, pParentWnd, _T("HIGH"));
    }
    else if (sGetTemp.CompareNoCase(_T("HIGH")) == 0)
    {
        *pOutSpeedGain = MOTOR_JOG_LOW;
        SetString_to_UI(iID_GUI, pParentWnd, _T("LOW"));
    }

    return true;
}

void DeletePath(CString strPath)
{//���� �� ���丮 ����
    CFileFind finder;
    BOOL bContinue = TRUE;

    if (strPath.Right(1) != _T("\\"))
        strPath += _T("\\");

    strPath += _T("*.*");
    bContinue = finder.FindFile(strPath);
    while (bContinue)
    {
        bContinue = finder.FindNextFile();
        if (finder.IsDots()) // Ignore this item.
        {
            continue;
        }
        else if (finder.IsDirectory()) // Delete all sub item.
        {
            DeletePath(finder.GetFilePath());
            ::RemoveDirectory((LPCTSTR)finder.GetFilePath());
        }
        else // Delete file.
        {
            ::DeleteFile((LPCTSTR)finder.GetFilePath());
        }
        Sleep(15);
    }

    strPath = strPath.Left(strPath.ReverseFind('\\'));
    ::RemoveDirectory((LPCTSTR)strPath);
    finder.Close();
}

void GetDateAndTime(int &nYear, int &nMonth, int &nDay, int &nHour, int &nMinute, int &nSecond)
{
    SYSTEMTIME SystemTime;

    GetLocalTime(&SystemTime);

    nYear = (int)SystemTime.wYear;
    nMonth = (int)SystemTime.wMonth;
    nDay = (int)SystemTime.wDay;
    nHour = (int)SystemTime.wHour;
    nMinute = (int)SystemTime.wMinute;
    nSecond = (int)SystemTime.wSecond;
}

void CheckHandle()
{
    MSG msg;
    PeekMessage(&msg, ::GetActiveWindow(), NULL, NULL, PM_REMOVE);
    //	PreTranslateMessage(&msg);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

bool ChartPatternMatching(int iField)
{
    double dCenterPosX, dCenterPosY, dAngle, dRate;
    CDPoint Center;

    CAABonderDlg* pFrame = (CAABonderDlg*)AfxGetApp()->m_pMainWnd;

    //	double dTop,dBottom,dLeft,dRight;

    CString FolderName;
    FolderName.Format("%s", AA_MODEL_LOAD_DIR);
    CString strType = "CHART";
    CString strPatDir;

    strPatDir.Format("%s\\%s\\Data\\Mark\\%s_Mark_%d-Mask.pat", FolderName, model.mCurModelName, strType, iField);
    MpatRestore(vision.MilSystem, strPatDir.GetBuffer(999), &vision.FieldPatternImage[iField]);

    if (FieldPatternMatching(iField, dCenterPosX, dCenterPosY, dAngle, dRate))
    {
        Center.x = model.m_FieldPattern_Pos[iField].x + (model.m_FieldPattern_Size[iField].x / 2);
        Center.y = model.m_FieldPattern_Pos[iField].y + (model.m_FieldPattern_Size[iField].y / 2);

        pFrame->ChartCenterOffsetX = Center.x - dCenterPosX;
        pFrame->ChartCenterOffsetY = Center.y - dCenterPosY;
    }
    else
    {
        Center.x = (unsigned char)gMIUDevice.nWidth >> 1;
        Center.y = (unsigned char)gMIUDevice.nHeight >> 1;
        dRate = 0.0;
        return false;
    }
    return true;
}


bool ChartCenterPatternMatching()
{
    CDPoint Center;
	double dRate = 0.0;
    double dAngle = 0.0;

    CRect rcArea;
    rcArea.left = (int)(gMIUDevice.nWidth / 2 - 100);
    rcArea.top = (int)(gMIUDevice.nHeight / 2 - 100);
    rcArea.right = (int)(gMIUDevice.nWidth / 2 + 100);
    rcArea.bottom = (int)(gMIUDevice.nHeight / 2 + 100);

    if (vision.ChartCenterPatternResult)
    {
        MpatFree(vision.ChartCenterPatternResult);
        vision.ChartCenterPatternResult = NULL;
    }
	//ChartCenterPatternImage
    MpatAllocResult(vision.MilSystem, M_DEFAULT, &vision.ChartCenterPatternResult);
    MpatSetPosition(vision.FieldPatternImage[0], rcArea.left, rcArea.top, (rcArea.right - rcArea.left), (rcArea.bottom - rcArea.top));

    MpatSetAcceptance(vision.FieldPatternImage[0], 60);
    MpatSetAccuracy(vision.FieldPatternImage[0], M_MEDIUM);
    MpatSetCertainty(vision.FieldPatternImage[0], 60);
    MpatSetNumber(vision.FieldPatternImage[0], 1L);
    MpatSetSpeed(vision.FieldPatternImage[0], M_MEDIUM);
    MpatSetSearchParameter(vision.FieldPatternImage[0], M_FAST_FIND, M_DISABLE);

    /*MpatPreprocModel(vision.MilProcImageChild[3], vision.FieldPatternImage[0], M_DEFAULT);
    MpatFindModel(vision.MilProcImageChild[3], vision.FieldPatternImage[0], vision.ChartCenterPatternResult);*/

	MpatPreprocModel(vision.MilGrabImageChild[3], vision.FieldPatternImage[0], M_DEFAULT);
	MpatFindModel(vision.MilGrabImageChild[3], vision.FieldPatternImage[0], vision.ChartCenterPatternResult);
    long lObjNum = MpatGetNumber(vision.ChartCenterPatternResult, M_NULL);

    if (lObjNum > 0)
    {
        MpatGetResult(vision.ChartCenterPatternResult, M_POSITION_X, &Center.x);
        MpatGetResult(vision.ChartCenterPatternResult, M_POSITION_Y, &Center.y);
        MpatGetResult(vision.ChartCenterPatternResult, M_ANGLE, &dAngle);
        MpatGetResult(vision.ChartCenterPatternResult, M_SCORE, &dRate);
    }
    else
    {
        Center.x = (unsigned char)gMIUDevice.nWidth >> 1;
        Center.y = (unsigned char)gMIUDevice.nHeight >> 1;
        dRate = 0.0;
        return false;
    }

    if (model.m_FieldPatternCenter[CHART_CENTER].x > 0 && model.m_FieldPatternCenter[CHART_CENTER].y > 0
        && model.m_FieldPattern_Pos[CHART_CENTER].x > 0 && model.m_FieldPattern_Pos[CHART_CENTER].y > 0
        && model.m_FieldPattern_Size[CHART_CENTER].x > 0 && model.m_FieldPattern_Size[CHART_CENTER].y > 0
        )
    {
        theApp.MainDlg->ChartCenterOffsetX = Center.x - gMIUDevice.nWidth / 2;//-((model.m_FieldPattern_Pos[CHART_CENTER].x+model.m_FieldPattern_Size[CHART_CENTER].x)/2-model.m_FieldPatternCenter[CHART_CENTER].x);
		theApp.MainDlg->ChartCenterOffsetY = Center.y - gMIUDevice.nHeight / 2;//-((model.m_FieldPattern_Pos[CHART_CENTER].y+model.m_FieldPattern_Size[CHART_CENTER].y)/2-model.m_FieldPatternCenter[CHART_CENTER].y);;
    }
    else
    {
		theApp.MainDlg->ChartCenterOffsetX = Center.x - gMIUDevice.nWidth / 2;
		theApp.MainDlg->ChartCenterOffsetY = Center.y - gMIUDevice.nHeight / 2;
    }

    return true;
}

bool FieldPatternMatching(int iFieldNo, double &dCenterPosX, double &dCenterPosY, double &dAngle, double &dRate)
{
    CRect rcArea;
    bool bResult = false;
    rcArea.left = (int)model.m_FieldPattern_Pos[iFieldNo].x - 100;
    rcArea.top = (int)model.m_FieldPattern_Pos[iFieldNo].y - 100;
    rcArea.right = (int)model.m_FieldPattern_Pos[iFieldNo].x + model.m_FieldPattern_Size[iFieldNo].x + 100;
    rcArea.bottom = (int)model.m_FieldPattern_Pos[iFieldNo].y + model.m_FieldPattern_Size[iFieldNo].y + 100;

    rcArea.left = (rcArea.left<0) ? 0 : rcArea.left;
    rcArea.top = (rcArea.top<0) ? 0 : rcArea.top;
    rcArea.right = (rcArea.right>gMIUDevice.nWidth) ? gMIUDevice.nWidth : rcArea.right;
    rcArea.bottom = (rcArea.bottom>gMIUDevice.nHeight) ? gMIUDevice.nHeight : rcArea.bottom;

    if (vision.FieldPatternResult[iFieldNo])
    {
        MpatFree(vision.FieldPatternResult[iFieldNo]);
        vision.FieldPatternResult[iFieldNo] = NULL;
    }

    MpatAllocResult(vision.MilSystem, M_DEFAULT, &vision.FieldPatternResult[iFieldNo]);
	if (iFieldNo == 0) 
	{
		MpatSetPosition(vision.FieldPatternImage[iFieldNo], rcArea.left, rcArea.top, model.m_FieldPattern_Size[iFieldNo].x + 50, model.m_FieldPattern_Size[iFieldNo].y + 50);
	}
	else
	{
		MpatSetPosition(vision.FieldPatternImage[iFieldNo], rcArea.left, rcArea.top, model.m_FieldPattern_Size[iFieldNo].x + 50, model.m_FieldPattern_Size[iFieldNo].y + 50);
	}
    MpatSetAcceptance(vision.FieldPatternImage[iFieldNo], 80);
    MpatSetAccuracy(vision.FieldPatternImage[iFieldNo], M_LOW);
    MpatSetCertainty(vision.FieldPatternImage[iFieldNo], 70);
    MpatSetNumber(vision.FieldPatternImage[iFieldNo], 1L);
    MpatSetSpeed(vision.FieldPatternImage[iFieldNo], M_HIGH);
    MpatSetSearchParameter(vision.FieldPatternImage[iFieldNo], M_FAST_FIND, M_DISABLE);

    MpatPreprocModel(vision.MilGrabImageChild[3], vision.FieldPatternImage[iFieldNo], M_DEFAULT);
    MpatFindModel(vision.MilGrabImageChild[3], vision.FieldPatternImage[iFieldNo], vision.FieldPatternResult[iFieldNo]);

    long lObjNum = MpatGetNumber(vision.FieldPatternResult[iFieldNo], M_NULL);
    double px;
    if (lObjNum > 0)
    {
        MpatGetResult(vision.FieldPatternResult[iFieldNo], M_POSITION_X, &dCenterPosX);
        MpatGetResult(vision.FieldPatternResult[iFieldNo], M_POSITION_Y, &dCenterPosY);
        MpatGetResult(vision.FieldPatternResult[iFieldNo], M_ANGLE, &dAngle);
        MpatGetResult(vision.FieldPatternResult[iFieldNo], M_SCORE, &dRate);

        bResult = true;
    }
    else
    {
        dCenterPosX = (unsigned char)gMIUDevice.nWidth >> 1;
        dCenterPosY = (unsigned char)gMIUDevice.nHeight >> 1;
        dRate = 0.0;
        bResult = false;
    }

    MpatFree(vision.FieldPatternResult[iFieldNo]);
    vision.FieldPatternResult[iFieldNo] = NULL;
    return bResult;
}


bool getDistortionCenter(CPoint& dc)
{
    cv::Mat src, src_gray;
    cv::Mat distortionPatchRoiT, distortionPatchRoiT_threshold;
    cv::Mat distortionPatchRoiB, distortionPatchRoiB_threshold;

    src = cv::imread("DistortionCenter.BMP");
    cv::cvtColor(src, src_gray, CV_BGR2GRAY);

    distortionPatchRoiT = src_gray(cv::Rect(320, 140, 640, 320));
    distortionPatchRoiB = src_gray(cv::Rect(320, 500, 640, 320));

    cv::threshold(distortionPatchRoiT, distortionPatchRoiT_threshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::threshold(distortionPatchRoiB, distortionPatchRoiB_threshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Size boardSizeT = cv::Size(13, 4);//cv::Size(13, 6);
    cv::Size boardSizeB = cv::Size(13, 4);//cv::Size(13, 6);
    cv::Size boardSize = cv::Size(13, 8);//cv::Size(13, 12);

    cv::vector<cv::Point2f> pointBufT;
    cv::vector<cv::Point2f> pointBufB;
    cv::vector<cv::Point2f> pointBuf;

    bool found1 = cv::findCirclesGrid(distortionPatchRoiT_threshold, boardSizeT, pointBufT);
    bool found2 = cv::findCirclesGrid(distortionPatchRoiB_threshold, boardSizeB, pointBufB);

    cv::Point2f cvp;

    if (found1 && found2)
    {
        for (int i = 0; i < boardSizeT.height * boardSizeT.width; i++)
        {
            pointBufT.at(i) += cv::Point2f(320, 140);
            pointBufB.at(i) += cv::Point2f(320, 500);
        }

        pointBuf.reserve(pointBufT.size() + pointBufB.size());
        pointBuf.insert(pointBuf.end(), pointBufT.begin(), pointBufT.end());
        pointBuf.insert(pointBuf.end(), pointBufB.begin(), pointBufB.end());

        const float alpha = -1.0f;
        const float beta = 2.0f;
        const float gamma = 0.5f;

        cvp = evaluateDistortionCenter(boardSize, pointBuf, alpha, beta, gamma);
        dc.x = cvp.x;
        dc.y = cvp.y;
        //		std::cout << "(" << dc.x << ", " << dc.y << ")" << std::endl;
    }

    return 0;
}


void RecursiveDelete(CString szPath)
{
    CFileFind ff;
    CString path = szPath;

    if (path.Right(1) != "\\")
        path += "\\";

    path += "*.*";

    BOOL res = ff.FindFile(path);

    while (res)
    {
        res = ff.FindNextFile();
        if (!ff.IsDots() && !ff.IsDirectory())
            DeleteFile(ff.GetFilePath());
        else if (ff.IsDirectory())
        {
            path = ff.GetFilePath();
            RecursiveDelete(path);
            RemoveDirectory(path);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

CString CovertToHex(CString data, int type)
{
	CString rtnValue;
	CString temporary;
	int value = 0;
	float fNum = 0.0;
	fNum = (_ttof(data));
	char *pChar = (char*)&fNum;
	unsigned char szTemp[MAX_PATH];
	memcpy(szTemp, &fNum, sizeof(float));
	int len = data.GetLength();
	if (type == FLOAT_MODE)
	{
		len = 4;
	}
	for (int x = 0; x < len; x++)
	{
		if (type == ASCII_MODE)
		{
			value = (int)(data[x]);
			temporary.Format("%02X", value);
		}
		else if (type == FLOAT_MODE)
		{
			temporary.Format("%X", szTemp[3 - x]);
		}
		else if (type == DEC_MODE)
		{

		}
		rtnValue += temporary;
	}
	return rtnValue;
}
CString CovertToChar(CString data, int type)
{
	CString rtnValue;
	CString temporary;
	data.Replace(" ", "");
	int len = (int)data.GetLength();
	byte value;
	UINT32 AAA;
	float fNum = 0.0;
	if (type == ASCII_MODE)
	{
		for (int x = 0; x < len; x += 2)
		{
			value = (byte)_tcstol(data.Mid(x, 2), NULL, 16);
			temporary.Format("%c", value);
			rtnValue += temporary;
		}
	}
	else if (type == FLOAT_MODE)
	{
		sscanf_s(data.GetBuffer(0), "%x", &AAA);
		fNum = *((float*)&AAA);
		rtnValue.Format("%f", fNum);
	}
	else if (type == DEC_MODE)
	{

	}

	return rtnValue;
}
double AutoZSearchCalc()
{
    if (model.m_AUTO_Z_SEARCH_CNT == 0)
    {
        return 0.0;
    }

    int pitch = MbufInquire(vision.MilProcImageChild[4], M_PITCH, NULL);
    int width = MbufInquire(vision.MilProcImageChild[4], M_SIZE_X, NULL);
    int Height = MbufInquire(vision.MilProcImageChild[4], M_SIZE_Y, NULL);

    double o1, o2, d1, d2, z, zMax, zMin, zSum;
    z = zSum = 0;
    bool bRtn = false;
    Task.getROI();
    for (int i = 0; i < model.m_AUTO_Z_SEARCH_CNT; i++)
    {
        vision.MilBufferUpdate();
        bRtn = Task.getROI_SFR(SFR_FINAL);

        if (!bRtn)return 0.0;

        o1 = sqrt(pow((double)model.m_Fiducial_Mark[2].x - (double)model.m_Fiducial_Mark[1].x, 2) + pow((double)model.m_Fiducial_Mark[2].y - (double)model.m_Fiducial_Mark[1].y, 2));
        o2 = sqrt(pow((double)model.m_Fiducial_Mark[3].x - (double)model.m_Fiducial_Mark[0].x, 2) + pow((double)model.m_Fiducial_Mark[3].y - (double)model.m_Fiducial_Mark[0].y, 2));

        d1 = sqrt(pow((double)Task.m_CircleP[2].x - (double)Task.m_CircleP[1].x, 2) + pow((double)Task.m_CircleP[2].y - (double)Task.m_CircleP[1].y, 2));
        d2 = sqrt(pow((double)Task.m_CircleP[3].x - (double)Task.m_CircleP[0].x, 2) + pow((double)Task.m_CircleP[3].y - (double)Task.m_CircleP[0].y, 2));

        z = (model.m_AUTO_Z_SEARCH_VAL * ((o1 + o2) / 2 - (d1 + d2) / 2) * model.m_dSize_CCD_Cell) / 1000.0;
        z = cvRound(z * 100.0) / 100.0;

        zSum += z;
    }

    double dMovePos = (double)(zSum / model.m_AUTO_Z_SEARCH_CNT);

    dMovePos = (dMovePos > 0) && (dMovePos < 0.025) ? 0 : dMovePos;
    dMovePos = (dMovePos< 0) && (dMovePos > -0.025) ? 0 : dMovePos;

    dMovePos = (dMovePos > 0) && (dMovePos > 0.2) ? 0.2 : dMovePos;
    dMovePos = (dMovePos< 0) && (dMovePos < -0.90) ? -0.90 : dMovePos;

    return dMovePos;
}
double getAngle(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	double rad = atan2(dx, dy);
	double degree = (rad * 180) / PI;

	return degree;
}
double getRadianFromAng(double Angle)
{
	return (PI / 180) * Angle;
}
double getAngFromRadian(double Angle)
{
	return (180 / PI) * Angle;
}