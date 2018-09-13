//
// UICtrlDataSpyDlg.h
//
// Author: Nishad S
// Date:   09-09-2009
//

#if !defined(AFX_UICTRLDATASPYDLG_H__249CC8A3_8160_4BB8_B73F_FE2B42DBC520__INCLUDED_)
#define AFX_UICTRLDATASPYDLG_H__249CC8A3_8160_4BB8_B73F_FE2B42DBC520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndFinder.h"

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyDlg dialog

class UICtrlDataSpyDlg : public CDialog
{
// Construction
public:
	UICtrlDataSpyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(UICtrlDataSpyDlg)
	enum { IDD = IDD_MAIN };
	WndFinder	m_wndfinder;
	BOOL	m_bSmallestTop;
	BOOL	m_bAutoSize;
	BOOL	m_bGetMore;
	CButton	m_btnGetMore;
	CString	m_csHWND;
	CString	m_csClass;
	CString	m_csCaption;
	CString	m_csData;
	CEdit	m_editData;
	BOOL	m_bListInvisible;
	CTreeCtrl	m_treeWnd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UICtrlDataSpyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(UICtrlDataSpyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSmallestTop();
	afx_msg void OnGetData();
	afx_msg void OnCopyData();
	afx_msg void OnFlash();
	afx_msg void OnLocate();
	afx_msg void OnListInvisible();
	afx_msg void OnRefresh();
	afx_msg void OnSelChangedWndTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickWndTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAbout();
	afx_msg void OnClose();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

    void OnOK() {}
    void OnCancel() {}

    LRESULT OnRMWFActionStatus( WPARAM wParam, LPARAM lParam );
    LRESULT OnRMWFWindowFound( WPARAM wParam, LPARAM lParam );

    void GetFullCaptionData();
    void GetListBoxData();
    void GetComboBoxData();
    void GetListViewData();
    void GetHeaderCtrlData();
    void GetTreeViewData();

    void PopulateChildWindows( CWnd* pWndParent, HTREEITEM hTreeItemRoot );
    void FormatWindowInfo( CWnd* pWnd, CString& csInfo );
    HTREEITEM LocateWindow( HTREEITEM hTIRoot, HWND hWnd );
    void SetGetMore( LPCTSTR lpctszStr = 0 );
    void DisplayInvalidWndMsg();
    void DisplayOwnerDrawMsg();
    void DisplayOwnerDataMsg();

private:
    HWND m_hWndCtrl;
    CSize m_sizeNormal;
    CSize m_sizeMinimal;
    CImageList m_ilWndTree;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UICTRLDATASPYDLG_H__249CC8A3_8160_4BB8_B73F_FE2B42DBC520__INCLUDED_)
