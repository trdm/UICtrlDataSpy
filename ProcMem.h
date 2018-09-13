//
// ProcMem.h
//
// Author: Nishad S
// Date:   09-09-2009
//

#if !defined(AFX_PROCMEM_H__06054F1F_9205_4AE4_8D9C_C601D1BDCD4E__INCLUDED_)
#define AFX_PROCMEM_H__06054F1F_9205_4AE4_8D9C_C601D1BDCD4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// utility class for managing process memory
class ProcMem  
{
public:
	ProcMem( HWND hWnd );
	virtual ~ProcMem();

    LPVOID Allocate( DWORD dwSize );
    BOOL Write( LPVOID lpProcMem, LPVOID lpLocalMem, DWORD dwSize );
    BOOL Read( LPVOID lpProcMem, LPVOID lpLocalMem, DWORD dwSize );

private:
    HANDLE m_hProcess;
    CArray<LPVOID, LPVOID> m_arrayMemoryAllocated;
};

#endif // !defined(AFX_PROCMEM_H__06054F1F_9205_4AE4_8D9C_C601D1BDCD4E__INCLUDED_)
