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
	m_index = 0;
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
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_clearButton);
	DDX_Control(pDX, IDC_BUT_SPIN, m_butSpin);
}

BEGIN_MESSAGE_MAP(CDebugWindow, CDialog)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDebugWindow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDebugWindow::OnClickedButtonClear)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BUT_SPIN, &CDebugWindow::OnDeltaposButSpin)
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

bool CDebugWindow::parseButtonInfo2(UINT8 tabIndex)
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
		return false;
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
	return true;
}

BOOL CDebugWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_index = ::GetPrivateProfileInt(_T("debug"), _T("buttonIndex"), 0, _T("guiconsole.ini"));
	parseButtonInfo2(m_index);
	return TRUE; 
};

void CDebugWindow::AddButton(CString & title, CString & code, UINT8 type, UINT8 index)
{
	std::shared_ptr<CRButton> btn(new CRButton(title, code, type, index));
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_LEFTTEXT;
	CRect rect;
	GetClientRect(&rect);

	int butWidth = int(rect.right * 0.05);
	btn->Create(title, dwStyle,
		CRect(5 + butWidth * (m_buttonvec.size() + 3),
			int(rect.bottom * 0.70),
			5 + butWidth * (m_buttonvec.size() + 4),
			int(rect.bottom * 0.75)),
		this, IDC_MY_BUTTON + m_buttonvec.size());
	m_buttonvec.push_back(btn);
}

void CDebugWindow::OnBnClickedButton1()
{
	CAddButton wnd;
	if (wnd.DoModal() == IDOK)
	{
		AddButton(wnd.m_title, wnd.m_code, wnd.m_type, m_index);
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

	int butWidth = int(rect.right * 0.05);
	m_addbutton.MoveWindow(5 + butWidth * 0, int(rect.bottom * 0.70), int(rect.right * 0.05), int(rect.bottom * 0.05), TRUE);
	m_clearButton.MoveWindow(5 + butWidth * 1, int(rect.bottom * 0.70), int(rect.right * 0.05), int(rect.bottom * 0.05), TRUE);
	m_butSpin.MoveWindow(5 + butWidth * 2, int(rect.bottom * 0.70), int(rect.right * 0.05), int(rect.bottom * 0.05), TRUE);
	int length = m_buttonvec.size();
	for (int i = 0; i < length; i++)
	{
		m_buttonvec[i]->MoveWindow(5 + butWidth * (i + 3), int(rect.bottom * 0.70),
			int(rect.right * 0.05), int(rect.bottom * 0.05), TRUE);
	}
}

void CDebugWindow::OnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit* lpEdit = (CEdit*)this->displaybufferWnd();
	lpEdit->SetWindowText(L"");
}


void CDebugWindow::OnDeltaposButSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int oldIndex = m_index;
	if (pNMUpDown->iDelta == 1) {
		if (m_buttonvec.size() != 0)
		{
			m_index += 1;
		}
	}
	else {
		if (m_index > 0)
		{
			m_index -= 1;
		}
	}

	if (oldIndex != m_index)
	{
		for (auto i : m_buttonvec)
		{
			i->DestroyWindow();
		}
		m_buttonvec.clear();

		parseButtonInfo2(m_index);
		CString value;
		value.Format(_T("%d"), m_index);
		::WritePrivateProfileString(_T("debug"), _T("buttonIndex"), value, _T("guiconsole.ini"));
	}
}
