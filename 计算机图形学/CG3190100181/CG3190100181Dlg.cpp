
// CG3190100181Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CG3190100181.h"
#include "CG3190100181Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCG3190100181Dlg 对话框



CCG3190100181Dlg::CCG3190100181Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CG3190100181_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCG3190100181Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CCG3190100181Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CCG3190100181Dlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CCG3190100181Dlg 消息处理程序

BOOL CCG3190100181Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化选项卡控件
	m_tab.InsertItem(0, TEXT("CG1"));
	m_tab.InsertItem(1, TEXT("CG2"));
	m_tab.InsertItem(2, TEXT("CG3"));
	m_tab.InsertItem(3, TEXT("CG4"));
	m_tab.InsertItem(4, TEXT("CG5"));
	m_tab.InsertItem(5, TEXT("CG6"));
	m_tab.InsertItem(6, TEXT("CG7"));
	m_tab.InsertItem(7, TEXT("CG8"));

	//初始化实习一对话框
	SX1Dlg.Create(IDD_DIALOG_SX1);
	SX2Dlg.Create(IDD_DIALOG_SX2);

	//初始化实习二对话框
	SX3Dlg.Create(IDD_DIALOG_SX3);

	//初始化实习三对话框
	SX4Dlg.Create(IDD_DIALOG_SX4);

	//初始化实习四对话框
	SX5Dlg.Create(IDD_DIALOG_SX5);

	//初始化实习五对话框
	SX6Dlg.Create(IDD_DIALOG_SX6);

	//初始化实习六对话框
	SX7Dlg.Create(IDD_DIALOG_SX7);

	//初始化实习七对话框
	SX8Dlg.Create(IDD_DIALOG_SX8);

	//界面布局
	Layout();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCG3190100181Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCG3190100181Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCG3190100181Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CCG3190100181Dlg::Layout() {
	//获取窗口大小
	CRect rect;
	this->GetClientRect(&rect);

	//移动选项卡控件
	m_tab.MoveWindow(0, 0, rect.Width(), rect.Height() - 40);

	//移动实习一对话框
	SX1Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);
	SX2Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	//移动实习二对话框
	SX3Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	//移动实习三对话框
	SX4Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	//移动实习四对话框
	SX5Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	//移动实习五对话框
	SX6Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	//移动实习六对话框
	SX7Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	//移动实习七对话框
	SX8Dlg.MoveWindow(0, 40, rect.Width(), rect.Height() - 40);

	return true;
}

void CCG3190100181Dlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//获取当前选项卡焦点
	int iCurSel = m_tab.GetCurSel();
	//如果选择选项卡1，显示实习一对话框
	if (iCurSel == 0) {
		SX1Dlg.ShowWindow(SW_SHOW);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 1) {
		SX2Dlg.ShowWindow(SW_SHOW);
		SX1Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 2) {
		SX1Dlg.ShowWindow(SW_HIDE);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_SHOW);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 3) {
		SX1Dlg.ShowWindow(SW_HIDE);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_SHOW);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 4) {
		SX1Dlg.ShowWindow(SW_HIDE);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_SHOW);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 5) {
		SX1Dlg.ShowWindow(SW_HIDE);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_SHOW);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 6) {
		SX1Dlg.ShowWindow(SW_HIDE);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_SHOW);
		SX8Dlg.ShowWindow(SW_HIDE);
	}
	else if (iCurSel == 7) {
		SX1Dlg.ShowWindow(SW_HIDE);
		SX2Dlg.ShowWindow(SW_HIDE);
		SX3Dlg.ShowWindow(SW_HIDE);
		SX4Dlg.ShowWindow(SW_HIDE);
		SX5Dlg.ShowWindow(SW_HIDE);
		SX6Dlg.ShowWindow(SW_HIDE);
		SX7Dlg.ShowWindow(SW_HIDE);
		SX8Dlg.ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}
