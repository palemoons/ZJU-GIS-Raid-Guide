// CDLG_SX3.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX3.h"
#include "afxdialogex.h"


// CDLG_SX3 对话框

IMPLEMENT_DYNAMIC(CDLG_SX3, CDialog)

CDLG_SX3::CDLG_SX3(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SX3, pParent)
{

}

CDLG_SX3::~CDLG_SX3()
{
}

void CDLG_SX3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLG_SX3, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDLG_SX3 消息处理程序


BOOL CDLG_SX3::OnInitDialog()
{
	CDialog::OnInitDialog();

	pDC = this->GetDC();
	x0 = 0;
	y0 = 0;
	x1 = 0;
	y1 = 0;
	flag = 0;

	return TRUE;
}


void CDLG_SX3::OnLButtonDown(UINT nFlags, CPoint point)
{
	flag++;

	if (flag == 1) {
		x0 = point.x;
		y0 = point.y;
		CString str;
		str.Format(_T("X:%d, Y：%d"), x0, y0);
		this->GetDlgItem(IDC_EDIT_START)->SetWindowText(str);
		this->GetDlgItem(IDC_EDIT_END)->SetWindowText(_T(""));
	}
	else if (flag == 2) {
		x1 = point.x;
		y1 = point.y;
		flag = 0;
		CString str;
		str.Format(_T("X:%d, Y：%d"), x1, y1);
		this->GetDlgItem(IDC_EDIT_END)->SetWindowText(str);

		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);

		LARGE_INTEGER strat_PerformanceCount1;
		QueryPerformanceCounter(&strat_PerformanceCount1);
		for (int i = 0; i < 1000; i++) {
			DrawLine(x0, y0, x1, y1);
		}
		LARGE_INTEGER end_PerformanceCount1;
		QueryPerformanceCounter(&end_PerformanceCount1);
		DOUBLE run_time1 = (end_PerformanceCount1.QuadPart - strat_PerformanceCount1.QuadPart) / (DOUBLE)Frequency.QuadPart;

		LARGE_INTEGER strat_PerformanceCount2;
		QueryPerformanceCounter(&strat_PerformanceCount2);

		for (int i = 0; i < 1000; i++) {
			DDADrawLine(x0, y0, x1, y1);
		}

		LARGE_INTEGER end_PerformanceCount2;
		QueryPerformanceCounter(&end_PerformanceCount2);

		DOUBLE run_time2 = (end_PerformanceCount2.QuadPart - strat_PerformanceCount2.QuadPart) / (DOUBLE)Frequency.QuadPart;
		CString string;
		string.Format(_T("中点算法运行时间：%lf\nDDA算法运行时间：%lf"), run_time1, run_time2);
		MessageBox(string);

	}

	CDialog::OnLButtonDown(nFlags, point);
}


bool CDLG_SX3::DrawLine(int x0, int y0, int x1, int y1)
{
	COLORREF color = RGB(255, 0, 0);
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = dx - 2 * dy;
	int delta1 = -2 * dy;//di>0
	int delta2 = -2 * (dy - dx);//di<=0
	//reverse
	int dr = dy - 2 * dx;
	int deltaR1 = -2 * dx;
	int deltaR2 = -2 * (dx - dy);

	if (dx == 0) {//斜率为0
		if (dy > 0) for (int y = y0; y <= y1; pDC->SetPixel(x0, y, color), y++);
		else for (int y = y0; y >= y1; pDC->SetPixel(x0, y, color), y--);
	}
	if (dx > 0) {
		if (dy >= 0) {// m >= 0
			if (dy / dx == 0) {// m < 1
				for (int x = x0, y = y0; x <= x1; pDC->SetPixel(x, y, color)) {
					if (d > 0) d += delta1;
					else {
						d += delta2;
						y++;
					}
					x++;
				}
			}
			else {// y轴方向扫描
				for (int y = y0, x = x0; y <= y1; pDC->SetPixel(x, y, color)) {
					if (dr > 0) dr += deltaR1;
					else {
						dr += deltaR2;
						x++;
					}
					y++;
				}
			}
		}
		else {
			y1 = 2 * y0 - y1;
			dy *= -1;
			if (dy / dx == 0) {
				d = dx - 2 * dy;
				delta1 = -2 * dy;
				delta2 = -2 * (dy - dx);
				for (int x = x0, y = y0; x <= x1; pDC->SetPixel(x, 2 * y0 - y, color)) {
					if (d > 0) d += delta1;
					else {
						d += delta2;
						y++;
					}
					x++;
				}
			}
			else {// y轴方向扫描
				dr = dy - 2 * dx;
				deltaR1 = -2 * dx;
				deltaR2 = -2 * (dx - dy);
				for (int y = y0, x = x0; y <= y1; pDC->SetPixel(x, 2 * y0 - y, color)) {
					if (dr > 0) dr += deltaR1;
					else {
						dr += deltaR2;
						x++;
					}
					y++;
				}
			}
		}
	}
	else DrawLine(x1, y1, x0, y0);

	return true;
}

bool CDLG_SX3::DDADrawLine(int x0, int y0, int x1, int y1) {
	if (x0 >= x1) DDADrawLine(x1, y1, x0, y0);
	else {
		double dx = (double)x1 - x0;
		double dy = (double)y1 - y0;
		COLORREF color = RGB(255, 0, 0);
		if (dx == 0) {
			if (dy > 0) for (int y = y0; y <= y1; pDC->SetPixel(x0, y, color), y++);
			else for (int y = y0; y >= y1; pDC->SetPixel(x0, y, color), y--);
		}
		else if (abs(dy / dx) <= 1) {
			for (double x = x0, y = y0; x <= x1; x++, y += dy / dx)
				pDC->SetPixel(x, y, color);
		}
		else {
			if (dy >= 0) {
				for (double x = x0, y = y0; y <= y1; y++, x += dx / dy)
					pDC->SetPixel(x, y, color);
			}
			else {
				for (double x = x0, y = y0; y >= y1; y--, x -= dx / dy)
					pDC->SetPixel(x, y, color);
			}
		}
	}
	return true;
}

void CDLG_SX3::OnMouseMove(UINT nFlags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	CString str;
	str.Format(_T("X: %d, Y:%d"), x, y);
	this->GetDlgItem(IDC_EDIT_CURRENT)->SetWindowText(str);

	CDialog::OnMouseMove(nFlags, point);
}
