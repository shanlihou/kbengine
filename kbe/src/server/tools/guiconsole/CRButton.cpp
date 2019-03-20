// CRButton.cpp: 实现文件
//

#include "stdafx.h"
#include "guiconsole.h"
#include "guiconsoleDlg.h"
#include "CAddButton.h"
#include "CRButton.h"


// CRButton

IMPLEMENT_DYNAMIC(CRButton, CButton)

CRButton::CRButton()
{

}

CRButton::CRButton(CString title, CString code, UINT type, UINT index):m_title(title), m_code(code), m_type(type), m_index(index)
{
}

CRButton::~CRButton()
{
}

std::string CRButton::genFullPath()
{
	CString appPath = GetAppPath();
	CString fullPath = appPath + L"\\code\\";

	char dirName[4096] = { 0 };

	int len = WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), dirName, len, NULL, NULL);

	sprintf(dirName + len, "%d\\", m_index);
	return std::string(dirName);
}

void CRButton::removeMyFile()
{
	auto fullPath = genFullPath();
	char fileName[256];
	std::string out;

	strutil::wchar2utf8(m_title.GetBuffer(0), out);
	sprintf(fileName, "%s\\%s-%d.py", fullPath.c_str(), out.c_str(), m_type);
	remove(fileName);
}

void createPath(const char *dirName)
{
	fstream _file;
	_file.open(dirName, ios::in);
	if (!_file)
	{
		CreateDirectoryA(dirName, NULL);
	}
}

void CRButton::saveButton()
{

	CString appPath = GetAppPath();
	CString fullPath = appPath + L"\\code";

	char dirName[4096] = { 0 };

	int len = WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), dirName, len, NULL, NULL);
	createPath(dirName);

	sprintf(dirName + len, "\\%d", m_index);
	createPath(dirName);

	char fileName[256];
	std::string out;

	strutil::wchar2utf8(m_title.GetBuffer(0), out);
	sprintf(fileName, "%s\\%s-%d.py", dirName, out.c_str(), m_type);

	std::unique_ptr<FILE, decltype(fclose)*> fp(fopen(fileName, "wb"), fclose);
	strutil::wchar2utf8(m_code.GetBuffer(0), out);
	fprintf(fp.get(), "%s", out.c_str());
}


BEGIN_MESSAGE_MAP(CRButton, CButton)
	ON_WM_RBUTTONDOWN()
	ON_CONTROL_REFLECT(BN_CLICKED, &CRButton::OnBnClicked)
END_MESSAGE_MAP()



// CRButton 消息处理程序




void CRButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CAddButton wnd;
	wnd.m_title = m_title;
	wnd.m_code = m_code;
	wnd.m_type = m_type;
	if (wnd.DoModal() == IDOK)
	{
		removeMyFile();
		m_title = wnd.m_title;
		m_code = wnd.m_code;
		m_type = wnd.m_type;
		this->SetWindowText(m_title);
		saveButton();
	}

	CButton::OnRButtonDown(nFlags, point);
}


void CRButton::OnBnClicked()
{
	auto dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);

	std::vector<CString> retList;
	splitCmdStr(m_code, retList);
	for (auto i : retList)
	{
		dlg->commitPythonCommandByType(i, m_type);
	}
	// TODO: 在此添加控件通知处理程序代码
}
