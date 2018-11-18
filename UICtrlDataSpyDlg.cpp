//
// UICtrlDataSpyDlg.cpp
//
// Author: Nishad S
// Date:   09-09-2009
//

#include "stdafx.h"
#include "UICtrlDataSpy.h"
#include "UICtrlDataSpyDlg.h"
#include "ProcMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyDlg dialog

UICtrlDataSpyDlg::UICtrlDataSpyDlg(CWnd* pParent /*=NULL*/)
: CDialog(UICtrlDataSpyDlg::IDD, pParent)
, m_hWndCtrl( 0 )
, m_sizeNormal( 0 )
, m_sizeMinimal( 0 )
{
	//{{AFX_DATA_INIT(UICtrlDataSpyDlg)
	m_bSmallestTop = TRUE;
	m_bAutoSize = TRUE;
	m_bGetMore = FALSE;
	m_csHWND = _T("");
	m_csClass = _T("");
	m_csCaption = _T("");
	m_csData = _T("");
	m_csFindStr =  _T("Ctrl+Shift+Alt");
	m_bListInvisible = FALSE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void UICtrlDataSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(UICtrlDataSpyDlg)
	DDX_Control(pDX, IDC_WNDFINDER, m_wndfinder);
	DDX_Check(pDX, IDC_SMALLESTTOP, m_bSmallestTop);
	DDX_Check(pDX, IDC_AUTOSIZE, m_bAutoSize);
	DDX_Check(pDX, IDC_GETMORE, m_bGetMore);
	DDX_Control(pDX, IDC_GETMORE, m_btnGetMore);
	DDX_Control(pDX, IDC_GETMENUDATA, m_btnGetMenuData);	
	DDX_Text(pDX, IDC_HWND, m_csHWND);
	DDX_Text(pDX, IDC_CLASS, m_csClass);
	DDX_Text(pDX, IDC_CAPTION, m_csCaption);
	DDX_Text(pDX, IDC_DATA, m_csData);
	DDX_Text(pDX, IDC_EDIT_FND, m_csFindStr );	
	DDX_Control(pDX, IDC_DATA, m_editData);
	DDX_Check(pDX, IDC_LISTINVISIBLE, m_bListInvisible);
	DDX_Control(pDX, IDC_WNDTREE, m_treeWnd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(UICtrlDataSpyDlg, CDialog)
	//{{AFX_MSG_MAP(UICtrlDataSpyDlg)
	ON_WM_PAINT()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SMALLESTTOP, OnSmallestTop)
	ON_BN_CLICKED(IDC_GETDATA, OnGetData)
	ON_BN_CLICKED(IDC_GETMENUDATA, OnGetMenuData)	
	ON_BN_CLICKED(IDC_COPYDATA, OnCopyData)
	ON_BN_CLICKED(IDC_BTN_SRCH, onSearche)	
	ON_BN_CLICKED(IDC_FLASH, OnFlash)
	ON_BN_CLICKED(IDC_LOCATE, OnLocate)
	ON_BN_CLICKED(IDC_LISTINVISIBLE, OnListInvisible)
	ON_COMMAND(IDC_REFRESH, OnRefresh)
	ON_NOTIFY(TVN_SELCHANGED, IDC_WNDTREE, OnSelChangedWndTree)
	ON_NOTIFY(NM_RCLICK, IDC_WNDTREE, OnRClickWndTree)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	//}}AFX_MSG_MAP
    ON_REGISTERED_MESSAGE( RM_WF_WINDOWFOUND, OnRMWFWindowFound )
    ON_REGISTERED_MESSAGE( RM_WF_ACTIONSTATUS, OnRMWFActionStatus )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UICtrlDataSpyDlg message handlers

BOOL UICtrlDataSpyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    CRect rect;
    GetClientRect( &rect );
    m_sizeNormal.cx = rect.Width();
    m_sizeNormal.cy = rect.Height();
    m_sizeMinimal.cx = 360; // 391
    m_sizeMinimal.cy = 85; //113
    m_editData.GetWindowRect( &rect );
	m_sizeNormalEdit.cx = rect.Width();
    m_sizeNormalEdit.cy = rect.Height();

    SetGetMore();
    m_editData.SetTabStops( 8 ); // 4 мало
    m_wndfinder.FindSmallestTop( FALSE != m_bSmallestTop );

    m_ilWndTree.Create( 16, 16, ILC_COLOR32, 0, 2 );
    m_ilWndTree.Add( AfxGetApp()->LoadIcon( IDI_CTRLVISIBLE ));
    m_ilWndTree.Add( AfxGetApp()->LoadIcon( IDI_CTRLINVISIBLE ));
    m_treeWnd.SetImageList( &m_ilWndTree, TVSIL_NORMAL );

    m_treeWnd.GetWindowRect( &rect );
	m_sizeNormalTree.cx = rect.Width();
    m_sizeNormalTree.cy = rect.Height();
	

    PostMessage( WM_COMMAND, MAKEWPARAM( IDC_REFRESH, BN_CLICKED ));

	CRect rc;
	CWnd* pWnd;
	m_spliterIsCreate = false; // Начинает 
	if (m_spliterIsCreate) 	{
		pWnd = GetDlgItem(IDC_SPLITTER1);
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		BOOL bRet = m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER1, SPS_VERTICAL|SPS_DELTA_NOTIFY);//, RGB(255, 0, 0));
		if (FALSE == bRet)
		{
			AfxMessageBox(_T("m_wndSplitter1 create failed"));
		}

		//  register windows for splitter
		this->m_wndSplitter1.RegisterLinkedWindow(SPLS_LINKED_LEFT,     GetDlgItem(IDC_WNDTREE));
		this->m_wndSplitter1.RegisterLinkedWindow(SPLS_LINKED_RIGHT,    GetDlgItem(IDC_DATA));
		
		//  relayout the splotter to make them good look
		this->m_wndSplitter1.Relayout();
	} else {
		pWnd = GetDlgItem(IDC_WNDTREE);
		//pWnd->SetWindo
	}	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

int MIN(int x, int y) {
	if (x < y) return x;
	return y;
}

void UICtrlDataSpyDlg::ResizeCtrls(){
	CRect wrct;
	GetWindowRect(wrct);
	int delta = (wrct.bottom-wrct.top) - m_sizeNormal.cy - 27;// размер заголовка
	int deltaX = (wrct.right-wrct.left) - m_sizeNormal.cx;
	int minX = (wrct.right-wrct.left);
	
	RECT R_ed;	m_editData.GetWindowRect(&R_ed);	ScreenToClient(&R_ed);
	m_editData.MoveWindow(R_ed.left, R_ed.top, MIN(m_sizeNormalEdit.cx  +deltaX,minX),  m_sizeNormalEdit.cy  +delta );

	RECT R_tr;
	m_treeWnd.GetWindowRect(&R_tr);	ScreenToClient(&R_tr);
	m_treeWnd.MoveWindow(R_tr.left, R_tr.top, R_tr.right-R_tr.left,  m_sizeNormalTree.cy  +delta ); // Норм.
	if (m_spliterIsCreate)	{
		m_wndSplitter1.GetWindowRect(&R_tr);	ScreenToClient(&R_tr);
		m_wndSplitter1.MoveWindow(R_tr.left, R_tr.top, R_tr.right-R_tr.left,  m_sizeNormalTree.cy  +delta ); 
	}
}

void UICtrlDataSpyDlg::OnSizing(UINT nSide, LPRECT lpRect) {
	CDialog::OnSizing(nSide, lpRect);
	//ResizeCtrls();
	return;

	int delta = (lpRect->bottom-lpRect->top) - m_sizeNormal.cy - 27;// размер заголовка

	RECT R_ed;
	m_editData.GetWindowRect(&R_ed);
	ScreenToClient(&R_ed);
	m_editData.MoveWindow(R_ed.left, R_ed.top, R_ed.right-R_ed.left,  m_sizeNormalEdit.cy  +delta );
}
void UICtrlDataSpyDlg::OnSize(UINT nType, int cx, int cy) {
	CDialog::OnSize(nType, cx, cy);
	if (IsWindowVisible())
		ResizeCtrls();
}

void UICtrlDataSpyDlg::OnPaint() 
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

HCURSOR UICtrlDataSpyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH UICtrlDataSpyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // set the background of read-only edit to white color
    if( CTLCOLOR_STATIC == nCtlColor )
    {
        TCHAR szClassName[32];
        GetClassName( pWnd->m_hWnd, szClassName, 32 );
        if( 0 == lstrcmpi( szClassName, _T( "EDIT" )))
        {
            pDC->SetBkColor( RGB( 255, 255, 255 ));
            hbr = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
        }
    }

	return hbr;
}

void UICtrlDataSpyDlg::OnSmallestTop() 
{
    UpdateData();

    m_wndfinder.FindSmallestTop( FALSE != m_bSmallestTop );
}

// trdm кнопка F3
void UICtrlDataSpyDlg::onSearche(){
	UpdateData();
	CString sData = m_csFindStr;	sData.MakeLower();

	int ln = sData.GetLength();
	int vPos = 0;
	CString lineData, lineDataCpy;
	CStringArray lines;
	if (sData.IsEmpty() || m_csDataCpy.IsEmpty()) {
		if (m_csData == m_csDataCpy)
			return;
		m_csData = m_csDataCpy;
		UpdateData( FALSE );
		return;
	}
	m_csData = "";
	int dLen = m_csDataCpy.GetLength();
	for (int i = 0; i< dLen; i++)	{
		if (m_csDataCpy.Mid(i,1) == _T("\r")){
			if (!lineData.IsEmpty()){
				lineDataCpy = lineData;
				lineDataCpy.MakeLower();
				vPos = lineDataCpy.Find(sData);
				if (vPos >= 0) {
					m_csData += lineData;
					m_csData += _T("\r\n");
				}	
			}
			lineData = "";
		} else {
			if (m_csDataCpy.Mid(i,1) != _T("\n"))
				lineData += m_csDataCpy.Mid(i,1);
		}
	}
	if (!lineData.IsEmpty()){
		lineDataCpy = lineData;
		lineDataCpy.MakeLower();
		vPos = lineDataCpy.Find(sData);
		if (vPos >= 0) {
			m_csData += lineData;
			m_csData += _T("\r\n");
		}	
	}
	UpdateData( FALSE );
}

char* chOutFile[MAX_PATH];

void UICtrlDataSpyDlg::scanMenu(HMENU psHMenu, TCHAR* inputCapt = 0){
	int iCount = ::GetMenuItemCount(psHMenu);
	TCHAR menu_title[256];
	TCHAR str[256];
	char str_2[256];
	CString rusBuf;
	CString tStr = "";
	CString tStr2 = "";
	CString csStr;
	for (int i = 0; i < iCount; i++) {
		UINT uim = ::GetMenuItemID(psHMenu, i);
		int len = ::GetMenuString(psHMenu, i, menu_title, MAX_PATH,MF_BYPOSITION);


		rusBuf = menu_title;
		str[0]='\0';
		//cout << "Menu item uim - " << uim << "title:" << Rus(str) << endl;
		MENUITEMINFO lpmii;
		ZeroMemory(&lpmii, sizeof(lpmii));
		lpmii.cbSize = sizeof(lpmii); //?????!
		lpmii.fMask = MIIM_SUBMENU;     // information to get 
		int res = ::GetMenuItemInfo(psHMenu, i, true, &lpmii);
		tStr2.Empty();
		if (!lpmii.hSubMenu){
			//tStr2.Format(_T("(%d)"), uim);
		}
		tStr.Empty();
		if (inputCapt) {
			tStr = inputCapt;
			tStr += _T("->");
			tStr += rusBuf + tStr2;
			m_csData += tStr;
		} else {
			tStr = rusBuf + tStr2;
			m_csData += tStr;
		}
		m_csData += _T( "\r\n" );

		
		if (lpmii.hSubMenu) {
			int nLen = 0;
			if (inputCapt) {
				nLen = _tcslen(inputCapt);
				if (nLen> 0) nLen += 2;
			}
			nLen += (len+3);
			TCHAR newCap[1000];// = new char(nLen);
			if (inputCapt){
				_tcscpy(newCap,inputCapt);				
				_tcscat(newCap,_T("->"));
				_tcscat(newCap,rusBuf);
			} else {
				_tcscpy(newCap,rusBuf);
			}
			scanMenu(lpmii.hSubMenu, newCap);
		}		
	}	
}

void UICtrlDataSpyDlg::OnGetMenuData() {
    UpdateData();
	
    m_csData.Empty();
	
    if( !::IsWindow( m_hWndCtrl ))
    {
        DisplayInvalidWndMsg();
        return;
    }
	HMENU hMenu = ::GetMenu(m_hWndCtrl);
	if (hMenu != NULL) {
		m_csData.Empty();
		scanMenu(hMenu,NULL);
	}	
	UpdateData( FALSE );
	m_csDataCpy = m_csData;

}

void UICtrlDataSpyDlg::OnGetData() 
{
    UpdateData();

    m_csData.Empty();

    if( !::IsWindow( m_hWndCtrl ))
    {
        DisplayInvalidWndMsg();
        return;
    }

    if( 0 == m_csClass.CompareNoCase( _T( "SysListView32" )))
    {
        GetListViewData();
    }
    else if( 0 == m_csClass.CompareNoCase( _T( "SysHeader32" )))
    {
        GetHeaderCtrlData();
    }
    else if( 0 == m_csClass.CompareNoCase( _T( "SysTreeView32" )))
    {
        GetTreeViewData();
    }
    else if( 0 == m_csClass.CompareNoCase( _T( "ComboBox" )))
    {
        GetComboBoxData();
    }
    else if( 0 == m_csClass.CompareNoCase( _T( "ListBox" )))
    {
        GetListBoxData();
    }
    else
    {
        GetFullCaptionData();
    }

    UpdateData( FALSE );
	m_csDataCpy = m_csData;
}

void UICtrlDataSpyDlg::OnCopyData() 
{
    if( !OpenClipboard())
    {
        AfxMessageBox( _T( "Clipboard operation failed!" ), MB_ICONERROR );
        return;
    }

    EmptyClipboard();

    DWORD dwSize = ( m_csData.GetLength() + 1 ) * sizeof( TCHAR );
    HGLOBAL hData = GlobalAlloc( GMEM_MOVEABLE, dwSize );
    CopyMemory( GlobalLock( hData ), m_csData.operator LPCTSTR(), dwSize );
    GlobalUnlock( hData );
    
    UINT uFmt = ( sizeof( TCHAR ) == sizeof( WCHAR )) ? CF_UNICODETEXT : CF_TEXT;
    if( 0 == ::SetClipboardData( uFmt, hData ))
    {
        AfxMessageBox( _T( "Clipboard operation failed!" ), MB_ICONERROR );
    }  
    
    CloseClipboard();
}

void UICtrlDataSpyDlg::OnFlash() 
{
    m_wndfinder.FlashWindow( m_hWndCtrl );
}

void UICtrlDataSpyDlg::OnLocate() 
{
    if( !::IsWindow( m_hWndCtrl ))
    {
        DisplayInvalidWndMsg();
        return;
    }

    HTREEITEM hTI = LocateWindow( TVI_ROOT, m_hWndCtrl );
    if( 0 != hTI )
    {
        m_treeWnd.Select( hTI, TVGN_CARET );
        m_treeWnd.EnsureVisible( hTI );
    }
    else
    {
        AfxMessageBox( _T( "Cannot find the window!\r\nRefresh the list and try again." ), MB_ICONERROR );
    }
}

void UICtrlDataSpyDlg::OnListInvisible() 
{
    OnRefresh();
}

void UICtrlDataSpyDlg::OnRefresh() 
{
    UpdateData();

    m_treeWnd.SetRedraw( FALSE );
    
    m_treeWnd.DeleteAllItems();
    
    CWnd* pWnd = GetDesktopWindow();
    PopulateChildWindows( pWnd, TVI_ROOT );
    
    HTREEITEM hTreeItem = m_treeWnd.GetRootItem();
    if( 0 != hTreeItem )
    {
        m_treeWnd.Expand( hTreeItem, TVE_EXPAND );
    }
    
    m_treeWnd.SetRedraw( TRUE );
}

void UICtrlDataSpyDlg::OnSelChangedWndTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

    if( 0 == pNMTreeView )
    {
        return;
    }

    if( 0 == pNMTreeView->itemNew.hItem )
    {
        return;
    }

    HWND hWnd = ( HWND ) m_treeWnd.GetItemData( pNMTreeView->itemNew.hItem );
    OnRMWFWindowFound( 0, ( LPARAM ) hWnd );
}

void UICtrlDataSpyDlg::OnRClickWndTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

    CPoint pointScreen( ::GetMessagePos());
    CPoint pointClient( pointScreen );
    m_treeWnd.ScreenToClient( &pointClient );

    UINT uFlgs = 0;
    HTREEITEM hTISel = m_treeWnd.HitTest( pointClient, &uFlgs );
    if( uFlgs & ( TVHT_ONITEMINDENT | TVHT_ONITEMBUTTON | TVHT_ONITEM | TVHT_ONITEMRIGHT ))
    {
        HWND hWnd = ( HWND ) m_treeWnd.GetItemData( hTISel );
        CMenu menu;
        menu.LoadMenu( IDR_CONTEXT );
        CMenu* pMenuWT = menu.GetSubMenu( 0 );
        pMenuWT->EnableMenuItem( ID_WNDTREE_FLASH, MF_BYCOMMAND | ( ::IsWindowVisible( hWnd ) ? MF_ENABLED : MF_GRAYED ));
        
        UINT uCmd = pMenuWT->TrackPopupMenu( TPM_RETURNCMD, pointScreen.x, pointScreen.y, this );

        if( ID_WNDTREE_FLASH == uCmd )
        {
            m_wndfinder.FlashWindow( hWnd );
        }
    }
}

void UICtrlDataSpyDlg::OnAbout() 
{
    CDialog dlg( IDD_ABOUT, this );
    dlg.DoModal();
}

void UICtrlDataSpyDlg::OnClose() 
{
    OnExit();
}

void UICtrlDataSpyDlg::OnExit() 
{
    EndDialog( 0 );
}

//////////////////////////////////////////////////////////////////////////

LRESULT UICtrlDataSpyDlg::OnRMWFActionStatus( WPARAM wParam, LPARAM lParam )
{
    UpdateData();

    if( m_bAutoSize )
    {
        if( 0 != wParam )
        {
            CRect rect( 0, 0, m_sizeMinimal.cx, m_sizeMinimal.cy );
            AdjustWindowRectEx( &rect, GetStyle(), FALSE, GetExStyle());
            SetWindowPos( 0, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE );
        }
        else
        {
            CRect rect( 0, 0, m_sizeNormal.cx, m_sizeNormal.cy );
            AdjustWindowRectEx( &rect, GetStyle(), FALSE, GetExStyle());
            SetWindowPos( 0, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE );
        }
    }

    return 0;
}

LRESULT UICtrlDataSpyDlg::OnRMWFWindowFound( WPARAM wParam, LPARAM lParam )
{
    UpdateData();

    HWND hWnd = ( HWND ) lParam;

    if( !::IsWindow( hWnd ))
    {
        m_hWndCtrl = 0;
        m_csHWND = _T( "<Invalid>" );
        m_csCaption.Empty();
        m_csClass.Empty();
        SetGetMore();
        m_csData.Empty();
        UpdateData( FALSE );
        return 0;
    }

    if( hWnd == m_hWndCtrl )
    {
        return 0;
    }

    // window handle
    m_hWndCtrl = hWnd;
    m_csHWND.Format( _T( "0x%08lX" ), hWnd );

    // caption
    DWORD dwResult = 0;
    SendMessageTimeout( hWnd, WM_GETTEXT, 256, ( LPARAM ) m_csCaption.GetBuffer( 256 ),
        SMTO_ABORTIFHUNG, 100, &dwResult );
    m_csCaption.ReleaseBuffer();
    
    // class name
    ::GetClassName( hWnd, m_csClass.GetBuffer( 256 ), 256 );
    m_csClass.ReleaseBuffer();

    if( 0 == m_csClass.CompareNoCase( _T( "SysListView32" )))
    {
        SetGetMore( _T( "Include subitems" ));
    }
    else if( 0 == m_csClass.CompareNoCase( _T( "SysTreeView32" )))
    {
        SetGetMore( _T( "Include collapsed items" ));
    }
    else
    {
        SetGetMore();
    }
	m_btnGetMenuData.EnableWindow( FALSE );
	HMENU hMenu = ::GetMenu(hWnd);
	if (hMenu != NULL) {
		int iCount = ::GetMenuItemCount(hMenu);
		if (iCount > 0){
			m_btnGetMenuData.EnableWindow( TRUE );
		}
	}	


    // data
    m_csData.Empty();

    UpdateData( FALSE );
    return 0;
}

//////////////////////////////////////////////////////////////////////////

void UICtrlDataSpyDlg::GetFullCaptionData()
{
    int nLen = ::SendMessage( m_hWndCtrl, WM_GETTEXTLENGTH, 0, 0 );
    if( nLen <= 0 )
    {
        return;
    }
    
    LPTSTR lptszStr = m_csData.GetBuffer( nLen + 1 );
    if( 0 == lptszStr )
    {
        return;
    }
    
    DWORD dwResult = 0;
    ::SendMessage( m_hWndCtrl, WM_GETTEXT, nLen + 1, ( LPARAM ) lptszStr );
    m_csData.ReleaseBuffer();
}

void UICtrlDataSpyDlg::GetListBoxData()
{
    DWORD dwStyles = ::GetWindowLong( m_hWndCtrl, GWL_STYLE );
    if(( dwStyles & LBS_OWNERDRAWFIXED ) || ( dwStyles & LBS_OWNERDRAWVARIABLE ))
    {
        DisplayOwnerDrawMsg();
    }
    
    CString csStr;
    int nItemCount = ::SendMessage( m_hWndCtrl, LB_GETCOUNT, 0, 0 );
    for( int nIdx = 0; nIdx < nItemCount; nIdx++ )
    {
        int nLen = ::SendMessage( m_hWndCtrl, LB_GETTEXTLEN, nIdx, 0 );
        LPTSTR lptszStr = csStr.GetBuffer( nLen );
        ::SendMessage( m_hWndCtrl, LB_GETTEXT, nIdx, ( LPARAM ) lptszStr );
        m_csData += lptszStr;
        m_csData += _T( "\r\n" );
    }
}

void UICtrlDataSpyDlg::GetComboBoxData()
{
    DWORD dwStyles = ::GetWindowLong( m_hWndCtrl, GWL_STYLE );
    if(( dwStyles & CBS_OWNERDRAWFIXED ) || ( dwStyles & CBS_OWNERDRAWVARIABLE ))
    {
        DisplayOwnerDrawMsg();
    }

    CString csStr;
    int nItemCount = ::SendMessage( m_hWndCtrl, CB_GETCOUNT, 0, 0 );
    for( int nIdx = 0; nIdx < nItemCount; nIdx++ )
    {
        int nLen = ::SendMessage( m_hWndCtrl, CB_GETLBTEXTLEN, nIdx, 0 );
        LPTSTR lptszStr = csStr.GetBuffer( nLen );
        ::SendMessage( m_hWndCtrl, CB_GETLBTEXT, nIdx, ( LPARAM ) lptszStr );
        m_csData += lptszStr;
        m_csData += _T( "\r\n" );
    }
}

void UICtrlDataSpyDlg::GetListViewData()
{
    DWORD dwStyles = ::GetWindowLong( m_hWndCtrl, GWL_STYLE );
    if( dwStyles & LVS_OWNERDATA )
    {
        DisplayOwnerDataMsg();
        return;
    }

    if( dwStyles & LVS_OWNERDRAWFIXED )
    {
        DisplayOwnerDrawMsg();
    }

    int nItemCount = ::SendMessage( m_hWndCtrl, LVM_GETITEMCOUNT, 0, 0 );
    if( nItemCount <= 0 )
    {
        return;
    }
    
    int nSubItemCount = 1;
    if( m_bGetMore )
    {
        HWND hWndHdr = ( HWND )::SendMessage( m_hWndCtrl, LVM_GETHEADER, 0, 0 );
        if( ::IsWindow( hWndHdr ))
        {
            nSubItemCount = ::SendMessage( hWndHdr, HDM_GETITEMCOUNT, 0, 0 );
        }
    }
    
    ProcMem procMem( m_hWndCtrl );
    
    const int STR_SIZE = 1024;
    LV_ITEM* pstLVI = ( LV_ITEM* ) procMem.Allocate( sizeof( LV_ITEM ));
    LPTSTR lptszStr = ( LPTSTR ) procMem.Allocate( STR_SIZE * sizeof( TCHAR ));
    LV_ITEM stLVI = { 0 };
    stLVI.cchTextMax = STR_SIZE;
    stLVI.pszText = lptszStr;
    TCHAR tszStr[STR_SIZE];

    for( int nIdx = 0; nIdx < nItemCount; nIdx++ )
    {
        for( int nSubIdx = 0; nSubIdx < nSubItemCount; nSubIdx++ )
        {
            stLVI.iSubItem = nSubIdx;
            procMem.Write( pstLVI, &stLVI, sizeof( LV_ITEM ));
            ::SendMessage( m_hWndCtrl, LVM_GETITEMTEXT, nIdx, ( LPARAM ) pstLVI );
            procMem.Read( lptszStr, tszStr, STR_SIZE * sizeof( TCHAR ));
            if( nSubIdx > 0 )
            {
                m_csData += _T( "\t" );
            }
            m_csData += tszStr;
        }
        m_csData += _T( "\r\n" );
    }
}

void UICtrlDataSpyDlg::GetHeaderCtrlData()
{
    int nItemCount = ::SendMessage( m_hWndCtrl, HDM_GETITEMCOUNT, 0, 0 );
    if( nItemCount <= 0 )
    {
        return;
    }

    ProcMem procMem( m_hWndCtrl );
    
    const int STR_SIZE = 1024;
    HDITEM* pstHDI = ( HDITEM* ) procMem.Allocate( sizeof( HDITEM ));
    LPTSTR lptszStr = ( LPTSTR ) procMem.Allocate( STR_SIZE * sizeof( TCHAR ));
    HDITEM stHDI = { 0 };
    stHDI.mask = HDI_TEXT;
    stHDI.cchTextMax = STR_SIZE;
    stHDI.pszText = lptszStr;
    TCHAR tszStr[STR_SIZE];
    
    for( int nIdx = 0; nIdx < nItemCount; nIdx++ )
    {
        procMem.Write( pstHDI, &stHDI, sizeof( HDITEM ));
        ::SendMessage( m_hWndCtrl, HDM_GETITEM, nIdx, ( LPARAM ) pstHDI );
        procMem.Read( lptszStr, tszStr, STR_SIZE * sizeof( TCHAR ));
        m_csData += tszStr;
        m_csData += _T( "\r\n" );
    }
}
// trdm 2018-09-13 06:25:23 
#define TVM_GETITEMSTATE (TV_FIRST + 39)

void UICtrlDataSpyDlg::GetTreeViewData()
{
    ProcMem procMem( m_hWndCtrl );
    
    const int STR_SIZE = 1024;
    TVITEM* pstTVI = ( TVITEM* ) procMem.Allocate( sizeof( TVITEM ));
    LPTSTR lptszStr = ( LPTSTR ) procMem.Allocate( STR_SIZE * sizeof( TCHAR ));
    TVITEM stTVI = { 0 };
    stTVI.mask = TVIF_TEXT;
    stTVI.pszText = lptszStr;
    stTVI.cchTextMax = STR_SIZE;
    TCHAR tszStr[STR_SIZE];

    const int STACK_SIZE = 1024;
    HTREEITEM hTIStack[STACK_SIZE] = { 0 };
    int nStackIdx = -1;

    //hTIStack[nStackIdx] = 0;
    HTREEITEM hTICur = ( HTREEITEM ) ::SendMessage( m_hWndCtrl, TVM_GETNEXTITEM, TVGN_ROOT, 0 );
    while( 0 != hTICur )
    {
        stTVI.hItem = hTICur;
        procMem.Write( pstTVI, &stTVI, sizeof( TVITEM ));
        ::SendMessage( m_hWndCtrl, TVM_GETITEM, 0, ( LPARAM ) pstTVI );
        procMem.Read( lptszStr, tszStr, STR_SIZE * sizeof( TCHAR ));
        for( int nIdx = 0; nIdx <= nStackIdx; nIdx++ )
        {
            m_csData += _T( "\t" );
        }
        m_csData += tszStr;
        m_csData += _T( "\r\n" );

        bool bIncludeChildren = true;
        if( !m_bGetMore )
        {
            UINT uState = ::SendMessage( m_hWndCtrl, TVM_GETITEMSTATE, ( WPARAM ) hTICur, TVIS_EXPANDED );
            if( 0 == ( uState & TVIS_EXPANDED ))
            {
                bIncludeChildren = false;
            }
			//TreeView_GetItemState
        }

        HTREEITEM hTIChild = 0;
        if( bIncludeChildren &&
          ( 0 != ( hTIChild = ( HTREEITEM ) ::SendMessage( m_hWndCtrl, TVM_GETNEXTITEM, TVGN_CHILD, ( LPARAM ) hTICur ))))
        {
            nStackIdx++;
            if( nStackIdx >= STACK_SIZE )
            {
                AfxMessageBox( _T( "Stack overfloaw!\r\nAborting..." ), MB_ICONERROR );
                return;
            }
            hTIStack[nStackIdx] = hTICur;
            hTICur = hTIChild;
        }
        else
        {
            while( 0 != hTICur )
            {
                HTREEITEM hTINext = ( HTREEITEM ) ::SendMessage(
                    m_hWndCtrl, TVM_GETNEXTITEM, TVGN_NEXT, ( LPARAM ) hTICur );
                if( 0 == hTINext )
                {
                    if( nStackIdx < 0 )
                    {
                        hTICur = 0;
                        break;
                    }

                    hTICur = hTIStack[nStackIdx];
                    nStackIdx--;
                }
                else
                {
                    hTICur = hTINext;
                    break;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void UICtrlDataSpyDlg::PopulateChildWindows( CWnd* pWndParent, HTREEITEM hTreeItemRoot )
{
    if(( 0 == pWndParent ) || ( 0 == hTreeItemRoot ))
    {
        return;
    }

    BOOL bIsWindowVisible = pWndParent->IsWindowVisible();
    if( !m_bListInvisible && !bIsWindowVisible )
    {
        return;
    }

    CString csWndInfo;
    FormatWindowInfo( pWndParent, csWndInfo );

    int nImg =  bIsWindowVisible ? 0 : 1;
    HTREEITEM hTreeItem = m_treeWnd.InsertItem( csWndInfo, nImg, nImg, hTreeItemRoot, TVI_LAST );
    m_treeWnd.SetItemData( hTreeItem, reinterpret_cast<DWORD>( pWndParent->m_hWnd ));

    CWnd* pWnd = pWndParent->GetNextWindow( GW_CHILD );
    while( 0 != pWnd )
    {
        PopulateChildWindows( pWnd, hTreeItem );
        pWnd = pWnd->GetNextWindow( GW_HWNDNEXT );
    }
}

void UICtrlDataSpyDlg::FormatWindowInfo( CWnd* pWnd, CString& csInfo )
{
    // caption
    CString csCaption;
    DWORD dwResult = 0;
    SendMessageTimeout( pWnd->m_hWnd, WM_GETTEXT, 256, ( LPARAM ) csCaption.GetBuffer( 256 ),
        SMTO_ABORTIFHUNG, 100, &dwResult );
    csCaption.ReleaseBuffer();

    // class name
    TCHAR szClassName[256];
    GetClassName( pWnd->m_hWnd, szClassName, 256 );

    // special classes
    CString csClassExtra;
    if( 0 == lstrcmpi( szClassName, _T( "#32770" )))
    {
        csClassExtra = _T( " (Dialog)" );
    }
    else if( 0 == lstrcmpi( szClassName, _T( "#32769" )))
    {
        csClassExtra = _T( " (Desktop)" );
    }

    csInfo.Format( _T( "%08X [%s%s] \"%s\"" ), pWnd->m_hWnd, szClassName,
        csClassExtra.operator LPCTSTR(), csCaption.operator LPCTSTR() );
}

HTREEITEM UICtrlDataSpyDlg::LocateWindow( HTREEITEM hTIRoot, HWND hWnd )
{
    HTREEITEM hTIChild = m_treeWnd.GetChildItem( hTIRoot );
    while( 0 != hTIChild )
    {
        if( m_treeWnd.GetItemData( hTIChild ) == ( DWORD ) hWnd )
        {
            return hTIChild;
        }

        HTREEITEM hTILoc = LocateWindow( hTIChild, hWnd );
        if( 0 != hTILoc )
        {
            m_treeWnd.Expand( hTIChild, TVE_EXPAND );
            return hTILoc;
        }

        hTIChild = m_treeWnd.GetNextSiblingItem( hTIChild );
    }

    return 0;
}

void UICtrlDataSpyDlg::SetGetMore( LPCTSTR lpctszStr )
{
    if( 0 == lpctszStr )
    {
        m_btnGetMore.EnableWindow( FALSE );		
        m_btnGetMore.SetWindowText( _T( "More data..." ));
    }
    else
    {
        m_btnGetMore.EnableWindow( TRUE );
        m_btnGetMore.SetWindowText( lpctszStr );
    }
	m_btnGetMenuData.EnableWindow( FALSE );
}

void UICtrlDataSpyDlg::DisplayInvalidWndMsg()
{
    AfxMessageBox( _T( "Invalid window handle!" ), MB_ICONERROR );
}

void UICtrlDataSpyDlg::DisplayOwnerDrawMsg()
{
    AfxMessageBox( _T( "This control is having OwnerDraw style!\r\n" )
        _T( "So the contents displayed in the control may differ from captured data." ),
        MB_ICONWARNING );
}

void UICtrlDataSpyDlg::DisplayOwnerDataMsg()
{
    AfxMessageBox( _T( "This control is having OwnerData style!\r\n" )
        _T( "So the contents cannot be captured." ),
        MB_ICONWARNING );
}
