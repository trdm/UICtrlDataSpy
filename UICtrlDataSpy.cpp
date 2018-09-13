//
// UICtrlDataSpy.cpp
//
// Author: Nishad S
// Date:   09-09-2009
//

#include "stdafx.h"
#include "UICtrlDataSpy.h"
#include "UICtrlDataSpyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyApp

BEGIN_MESSAGE_MAP(UICtrlDataSpyApp, CWinApp)
	//{{AFX_MSG_MAP(UICtrlDataSpyApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyApp construction

UICtrlDataSpyApp::UICtrlDataSpyApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only UICtrlDataSpyApp object

UICtrlDataSpyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyApp initialization

BOOL UICtrlDataSpyApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	UICtrlDataSpyDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
