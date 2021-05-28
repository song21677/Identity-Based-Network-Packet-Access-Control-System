// InfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "FINET_MFC.h"
#include "InfoDlg.h"
#include "afxdialogex.h"
#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#include <winbio.h>
#pragma comment(lib, "Winbio.lib")
char* getMAC();
CString SaveGuid(__in PWINBIO_UUID Guid);
CString DataBases();

// InfoDlg 대화 상자

IMPLEMENT_DYNAMIC(InfoDlg, CDialogEx)

InfoDlg::InfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINET_INFO, pParent)
{

}

InfoDlg::~InfoDlg()
{
}

void InfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PW_EDIT, mac_edit);
    DDX_Control(pDX, IDC_BIO_EDIT, bio_edit);
}


BEGIN_MESSAGE_MAP(InfoDlg, CDialogEx)
	
	ON_BN_CLICKED(IDCANCEL, &InfoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MAC_BTN, &InfoDlg::OnBnClickedMacBtn)
	ON_BN_CLICKED(IDC_BIO_BTN, &InfoDlg::OnBnClickedBioBtn)
	ON_EN_CHANGE(IDC_MAC_EDIT, &InfoDlg::OnEnChangeMacEdit)
	ON_EN_CHANGE(IDC_BIO_EDIT, &InfoDlg::OnEnChangeBioEdit)
	ON_EN_CHANGE(IDC_PW_EDIT, &InfoDlg::OnEnChangePwEdit)
END_MESSAGE_MAP()


// InfoDlg 메시지 처리기



void InfoDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void InfoDlg::OnBnClickedMacBtn()
{
    char* macadd = getMAC();
    CString mac;
    mac = (LPCSTR)(LPCSTR)macadd;
	mac_edit.SetWindowTextW(mac);
    

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void InfoDlg::OnBnClickedBioBtn()
{
    CString guid = DataBases();
    bio_edit.SetWindowTextW(guid);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void InfoDlg::OnEnChangeMacEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// guid 값
void InfoDlg::OnEnChangeBioEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// mac 주소
void InfoDlg::OnEnChangePwEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

char* getMAC() {
    int num = 0;
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
    char* mac_addr = (char*)malloc(18);

    AdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    if (AdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        free(mac_addr);
        return NULL; // it is safe to call free(NULL)
    }

    // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(AdapterInfo);
        AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
        if (AdapterInfo == NULL) {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
            free(mac_addr);
            return NULL;
        }
    }

    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
        // Contains pointer to current adapter info
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
        do {
            num++;
            if (num == 5) break;
            // technically should look at pAdapterInfo->AddressLength
            //   and not assume it is 6.
            sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                pAdapterInfo->Address[0], pAdapterInfo->Address[1],
                pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
            // printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
            // print them all, return the last one.
            // return mac_addr;

            // printf("\n");
            pAdapterInfo = pAdapterInfo->Next;
        } while (pAdapterInfo);
    }
    free(AdapterInfo);
    return mac_addr; // caller must free.
}

CString SaveGuid(__in PWINBIO_UUID Guid) {
    char S_Guid[16];
    char buffer[64];

    sprintf(
        buffer,
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        Guid->Data1,
        Guid->Data2,
        Guid->Data3,
        Guid->Data4[0],
        Guid->Data4[1],
        Guid->Data4[2],
        Guid->Data4[3],
        Guid->Data4[4],
        Guid->Data4[5],
        Guid->Data4[6],
        Guid->Data4[7]
    );
    CString guid(buffer);

    return guid;
}


// 현재 사용자 ID 검색 __inout : 표기만을 위한 매크로
CString DataBases() {
    HRESULT hr = S_OK;
    PWINBIO_STORAGE_SCHEMA storageSchemaArray = NULL;
    SIZE_T storageCount = 0;
    SIZE_T index = 0;
    CString guid;

    hr = WinBioEnumDatabases(
        WINBIO_TYPE_FINGERPRINT,
        &storageSchemaArray,
        &storageCount
    );
    if (FAILED(hr)) {
        wprintf_s(L"\nWinBioEnumDatabases failed. hr = 0x%x\n", hr);
        WinBioFree(storageSchemaArray);
        storageSchemaArray = NULL;
    }
    guid = SaveGuid(&storageSchemaArray[0].DatabaseId);

    return guid;
}