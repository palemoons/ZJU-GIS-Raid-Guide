// CDLG_SX7.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX7.h"
#include "afxdialogex.h"


// CDLG_SX7 对话框

IMPLEMENT_DYNAMIC(CDLG_SX7, CDialog)

CDLG_SX7::CDLG_SX7(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SX7, pParent)
{

}

CDLG_SX7::~CDLG_SX7()
{
}

void CDLG_SX7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLG_SX7, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLG_SX7::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDLG_SX7::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDLG_SX7::OnBnClickedButton3)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDLG_SX7 消息处理程序


BOOL CDLG_SX7::OnInitDialog()
{
	CDialog::OnInitDialog();

	pDC = this->GetDC();

	//初始化顶点坐标
	iLeft = iRight = iTop = iBottom = 0;
	x1 = x2 = y1 = y2 = 0;

	//初始化绘图状态
	iStatus = 0;

	//初始化动态线、动态矩形擦除标识
	bOldLine = bOldRect = false;

	//初始化鼠标左键按键状态
	bBtnDown = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CDLG_SX7::DrawLine() {
	int nOldRop = pDC->SetROP2(R2_NOTXORPEN);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);

	pDC->SetROP2(nOldRop);

	return true;
}

bool CDLG_SX7::DrawRect() {
	int nOldRop = pDC->SetROP2(R2_NOTXORPEN);

	int iMinX = iLeft < iRight ? iLeft : iRight;
	int iMaxX= iLeft > iRight ? iLeft : iRight;
	int iMinY = iTop < iBottom ? iTop : iBottom;
	int iMaxY = iTop > iBottom ? iTop : iBottom;

	pDC->MoveTo(iMinX, iMinY);
	pDC->LineTo(iMaxX, iMinY);
	pDC->LineTo(iMaxX, iMaxY);
	pDC->LineTo(iMinX, iMaxY);
	pDC->LineTo(iMinX, iMinY);

	pDC->SetROP2(nOldRop);

	return true;
}

bool CDLG_SX7::ClearOldLine() {
	DrawLine();
	bOldLine = false;
	return true;
}

bool CDLG_SX7::ClearOldRect() {
	DrawRect();
	bOldRect = false;
	return true;
}

bool CDLG_SX7::Clip() {
	int isVisible = -1;//0显然不可见  1完全可见 -1需要求交

	int iMinX = iLeft < iRight ? iLeft : iRight;
	int iMaxX = iLeft > iRight ? iLeft : iRight;
	int iMinY = iTop < iBottom ? iTop : iBottom;
	int iMaxY = iTop > iBottom ? iTop : iBottom;

	int code1, code2;
	code1 = (y1 > iMaxY ? 8 : 0) + (y1 < iMinY ? 4 : 0) + (x1 > iMaxX ? 2 : 0) + (x1 < iMinX ? 1 : 0);
	code2 = (y2 > iMaxY ? 8 : 0) + (y2 < iMinY ? 4 : 0) + (x2 > iMaxX ? 2 : 0) + (x2 < iMinX ? 1 : 0);

	if (code1 == 0 && code2 == 0) isVisible = 1;
	else if ((code1 & code2) != 0) isVisible = 0;

	if (isVisible == 0) x1 = x2 = y1 = y2 = 0;
	if (isVisible == -1) {
		//求交测试
		int code3 = code1 | code2;
		int binary[4], isCross[4] = { 0,0,0,0 };
		CPoint insection[4];
		for (int i = 3; i >= 0; i--) {
			binary[i] = code3 % 2;
			code3 /= 2;
		}
		if (binary[0] == 1) {//上
			int tmp = (int)(x1 + (double)(x2 - x1) / (y2 - y1) * (iMaxY - y1));
			if (tmp >= iMinX && tmp <= iMaxX) {
				insection[0].x = tmp;
				insection[0].y = iMaxY;
				isCross[0] = 1;
			}
		}
		if (binary[1] == 1) {//下
			int tmp = (int)(x1 + (double)(x2 - x1) / (y2 - y1) * (iMinY - y1));
			if (tmp >= iMinX && tmp <= iMaxX) {
				insection[1].x = tmp;
				insection[1].y = iMinY;
				isCross[1] = 1;
			}
				
		}
		if (binary[2] == 1) {
			int tmp = (int)(y1 + (double)(y2 - y1) / (x2 - x1) * (iMaxX - x1));
			if (tmp >= iMinY && tmp <= iMaxY) {
				insection[2].x = iMaxX;
				insection[2].y = tmp;
				isCross[2] = 1;
			}
		}
		if (binary[3] == 1) {
			int tmp = (int)(y1 + (double)(y2 - y1) / (x2 - x1) * (iMinX - x1));
			if (tmp >= iMinY && tmp <= iMaxY) {
				insection[3].x = iMinX;
				insection[3].y = tmp;
				isCross[3] = 1;
			}
		}

		if (code1 == 0) {
			int i;
			for (i = 0; isCross[i] == 0; i++);
			x2 = insection[i].x;
			y2 = insection[i].y;
		}
		else if (code2 == 0) {
			int i;
			for (i = 0; isCross[i] == 0; i++);
			x1 = insection[i].x;
			y1 = insection[i].y;
		}
		else {
			int i;
			for (i = 0; isCross[i] == 0; i++);
			x1 = insection[i].x;
			y1 = insection[i].y;
			for (i++; isCross[i] == 0; i++);
			x2 = insection[i].x;
			y2 = insection[i].y;
		}
	}
	return true;
}

void CDLG_SX7::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	iStatus = 1;
}


void CDLG_SX7::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	iStatus = 2;
}


void CDLG_SX7::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearOldLine();
	Clip();
	if (!bOldLine) DrawLine();
}


void CDLG_SX7::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (iStatus == 1) {
		if (bOldRect) ClearOldRect();
		iLeft = point.x;
		iTop = point.y;
	}
	else if (iStatus == 2) {
		if (bOldLine) ClearOldLine();
		x1 = point.x;
		y1 = point.y;
	}
	bBtnDown = true;

	CDialog::OnLButtonDown(nFlags, point);
}


void CDLG_SX7::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bBtnDown = false;

	CDialog::OnLButtonUp(nFlags, point);
}


void CDLG_SX7::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (iStatus == 1 && bBtnDown == true) {
		if (bOldRect) ClearOldRect();

		iRight = point.x;
		iBottom = point.y;

		DrawRect();

		bOldRect = true;
	}
	if (iStatus == 2 && bBtnDown == true) {
		if (bOldLine) ClearOldLine();

		x2 = point.x;
		y2 = point.y;

		DrawLine();

		ptOldLine = point;
		bOldLine = true;
	}

	CDialog::OnMouseMove(nFlags, point);
}
