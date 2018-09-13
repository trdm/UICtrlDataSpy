//
// UICtrlDataSpy.h
//
// Author: Nishad S
// Date:   09-09-2009
//

#if !defined(AFX_UICTRLDATASPY_H__30C7D49C_5D3E_4081_A03A_6BB7CC1B1513__INCLUDED_)
#define AFX_UICTRLDATASPY_H__30C7D49C_5D3E_4081_A03A_6BB7CC1B1513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyApp:
// See UICtrlDataSpy.cpp for the implementation of this class
//

class UICtrlDataSpyApp : public CWinApp
{
public:
	UICtrlDataSpyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UICtrlDataSpyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(UICtrlDataSpyApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UICTRLDATASPY_H__30C7D49C_5D3E_4081_A03A_6BB7CC1B1513__INCLUDED_)
