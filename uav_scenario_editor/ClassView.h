
#pragma once

#include "ViewTree.h"
#include <vector>

enum E_ITEM_TYPE {ITEM_UAV, ITEM_WAYPOINT, ITEM_BASE};
struct ListItemData {
	ListItemData(E_ITEM_TYPE type, int index, int uavIndex = 0)
		: index(index), uavIndex(uavIndex), type(type) {}
	int index;
	int uavIndex;
	E_ITEM_TYPE type;
};

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CObjectEventView : public CDockablePane
{
public:
	CObjectEventView();
	virtual ~CObjectEventView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar	m_wndToolBar;
	CViewTree		m_wndClassView;
	CImageList		m_ClassViewImages;
	UINT			m_nCurrSort;

	HTREEITEM		hObjectRoot;
	HTREEITEM		hEventRoot;
	std::vector<ListItemData *> itemData; // to delete allocated structs
	std::vector<ListItemData *>::iterator itemItr;

	HTREEITEM FindSubObjectByIndex(E_ITEM_TYPE type, int index, int uavIndex);
	HTREEITEM FindChildByIndex(HTREEITEM parent, E_ITEM_TYPE type, int index);
public:
	void AddObject(LPCTSTR lpszItem, E_ITEM_TYPE type, int index, int uavIndex = -1);
	void FillBaseClassView(); // creates the root nodes (Object and Event)
	void ClearClassView();	// safely deletes all the item data

	ListItemData * lastData; // haha really really bad idea xD

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

