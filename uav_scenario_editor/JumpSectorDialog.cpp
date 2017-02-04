// JumpSectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "UAVEditor.h"
#include "JumpSectorDialog.h"
#include "SceneGlobals.h"
#include <irrlicht.h>


// CJumpSectorDialog dialog

IMPLEMENT_DYNAMIC(CJumpSectorDialog, CDialog)

CJumpSectorDialog::CJumpSectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CJumpSectorDialog::IDD, pParent)
{
}

CJumpSectorDialog::~CJumpSectorDialog()
{
}

void CJumpSectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJumpSectorDialog, CDialog)
	ON_WM_CHAR()
	ON_EN_CHANGE(IDC_JUMP_SECTOR_X, &CJumpSectorDialog::OnEnChangeJumpSectorX)
	ON_EN_CHANGE(IDC_JUMP_SECTOR_Y, &CJumpSectorDialog::OnEnChangeJumpSectorY)
	ON_BN_CLICKED(IDOK, &CJumpSectorDialog::OnBnClickedOk)
END_MESSAGE_MAP()

// CJumpSectorDialog message handlers
void CJumpSectorDialog::OnEnChangeJumpSectorX()
{
	CEdit* pData = (CEdit*)GetDlgItem(IDC_JUMP_SECTOR_X);
	EnforceStringNumbers(valX, dataX, pData);
}

void CJumpSectorDialog::OnEnChangeJumpSectorY()
{
	CEdit* pData = (CEdit*)GetDlgItem(IDC_JUMP_SECTOR_Y);
	EnforceStringNumbers(valY, dataY, pData);
}

void CJumpSectorDialog::OnBnClickedOk()
{
	if(valX.GetLength() == 0)
		AfxMessageBox(L"Please enter a value for sector X.");
	else if(valY.GetLength() == 0)
		AfxMessageBox(L"Please enter a value for sector Y.");
	else
		OnOK();
}

void CJumpSectorDialog::EnforceStringNumbers(CString &val, int &data, CEdit * pData)
{
	CString oldVal = val;
	pData->GetWindowText(val);
	bool valid = true;

	// make sure the string is not longer than 5 digits
	if(val.GetLength() > 5)
		valid = false;

	// make sure the string contains only digits
	for(int i = 0; i < val.GetLength(); i++)
	{
		if(!iswdigit(val[i]))
		{
			valid = false;
			break;
		}
	}

	// revert to the last valid string if invalid
	if(!valid)
	{
		val = oldVal;
		pData->SetWindowTextW(val);
	}

	// set the value data (must be a valid integer)
	int oldData = data;
	if(val.GetLength() > 0)
		data = _ttoi(val);

	// revert to the last valid string if the data value is invalid (aka integer too big)
	if(data > city_size)
	{
		data = oldData;
		val = oldVal;
		pData->SetWindowTextW(val);
	}
}