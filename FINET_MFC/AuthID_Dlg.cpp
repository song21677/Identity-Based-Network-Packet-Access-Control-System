// AuthID_Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "FINET_MFC.h"
#include "AuthID_Dlg.h"
#include "afxdialogex.h"

#include "../FIDOFunction/FIDOFunctionAPI.h" 
#pragma comment( lib, "FIDOFunction.lib" )

// AuthID_Dlg 대화 상자

IMPLEMENT_DYNAMIC(AuthID_Dlg, CDialogEx)

AuthID_Dlg::AuthID_Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINET_AUTH_ID, pParent)
{

}

AuthID_Dlg::~AuthID_Dlg()
{
}

void AuthID_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_EDIT, InputID);
	DDX_Control(pDX, IDC_PW_EDIT, InputPW);
}


BEGIN_MESSAGE_MAP(AuthID_Dlg, CDialogEx)
	ON_EN_CHANGE(IDC_ID_EDIT, &AuthID_Dlg::OnEnChangeIdEdit)
	ON_EN_CHANGE(IDC_PW_EDIT, &AuthID_Dlg::OnEnChangePwEdit)
	ON_BN_CLICKED(IDOK, &AuthID_Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// AuthID_Dlg 메시지 처리기

// ID 입력 컨트롤
void AuthID_Dlg::OnEnChangeIdEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// PW 입력 컨트롤
void AuthID_Dlg::OnEnChangePwEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 확인 버튼
void AuthID_Dlg::OnBnClickedOk()
{
	int check = 2;

	CString ID = _T("");
	InputID.GetWindowTextW(ID);


	CString PW = _T("");
	InputPW.GetWindowTextW(PW);

	if (ID.IsEmpty() != TRUE && PW.IsEmpty() != TRUE) {
		int result = fido(ID, PW, check);
		if (result == 1) MessageBox(TEXT("성공"));
		else MessageBox(TEXT("실패"));
	}
	else MessageBox(TEXT("아이디 또는 비밀번호를 입력해주세요!"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
