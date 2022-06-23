// CDLG_SX1.cpp: 实现文件
//

#include "pch.h"
#include "CG3190100181.h"
#include "CDLG_SX1.h"
#include "afxdialogex.h"


// CDLG_SX1 对话框

IMPLEMENT_DYNAMIC(CDLG_SX1, CDialogEx)

CDLG_SX1::CDLG_SX1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SX1, pParent)
	, iPos(0)
{

}

CDLG_SX1::~CDLG_SX1()
{
}

void CDLG_SX1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, iPos);
	DDV_MinMaxInt(pDX, iPos, 0, 100);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CDLG_SX1, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDLG_SX1::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLG_SX1::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDLG_SX1 消息处理程序


BOOL CDLG_SX1::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化位置变量为0
	iPos = 0;

	//设置滑动条控件数据范围并将位置初始化为0
	m_slider.SetRange(0, 100);
	m_slider.SetPos(0);

	//更新界面控件变量
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDLG_SX1::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	//获取当前滑动条位置
	iPos = m_slider.GetPos();

	//更新界面变量
	UpdateData(FALSE);

	*pResult = 0;
}


BOOL CDLG_SX1::PreTranslateMessage(MSG* pMsg)
{
	//判断是否接收到键盘按下消息
	if (WM_KEYDOWN == pMsg->message) {
		//获取文本编辑框控件指针
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);

		//判断是否在文本编辑框中按下回车键
		if (pMsg->hwnd == pEdit->GetSafeHwnd() && VK_RETURN == pMsg->wParam) {
			UpdateData(TRUE);

			//更新滑动条滑块位置
			if (iPos >= 0 && iPos <= 100) m_slider.SetPos(iPos);

			return true;
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDLG_SX1::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE, _T("PICTURE"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(*.bmp;*.jpg;*.tif)|*.bmp;*.jpg;*.tif||"));
	CString strFilePath;
	if (dlg.DoModal() == IDOK) strFilePath = dlg.GetPathName();
	else return;
	//使用CImage类读取选择的图片文件
	CImage image;
	image.Load(strFilePath);

	//获取picture control控件大小
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect);

	//获取picture control控件gdi设备上下文句柄
	CDC* pDC = GetDlgItem(IDC_PIC)->GetDC();

	//设置栅格图像缩放采样模式为高精度
	pDC->SetStretchBltMode(STRETCH_HALFTONE);

	//将图像内容绘制在控件上
	image.Draw(pDC->m_hDC, rect);

	//释放GDI设备资源
	ReleaseDC(pDC);
}
