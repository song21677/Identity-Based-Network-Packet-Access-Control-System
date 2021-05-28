#pragma once
#include <thread>
using namespace std;


// AuthDlg 대화 상자

class AuthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AuthDlg)

public:
	AuthDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AuthDlg();
	thread _t1, _t2;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINET_AUTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void Wait(DWORD dwMillisecond);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedIdBtn();
	//afx_msg void OnDestroy();
	//afx_msg void OnBnClickedIdBtn2();
	//afx_msg void OnStnClickedVerifyFingerprint();
};
