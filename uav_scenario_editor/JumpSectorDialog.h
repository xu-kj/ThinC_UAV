#pragma once


// CJumpSectorDialog dialog

class CJumpSectorDialog : public CDialog
{
	DECLARE_DYNAMIC(CJumpSectorDialog)

public:
	CJumpSectorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJumpSectorDialog();

	// Dialog Data
	enum { IDD = IDD_DIALOG_JUMP_SECTOR };
	int dataX;
	int dataY;
private:
	CString valX;
	CString valY;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnEnChangeJumpSectorX();
	afx_msg void OnEnChangeJumpSectorY();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

	void EnforceStringNumbers(CString &val, int &data, CEdit * pData);
};
