
// UAVEditorView.cpp : implementation of the CUAVEditorView class
//

#include "stdafx.h"
#include "UAVEditor.h"

#include "UAVEditorDoc.h"
#include "UAVEditorView.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "CityScene.h"
#include "InputGlobals.h"
#include "SceneGlobals.h"

#include "JumpSectorDialog.h"

#pragma warning(disable : 4996)

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const UINT KEY_Q		= 81;
const UINT KEY_E		= 69;
const UINT KEY_W		= 87;
const UINT KEY_S		= 83;
const UINT KEY_A		= 65;
const UINT KEY_D		= 68;
const UINT KEY_SHIFT	= 16;
const UINT KEY_P		= 80;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUAVEditorView

IMPLEMENT_DYNCREATE(CUAVEditorView, CView)

BEGIN_MESSAGE_MAP(CUAVEditorView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_POPUP_SHOWCITYBOUNDS, &CUAVEditorView::OnPopupShowCityBounds)
	ON_COMMAND(ID_POPUP_SHOWSECTORBOUNDS, &CUAVEditorView::OnPopupShowSectorBounds)
	ON_COMMAND(ID_POPUP_SHOWBUILDINGBOUNDS, &CUAVEditorView::OnPopupShowBuildingBounds)
	ON_COMMAND(ID_POPUP_JUMPTOSECTOR, &CUAVEditorView::OnPopupJumpToSector)
	ON_COMMAND(ID_POPUP_SHOWFOG, &CUAVEditorView::OnPopupShowFog)
	ON_COMMAND(ID_POPUP_SWITCHTO, &CUAVEditorView::OnPopupSwitchToSceneView)
	ON_COMMAND(ID_POPUP_SWITCHTONAVIGATIONVIEW, &CUAVEditorView::OnPopupSwitchToNavigationView)
	ON_COMMAND(ID_POPUP_ADDUAV, &CUAVEditorView::OnPopupAddUAV)
	ON_COMMAND(ID_POPUP_ADDWAYPOINT32792, &CUAVEditorView::OnPopupAddWaypoint)
	ON_COMMAND(ID_POPUP_ADDBASE, &CUAVEditorView::OnPopupAddBase)
	ON_COMMAND(ID_POPUP_ADDEVENT, &CUAVEditorView::OnPopupAddEvent)
	ON_COMMAND(ID_POPUP_DELETE_WAYPOINT, &CUAVEditorView::OnPopupDeleteWaypoint)
	ON_COMMAND(ID_POPUP_JUMP_TO_WAYPOINT_SECTOR, &CUAVEditorView::OnPopupJumpToWaypointSector)
	ON_COMMAND(ID_POPUP_DELETE_BASE, &CUAVEditorView::OnPopupDeleteBase)
	ON_COMMAND(ID_POPUP_JUMP_TO_BASE_SECTOR, &CUAVEditorView::OnPopupJumpToBaseSector)
	ON_COMMAND(ID_POPUP_JUMP_TO_UAV_SECTOR, &CUAVEditorView::OnPopupJumpToUAVSector)
	ON_COMMAND(ID_POPUP_DELETE_UAV, &CUAVEditorView::OnPopupDeleteUAV)
	ON_COMMAND(ID_POPUP_ADD_UAV_WAYPOINT, &CUAVEditorView::OnPopupAddUAVWaypoint)
	ON_COMMAND(ID_POPUP_REMOVE_SCENE, &CUAVEditorView::OnPopupRemoveScene)
	ON_COMMAND(ID_POPUP_DELETE_SCENE_VIEW, &CUAVEditorView::OnPopupDeleteSceneView)
	ON_COMMAND(ID_POPUP_LOAD_SCENE, &CUAVEditorView::OnPopupLoadScene)
	ON_COMMAND(ID_POPUP_SHOW_UAV_PATHS, &CUAVEditorView::OnPopupShowUAVPaths)
	ON_COMMAND(ID_POPUP_JUMP_TO_SECTOR_SCENE_VIEW, &CUAVEditorView::OnPopupJumpToSectorSceneView)
	ON_COMMAND(ID_POPUP_SNAP_TO_SECTOR_CENTERS, &CUAVEditorView::OnPopupSnapToSectorCenters)
	ON_COMMAND(ID_POPUP_SHOW_SECTOR_GRID, &CUAVEditorView::OnPopupShowSectorGrid)
END_MESSAGE_MAP()

// CUAVEditorView construction/destruction
CUAVEditorView::CUAVEditorView()
{
	device = 0;
	renderThread = 0;
	view = VIEW_SCENE;

	running = false;

	mouseMoveValid = false;
	mouseLButtonDown = false;

	keys.resize(100);
	for(unsigned int i=0; i<keys.size(); i++)
		keys[i] = false;
}
CUAVEditorView::~CUAVEditorView()
{
}

BOOL CUAVEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CUAVEditorView::OnActivateFrame(UINT nState, CFrameWnd*) {
	if(nState == WA_INACTIVE)
		ReleaseCapture();
}

UINT CUAVEditorView::RenderThread(LPVOID param) {
	CUAVEditorView *pView = (CUAVEditorView*)param;
	COutputWnd &output = ((CMainFrame*)AfxGetMainWnd())->GetOutputWindow();

	pView->renderLock.Lock();
	pView->CreateDevice();
	if(!pView->device)
	{
		pView->running = false;
		AfxMessageBox(L"Failed to create device");
		AfxEndThread(0);
		return 0;
	}
	pView->LoadScene();

	// Check to see if the user wants to load a scenario
	int response = AfxMessageBox(L"Load a scenario file?", MB_YESNO|MB_ICONQUESTION);
	if(response == IDYES)
		pView->LoadScenario();
	pView->renderLock.Unlock();

	// Mouse position information
	bool newMousePos = false;
	CPoint mousePos;

	// Set up timekeeping
	DWORD lastTime, syncStart, frames = 0;
	lastTime = GetTickCount();

	// Run the device loop
	while(pView->running)
	{
		pView->renderLock.Lock();

		// Get the timestep 
		syncStart = GetTickCount();
		f32 timeDiff = (f32)(GetTickCount() - lastTime);
		lastTime = GetTickCount();

		// Handle input
		pView->HandleBoundaryInput();
		pView->HandleMouseInput(timeDiff, &newMousePos, &mousePos);
		pView->HandleKeyInput(timeDiff);
		pView->HandleMoveBoundaries();

		// Render
		pView->GetDocument()->dataLock.Lock();
		if(pView->view == VIEW_SCENE)
			pView->scene.Render(newMousePos);
		else if(pView->view == VIEW_NAVIGATION)
			pView->nav.Render(newMousePos);
		pView->GetDocument()->dataLock.Unlock();

		pView->renderLock.Unlock();
		Sleep(1);
	}

	pView->device->closeDevice();
	pView->device->drop();

	AfxEndThread(0);
    return 0;
}

void CUAVEditorView::HandleKeyInput(irr::f32 timeDiff)
{
	keyLock.Lock();

	if(view == VIEW_SCENE)
		scene.HandleKeyInput(timeDiff, keys);

	keyLock.Unlock();
}

void CUAVEditorView::HandleMouseInput(irr::f32 timeDiff, bool * newMousePos, CPoint * mousePos)
{
	mouseLock.Lock();

	// Save out the mouse position
	if(mouseMoveValid && newMousePos && mousePos)
	{
		*mousePos = mousePoint;
		*newMousePos = true;
	}

	if(mouseMoveValid)
	{
		// Find the relative mouse movement
		CPoint relMousePos;
		relMousePos.x = (mousePoint.x - mouseLastPoint.x);
		relMousePos.y = (mousePoint.y - mouseLastPoint.y);

		// The scene view only handles mouse input when the right mouse
		// button is held down
		if(mouseLButtonDown && view == VIEW_SCENE)
			scene.HandleMouseInput(timeDiff, relMousePos);

		// The navigation view always handles mouse input
		if(view == VIEW_NAVIGATION)
		{
			GetDocument()->dataLock.Lock();
			nav.HandleMouseInput(*mousePos, relMousePos, mouseLButtonDown);
			GetDocument()->dataLock.Unlock();
		}
	}

	mouseMoveValid = false;
	mouseLock.Unlock();
}

void CUAVEditorView::HandleBoundaryInput()
{
	if(view == VIEW_SCENE)
		scene.HandleBoundaryInput();

	// See if shift is held down
	shiftHeld = false;
	keyLock.Lock();
	if(keys[::KEY_SHIFT])
		shiftHeld = true;
	keyLock.Unlock();
}

void CUAVEditorView::HandleMoveBoundaries()
{
	if(view == VIEW_SCENE)
		scene.HandleMoveBoundaries(shiftHeld);
}

void CUAVEditorView::OnInitialUpdate()
{
}

// CUAVEditorView drawing
void CUAVEditorView::OnDraw(CDC* /*pDC*/)
{
	if((!running) && (!renderThread))
	{
		running = true;
		renderThread = AfxBeginThread((AFX_THREADPROC)&CUAVEditorView::RenderThread, this);
	}
}

BOOL CUAVEditorView::OnEraseBkgnd(CDC *pDC) {
       return TRUE;
}

int CUAVEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if(CView::OnCreate(lpCreateStruct) == -1) return -1;
	return 0;
}

void CUAVEditorView::OnDestroy() {
	renderLock.Lock();
	running = false;
	renderLock.Unlock();
	CView::OnDestroy();
}

void CUAVEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
}

void CUAVEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	keyLock.Lock();
	if(nChar >= 0 && nChar < keys.size())
	{
		keys[nChar] = true;

		// Don't let the WSAD interface lock if opposite keys are pressed;
		// go with the one that was pressed last
		if(nChar == KEY_W)	keys[KEY_S] = false;
		if(nChar == KEY_S)	keys[KEY_W] = false;
		if(nChar == KEY_A)	keys[KEY_D] = false;
		if(nChar == KEY_D)	keys[KEY_A] = false;
	}
	keyLock.Unlock();
}

void CUAVEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	keyLock.Lock();
	if(nChar >= 0 && nChar < keys.size())
		keys[nChar] = false;
	keyLock.Unlock();
}

void CUAVEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	mouseLock.Lock();
	mouseMoveValid = true;
	mouseLastPoint = mousePoint;
	mousePoint = point;
	mouseLock.Unlock();
	CView::OnMouseMove(nFlags, point);
}

void CUAVEditorView::OnLButtonDown(UINT nFlags, CPoint point) {
	mouseLock.Lock();
	mouseLButtonDown = true;
	mouseLock.Unlock();
	SetCapture();
}

void CUAVEditorView::OnLButtonUp(UINT nFlags, CPoint point) {
	mouseLock.Lock();
	mouseLButtonDown = false;
	mouseLock.Unlock();
	ReleaseCapture();
}

void CUAVEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUAVEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// Disabe key input movement
	keyLock.Lock();
	for(unsigned int i=0; i<keys.size(); i++)
		keys[i] = false;
	keyLock.Unlock();

	// Disable mouse movement
	mouseLock.Lock();
	mouseLButtonDown = false;
	mouseMoveValid = false;
	mouseLock.Unlock();

	// Create the menu
	CMenu menu;

	if(view == VIEW_SCENE)
	{
		menu.LoadMenuW(IDR_SCENE_CONTEXT);
		CMenu * cMenu = menu.GetSubMenu(0);

		// Disable loading and removing scenes
		if(!(scene.showSelectedSector && scene.IsCurrentSceneLoaded()))
			cMenu->EnableMenuItem(3, MF_DISABLED | MF_BYPOSITION);
		if(!(scene.showSelectedSector && !scene.IsCurrentSceneLoaded()))
			cMenu->EnableMenuItem(2, MF_DISABLED | MF_BYPOSITION);

		// Set the boundary checks
		if(scene.showCityBounds)	cMenu->CheckMenuItem(5, MF_CHECKED | MF_BYPOSITION);
		else						cMenu->CheckMenuItem(5, MF_UNCHECKED | MF_BYPOSITION);
		if(scene.showSectorBounds)	cMenu->CheckMenuItem(6, MF_CHECKED | MF_BYPOSITION);
		else						cMenu->CheckMenuItem(6, MF_UNCHECKED | MF_BYPOSITION);
		if(scene.showBuildingBounds)cMenu->CheckMenuItem(7, MF_CHECKED | MF_BYPOSITION);
		else						cMenu->CheckMenuItem(7, MF_UNCHECKED | MF_BYPOSITION);
		if(scene.enableFog)			cMenu->CheckMenuItem(8, MF_CHECKED | MF_BYPOSITION);
		else						cMenu->CheckMenuItem(8, MF_UNCHECKED | MF_BYPOSITION);

		cMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	else if(view == VIEW_NAVIGATION)
	{
		mouseLock.Lock();
		nav.SetContextMousePosition(position2di(mousePoint.x, mousePoint.y));
		mouseLock.Unlock();

		E_NAV_CONTEXT_TYPE nType = nav.GetContextType();
		switch(nType)
		{
		case ENC_UAV:
			menu.LoadMenuW(IDR_UAV_CONTEXT);
			break;

		case ENC_BASE:
			menu.LoadMenuW(IDR_BASE_CONTEXT);
			break;

		case ENC_WP:
			menu.LoadMenuW(IDR_WAYPOINT_CONTEXT);
			break;

		case ENC_GENERAL:
		default:
			menu.LoadMenuW(IDR_NAV_CONTEXT);
		}

		CMenu * cMenu = menu.GetSubMenu(0);

		if(nType == ENC_GENERAL)
		{
			if(nav.showPaths)	cMenu->CheckMenuItem(7, MF_CHECKED | MF_BYPOSITION);
			else				cMenu->CheckMenuItem(7, MF_UNCHECKED | MF_BYPOSITION);
			if(nav.showGrid)	cMenu->CheckMenuItem(8, MF_CHECKED | MF_BYPOSITION);
			else				cMenu->CheckMenuItem(8, MF_UNCHECKED | MF_BYPOSITION);
			if(nav.snapToSectorCenters)	cMenu->CheckMenuItem(9, MF_CHECKED | MF_BYPOSITION);
			else						cMenu->CheckMenuItem(9, MF_UNCHECKED | MF_BYPOSITION);
		}

		cMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}


// CUAVEditorView diagnostics

#ifdef _DEBUG
void CUAVEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CUAVEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUAVEditorDoc* CUAVEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUAVEditorDoc)));
	return (CUAVEditorDoc*)m_pDocument;
}
#endif //_DEBUG

bool CUAVEditorView::CreateDevice()
{
	// Create the irrlict device
	try
	{
		SIrrlichtCreationParameters param;
		param.WindowId = (void*)m_hWnd;
		param.DriverType = video::EDT_DIRECT3D9;
	
		device = createDeviceEx(param);
		if(!device)
			return false;
		device->run();
	}
	catch(...)
	{
		return false;
	}

	// Link all the managers for ease of use later
	//device->setResizeAble(true);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	return true;
}

void CUAVEditorView::LoadScene()
{
	// Create a skybox
	irr::scene::ISceneNode * skybox = smgr->addSkyBoxSceneNode(
	driver->getTexture("../media/sb/irrlicht2_up.png"),
	driver->getTexture("../media/sb/irrlicht2_dn.png"),
	driver->getTexture("../media/sb/irrlicht2_lf.png"),
	driver->getTexture("../media/sb/irrlicht2_rt.png"),
	driver->getTexture("../media/sb/irrlicht2_ft.png"),
	driver->getTexture("../media/sb/irrlicht2_bk.png"));

	// Set the camera
	camera = smgr->addCameraSceneNode(0, vector3df(0,200,0), vector3df(0, 100, 1000));
	camera->setFarValue(10000.f);

	// Create the font 
	IGUIFont * font = guienv->getFont("../media/fonts/guifont.bmp");
	guienv->getSkin()->setFont(font);

	// Set up the views
	GetDocument()->dataLock.Lock();
	scene.SetUp(GetDocument(), device, camera);
	nav.SetUp(GetDocument(), device, camera);
	GetDocument()->dataLock.Unlock();

	// Show a start-up message
	driver->beginScene(true, true, video::SColor(255,100,101,140));
	rect<s32> fontRect(10,10,400,30);
	font->draw(L" GENERATING CITY ... please wait", fontRect, SColor(), false, false, &fontRect);
	driver->endScene();
}


// CUAVEditorView message handlers

void CUAVEditorView::OnPopupShowCityBounds()
{
	scene.showCityBounds = !scene.showCityBounds;
}

void CUAVEditorView::OnPopupShowSectorBounds()
{
	scene.showSectorBounds = !scene.showSectorBounds;
}

void CUAVEditorView::OnPopupShowBuildingBounds()
{
	scene.showBuildingBounds = !scene.showBuildingBounds;
}

void CUAVEditorView::OnPopupShowFog()
{
	scene.enableFog = !scene.enableFog;
	scene.fogChanged = true;
}

void CUAVEditorView::OnPopupJumpToSector()
{
	// Create the dialog box and launch it
	CJumpSectorDialog jump;
	INT_PTR result = jump.DoModal();

	// Use the results
	switch(result)
	{
	case IDOK:
		scene.JumpToSector(jump.dataX, jump.dataY);
		break;

	case IDCANCEL:
		break;

	default: // this shouldn't happen
		break;
	}
}
void CUAVEditorView::OnPopupSwitchToSceneView()
{
	view = VIEW_SCENE;
}

void CUAVEditorView::OnPopupSwitchToNavigationView()
{
	view = VIEW_NAVIGATION;
}

void CUAVEditorView::OnPopupAddUAV()
{
	GetDocument()->dataLock.Lock();
	nav.AddUAV();
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupAddWaypoint()
{
	GetDocument()->dataLock.Lock();
	nav.AddWaypoint();
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupAddBase()
{
	GetDocument()->dataLock.Lock();
	nav.AddBase();
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupAddEvent()
{
	// TODO: Add your command handler code here
}

void CUAVEditorView::OnPopupAddUAVWaypoint()
{
	UAVObject * uav = GetContextUAVPointer();

	GetDocument()->dataLock.Lock();
	nav.AddWaypoint(uav);
	GetDocument()->dataLock.Unlock();
}


void CUAVEditorView::OnPopupDeleteWaypoint()
{
	WaypointObject * wp = GetContextWaypointPointer();

	if(wp->getParent() && ((UAVObject *)wp->getParent())->getWPList()->size() == 1)
	{
		AfxMessageBox(L"Cannot delete this waypoint because it is the only waypoint\nattached to its UAV. Every UAV must have at least one waypoint.\n\nIf you want to delete this waypoint, delete its parent UAV.");
	}
	else
	{
		GetDocument()->dataLock.Lock();
		GetDocument()->DeleteWaypoint(wp);
		GetDocument()->dataLock.Unlock();
	}
}

void CUAVEditorView::OnPopupDeleteBase()
{
	SimObject * base = GetContextBasePointer();

	if(base->getParent() && ((UAVObject *)base->getParent())->getWPList()->size() == 1)
	{
		AfxMessageBox(L"Cannot delete this base because it is the only base\nattached to its UAV. Every UAV must have at least one base.\n\nIf you want to delete this base, delete its parent UAV.");
	}
	else
	{
		GetDocument()->dataLock.Lock();
		GetDocument()->DeleteBase(base);
		GetDocument()->dataLock.Unlock();
	}
}

void CUAVEditorView::OnPopupDeleteUAV()
{
	UAVObject * uav = GetContextUAVPointer();

	GetDocument()->dataLock.Lock();
	GetDocument()->DeleteUAV(uav);
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupJumpToWaypointSector()
{
	WaypointObject * wp = GetContextWaypointPointer();

	OnPopupSwitchToSceneView();
	scene.JumpToSectorFromAbsolute(wp->getPosition2D().X, wp->getPosition2D().Y);
}

void CUAVEditorView::OnPopupJumpToBaseSector()
{
	SimObject * base = GetContextBasePointer();

	OnPopupSwitchToSceneView();
	scene.JumpToSectorFromAbsolute(base->getPosition2D().X, base->getPosition2D().Y);
}

void CUAVEditorView::OnPopupJumpToUAVSector()
{
	SimObject * uav = GetContextUAVPointer();

	OnPopupSwitchToSceneView();
	scene.JumpToSectorFromAbsolute(uav->getPosition2D().X, uav->getPosition2D().Y);
}

void CUAVEditorView::OnPopupRemoveScene()
{
	GetDocument()->dataLock.Lock();
	scene.RemoveCurrentScene();
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupDeleteSceneView()
{
	// Get the selected scene's index
	CFileView &view = ((CMainFrame*)theApp.GetMainWnd())->GetFileView();
	int index = view.contextIndex;
	if(index < 0) return;
	if(unsigned(index) >= GetDocument()->scenes.size()) return;

	GetDocument()->dataLock.Lock();
	scene.selectedSector.x = GetDocument()->scenes[index]->x;
	scene.selectedSector.y = GetDocument()->scenes[index]->y;
	scene.RemoveCurrentScene();
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupLoadScene()
{
	GetDocument()->dataLock.Lock();
	scene.LoadCurrentScene();
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupShowUAVPaths()
{
	nav.showPaths = !nav.showPaths;
}

void CUAVEditorView::OnPopupJumpToSectorSceneView()
{
	view = VIEW_SCENE;

	CFileView &view = ((CMainFrame*)theApp.GetMainWnd())->GetFileView();
	int index = view.contextIndex;
	if(index < 0) return;
	if(unsigned(index) >= GetDocument()->scenes.size()) return;

	GetDocument()->dataLock.Lock();
	scene.JumpToSector(GetDocument()->scenes[index]->x, GetDocument()->scenes[index]->y);
	GetDocument()->dataLock.Unlock();
}

void CUAVEditorView::OnPopupSnapToSectorCenters()
{
	nav.snapToSectorCenters = !nav.snapToSectorCenters;
}

void CUAVEditorView::OnPopupShowSectorGrid()
{
	nav.showGrid = !nav.showGrid;
}

// This function should only be called from an OnPopup...Waypoint function
WaypointObject * CUAVEditorView::GetContextWaypointPointer()
{
	WaypointObject * wp;

	E_NAV_CONTEXT_TYPE nType = nav.GetContextType();
	if(nType == ENC_WP)
	{
		// The context was launched from the Navigation View
		wp = (WaypointObject *)nav.GetMouseObject(); // dangerous
		nav.DropMouseObject(); // for safety
	}
	else
	{
		// The context was launched from the Object/Event View
		CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
		wp = GetDocument()->wps[view.lastData->index]; // dangerous
	}

	return wp;
}

// This function should only be called from an OnPopup...Base function
SimObject * CUAVEditorView::GetContextBasePointer()
{
	SimObject * base;

	E_NAV_CONTEXT_TYPE nType = nav.GetContextType();
	if(nType == ENC_BASE)
	{
		// The context was launched from the Navigation View
		base = (SimObject *)nav.GetMouseObject(); // dangerous
		nav.DropMouseObject(); // for safety
	}
	else
	{
		// The context was launched from the Object/Event View
		CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
		base = GetDocument()->bases[view.lastData->index]; // dangerous
	}

	return base;
}

// This function should only be called from an OnPopup...UAV function
UAVObject * CUAVEditorView::GetContextUAVPointer()
{
	UAVObject * uav;

	E_NAV_CONTEXT_TYPE nType = nav.GetContextType();
	if(nType == ENC_UAV)
	{
		// The context was launched from the Navigation View
		uav = (UAVObject *)nav.GetMouseObject(); // dangerous
		nav.DropMouseObject(); // for safety
	}
	else
	{
		// The context was launched from the Object/Event View
		CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
		uav = GetDocument()->uavs[view.lastData->index]; // dangerous
	}

	return uav;
}

void CUAVEditorView::LoadScenario()
{
	CFileDialog fOpenDlg(TRUE, L"uav", L"", OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,
		L"UAV Scene Files (*.uav)|*.uav||");

	fOpenDlg.m_pOFN->lpstrTitle = L"Select UAV Scenario File";
	fOpenDlg.m_pOFN->lpstrInitialDir = L"../";

	// Get the current directory and transform it to the ../scenario/ folder
	wchar_t lpBuffer[1024];
	GetCurrentDirectory(1024, lpBuffer);
	stringw dir = lpBuffer;
	s32 slash = dir.findLast(wchar_t('\\'));
	dir = dir.subString(0, slash + 1);
	dir += "scenario";

	if(fOpenDlg.DoModal() == IDOK)
	{
		// Convert the filename to ASCII
		CString file = fOpenDlg.GetFileName();
		char buffer[1024];
		wcstombs(buffer, file, 1024);
	
		// Load the scenario
		stringc irrFile = "../scenario/";
		irrFile += buffer;

		GetDocument()->dataLock.Lock();
		LoadScenarioFile(irrFile);
		GetDocument()->dataLock.Unlock();
	}
}

void CUAVEditorView::LoadScenarioFile(const irr::core::stringc &file)
{
	IXMLReader * reader = device->getFileSystem()->createXMLReader(file.c_str());
	if(!reader)
	{
		AfxMessageBox(L"Failed to open scenario file.");
	}
	else
	{
		while(reader->read())
		{
			stringc node = reader->getNodeName();
			if(stringw(L"scene") == node)
			{
				ReadSceneNode(reader);
			}
			if(stringw(L"waypoint") == node)
			{
				ReadWaypointNode(reader);
			}
			else if(stringw(L"uav") == node)
			{
				ReadUAVNode(reader);
			}
			else if(stringw(L"base") == node)
			{
				ReadBaseNode(reader);
			}
			else if(stringw(L"ufo") == node)
			{
				//readUFONode(reader);
			}
			else if(stringw(L"event") == node)
			{
				//readEventNode(reader);
			}
		}
		reader->drop();
	}
}

void CUAVEditorView::ReadSceneNode(IXMLReader * reader)
{
	if(!reader)
		return;

	stringc node_name;
	stringc file;
	int cityX;
	int cityY;

	bool end = false;
	while(!end && reader->read())
	{
		stringw nodeName = reader->getNodeName();
		switch(reader->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(stringw(L"scene") == nodeName)
				end = true;
			break;

		case EXN_ELEMENT:
			if(stringw(L"string") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				stringc value = reader->getAttributeValue(stringw(L"value").c_str());

				if(stringw(L"Name") == name)
					node_name = value;
				else if(stringw(L"File") == name)
					file = value;
			}

			else if(stringw(L"int") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());

				if(stringw(L"CityX") == name)
					cityX = value;
				else if(stringw(L"CityY") == name)
					cityY = value;
			}
			break;
		}
	}

	// create the object
	if(node_name != "")
	{
		scene.AddScene(file, node_name, cityX, cityY);
	}
}

void CUAVEditorView::ReadUAVNode(irr::io::IXMLReader * reader)
{
	if(!reader)
		return;

	UAVObject * uav = 0;
	stringc node_name;
	float pos_x = -1;
	float pos_y = -1;
	float pos_z = -1;
	float fuel = 1.f;

	bool end = false;
	while(!end && reader->read())
	{
		if(!uav && pos_x != 0 && pos_y != -1 && pos_z != -1)
		{
			nav.AddUAV(node_name, vector3df(pos_x, pos_y, pos_z));
			uav = GetDocument()->uavs.back();
		}

		stringw nodeName = reader->getNodeName();
		switch(reader->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(stringw(L"uav") == nodeName) end = true;
			break;

		case EXN_ELEMENT:
			if(stringw(L"string") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				stringc value = reader->getAttributeValue(stringw(L"value").c_str());
				if(stringw(L"Name") == name) node_name = value;
			}
			else if(stringw(L"float") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
				if(stringw(L"PosX") == name) pos_x = value;
				if(stringw(L"PosY") == name) pos_y = value;
				if(stringw(L"PosZ") == name) pos_z = value;
				if(stringw(L"Fuel") == name) fuel = value;
			}
			else if(stringw(L"waypoint") == nodeName)
			{
				ReadWaypointNode(reader, uav);
			}
			else if(stringw(L"base") == nodeName)
			{
				ReadBaseNode(reader, uav);
			}
			break;
		}
	}

	if(uav)
		uav->setFuel(fuel);
}

SimObject * CUAVEditorView::ReadBaseNode(IXMLReader * reader, UAVObject * parent)
{
	if(!reader)
		return 0;

	SimObject * base = 0;
	stringc node_name;
	float pos_x;
	float pos_y;
	float pos_z;

	bool end = false;
	while(!end && reader->read())
	{
		stringw nodeName = reader->getNodeName();
		switch(reader->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(stringw(L"base") == nodeName) end = true;
			break;

		case EXN_ELEMENT:
			if(stringw(L"string") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				stringc value = reader->getAttributeValue(stringw(L"value").c_str());
				if(stringw(L"Name") == name) node_name = value;
			}
			else if(stringw(L"float") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
				if(stringw(L"PosX") == name) pos_x = value;
				if(stringw(L"PosY") == name) pos_y = value;
				if(stringw(L"PosZ") == name) pos_z = value;
			}
			break;
		}
	}

	// create the object
	if(node_name != "")
	{
		vector3df position(pos_x, pos_y, pos_z);
		nav.AddBase(node_name, position, parent);
	}
	return base;
}

WaypointObject * CUAVEditorView::ReadWaypointNode(IXMLReader * reader, UAVObject * parent)
{
	if(!reader)
		return 0;

	WaypointObject * waypoint = 0;
	stringc node_name;
	float pos_x;
	float pos_y;
	float pos_z;
	bool has_feature = true;

	bool end = false;
	while(!end && reader->read())
	{
		stringw nodeName = reader->getNodeName();
		switch(reader->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(stringw(L"waypoint") == nodeName)
				end = true;
			break;

		case EXN_ELEMENT:
			if(stringw(L"string") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				stringc value = reader->getAttributeValue(stringw(L"value").c_str());

				if(stringw(L"Name") == name)
					node_name = value;
			}

			else if(stringw(L"bool") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				stringc value = reader->getAttributeValue(stringw(L"value").c_str());
				if(stringw(L"Feature") == name)
				{
					if     (stringc("True") == value) has_feature = true;
					else if(stringc("true") == value) has_feature = true;
					else if(stringc("False") == value) has_feature = false;
					else if(stringc("false") == value) has_feature = false;
				};
			}

			else if(stringw(L"float") == nodeName)
			{
				stringc name = reader->getAttributeValue(stringw(L"name").c_str());
				float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());

				if(stringw(L"PosX") == name)
					pos_x = value;
				if(stringw(L"PosY") == name)
					pos_y = value;
				if(stringw(L"PosZ") == name)
					pos_z = value;
			}
			break;
		}
	}

	// create the object
	if(node_name != "")
	{
		vector3df position(pos_x, pos_y, pos_z);
		nav.AddWaypoint(node_name, position, has_feature, parent);
	}
	return waypoint;
}