// DebugWindow.cpp : implementation file
//

#include "stdafx.h"
#include "guiconsole.h"
#include "DebugWindow.h"
#include "CAddButton.h"
#include "guiconsoleDlg.h"
#include "xml/xml.h"

// CDebugWindow dialog

IMPLEMENT_DYNAMIC(CDebugWindow, CDialog)

CguiconsoleDlg* g_dlg = static_cast<CguiconsoleDlg*>(theApp.m_pMainWnd);
CDebugWindow::CDebugWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugWindow::IDD, pParent)
{
}

CDebugWindow::~CDebugWindow()
{
}

void CDebugWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_displaybuffer);
	DDX_Control(pDX, IDC_EDIT2, m_sendbuffer);
	DDX_Control(pDX, IDC_BUTTON1, m_addbutton);
}

BEGIN_MESSAGE_MAP(CDebugWindow, CDialog)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDebugWindow::OnBnClickedButton1)
END_MESSAGE_MAP()

void createDir(const char *dirName)
{
	fstream _file;
	_file.open(dirName, ios::in);
	if (!_file)
	{
		CreateDirectoryA(dirName, NULL);
	}
}

void CDebugWindow::parseButtonInfo2(UINT8 tabIndex)
{
	CString appPath = GetAppPath();
	CString fullPath = appPath + L"\\code";

	char dirName[4096] = { 0 };

	int len = WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), dirName, len, NULL, NULL);

	sprintf(dirName + len, "\\%d\\*.*", tabIndex);

	_finddata_t findData;
	auto handle = _findfirst(dirName, &findData);
	if (handle == -1)
	{
		return;
	}

	auto dirBase = std::string(dirName);
	dirBase = dirBase.substr(0, dirBase.size() - 3);
	do
	{
		std::string name = findData.name;
		if (name == "." || name == "..")
			continue;

		std::string fullName = dirBase + name;

		UINT8 index = name.rfind('-');
		std::string title = name.substr(0, index);
		std::string type = name.substr(index + 1);


		ifstream infile(fullName, ios::in | ios::binary | ios::ate);

		UINT32 size = infile.tellg();
		infile.seekg(0, ios::beg);
		std::unique_ptr<char[]> buffer(new char[size + 1]);
		infile.read(buffer.get(), size);
		buffer.get()[size] = 0;
		std::string code(buffer.get());

		CString wTitle(title.c_str());
		CString wCode(code.c_str());
		UINT8 uType = type.at(0) == '1' ? 1 : 0;
		AddButton(wTitle, wCode, uType, 0);

	} while (_findnext(handle, &findData) == 0);
}

BOOL CDebugWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	parseButtonInfo2(0);
	return TRUE; 
};

void CDebugWindow::AddButton(CString & title, CString & code, UINT8 type, UINT8 index)
{
	std::shared_ptr<CRButton> btn(new CRButton(title, code, type, index));
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_LEFTTEXT;
	CRect rect;
	GetClientRect(&rect);
	btn->Create(title, dwStyle,
		CRect(5 + int(rect.right * 0.06) * (m_buttonvec.size() + 1),
			int(rect.bottom * 0.70),
			5 + int(rect.right * 0.06) * (m_buttonvec.size() + 2),
			int(rect.bottom * 0.75)),
		this, IDC_MY_BUTTON + m_buttonvec.size());
	m_buttonvec.push_back(btn);
}

void CDebugWindow::OnBnClickedButton1()
{
	CAddButton wnd;
	if (wnd.DoModal() == IDOK)
	{
		AddButton(wnd.m_title, wnd.m_code, wnd.m_type, 0);
		(m_buttonvec.end() - 1)->get()->saveButton();
	}
    int ret = GetLastError();
    ret = ret;
}

// CDebugWindow message handlers

void CDebugWindow::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
}

void CDebugWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CDebugWindow::autoWndSize()
{
	CRect rect;
	GetClientRect(&rect);
	m_displaybuffer.MoveWindow(5, 5, rect.right, (int)(rect.bottom * 0.69), TRUE);
	m_sendbuffer.MoveWindow(5, int(rect.bottom * 0.76), rect.right, int(rect.bottom * 0.25), TRUE);
	m_addbutton.MoveWindow(5, int(rect.bottom * 0.70), int(rect.right * 0.05), int(rect.bottom * 0.05), TRUE);
	int length = m_buttonvec.size();
	for (int i = 0; i < length; i++)
	{
		m_buttonvec[i]->MoveWindow(5 + int(rect.right * 0.06) * (i + 1), int(rect.bottom * 0.70),
			int(rect.right * 0.05), int(rect.bottom * 0.05), TRUE);
	}
}


//
//
//void CDebugWindow::saveButton2(UINT8 tabIndex)
//{
//	CString appPath = GetAppPath();
//	CString fullPath = appPath + L"\\code";
//
//	char dirName[4096] = { 0 };
//
//	int len = WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), NULL, 0, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), dirName, len, NULL, NULL);
//	createDir(dirName);
//
//	sprintf(dirName + len, "\\%d", tabIndex);
//	createDir(dirName);
//
//	UINT8 lastNum = m_buttoninfoVec.size() - 1;
//	auto lastInfo = m_buttoninfoVec.end() - 1;
//	char fileName[256];
//	std::string out;
//
//	strutil::wchar2utf8(lastInfo->m_title.GetBuffer(0), out);
//	sprintf(fileName, "%s\\%s-%d.py", dirName, out.c_str(), lastInfo->m_type);
//
//	std::unique_ptr<FILE, decltype(fclose)*> fp(fopen(fileName, "wb"), fclose);
//	strutil::wchar2utf8(lastInfo->m_code.GetBuffer(0), out);
//	fprintf(fp.get(), "%s", out.c_str());
//}
//
//void CDebugWindow::saveButton()
//{
//
//	TiXmlDocument *pDocument = new TiXmlDocument();
//
//	int i = 0;
//	TiXmlElement *rootElement = new TiXmlElement("root");
//	pDocument->LinkEndChild(rootElement);
//	std::string out;
//	char buf[2];
//
//	for (auto i : m_buttoninfoVec)
//	{
//		strutil::wchar2utf8(i.m_title.GetBuffer(0), out);
//		TiXmlElement *rootElementChild = new TiXmlElement(out.data());
//		rootElement->LinkEndChild(rootElementChild);
//
//		//CString code = i.m_code;
//		strutil::wchar2utf8(i.m_code.GetBuffer(0), out);
//		TiXmlText *content = new TiXmlText(out.data());
//		rootElementChild->LinkEndChild(content);
//
//		/*
//		kbe_snprintf(buf, 2, "%d", i.m_type);
//		TiXmlElement *type = new TiXmlElement(buf);
//		rootElementChild->LinkEndChild(type);*/
//	}
//
//	CString appPath = GetAppPath();
//	CString fullPath = appPath + L"\\buttonInfo.xml";
//
//	char fname[4096] = { 0 };
//
//	int len = WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), NULL, 0, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), fname, len, NULL, NULL);
//	fname[len] = '\0';
//
//	pDocument->SaveFile(fname);
//}

//
//
//void CDebugWindow::parseButtonInfo()
//{
//
//	CString appPath = GetAppPath();
//	CString fullPath = appPath + L"\\buttonInfo.xml";
//
//	char fname[4096] = { 0 };
//
//	int len = WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), NULL, 0, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, fullPath, fullPath.GetLength(), fname, len, NULL, NULL);
//	fname[len + 1] = '\0';
//
//	TiXmlDocument *pDocument = new TiXmlDocument(fname);
//	if (pDocument == NULL)
//		return;
//
//	if (!pDocument->LoadFile(TIXML_ENCODING_UTF8))
//	{
//		const char *errDesc = pDocument->ErrorDesc();
//		return;
//	}
//
//	TiXmlElement *rootElement = pDocument->RootElement();
//	TiXmlNode* node = rootElement->FirstChild();
//	if (node)
//	{
//		do
//		{
//			if (node->FirstChild() != NULL)
//			{
//				std::string title = node->Value();
//				std::string code = node->FirstChild()->Value();
//				std::string type = node->FirstChild()->NextSibling()->Value();
//				CString wTitle(title.c_str());
//				CString wCode(code.c_str());
//				wCode.Replace('`', ' ');
//				UINT8 uType = type.at(0) == '1' ? 1 : 0;
//				AddButton(wTitle, wCode, uType, 0);
//				m_buttoninfoVec.push_back(ButtonInfo(wTitle, wCode, uType));
//				//std::string d = node->FirstChild()->FirstChild()->Value();
//			}
//		} while ((node = node->NextSibling()));
//	}
//
//	pDocument->Clear();
//	delete pDocument;
//}