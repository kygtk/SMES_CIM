// PLC_Version.h: interface for the PLC_Version class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLC_VERSION_H__7CFDF92C_5086_4937_B1CB_71C4EA8140E8__INCLUDED_)
#define AFX_PLC_VERSION_H__7CFDF92C_5086_4937_B1CB_71C4EA8140E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////
#define PLC_VERSION			"1.04"		// 090715 PSK 
///////////////////////////////////////////////////////////////////////////////////

class PLC_Version  
{
public:
// 	PLC_Version(){};
// 	virtual ~PLC_Version(){};

private:
	void V102_20090307();
	void V103_20090522();
	void V104_20090715();
};

#endif // !defined(AFX_PLC_VERSION_H__7CFDF92C_5086_4937_B1CB_71C4EA8140E8__INCLUDED_)
