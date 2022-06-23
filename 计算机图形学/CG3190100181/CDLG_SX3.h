#pragma once


// CDLG_SX3 对话框

class CDLG_SX3 : public CDialog
{
	DECLARE_DYNAMIC(CDLG_SX3)

public:
	CDLG_SX3(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CDC* pDC;//获取上下文操作句柄
	int x0, y0, x1, y1;
	int flag;//直线绘制状态标记
public:
	BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool DrawLine(int x0, int y0, int x1, int y1);
	bool DDADrawLine(int x0, int y0, int x1, int y1);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
