#pragma once
#include "afxwin.h"
#include "EditSendbuffer.h"
#include "CRButton.h"
#include <memory>

// CDebugWindow dialog
#define IDC_MY_BUTTON 2001

class CDebugWindow : public CDialog
{
	DECLARE_DYNAMIC(CDebugWindow)

public:
	CDebugWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugWindow();

// Dialog Data
	enum { IDD = IDD_DEBUG };
	
	CEdit* displaybufferWnd(){ return &m_displaybuffer; }
	CEdit* sendbufferWnd(){ return &m_sendbuffer; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnBnClickedButton1();
	bool parseButtonInfo2(UINT8 tabIndex);
	void AddButton(CString &title, CString &code, UINT8 type, UINT8 index);


	DECLARE_MESSAGE_MAP()
protected:
	CEdit m_displaybuffer;
	CEditSendbuffer m_sendbuffer;
	CMFCButton m_addbutton;
	std::vector<std::shared_ptr<CRButton>> m_buttonvec;
	int m_index;
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

	void autoWndSize();
	afx_msg void OnClickedButtonClear();
	CButton m_clearButton;
	CSpinButtonCtrl m_butSpin;
	afx_msg void OnDeltaposButSpin(NMHDR *pNMHDR, LRESULT *pResult);
};
