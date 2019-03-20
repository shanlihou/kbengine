#pragma once


// CToolBarWithEdit

class CToolBarWithEdit : public CToolBar
{
	DECLARE_DYNAMIC(CToolBarWithEdit)

public:
	CToolBarWithEdit();
	virtual ~CToolBarWithEdit();

public:
	CEdit m_Edit;
	int m_uid;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//virtual void DoDataExchange(CDataExchange* pDX);
};


