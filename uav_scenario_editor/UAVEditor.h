
// UAVEditor.h : main header file for the UAVEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CUAVEditorApp:
// See UAVEditor.cpp for the implementation of this class
//

class CUAVEditorApp : public CWinAppEx
{
public:
	CUAVEditorApp();

private:
	bool started;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
};

extern CUAVEditorApp theApp;
