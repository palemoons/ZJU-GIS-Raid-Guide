#pragma once


// CDLG_SX8 对话框

class CDLG_SX8 : public CDialog
{
	DECLARE_DYNAMIC(CDLG_SX8)

public:
	CDLG_SX8(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX8();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	int iOption;
	CDC* pDC;//设备上下文
	CPoint ptList[50];//折线段节点列表
	int iNum;//折线段节点数
	CPoint OldPt;//动态线端点
	bool bOldLine;//动态线擦除标记
	bool bPolyLine;//折线段绘制完成标识
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
