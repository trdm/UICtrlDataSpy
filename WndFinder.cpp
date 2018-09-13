//
// WndFinder.cpp
//
// Author: Nishad S
// Date:   09-09-2009
//

#include "stdafx.h"
#include "UICtrlDataSpy.h"
#include "WndFinder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT RM_WF_WINDOWFOUND = RegisterWindowMessage( _T( "WndFinder__RM_WF_WINDOWFOUND" ));
UINT RM_WF_ACTIONSTATUS = RegisterWindowMessage( _T( "WndFinder__RM_WF_ACTIONSTATUS" ));

/////////////////////////////////////////////////////////////////////////////
// WndFinder

WndFinder::WndFinder()
: m_bCaptured( false )
, m_hWndOld( 0 )
, m_bFindSmallestTop( false )
{
}

WndFinder::~WndFinder()
{
}

BEGIN_MESSAGE_MAP(WndFinder, CStatic)
	//{{AFX_MSG_MAP(WndFinder)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define IDC_HAND            MAKEINTRESOURCE(32649)
/////////////////////////////////////////////////////////////////////////////
// WndFinder message handlers

BOOL WndFinder::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    static HCURSOR hCursorHand = ::LoadCursor( 0, IDC_HAND );
    ::SetCursor( hCursorHand );
    return TRUE;
}

void WndFinder::OnLButtonDown(UINT nFlags, CPoint point) 
{
    StartFind();
}

void WndFinder::OnLButtonUp(UINT nFlags, CPoint point) 
{
    StopFind();
}

void WndFinder::OnKillFocus(CWnd* pNewWnd) 
{
    StopFind();
}

void WndFinder::OnMouseMove(UINT nFlags, CPoint point) 
{
    ContinueFind( point );
}

//////////////////////////////////////////////////////////////////////////

void WndFinder::FlashWindow( HWND hWnd )
{
    if( !::IsWindow( hWnd ))
    {
        return;
    }

    if( !::IsWindowVisible( hWnd ))
    {
        return;
    }

    for( int nIdx = 0; nIdx < 3; nIdx++ )
    {
        HighlightWindow( hWnd );
        Sleep( 100 );
        ClearHighlight();
        Sleep( 100 );
    }
}

void WndFinder::StartFind()
{
    GetParent()->SendMessage( RM_WF_ACTIONSTATUS, TRUE, 0 );
    SetFocus();
    SetCapture();
    m_hWndOld = 0;
    m_bCaptured = true;
}

void WndFinder::ContinueFind( CPoint& point )
{
    if( m_bCaptured )
    {
        ClientToScreen( &point );
        HWND hWnd = ::WindowFromPoint( point );

        if( 0 == hWnd )
        {
            ClearHighlight();
        }
        else
        {
            // include disabled child windows
            CPoint pointClient( point );
            ::ScreenToClient( hWnd, &pointClient );
            HWND hWndChild = ::ChildWindowFromPoint( hWnd, pointClient );
            if(( 0 != hWndChild ) && ::IsWindowVisible( hWndChild ))
            {
                hWnd = hWndChild;
            }

            // smallest top window search
            if( m_bFindSmallestTop )
            {
                DWORD dwWndArea = MAXDWORD;

                // avoid top level windows
                HWND hWndParent = ::GetParent( hWnd );
                if( 0 != hWndParent )
                {
                    if( 0 == ( ::GetWindowLong( hWnd, GWL_STYLE ) & WS_POPUP ))
                    {
                        // walk through child windows
                        HWND hWndChild = ::GetWindow( hWndParent, GW_CHILD );
                        while( 0 != hWndChild )
                        {
                            // ignore hidden windows
                            if( ::IsWindowVisible( hWndChild ))
                            {
                                // window under the point
                                CRect rect;
                                ::GetWindowRect( hWndChild, &rect );
                                if( rect.PtInRect( point ))
                                {
                                    // find smallest
                                    DWORD dwChildArea = rect.Width() * rect.Height();
                                    if( dwChildArea < dwWndArea )
                                    {
                                        dwWndArea = dwChildArea;
                                        hWnd = hWndChild;
                                    }
                                }
                            }
                            hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT );
                        }
                    }
                }
            }

            // no need to draw again and again
            if( m_hWndOld == hWnd )
            {
                return;
            }

            ClearHighlight();

            HighlightWindow( hWnd );

            GetParent()->SendMessage( RM_WF_WINDOWFOUND, 0, ( LPARAM ) hWnd );
        }
    }
}

void WndFinder::StopFind()
{
    if( m_bCaptured )
    {
        m_bCaptured = false;
        ClearHighlight();
        ReleaseCapture();
        GetParent()->SendMessage( RM_WF_ACTIONSTATUS, FALSE, 0 );
    }
}

void WndFinder::HighlightWindow( HWND hWnd )
{
    CWnd* pWnd = CWnd::FromHandle( hWnd );
    if( 0 == pWnd )
    {
        return;
    }

    // keep the window handle to clear the drawing later
    m_hWndOld = hWnd;

    CWindowDC dc( pWnd );
    COLORREF clr = RGB( 255, 192, 0 );
    // if the window has a region, use it
    CRgn rgn;
    rgn.CreateRectRgn( 0, 0, 0, 0 );
    if( ERROR != pWnd->GetWindowRgn( rgn ))
    {
        CBrush br( clr );
        dc.FrameRgn( &rgn, &br, 3, 3 );
    }
    else
    {
        // no window region, so take the window rect
        CRect rect;
        pWnd->GetWindowRect( &rect );
        rect.OffsetRect( -rect.left, -rect.top );
        for( int nIdx = 0; nIdx < 3; nIdx++ )
        {
            rect.DeflateRect( 1, 1 );
            dc.Draw3dRect( &rect, clr, clr );
        }
    }
}

void WndFinder::ClearHighlight()
{
    if( ::IsWindow( m_hWndOld ))
    {
        ::RedrawWindow( m_hWndOld, 0, 0,
            RDW_FRAME | RDW_ERASE | RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW );
    }

    m_hWndOld = 0;
}
