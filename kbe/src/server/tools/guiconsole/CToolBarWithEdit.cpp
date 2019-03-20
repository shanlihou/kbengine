// CToolBarWithEdit.cpp: 实现文件
//

#include "stdafx.h"
#include "guiconsole.h"
#include "guiconsoleDlg.h"
#include "CToolBarWithEdit.h"


// CToolBarWithEdit

IMPLEMENT_DYNAMIC(CToolBarWithEdit, CToolBar)

CToolBarWithEdit::CToolBarWithEdit()
{

}

CToolBarWithEdit::~CToolBarWithEdit()
{
}


BEGIN_MESSAGE_MAP(CToolBarWithEdit, CToolBar)
END_MESSAGE_MAP()



// CToolBarWithEdit 消息处理程序




BOOL CToolBarWithEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN || WM_SYSKEYDOWN == pMsg->message)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CString content;
			m_Edit.GetWindowText(content);
			std::string sContent = CS2S(content);
			CguiconsoleDlg* dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);
			dlg->addConfig("uid", sContent);
			pMsg->wParam = 0;
		}
	}

	return CToolBar::PreTranslateMessage(pMsg);
}
//
//
//void CToolBarWithEdit::DoDataExchange(CDataExchange* pDX)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	CToolBar::DoDataExchange(pDX);
//
//	//DDX_Text(pDX, ID_BUTTON32785, m_uid);
//}
