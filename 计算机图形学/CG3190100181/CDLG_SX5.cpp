// CDLG_SX5.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX5.h"
#include "afxdialogex.h"


// CDLG_SX5 对话框

IMPLEMENT_DYNAMIC(CDLG_SX5, CDialog)

CDLG_SX5::CDLG_SX5(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SX5, pParent)
{

}

CDLG_SX5::~CDLG_SX5()
{
}

void CDLG_SX5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLG_SX5, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDLG_SX5 消息处理程序
BOOL CDLG_SX5::OnInitDialog() {
	CDialog::OnInitDialog();

	//获取设备上下文
	pDC = this->GetDC();

	//初始化多边形列表
	for (int i = 0; i < 50; i++) {
		ptList[i].x = 0;
		ptList[i].y = 0;
	}

	//初始化多边形节点
	iNum = 0;

	//初始化动态线端点
	OldPt = CPoint(0, 0);

	//初始化动态线标记为false
	bOldLine = false;

	//初始化多边形绘制状态标记为false
	bFlag = false;

	return TRUE;
}

bool CDLG_SX5::DrawLine(CPoint point) {
	//设置绘图模式
	int nRopMode = pDC->SetROP2(R2_NOTXORPEN);

	//绘制多边形最后一个顶点到最新定点的边
	pDC->MoveTo(ptList[iNum - 1]);
	pDC->LineTo(point);

	OldPt = point;

	//标记需要擦除动态线
	bOldLine = true;

	//设置绘图模式
	pDC->SetROP2(nRopMode);

	return true;
}

bool CDLG_SX5::ClearLine() {
	//反色绘制
	DrawLine(OldPt);

	bOldLine = false;

	return true;
}

bool CDLG_SX5::bIsPtInRegion(int ix, int iy) {
	int pointNum = 0;
	int iMaxX= ptList[0].x;
	for (int i = 0; i < iNum; i++) {
		if (ptList[i].x > iMaxX) iMaxX = ptList[i].x;
	}
	CPoint p1 = CPoint(ix, iy);
	CPoint p2 = CPoint(iMaxX, iy);
	for (int i = 0; i < iNum; i++) {
		CPoint p3 = ptList[i];
		CPoint p4 = ptList[i + 1];
		if (p3.y - p4.y != 0) {//计算4个点相对于对应线段的叉乘值
			int v3 = ((p3.x - p1.x) * (p2.y - p1.y)) - ((p2.x - p1.x) * (p3.y - p1.y));
			int v4 = ((p4.x - p1.x) * (p2.y - p1.y)) - ((p2.x - p1.x) * (p4.y - p1.y));
			int v1 = ((p1.x - p3.x) * (p4.y - p3.y)) - ((p4.x - p3.x) * (p1.y - p3.y));
			int v2 = ((p2.x - p3.x) * (p4.y - p3.y)) - ((p4.x - p3.x) * (p2.y - p3.y));

			if ((v1 <= 0 && v2 >= 0) || (v1 >= 0 && v2 <= 0)) {
				if ((v3 < 0 && v4 > 0) || (v3 > 0 && v4 < 0)) pointNum++;
				else if (v3 == 0 && p4.y < iy) pointNum++;//顶点在射线上视作点在射线的上方
				else if (v4 == 0 && p3.y < iy) pointNum++;
			}
		}
	}
	if (pointNum % 2 == 1) return true;
	return false;
}

bool CDLG_SX5::FillRegion() {
	//遍历多边形结点列表，找出多边形的外接矩形
	int iMinX, iMaxX, iMinY, iMaxY;
	iMinX = iMaxX = ptList[0].x;
	iMinY = iMaxY = ptList[0].y;
	for (int i = 0; i < iNum; i++) {
		if (ptList[i].x < iMinX) iMinX = ptList[i].x;
		if (ptList[i].x > iMaxX) iMaxX = ptList[i].x;
		if (ptList[i].y < iMinY) iMinY = ptList[i].y;
		if (ptList[i].y > iMaxY) iMaxY = ptList[i].y;
	}
	

	//遍历多边形外接矩形中的所有像素点
	for (int i = iMinX + 1; i < iMaxX; i++) {
		for (int j = iMinY + 1; j < iMaxY; j++) {
			if (bIsPtInRegion(i, j)) pDC->SetPixel(i, j, RGB(128, 128, 128));
		}
	}
	return true;
}

void CDLG_SX5::OnLButtonDown(UINT nFlags, CPoint point)
{
	//如果多边形已经绘制完成，则擦除
	if (bFlag) {
		Invalidate(TRUE);
		iNum = 0;
		bFlag = 0;
		bOldLine = false;
	}
	else {
		if (iNum >= 49) {
			SetWindowText(TEXT("最多保存50个节点，请双击"));
			return;
		}
		else ptList[iNum++] = point;
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CDLG_SX5::OnMouseMove(UINT nFlags, CPoint point)
{
	//如果多边形绘制未完成，且已有至少一个节点存在
	if (!bFlag && iNum > 0) {
		if (bOldLine) ClearLine();
		DrawLine(point);
	}

	CDialog::OnMouseMove(nFlags, point);
}



void CDLG_SX5::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//如果多边形绘制未完成，且至少有三个节点数已可以形成多边形
	if (!bFlag && iNum > 2) {
		ptList[iNum] = ptList[0];
		DrawLine(ptList[iNum]);

		//标记多边形绘制完成
		bFlag = true;

		FillRegion();
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}
