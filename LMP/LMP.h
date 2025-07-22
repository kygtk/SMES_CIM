// LMP.h : main header file for the LMP application
//

#if !defined(AFX_LMP_H__6C25ACC6_AC9D_42FD_9AF8_C90AE6F002A0__INCLUDED_)
#define AFX_LMP_H__6C25ACC6_AC9D_42FD_9AF8_C90AE6F002A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLMPApp:
// See LMP.cpp for the implementation of this class
//

class CLMPApp : public CWinApp
{
public:
	CLMPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLMPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLMPApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMP_H__6C25ACC6_AC9D_42FD_9AF8_C90AE6F002A0__INCLUDED_)
