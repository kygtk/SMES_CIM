// LMP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LMP.h"
#include "LMPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLMPApp

BEGIN_MESSAGE_MAP(CLMPApp, CWinApp)
	//{{AFX_MSG_MAP(CLMPApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLMPApp construction

CLMPApp::CLMPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLMPApp object

CLMPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLMPApp initialization

BOOL CLMPApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#if 0   // SonJaeWon Tray Icon
	CLMPDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
#endif

#if 1		// SonJaeWon Hide Window / Tray Icon
	CLMPDlg* pDlg = new CLMPDlg; 
	
    if (!pDlg->Create(IDD_LMP_DIALOG)) 
        return FALSE; 
	
    m_pMainWnd = pDlg; 
	
    pDlg->ShowWindow(SW_HIDE); 
    pDlg->UpdateWindow(); 
	
    return TRUE;    // ¹Ýµå½Ã TRUE 
#endif
}

