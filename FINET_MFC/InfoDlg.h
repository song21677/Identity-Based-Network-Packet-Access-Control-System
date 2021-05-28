#pragma once


// InfoDlg 대화 상자

class InfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~InfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINET_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMacBtn();
	afx_msg void OnBnClickedBioBtn();
	afx_msg void OnEnChangeMacEdit();
	afx_msg void OnEnChangeBioEdit();
	afx_msg void OnEnChangePwEdit();
	CEdit mac_edit;
	//char* getMAC();
	CEdit bio_edit;
};
