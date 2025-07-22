  // LMPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LMP.h"
#include "LMPDlg.h"
#include "LMP_Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT g_uShellRestart;
DWORD TimeCnt;
LogConfig* pLog;
int dirCnt;
unsigned __stdcall ThreadFunc( void* pArguments );
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_LmpVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_LmpVersion = _T(LMP_VERSION);
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_VERSION, m_LmpVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLMPDlg dialog

CLMPDlg::CLMPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLMPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLMPDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLMPDlg)

	DDX_Control(pDX, IDC_STATIC_TIME, m_staticTime);
	DDX_Control(pDX, IDC_EDIT_RANGE, m_editPeriod);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLMPDlg, CDialog)
	//{{AFX_MSG_MAP(CLMPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_REFRESH, OnButtonRefresh)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_TRAYICON_MSG, TrayIconMsg)
	ON_REGISTERED_MESSAGE(g_uShellRestart, OnTrayShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLMPDlg message handlers

BOOL CLMPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// TrayIcon 정리.
	TeleShellTrayRefresh::Refresh();

	m_bTrayFlag = FALSE;
	RegistTrayIcon();
	g_uShellRestart = RegisterWindowMessage(__TEXT("TaskbarCreated"));
	StartFunc();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLMPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLMPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLMPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 시작
void CLMPDlg::StartFunc()
{
	TimeCnt = 0;
	hThread = 0;
	dirCnt = 0;
	bThread = TRUE;
	m_stPath = "D:\\FPDCIM\\DATA\\CONFIG\\";
	hEvent = CreateEvent(NULL, FALSE, FALSE, "LPM_EVENT");

	GetLogConfig(pLogCfg);
	// 주기 - 초단위로..
	pLogCfg->period = pLogCfg->period*60*60;
	if( pLogCfg->period <0 )
		pLogCfg->period = DEFAULT_PERIOD*60*60;

	// SonJaeWon Test
	//pLogCfg->period = 3;			//TEST


	SetTimer(1, 1000, NULL);
	pLog = pLogCfg;
	
	hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc, this, 0, (unsigned *)&dwThread );
	if(hThread==0)
	{
		bThread = FALSE;
		MessageBox("Create Thread Error");
		return;
	}
}

// Timer
void CLMPDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bLog = FALSE;

	// SonJaeWon Test
	//pLogCfg->period = 3;			//TEST

	GetTime();
	if(TimeCnt == (unsigned)pLogCfg->period)
	{
		SetEvent(hEvent);
		TimeCnt = 0;
	}
	TimeCnt++;
	CDialog::OnTimer(nIDEvent);
}

// Thread Function
unsigned __stdcall ThreadFunc( void* pArguments )
{
	CLMPDlg* dlg = (CLMPDlg*)pArguments;
	BOOL bLog1 = FALSE;
	BOOL bLog2 = FALSE;

 	while(dlg->bThread)
 	{
 		if(!WaitForSingleObject(hEvent,INFINITE))
 		{
 			bLog1 = dlg->LogManageProcess(pLog->path1);
			//bLog2 = dlg->LogManageProcess(pLog->path2);

 			if(!bLog1 && !bLog2)
				break;
 		}
 	}
	return TRUE;
}


void CLMPDlg::OnDlgClose() 
{
	// TODO: Add your message handler code here and/or call default
	bThread = FALSE;
	KillTimer(1);
	
	CloseHandle(hThread);
	CloseHandle(hEvent);
}

// 새로고침
void CLMPDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	TimeCnt = 0;
	hThread = 0;
	bThread = TRUE;
	GetLogConfig(pLogCfg);
	pLogCfg->period = pLogCfg->period*60*60;
	if( pLogCfg->period < 0 )	pLogCfg->period = DEFAULT_PERIOD*60*60;
}

// LogConfig.cfg -> LogConfig structure
void CLMPDlg::GetLogConfig(LogConfig* pCfg)
{
	try{
		char strFileName[256] = {0,};
		//char strSectionName[255] = {0,};
		memset(pCfg, 0x00, sizeof(LogConfig));
		
		sprintf(strFileName,"%s%s", m_stPath, LOG_CONFIG_FILE);
		SetScriptFileName(strFileName);
		
		GetInt("LOG_INFO", "PERIOD"	, pCfg->period);
		GetString("LOG_INFO", "PATH1" , pCfg->path1);	
		GetString("LOG_INFO", "PATH2" , pCfg->path2);
		GetInt("LOG_INFO", "LOG_COUNT" , pCfg->nLogCount);			
		
		// Display
		CString Sectiontemp;
		char szSection[4] = {0,};
		
		for( long lIdx=0; lIdx <= pCfg->nLogCount; lIdx++ )
		{
			//Sectiontemp.Format("%d",lIdx);
			//memcpy(strSectionName,Sectiontemp,sizeof(strSectionName));		//1616
			itoa(lIdx, szSection, 10);

			GetString(szSection, "ITEM_NAME" , pCfg->stLogData[lIdx].nLog_Item_Name);	
			GetInt(szSection, "ITEM_DAY", pCfg->stLogData[lIdx].nLog_Item_Day);
			GetString(szSection, "ITEM_PATH", pCfg->stLogData[lIdx].nLog_Item_Path);
		}

		// Display
		CString temp;
		temp.Format("%d",pCfg->period);
		m_editPeriod.SetWindowText(temp);
		m_editPath.SetWindowText(pCfg->path1);

		//List Box 
		CListBox *pListDay = (CListBox *)GetDlgItem(IDC_LOG_LIST_DAY);			//LOG_DAY
		CListBox *pListName = (CListBox *)GetDlgItem(IDC_LOG_LIST);				//LOG_NAME
		CListBox *pListPath = (CListBox *)GetDlgItem(IDC_LIST_PATH);			//LOG_PATH

		pListDay->ResetContent();		//LOG_DAY
		pListName->ResetContent();		//LOG_NAME
		pListPath->ResetContent();		//LOG_PATH
		
		CString Daytemp;
		for( long lIdx3=0; lIdx3 <= pCfg->nLogCount; lIdx3++ )
		{
			//LOG_DAY
			Daytemp.Format("%d",pCfg->stLogData[lIdx3].nLog_Item_Day);			
			pListDay->AddString(Daytemp);		
			
			//LOG_NAME
			pListName->AddString(pCfg->stLogData[lIdx3].nLog_Item_Name);
			
			//LOG_PATH
			pListPath->AddString(pCfg->stLogData[lIdx3].nLog_Item_Path);	
		}

	}catch(...)
	{
		MessageBox("Config Read Error");
	}
}

// Dialog Current Time
void CLMPDlg::GetTime()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	WORD	wYear	=	systime.wYear;
	WORD	wMonth	=	systime.wMonth;
	WORD	wDay	=	systime.wDay;
	WORD	wHour	=	systime.wHour;
	WORD	wMin	=	systime.wMinute;
	WORD	wSec	=	systime.wSecond;
	
	today.Year = wYear;
	today.Month	= wMonth;
	today.Day = wDay;
	today.Hour = wHour;
	today.Min = wMin;
	today.Sec = wSec;
	m_stTime.Format("%04d년%02d월%02d일  %02d:%02d:%02d", wYear, wMonth, wDay, wHour, wMin, wSec);
	m_stDate.Format("%04d%02d%02d", wYear, wMonth, wDay);
	m_staticTime.SetWindowText(m_stTime);
}

// Log Management
BOOL CLMPDlg::LogManageProcess(CString path)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	CString fname, onlyPath;
	CString newpath;
	char drive[10], dir[500];
	CString szTemppath;

//	char tempname[50];

	FILETIME ftWrite;
	SYSTEMTIME stUTC, stLocal;

	BOOL bCheck = TRUE;

	if(lstrlen(path)<4)
	{
		MessageBox("None Path!!");
		return FALSE;
	}

	szTemppath = path;
	if(szTemppath.Find("\\*.*") < 0)
		szTemppath += "\\*.*";

	hSrch = FindFirstFile(szTemppath, &wfd);
	if(hSrch == INVALID_HANDLE_VALUE)
		return FALSE;
	_splitpath(szTemppath, drive, dir, NULL, NULL);

	do
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,".."))
			{
				newpath.Format("%s%s%s\\*.*", drive,dir,wfd.cFileName);
				LogManageProcess(newpath);
			}
		}
		else
		{
			// 최종 갱신일로 구분 짖는다.
			ftWrite = wfd.ftLastWriteTime;
			FileTimeToSystemTime(&ftWrite, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

// 			lstrcpy(tempname, wfd.cFileName);
// 			bCheck = CheckFileName(dir, tempname);

			onlyPath.Format("%s%s", drive,dir);
			bCheck = CheckTime(onlyPath, stLocal);
			if(!bCheck)
			{
				fname.Format("%s%s%s", drive,dir,wfd.cFileName);
				// Delete File
				DeleteFile(fname);
				// LOG
				SaveFile(fname);
			}			
		}
	} while(FindNextFile(hSrch, &wfd));
	FindClose(hSrch);
    return TRUE;
}

BOOL CLMPDlg::CheckFileName(char* dir, char* fname)
{
	char* date = NULL;
	char end = 'Z';
	CString stCheck, stFname, stDate;
	long ldate, lcheck = 0;

	BOOL bDestDayCheck = FALSE;

	stCheck = dir;
	stCheck.MakeUpper();
	stFname = fname;

	// GUI Log Pass
	if(stFname.Find(".INX") > 0)
		return TRUE;
	
	// 2008년04월17일 Type
	if(stFname.Find("년") > 0 && stFname.Find("월") > 0 && stFname.Find("일") > 0)
	{
		stFname.Replace("년", "");
		stFname.Replace("월", "");
		stFname.Replace("일", "");
		date = stFname.GetBuffer(stFname.GetLength());
	}
	else	// 20080417 Type
	{		
		strtok(fname, "_");
		while( lcheck == 0)
		{
			date = strtok(NULL, "_");
			if(lstrlen(date) <= 0)
			{
				date = &end;
				break;
			}
			lcheck = atol(date);	// 숫자인지 문자인지 Check
		}
				
		// 숫자가 아닐 경우..
		if( date[0] == 'Z')
			return TRUE;
	}
	
	stDate = date;
	ldate = atol(stDate.Left(8));
	oldday.Year = ldate/10000;
	oldday.Month = (ldate%10000)/100;
	oldday.Day = ldate%100;

	
	for( long lIdx=0; lIdx <= pLogCfg->nLogCount; lIdx++ )
	{
		if(stCheck.Find(pLogCfg->stLogData[lIdx].nLog_Item_Name) > 0)
		{
			GetDestDay(pLogCfg->stLogData[lIdx].nLog_Item_Day);
			bDestDayCheck = TRUE;
		}
	}

	if ( bDestDayCheck == FALSE ) GetDestDay(pLogCfg->stLogData[eDefaultDay].nLog_Item_Day);			//20091008_jir

	BOOL bComp = DayCompare();
	if(!bComp)	return FALSE;

	return TRUE;
}

BOOL CLMPDlg::CheckTime(CString path, SYSTEMTIME systime)
{
	BOOL bComp = FALSE;
	BOOL bDestDayCheck = FALSE;

	for( long lIdx=0; lIdx <= pLogCfg->nLogCount; lIdx++ )
	{
		if( memcmp(pLogCfg->stLogData[lIdx].nLog_Item_Path, path, lstrlen(pLogCfg->stLogData[lIdx].nLog_Item_Path)) == 0)
		{
			GetDestDay(pLogCfg->stLogData[lIdx].nLog_Item_Day);
			bDestDayCheck = TRUE;
		}
	}

	if ( bDestDayCheck == FALSE ) GetDestDay(pLogCfg->stLogData[eDefaultDay].nLog_Item_Day);			//20091008_jir

	oldday.Year = systime.wYear;
	oldday.Month = systime.wMonth;
	oldday.Day = systime.wDay;

	bComp =	DayCompare();
	if(!bComp)	return FALSE;
	
	return TRUE;
}

void CLMPDlg::GetDestDay(long term)
{
	destday.Year = today.Year;
	destday.Month = today.Month;
	destday.Day = today.Day;

	destday.Day -= term;
	while(destday.Day < 0)
	{
		if(destday.Day <= 0 )
		{
			destday.Month--;
			if(destday.Month == 0)
			{
				destday.Year--;
				destday.Month += 12;
			}
			destday.Day += 30;
		}		
	}
}

BOOL CLMPDlg::DayCompare()
{
	if(oldday.Year < destday.Year)
		return FALSE;
	else if(oldday.Year == destday.Year)
	{
		if(oldday.Month < destday.Month)
			return FALSE;
		else if(oldday.Month == destday.Month)
		{
			if(oldday.Day < destday.Day)
				return FALSE;
		}
	}
	return TRUE;
}

void CLMPDlg::SaveFile(CString filename)
{
	CreateDirectory("D:\\FPDCIM\\LOG\\LMP", NULL);
	CString fpath = "D:\\FPDCIM\\LOG\\LMP\\LMP_";
	CString name;
	CString logInfo;
	CStdioFile file;
	CFileException e;
	
	name = fpath + m_stDate + ".txt";
	if(!file.Open(name, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &e))
	{
		e.ReportError();
		return;
	}
	file.SeekToEnd();
	logInfo.Format("[%02d:%02d:%03d] %s", today.Hour, today.Min, today.Sec, filename);
	file.WriteString(logInfo + "\n");
	file.Close();
}

///////////////////////////////////////////////////////////////////////////////////// Tray Icon
void CLMPDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialog::PostNcDestroy();
}

void CLMPDlg::RegistTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // 메인 윈도우 핸들
	nid.uID = IDR_MAINFRAME; // 아이콘 리소스 ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
	nid.uCallbackMessage = WM_TRAYICON_MSG; // 콜백메시지 설정
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 아이콘 로드
	char strTitle[256];
	GetWindowText(strTitle, sizeof(strTitle)); // 캡션바에 출력된 문자열 얻음
	lstrcpy(nid.szTip, strTitle);
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bTrayFlag = TRUE;
}

long CLMPDlg::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{
	if(lParam == WM_LBUTTONDBLCLK)
	{
		ShowWindow(SW_SHOW);
	}
	return TRUE;
}

void CLMPDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_bTrayFlag) // 현재 트레이 아이콘으로 설정되었는지 확인
	{
		NOTIFYICONDATA nid;
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hWnd; // 메인 윈도우 핸들
		nid.uID = IDR_MAINFRAME;
		// 작업 표시줄(TaskBar)의 상태 영역에 아이콘을 삭제한다.
		Shell_NotifyIcon(NIM_DELETE, &nid);

		OnDlgClose();
	}	
}

LRESULT CLMPDlg::OnTrayShow(WPARAM wParam, LPARAM lParam)
{
	RegistTrayIcon();
	return TRUE;
}
