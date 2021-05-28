#pragma once


// AuthID_Dlg 대화 상자

class AuthID_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(AuthID_Dlg)

public:
	AuthID_Dlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AuthID_Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINET_AUTH_ID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeIdEdit();
	CEdit InputID;
	CEdit InputPW;
	afx_msg void OnEnChangePwEdit();
	afx_msg void OnBnClickedOk();
};
