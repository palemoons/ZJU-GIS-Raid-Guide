#pragma once


// CDLG_SX4 对话框

class CDLG_SX4 : public CDialogEx
{
	DECLARE_DYNAMIC(CDLG_SX4)

public:
	CDLG_SX4(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX4();

	//设备上下文句柄
	CDC* pDC;

	//记录矩形左上角和右下角两个顶点的坐标
	int x1, y1, x2, y2;

	//鼠标案件状态标识
	bool bFlag;

	//是否擦除矩形标识
	bool bOldRect;

	//绘制矩阵函数
	bool DrawRect();

	//填充矩阵函数
	bool FillRect();

	//擦除矩阵函数
	bool ClearOldRect();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
