
// UAVEditorView.h : interface of the CUAVEditorView class
//


#pragma once
#include "afxmt.h"
#include <vector>
#include "SceneView.h"
#include "NavView.h"

class SimObject;
class UAVObject;
class WaypointObject;

enum E_CURRENT_VIEW {VIEW_SCENE = 0, VIEW_NAVIGATION};

class CUAVEditorView : public CView
{
protected: // create from serialization only
	CUAVEditorView();
	DECLARE_DYNCREATE(CUAVEditorView)

// Attributes
public:
	CUAVEditorDoc* GetDocument() const;

	CCriticalSection renderLock;
	CWinThread * renderThread;
	static UINT RenderThread(LPVOID param);
	bool running;

	// input handlers
	void HandleKeyInput(irr::f32 timeDiff);
	void HandleMouseInput(irr::f32 timeDiff, bool * newMousePos = 0, CPoint * mousePos = 0);
	void HandleBoundaryInput();
	void HandleMoveBoundaries();

	// mouse input
	bool mouseLButtonDown;
	bool mouseMoveValid;
	CPoint mousePoint;
	CPoint mouseLastPoint;

	// keyboard input
	std::vector<bool> keys;

	// temporary (why the hell not)
	bool shiftHeld;		// is shift held this time around?

	// input locks
	CCriticalSection mouseLock;
	CCriticalSection keyLock;

	// easy access to irrlicht
	irr::IrrlichtDevice			 * device;
	irr::video::IVideoDriver	 * driver;
	irr::scene::ISceneManager	 * smgr;
	irr::gui::IGUIEnvironment	 * guienv;
	irr::scene::ICameraSceneNode * camera;

	// render objects
	E_CURRENT_VIEW view;
	SceneView scene;
	NavView nav;

// Operations
public:
	bool CreateDevice();
	void LoadScene();
	void LoadScenario();
	void LoadScenarioFile(const irr::core::stringc &file);
	void ReadSceneNode(irr::io::IXMLReader * reader);

	void ReadUAVNode(irr::io::IXMLReader * reader);
	SimObject * ReadBaseNode(irr::io::IXMLReader * reader, UAVObject * parent = 0);
	WaypointObject * ReadWaypointNode(irr::io::IXMLReader * reader, UAVObject * parent = 0);

// Overrides
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateFrame(UINT nState, CFrameWnd *pFrameWnd);
protected:

// Implementation
public:
	virtual ~CUAVEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPopupShowCityBounds();
	afx_msg void OnPopupShowSectorBounds();
	afx_msg void OnPopupShowBuildingBounds();
	afx_msg void OnPopupJumpToSector();
	afx_msg void OnPopupShowFog();
	afx_msg void OnPopupSwitchToSceneView();
	afx_msg void OnPopupSwitchToNavigationView();
	afx_msg void OnPopupAddUAV();
	afx_msg void OnPopupAddWaypoint();
	afx_msg void OnPopupAddBase();
	afx_msg void OnPopupAddEvent();
	afx_msg void OnPopupAddUAVWaypoint();
	afx_msg void OnPopupJumpToWaypointSector();
	afx_msg void OnPopupJumpToBaseSector();
	afx_msg void OnPopupJumpToUAVSector();
	afx_msg void OnPopupDeleteWaypoint();
	afx_msg void OnPopupDeleteBase();
	afx_msg void OnPopupDeleteUAV();
	afx_msg void OnPopupRemoveScene();
	afx_msg void OnPopupLoadScene();
	afx_msg void OnPopupShowUAVPaths();
	afx_msg void OnPopupJumpToSectorSceneView();
	afx_msg void OnPopupDeleteSceneView();
	afx_msg void OnPopupSnapToSectorCenters();
	afx_msg void OnPopupShowSectorGrid();

private:
	// OnPopup Helpers
	WaypointObject	* GetContextWaypointPointer();
	SimObject		* GetContextBasePointer();
	UAVObject		* GetContextUAVPointer();
};

#ifndef _DEBUG  // debug version in UAVEditorView.cpp
inline CUAVEditorDoc* CUAVEditorView::GetDocument() const
   { return reinterpret_cast<CUAVEditorDoc*>(m_pDocument); }
#endif

