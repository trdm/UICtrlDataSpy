//
// WndFinder.h
//
// Author: Nishad S
// Date:   09-09-2009
//

#if !defined(AFX_WNDFINDER_H__A5162CA0_8068_4B69_BE3B_9779892579FE__INCLUDED_)
#define AFX_WNDFINDER_H__A5162CA0_8068_4B69_BE3B_9779892579FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern UINT RM_WF_WINDOWFOUND;
extern UINT RM_WF_ACTIONSTATUS;

/////////////////////////////////////////////////////////////////////////////
// WndFinder window

class WndFinder : public CStatic
{
// Construction
public:
	WndFinder();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WndFinder)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~WndFinder();

	// Generated message map functions
protected:
	//{{AFX_MSG(WndFinder)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
    void FindSmallestTop( bool bEnable ) { m_bFindSmallestTop = bEnable; }
    void FlashWindow( HWND hWnd );

private:
    void StartFind();
    void ContinueFind( CPoint& point );
    void StopFind();
    void HighlightWindow( HWND hWnd );
    void ClearHighlight();

private:
    bool m_bCaptured;
    HWND m_hWndOld;
    bool m_bFindSmallestTop;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDFINDER_H__A5162CA0_8068_4B69_BE3B_9779892579FE__INCLUDED_)
