#pragma once


// CAddButton 对话框

class CAddButton : public CDialogEx
{
	DECLARE_DYNAMIC(CAddButton)

public:
	CAddButton(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddButton();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_type;
	CString m_code;
	CString m_title;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
