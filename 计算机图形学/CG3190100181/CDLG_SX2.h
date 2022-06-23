#pragma once


// CDLG_SX2 对话框

class CDLG_SX2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDLG_SX2)

public:
	CDLG_SX2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLG_SX2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SX2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float fAngle;
	float fSpeed;
	float fXSpeed;
	float fYSpeed;
	float fXpos;
	float fYpos;
	float fG;
	float fPI;
	float fTime;
	float fFPS;

	CDC* pDC;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBnLanuch();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
