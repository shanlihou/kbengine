// EditSendbuffer.cpp : implementation file
//

#include "stdafx.h"
#include "guiconsole.h"
#include "EditSendbuffer.h"
#include "guiconsoleDlg.h"

// CEditSendbuffer

IMPLEMENT_DYNAMIC(CEditSendbuffer, CEdit)

CEditSendbuffer::CEditSendbuffer()
{

}

CEditSendbuffer::~CEditSendbuffer()
{
}


BEGIN_MESSAGE_MAP(CEditSendbuffer, CEdit)
END_MESSAGE_MAP()

void splitCmdStr(CString &s, std::vector<CString> &retList)
{
	int index;
	int nStart = 0;
	FLOG("will split\n");
	std::string outcmd;
	strutil::wchar2utf8(s.GetBuffer(0), outcmd);
	FLOG("before %s\n----\n", outcmd.c_str());
	int cut = 0;
	while ((index = s.Find(L'\n', nStart)) != -1)
	{
		FLOG("find n:%d\n", index);
		TCHAR ch = s.GetAt(index + 1);
		if (ch == L' ' || ch == L'\t')
		{
			nStart = index + 1;
			continue;
		}

		retList.push_back(s.Mid(cut, index - cut));
		nStart = index + 1;
		cut = nStart;
		// not use
		strutil::wchar2utf8(retList[retList.size() - 1].GetBuffer(0), outcmd);
		FLOG("%s\n----\n", outcmd.c_str());
	}
	retList.push_back(s.Right(s.GetLength() - cut));
}

BOOL CEditSendbuffer::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN || WM_SYSKEYDOWN == pMsg->message)
	{
		CguiconsoleDlg* dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);

		//FLOG("wParam:%x", pMsg->wParam, );
		if(pMsg->wParam == VK_RETURN && GetKeyState(VK_CONTROL)&0x80)
		{
			CString s ;
			this->GetWindowTextW(s);

			std::vector<CString> retList;
			splitCmdStr(s, retList);
			for (auto i : retList)
			{
				dlg->commitPythonCommand(i);
			}
			//dlg->commitPythonCommand(s);
			this->SetWindowTextW(L"");
			return FALSE;  
		}
		else if((GetKeyState(VK_MENU) < 0 || GetKeyState(VK_CONTROL) < 0) && GetKeyState(VK_UP) < 0)
		{
			CString s = dlg->getHistoryCommand(false);
			if(s.GetLength() > 0)
			{
				this->SetWindowTextW(s);
			}
		}
		else if((GetKeyState(VK_MENU) < 0 || GetKeyState(VK_CONTROL) < 0)  && GetKeyState(VK_DOWN) < 0)
		{
			CString s = dlg->getHistoryCommand(true);
			if(s.GetLength() > 0)
			{
				this->SetWindowTextW(s);
			}
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
// CEditSendbuffer message handlers


