// dllmain.cpp : DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//
#undef _WINDOWS_
#include "pch.h"
#include "framework.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE FIDOFunctionDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved�� ����ϴ� ��� ������ �����Ͻʽÿ�.
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FIDOFunction.DLL�� �ʱ�ȭ�ϰ� �ֽ��ϴ�.\n");

		// Ȯ�� DLL�� �� ���� �ʱ�ȭ�մϴ�.
		if (!AfxInitExtensionModule(FIDOFunctionDLL, hInstance))
			return 0;

		// �� DLL�� ���ҽ� ü�ο� �����մϴ�.
		// ����: �� Ȯ�� DLL�� MFC ���� ���α׷���
		//  �ƴ� ActiveX ��Ʈ�Ѱ� ���� MFC �⺻ DLL��
		//  ���� ��������� ��ũ�Ǿ� �ִ� ��쿡��
		//  DllMain���� �� ���� �����ϰ�, ������ ���� �� Ȯ�� DLL����
		//  ������ ������ �Լ��� �߰��մϴ�.
		//  �׷� ���� �� Ȯ�� DLL�� ����ϴ� �⺻ DLL��
		//  �ش� �Լ��� ��������� ȣ���Ͽ� �� Ȯ�� DLL�� �߰��ؾ� �մϴ�.
		//  �׷��� ������ CDynLinkLibrary ��ü��
		//  �⺻ DLL�� ���ҽ� ü�ο� �߰����� �����Ƿ�
		//  �ɰ��� ������ �߻��մϴ�.

		new CDynLinkLibrary(FIDOFunctionDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FIDOFunction.DLL�� �����ϰ� �ֽ��ϴ�.\n");

		// �Ҹ��ڰ� ȣ��Ǳ� ���� ���̺귯���� �����մϴ�.
		AfxTermExtensionModule(FIDOFunctionDLL);
	}
	return 1;   // Ȯ��
}

//////////////// �Է� �κ� //////////////// 
#ifndef _DLL_FIDO_ 
#define _DLL_FIDO_ 
#endif 
#include "FIDOFunctionAPI.h" 
#include "data.h"

DLL_FIDO int fido(CString id, CString pw, int check)
{

	int result = 0;
	if (check == 1) {
		result = Verify(WINBIO_SUBTYPE_ANY);
		return result;
	}
	if (check == 2) {
		result = Finet(id, pw);
		return result;
	}
}
//////////////// �Է� �κ� ////////////////


