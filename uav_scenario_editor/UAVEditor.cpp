
// UAVEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "UAVEditor.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UAVEditorDoc.h"
#include "UAVEditorView.h"

#include <io.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUAVEditorApp

BEGIN_MESSAGE_MAP(CUAVEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CUAVEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CUAVEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CUAVEditorApp::OnFileOpen)
END_MESSAGE_MAP()


// CUAVEditorApp construction

CUAVEditorApp::CUAVEditorApp() : started(false)
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CUAVEditorApp object

CUAVEditorApp theApp;


// CUAVEditorApp initialization

BOOL CUAVEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	/*
	// CREATE CONSOLE
	AllocConsole();
	// setup stdout
	{
		int fd = _open_osfhandle( (long)GetStdHandle( STD_OUTPUT_HANDLE ), 0);
		FILE* fp = _fdopen( fd, "w" );

		*stdout = *fp;
		setvbuf( stdout, NULL, _IONBF, 0 );
	}
	// setup stdin
	{
		int fd = _open_osfhandle( (long)GetStdHandle( STD_INPUT_HANDLE ), 0);
		FILE* fp = _fdopen( fd, "r" );

		*stdin = *fp;
		setvbuf( stdin, NULL, _IONBF, 0 );
	}
	// DONE WITH CONSOLE
	*/


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_UAVEditorTYPE,
		RUNTIME_CLASS(CUAVEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CUAVEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Don't automatically make a new document
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

void CUAVEditorApp::OnFileNew()
{
	if(!started)
	{
		started = true;
		CWinAppEx::OnFileNew();
		return;
	}
	AfxMessageBox(L"This feature has been disabled.");
}

void CUAVEditorApp::OnFileOpen()
{
	/*
	if(!started)
	{
		started = true;
		CWinAppEx::OnFileOpen();
		return;
	}
	*/
	AfxMessageBox(L"This feature has been disabled.");
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CUAVEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUAVEditorApp customization load/save methods

void CUAVEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CUAVEditorApp::LoadCustomState()
{
}

void CUAVEditorApp::SaveCustomState()
{
}

// CUAVEditorApp message handlers



