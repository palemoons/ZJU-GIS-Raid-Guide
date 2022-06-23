// CDLG_SX2.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX2.h"
#include "afxdialogex.h"


// CDLG_SX2 对话框

IMPLEMENT_DYNAMIC(CDLG_SX2, CDialogEx)

CDLG_SX2::CDLG_SX2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SX2, pParent)
	, fAngle(0)
	, fSpeed(0)
{

}

CDLG_SX2::~CDLG_SX2()
{
}

void CDLG_SX2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANGLE, fAngle);
	DDX_Text(pDX, IDC_EDIT_SPEED, fSpeed);
}


BEGIN_MESSAGE_MAP(CDLG_SX2, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BN_LANUCH, &CDLG_SX2::OnBnClickedBnLanuch)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDLG_SX2 消息处理程序


BOOL CDLG_SX2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	fG = 9.8;
	fPI = 3.1415926;
	fFPS = 20;
	pDC = this->GetDC();//获取设备上下文

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDLG_SX2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//绘制目标靶
	pDC->Ellipse(550, 300, 580, 330);
	pDC->Ellipse(550, 305, 575, 325);
	pDC->Ellipse(550, 310, 570, 320);

	//绘制界限框
	pDC->MoveTo(0, 0);
	pDC->LineTo(600, 0);
	pDC->LineTo(600, 400);
	pDC->LineTo(0, 400);
	pDC->LineTo(0, 0);
}


void CDLG_SX2::OnBnClickedBnLanuch()
{
	//获取界面空间数值
	UpdateData(TRUE);

	//清屏
	Invalidate(TRUE);

	//分解速度
	fXSpeed = fSpeed * cos(fAngle * fPI / float(180));
	fYSpeed = fSpeed * sin(fAngle * fPI / float(180));

	//初始化位置和时间
	fXpos = 0;
	fYpos = 0;
	fTime = 0;

	//初始化绘图点
	pDC->MoveTo(0, 400);

	//启动定时器
	SetTimer(1, fFPS, NULL);

}


void CDLG_SX2::OnTimer(UINT_PTR nIDEvent)
{
	fXpos = fXSpeed * fTime;
	fYpos = fYSpeed * fTime - 0.5 * fG * fTime * fTime;
	
	pDC->LineTo(fXpos, 400 - fYpos);

	if (fXpos < 0 || fXpos>600 || fYpos < 0 || fYpos>400) {
		KillTimer(1);
		MessageBox(_T("Whoops!脱靶了"));
	}
	else if (fXpos > 550 && fXpos < 580 && fYpos > 70 && fYpos < 100) {
		KillTimer(1);
		MessageBox(_T("Yeah!打中了"));
	}
	fTime += fFPS / 1000.0;

	CDialogEx::OnTimer(nIDEvent);
}
