#pragma once


// CDLG_SX1 对话框

class CDLG_SX1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDLG_SX1)

public:
	CDLG_SX1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int iPos;
	CSliderCtrl m_slider;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
};
