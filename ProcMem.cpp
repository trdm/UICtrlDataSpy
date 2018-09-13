//
// ProcMem.cpp
//
// Author: Nishad S
// Date:   09-09-2009
//

#include "stdafx.h"
#include "UICtrlDataSpy.h"
#include "ProcMem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ProcMem::ProcMem( HWND hWnd ) 
: m_hProcess( 0 )
{
    // get the process handle of the given window
    DWORD dwProcID = 0;
    GetWindowThreadProcessId( hWnd, &dwProcID );
    if( 0 != dwProcID )
    {
        m_hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcID );
    }
}

ProcMem::~ProcMem()
{
    // free the memory and close the handle
    if( 0 != m_hProcess )
    {
        // Release the memory
        for( int nIdx = 0; nIdx < m_arrayMemoryAllocated.GetSize(); nIdx++ )
        {
            VirtualFreeEx( m_hProcess, m_arrayMemoryAllocated.GetAt( nIdx ), 0, MEM_RELEASE );
        }

        CloseHandle( m_hProcess );
    }
}

LPVOID ProcMem::Allocate( DWORD dwSize )
{
    LPVOID lpMem = VirtualAllocEx( m_hProcess, 0, dwSize, MEM_COMMIT, PAGE_READWRITE );
    if( 0 != lpMem )
    {
        m_arrayMemoryAllocated.Add( lpMem );
    }

    return lpMem;
}

BOOL ProcMem::Write( LPVOID lpProcMem, LPVOID lpLocalMem, DWORD dwSize )
{
    if(( 0 == lpProcMem ) || ( 0 == lpLocalMem ))
    {
        return FALSE;
    }

    return WriteProcessMemory( m_hProcess, lpProcMem, lpLocalMem, dwSize, 0 );
}

BOOL ProcMem::Read( LPVOID lpProcMem, LPVOID lpLocalMem, DWORD dwSize )
{
    if(( 0 == lpProcMem ) || ( 0 == lpLocalMem ))
    {
        return FALSE;
    }
    
    return ReadProcessMemory( m_hProcess, lpProcMem, lpLocalMem, dwSize, 0 );
}
