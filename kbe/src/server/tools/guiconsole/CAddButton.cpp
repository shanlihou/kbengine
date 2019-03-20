// CAddButton.cpp: 实现文件
//

#include "stdafx.h"
#include "guiconsole.h"
#include "CAddButton.h"
#include "afxdialogex.h"


// CAddButton 对话框

IMPLEMENT_DYNAMIC(CAddButton, CDialogEx)

CAddButton::CAddButton(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_type(0)
	, m_code(_T(""))
	, m_title(_T(""))
{

}

CAddButton::~CAddButton()
{
}

void CAddButton::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT2, m_type);
	DDX_Text(pDX, IDC_EDIT3, m_code);
	DDX_Text(pDX, IDC_EDIT1, m_title);
}


BEGIN_MESSAGE_MAP(CAddButton, CDialogEx)
END_MESSAGE_MAP()


// CAddButton 消息处理程序
