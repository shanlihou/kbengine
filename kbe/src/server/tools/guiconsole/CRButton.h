#pragma once


// CRButton

class CRButton : public CButton
{
	DECLARE_DYNAMIC(CRButton)

public:
	CRButton();
	CRButton(CString, CString, UINT, UINT);
	void saveButton();
	virtual ~CRButton();

protected:
	std::string genFullPath();
	void removeMyFile();
	DECLARE_MESSAGE_MAP()
protected:
	CString m_title;
	CString m_code;
	UINT m_type;
	UINT m_index;
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClicked();
};


