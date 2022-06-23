
// CG3190100181Dlg.h: 头文件
//

#pragma once
#include "CDLG_SX1.h"
#include "CDLG_SX2.h"
#include "CDLG_SX3.h"
#include "CDLG_SX4.h"
#include "CDLG_SX5.h"
#include "CDLG_SX6.h"
#include "CDLG_SX7.h"
#include "CDLG_SX8.h"

// CCG3190100181Dlg 对话框
class CCG3190100181Dlg : public CDialogEx
{
// 构造
public:
	CCG3190100181Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CG3190100181_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;

	CDLG_SX1 SX1Dlg;
	CDLG_SX2 SX2Dlg;
	CDLG_SX3 SX3Dlg;
	CDLG_SX4 SX4Dlg;
	CDLG_SX5 SX5Dlg;
	CDLG_SX6 SX6Dlg;
	CDLG_SX7 SX7Dlg;
	CDLG_SX8 SX8Dlg;

	bool Layout();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
