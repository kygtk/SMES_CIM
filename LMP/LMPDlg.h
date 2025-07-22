// LMPDlg.h : header file
//

#if !defined(AFX_LMPDLG_H__5940BE2D_4C67_4014_A0F6_18E0C83ACECC__INCLUDED_)
#define AFX_LMPDLG_H__5940BE2D_4C67_4014_A0F6_18E0C83ACECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LMPData.h"
#include "Script.h"
#include "TeleShellTrayRefresh.h"

#define WM_TRAYICON_MSG WM_USER + 1
/////////////////////////////////////////////////////////////////////////////
// CLMPDlg dialog

class CLMPDlg : public CDialog, public CScript
{
// Construction
public:
	void OnDlgClose();
	void SaveFile(CString filename);
	LRESULT OnTrayShow(WPARAM wParam, LPARAM lParam);
	void StartFunc();
	BOOL DayCompare();
	void GetDestDay(long term);
	BOOL CheckFileName(char* dir, char* fname);
	BOOL CheckTime(CString path, SYSTEMTIME systime);
	void GetLogConfig(LogConfig* pCfg);
	BOOL LogManageProcess(CString path);
	void GetTime();
	CLMPDlg(CWnd* pParent = NULL);	// standard constructor
	
	HANDLE hThread;
	DWORD dwThread;
	CString m_stTime, m_stDate;
	CString m_stPath;
	LogConfig pLogCfg[1];	
	DateTime today, oldday, destday;
	BOOL m_bTrayFlag;
	BOOL bThread;

	

// Dialog Data
	//{{AFX_DATA(CLMPDlg)
	enum { IDD = IDD_LMP_DIALOG };

	CStatic	m_staticTime;
	CEdit	m_editPeriod;
	CEdit	m_editPath;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLMPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	long TrayIconMsg(WPARAM wParam, LPARAM lParam);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLMPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonRefresh();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RegistTrayIcon();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMPDLG_H__5940BE2D_4C67_4014_A0F6_18E0C83ACECC__INCLUDED_)
