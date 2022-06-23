// CDLG_SX8.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX8.h"
#include "afxdialogex.h"


// CDLG_SX8 对话框

IMPLEMENT_DYNAMIC(CDLG_SX8, CDialog)

CDLG_SX8::CDLG_SX8(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SX8, pParent)
	, iOption(0)
{

}

CDLG_SX8::~CDLG_SX8()
{
}

void CDLG_SX8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, iOption);
}


BEGIN_MESSAGE_MAP(CDLG_SX8, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CDLG_SX8 消息处理程序


BOOL CDLG_SX8::OnInitDialog()
{
	CDialog::OnInitDialog();

	iOption = 0;
	pDC = this->GetDC();
	for (int i = 0; i < 50; i++) ptList[i].x = ptList[i].y = 0;
	iNum = 0;
	OldPt = CPoint(0, 0);
	bOldLine = false;
	bPolyLine = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDLG_SX8::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!bPolyLine) {
		if (iNum >= 49) {
			MessageBox(_T("最多保存50个节点，请双击结束"));
			return;
		}
	}
	ptList[iNum] = point;
	iNum++;

	CDialog::OnLButtonDown(nFlags, point);
}


void CDLG_SX8::OnMouseMove(UINT nFlags, CPoint point)
{
	if (iNum != 0 && !bPolyLine) {
		CPoint LastPT = ptList[iNum - 1];
		if (bOldLine) {
			int nOldROP = pDC->SetROP2(R2_NOTXORPEN);
			pDC->MoveTo(LastPT);
			pDC->LineTo(OldPt);
			pDC->SetROP2(nOldROP);
			bOldLine = false;
		}
		int nOldROP = pDC->SetROP2(R2_NOTXORPEN);
		pDC->MoveTo(LastPT);
		pDC->LineTo(point);
		pDC->SetROP2(nOldROP);
		bOldLine = true;
		OldPt = point;
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CDLG_SX8::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!bPolyLine && iNum > 2) {
		pDC->MoveTo(ptList[iNum - 2]);
		pDC->LineTo(ptList[iNum - 1]);
		bPolyLine = true;
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CDLG_SX8::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialog::OnPaint()
	for (int i = 0; i < iNum - 1; i++) {
		pDC->MoveTo(ptList[i]);
		pDC->LineTo(ptList[i + 1]);
	}
}


BOOL CDLG_SX8::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateData(true);
	//放大
	if (iOption == 1) {
		for (int i = 0; i < iNum; i++) {
			ptList[i].x *= 1.1;
			ptList[i].y *= 1.1;
		}
		Invalidate();
	}
	//缩小
	else if (iOption == 2) {
		for (int i = 0; i < iNum; i++) {
			ptList[i].x *= 0.9;
			ptList[i].y *= 0.9;
		}
		Invalidate();
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CDLG_SX8::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		//A:左移
		if (pMsg->wParam == 65) {
			for (int i = 0; i < iNum; i++) ptList[i].x -= 10;
			Invalidate();
		}
		//D:右移
		if (pMsg->wParam == 68) {
			for (int i = 0; i < iNum; i++) ptList[i].x += 10;
			Invalidate();
		}
		//W:上移
		if (pMsg->wParam == 87) {
			for (int i = 0; i < iNum; i++) ptList[i].y -= 10;
			Invalidate();
		}
		//S:下移
		if (pMsg->wParam == 83) {
			for (int i = 0; i < iNum; i++) ptList[i].y += 10;
			Invalidate();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
