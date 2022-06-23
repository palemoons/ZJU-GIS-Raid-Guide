// CDLG_SX4.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX4.h"
#include "afxdialogex.h"


// CDLG_SX4 对话框

IMPLEMENT_DYNAMIC(CDLG_SX4, CDialogEx)

CDLG_SX4::CDLG_SX4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SX4, pParent)
{

}

CDLG_SX4::~CDLG_SX4()
{
}

void CDLG_SX4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLG_SX4, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDLG_SX4 消息处理程序


BOOL CDLG_SX4::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 获取设备上下文
	pDC = this->GetDC();

	//初始化坐标变量
	x1 = y1 = x2 = y2 = 0;

	//初始化鼠标状态标识(即鼠标按键未按下)
	bFlag = false;
	
	//初始化擦除老矩阵标记(即当前没有绘制过的矩形需要擦除)
	bOldRect = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDLG_SX4::OnLButtonDown(UINT nFlags, CPoint point)
{
	//清屏
	Invalidate(true);

	//如果已存在绘制过的矩形，则将其擦除
	if (bOldRect) {
		ClearOldRect();
		bOldRect = false;
	}

	//记录矩阵的一个顶点坐标
	x1 = point.x;
	y1 = point.y;

	//标记左键已经按下
	bFlag = true;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDLG_SX4::OnMouseMove(UINT nFlags, CPoint point)
{
	//当左键按下时
	if (bFlag) {
		//如果有已经绘制过的矩形，则将其擦除
		if (bOldRect) ClearOldRect();

		//实时记录鼠标当前坐标，作为矩阵的另一个顶点
		x2 = point.x;
		y2 = point.y;

		//绘制新的矩形
		DrawRect();

		//标记已经存在绘制过的矩形
		bOldRect = true;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CDLG_SX4::OnLButtonUp(UINT nFlags, CPoint point)
{
	//标记鼠标左键已经弹起
	bFlag = false;

	//此时拖拽过程已经结束，填充矩形
	FillRect();

	CDialogEx::OnLButtonUp(nFlags, point);
}

bool CDLG_SX4::DrawRect() {
	//设置绘图模式为反色
	int nOldRop = pDC->SetROP2(R2_NOTXORPEN);

	//得到正确的矩形边界
	int iMinX = x1 < x2 ? x1 : x2;
	int iMinY = y1 < y2 ? y1 : y2;
	int iMaxX = x1 > x2 ? x1 : x2;
	int iMaxY = y1 > y2 ? y1 : y2;

	//绘制矩形边界
	pDC->MoveTo(iMinX, iMinY);
	pDC->LineTo(iMaxX, iMinY);
	pDC->LineTo(iMaxX, iMaxY);
	pDC->LineTo(iMinX, iMaxY);
	pDC->LineTo(iMinX, iMinY);

	//恢复原绘图模式
	pDC->SetROP2(nOldRop);

	return false;
}

bool CDLG_SX4::FillRect() {
	COLORREF color = RGB(255, 0, 0);
	int iMinX = x1 < x2 ? x1 : x2;
	int iMinY = y1 < y2 ? y1 : y2;
	int iMaxX = x1 > x2 ? x1 : x2;
	int iMaxY = y1 > y2 ? y1 : y2;

	pDC->MoveTo(iMinX, iMinY);
	/*
	for (int i = iMinY; i <= iMaxY; i++) {
		for (int j = iMinX; j <= iMaxX; j++)
			pDC->SetPixel(j, i, color);
	}
	*/

	for (int i = iMinY; i <= iMaxY;) {
		pDC->LineTo(iMinX, i);
		pDC->MoveTo(iMaxX, ++i);
	}
	return false;
}

bool CDLG_SX4::ClearOldRect() {
	DrawRect();
	return false;
}