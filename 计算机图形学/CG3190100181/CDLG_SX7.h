#pragma once


// CDLG_SX7 对话框

class CDLG_SX7 : public CDialog
{
	DECLARE_DYNAMIC(CDLG_SX7)

public:
	CDLG_SX7(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX7();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CDC* pDC;//设置上下文句柄

	//裁剪矩形坐标定点
	int iLeft, iRight, iTop, iBottom;

	//直线段顶点坐标
	int x1, y1, x2, y2;

	CPoint ptOldLine;//动态线端点坐标

	bool bOldRect;//是否需要擦除矩形

	bool bOldLine;//是否需要擦除直线

	bool bBtnDown;//鼠标按键状态

	int iStatus;//绘图状态：1画矩形 2画直线

	bool DrawLine();//画直线

	bool ClearOldLine();//擦除直线段

	bool DrawRect();//绘制矩形

	bool ClearOldRect();//擦除矩形

	bool Clip();//裁剪
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
