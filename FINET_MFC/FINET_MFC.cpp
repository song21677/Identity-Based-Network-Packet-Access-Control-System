
// FINET_MFC.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "FINET_MFC.h"
#include "FINET_MFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFINETMFCApp

BEGIN_MESSAGE_MAP(CFINETMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFINETMFCApp 생성

CFINETMFCApp::CFINETMFCApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CFINETMFCApp 개체입니다.

CFINETMFCApp theApp;


// CFINETMFCApp 초기화

BOOL CFINETMFCApp::InitInstance()
{
	CWinApp::InitInstance();
	CFINETMFCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}