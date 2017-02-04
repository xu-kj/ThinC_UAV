
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "UAVEditor.h"

class CObjectEventViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CObjectEventView;

	DECLARE_SERIAL(CObjectEventViewMenuButton)

public:
	CObjectEventViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CObjectEventViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectEventView::CObjectEventView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CObjectEventView::~CObjectEventView()
{
	ClearClassView();
}

BEGIN_MESSAGE_MAP(CObjectEventView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectEventView message handlers

int CObjectEventView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Object/Event View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CObjectEventViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CObjectEventViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CObjectEventViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data
	FillBaseClassView();

	return 0;
}

void CObjectEventView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

// FindSubObjectByIndex
// Returns a handle to the subobject with the given index.
// These can be Waypoints or Bases.
// Returns 0 on failure.
HTREEITEM CObjectEventView::FindSubObjectByIndex(E_ITEM_TYPE type, int index, int uavIndex)
{
	// Find the parent UAV first
	HTREEITEM hUAV = FindChildByIndex(hObjectRoot, ITEM_UAV, uavIndex);
	if(!hUAV) return 0;

	// Find the correct child of that UAV
	return FindChildByIndex(hUAV, type, index);
}

// FindChildByIndex
// Returns a handle to the child with the given index.
// These can be anything.
// Returns 0 on failure.
HTREEITEM CObjectEventView::FindChildByIndex(HTREEITEM parent, E_ITEM_TYPE type, int index)
{
	// Grab the first child
	HTREEITEM hItem = m_wndClassView.GetChildItem(parent);
	if(!hItem) return 0;

	// Find the child with the correct index (if exists)
	bool valid = false;
	while(!valid)
	{
		ListItemData * pData = (ListItemData *)m_wndClassView.GetItemData(hItem);
		if(pData->index == index && pData->type == type)
		{
			// Gotcha
			valid = true;
		}
		else
		{
			// Get next item
			hItem = m_wndClassView.GetNextSiblingItem(hItem);
			if(!hItem) return 0;
		}
	}
	return hItem;
}

// AddObject Parameters
// index		= the added object's index in the document's data list
// parentIndex	= the added object's parent UAV index in the document data list
// Assumes that any parents exist. Simply returns on failure.
void CObjectEventView::AddObject(LPCTSTR lpszItem, E_ITEM_TYPE type, int index, int uavIndex)
{
	// Find the parent item based on the type
	HTREEITEM hParent = hObjectRoot;
	int image;
	if(type == ITEM_UAV)
	{
		image = 1;
	}
	else if(type == ITEM_WAYPOINT)
	{
		if(uavIndex >= 0)
		{
			HTREEITEM hUAV = FindChildByIndex(hObjectRoot, ITEM_UAV, uavIndex);
			if(!hUAV) return;
			hParent = hUAV;
		}

		image = 3;
	}
	else if(type == ITEM_BASE)
	{
		if(uavIndex >= 0)
		{
			HTREEITEM hUAV = FindChildByIndex(hObjectRoot, ITEM_UAV, uavIndex);
			if(!hUAV) return;
			hParent = hUAV;
		}

		image = 5;
	}

	// Create the item data and store it in a vector
	ListItemData * pData = new ListItemData(type, index, uavIndex);
	itemData.push_back(pData);

	// Create the item and associate it with the vector
	HTREEITEM hItem = m_wndClassView.InsertItem(lpszItem, image, image, hParent);
	m_wndClassView.SetItemData(hItem, (DWORD_PTR)(pData));

	// Expand the root and the parent (sometimes we'll expand the root twice, yes)
	m_wndClassView.Expand(hParent, TVE_EXPAND);
	m_wndClassView.Expand(hObjectRoot, TVE_EXPAND);
}

void CObjectEventView::FillBaseClassView()
{
	// Create the Object root
	hObjectRoot = m_wndClassView.InsertItem(_T("Objects"), 0, 0);
	m_wndClassView.SetItemState(hObjectRoot, TVIS_BOLD, TVIS_BOLD);

	// Create the Event root
	hEventRoot = m_wndClassView.InsertItem(_T("Events"), 0, 0);
	m_wndClassView.SetItemState(hEventRoot, TVIS_BOLD, TVIS_BOLD);
}

void CObjectEventView::ClearClassView()
{
	m_wndClassView.DeleteAllItems();
	for(itemItr = itemData.begin(); itemItr != itemData.end(); itemItr++)
		delete (*itemItr);
	itemData.clear();
}

void CObjectEventView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	bool showWPMenu = false;
	bool showBaseMenu = false;
	bool showUAVMenu = false;

	// Stick a goofy do-while loop around this crap to skip it
	do {
		CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
		ASSERT_VALID(pWndTree);

		if (pWnd != pWndTree)
		{
			CDockablePane::OnContextMenu(pWnd, point);
			return;
		}

		if (point != CPoint(-1, -1))
		{
			// Select clicked item:
			CPoint ptTree = point;
			pWndTree->ScreenToClient(&ptTree);

			UINT flags = 0;
			HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
			if (hTreeItem != NULL)
			{
				pWndTree->SelectItem(hTreeItem);

				// This is where we inject our own menus based on the tree item
				ListItemData * pData = (ListItemData *)m_wndClassView.GetItemData(hTreeItem);
				if(pData && pData->type == ITEM_WAYPOINT)
				{
					lastData = pData;
					showWPMenu = true;
					break;
				}
				else if(pData && pData->type == ITEM_BASE)
				{
					lastData = pData;
					showBaseMenu = true;
					break;
				}
				else if(pData && pData->type == ITEM_UAV)
				{
					lastData = pData;
					showUAVMenu = true;
					break;
				}
			}
		}

		pWndTree->SetFocus();
		CMenu menu;
		menu.LoadMenu(IDR_POPUP_SORT);

		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return;

			((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
	} while(0);

	// Show menus based on which item is selected
	if(showWPMenu)
	{
		CMenu menu;
		menu.LoadMenu(IDR_WAYPOINT_CONTEXT);
		CMenu * cMenu = menu.GetSubMenu(0);
		cMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	else if(showBaseMenu)
	{
		CMenu menu;
		menu.LoadMenu(IDR_BASE_CONTEXT);
		CMenu * cMenu = menu.GetSubMenu(0);
		cMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	else if(showUAVMenu)
	{
		CMenu menu;
		menu.LoadMenu(IDR_UAV_CONTEXT);
		CMenu * cMenu = menu.GetSubMenu(0);
		cMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CObjectEventView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CObjectEventView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CObjectEventView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CObjectEventViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CObjectEventViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CObjectEventView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CObjectEventView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CObjectEventView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CObjectEventView::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CObjectEventView::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CObjectEventView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CObjectEventView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CObjectEventView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CObjectEventView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}
