/*******************************************************************
FileName:		TeleShellTrayRefresh.cpp

Description:	사라진 트레이 아이콘을 정리한다!

CREATED:		2006-09-12
LAST MODIFIED	2006-09-12

BY: 			KwangHee Yoo 
				cpueblo@cpueblo.com, yurchi@hanmail.net
				http://www.cpueblo.com
***********************************************************************/

//=========================================================
// INCLUDE 헤더 정의. MFC 용과 빌더용, 또는 자신의 Preheader 해더 파일로 대체
//=========================================================
#include "StdAfx.h"
#include "TeleShellTrayRefresh.h"


//=========================================================
// 구조체 정의
//=========================================================
struct TRAYDATA
{
	HWND hwnd;
	UINT uID;
	UINT uCallbackMessage;
	DWORD Reserved[2];
	HICON hIcon;
};

//=========================================================
// Tray 아이콘의 프로세스 이름을 얻기 위해 윈도우 핸들을 얻는 함수
//=========================================================
static HWND FindTrayToolbarWindow()
{
    HWND hWnd_ToolbarWindow32 = NULL;
	HWND hWnd_ShellTrayWnd;

    hWnd_ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if(hWnd_ShellTrayWnd)
	{
		HWND hWnd_TrayNotifyWnd = ::FindWindowEx(hWnd_ShellTrayWnd,NULL,_T("TrayNotifyWnd"), NULL);

		if(hWnd_TrayNotifyWnd)
		{
			HWND hWnd_SysPager = ::FindWindowEx(hWnd_TrayNotifyWnd,NULL,_T("SysPager"), NULL);	// WinXP

			// WinXP 에서는 SysPager 까지 추적            
			if(hWnd_SysPager)
			{
				hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_SysPager, NULL,_T("ToolbarWindow32"), NULL);
			}

            // Win2000 일 경우에는 SysPager 가 없이 TrayNotifyWnd -> ToolbarWindow32 로 넘어간다
        	else
            {
            	hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,_T("ToolbarWindow32"), NULL);
            }
		}
	}
    
	return hWnd_ToolbarWindow32;
}


//=========================================================
// 생성자
//=========================================================
TeleShellTrayRefresh::TeleShellTrayRefresh()
{

}


//=========================================================
// 소멸자
//=========================================================
TeleShellTrayRefresh::~TeleShellTrayRefresh()
{

}


//=========================================================
// Refresh 의 메인
//=========================================================
bool TeleShellTrayRefresh::Refresh()
{
	try
	{
		HANDLE 			m_hProcess;
		LPVOID 			m_lpData;
		TBBUTTON		tb;
		TRAYDATA		tray;
		DWORD 			dwTrayPid;
        int				TrayCount;
		
		// Tray 의 윈도우 핸들 얻기
		HWND m_hTrayWnd = FindTrayToolbarWindow();
        if (m_hTrayWnd == NULL)
			return false;
		
		// Tray 의 개수를 구하고
		TrayCount = (int)::SendMessage(m_hTrayWnd, TB_BUTTONCOUNT, 0, 0);
		
		// Tray 윈도우 핸들의 PID 를 구한다
		GetWindowThreadProcessId(m_hTrayWnd, &dwTrayPid);
		
        // 해당 Tray 의 Process 를 열어서 메모리를 할당한다
		m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTrayPid);
        if (!m_hProcess)
			return false;
		
        // 해당 프로세스 내에 메모리를 할당
		m_lpData = VirtualAllocEx(m_hProcess, NULL, sizeof (TBBUTTON), MEM_COMMIT, PAGE_READWRITE);
		if (!m_lpData)
			return false;
		
		// Tray 만큼 뺑뺑이
		for(int i = 0; i < TrayCount; i++)
		{
			::SendMessage(m_hTrayWnd, TB_GETBUTTON, i, (LPARAM)m_lpData);	
			
			// TBBUTTON 의 구조체와 TRAYDATA 의 내용을 얻기
			ReadProcessMemory(m_hProcess, m_lpData, (LPVOID)&tb, sizeof (TBBUTTON), NULL);
            ReadProcessMemory(m_hProcess, (LPCVOID)tb.dwData, (LPVOID)&tray, sizeof (tray), NULL);
            
			// 각각 트레이의 프로세스 번호를 얻어서
			DWORD dwProcessId = 0;
			GetWindowThreadProcessId(tray.hwnd, &dwProcessId);
			
			// Process 가 없는 경우 TrayIcon 을 삭제한다
			if (dwProcessId == 0)
			{
				NOTIFYICONDATA	icon;
				icon.cbSize	= sizeof(NOTIFYICONDATA);
				icon.hIcon	= tray.hIcon;
				icon.hWnd	= tray.hwnd;
				icon.uCallbackMessage = tray.uCallbackMessage;
				icon.uID	= tray.uID;
				
				Shell_NotifyIcon(NIM_DELETE, &icon);
			}
		}
		
        // 가상 메모리 해제와 프로세스 핸들 닫기
		VirtualFreeEx(m_hProcess, m_lpData, NULL, MEM_RELEASE);
		CloseHandle(m_hProcess);        
		
		return true;
	}
	catch (...)
	{
		return false;
	}
}

