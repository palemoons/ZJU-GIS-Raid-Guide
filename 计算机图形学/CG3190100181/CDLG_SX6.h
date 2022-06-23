#pragma once


// CDLG_SX6 对话框

class CDLG_SX6 : public CDialog
{
	DECLARE_DYNAMIC(CDLG_SX6)

public:
	CDLG_SX6(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX6();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	BOOL OnInitDialog();

	CDC* pDC;//设备上下文

	CPoint ptList[50]; //多边形节点列表

	int iNum;//节点个数

	CPoint OldPt;//动态线顶点

	bool bOldLine;//动态线擦除标识（true为需要擦除，false为不需要）

	bool bFlag;//多边形绘制状态标识（true为已完成，false为未完成）

	bool DrawLine(CPoint point);//绘制动态线

	bool ClearLine();//擦除动态线

	bool bIsPtInRegion(int ix, int iy);//射线法判断点在多边形内外

	bool FillRegion();//填充多边形

	bool FloodFill(int x, int y, COLORREF OldColor, COLORREF NewColor);//种子点递归填充
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
