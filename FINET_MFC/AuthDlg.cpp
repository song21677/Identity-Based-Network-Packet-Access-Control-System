// AuthDlg.cpp: 구현 파일
//

#include "pch.h"
#include "FINET_MFC.h"
#include "AuthDlg.h"
#include "afxdialogex.h"

#include "../FIDOFunction/FIDOFunctionAPI.h" 
#pragma comment( lib, "FIDOFunction.lib" )

// AuthDlg 대화 상자

IMPLEMENT_DYNAMIC(AuthDlg, CDialogEx)

AuthDlg::AuthDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINET_AUTH, pParent)
{

}

AuthDlg::~AuthDlg()
{
}

void AuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AuthDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &AuthDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ID_BTN, &AuthDlg::OnBnClickedIdBtn)
END_MESSAGE_MAP()


// AuthDlg 메시지 처리기

void AuthDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

#include "AuthID_Dlg.h"


void AuthDlg::OnInitialUpdate()
{
	CString id, pw;
	int check = 1, result = 3;
	for (int i = 1; i <= 3; i++) {
		result = fido(id, pw, check);
		if (result == 1) MessageBox(TEXT("지문인식 성공"));
		else MessageBox(TEXT("지문인식 실패"));
	}
}

// 지문인증이 안되시나요?
void AuthDlg::OnBnClickedIdBtn()
{
	AuthID_Dlg AuthID_dlg;
	//_t2 = thread(&AuthID_Dlg::DoModal, AuthID_dlg);
	AuthID_dlg.DoModal();

	/*
	AuthDlg* m_Authdlg;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Authdlg = new AuthDlg(this);

	//비정형 대화상자 생성
	m_Authdlg->Create(IDD_FINET_AUTH_ID, this);

	//화면 출력
	m_Authdlg->ShowWindow(SW_SHOW);
	*/

}