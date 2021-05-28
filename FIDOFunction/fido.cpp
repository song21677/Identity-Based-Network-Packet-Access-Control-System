#include "pch.h"
#include "data.h"
#pragma warning(disable:4996)


// 현재 사용자 ID 검색 __inout : 표기만을 위한 매크로
HRESULT GetCurrentUserIdentity(__inout PWINBIO_IDENTITY Identity) {  // PWINBIO_IDENTITY : WINBIO_PIPELINE(단일 생체 센서 유닛, 엔진, 저장 어댑터에 의해 사용되는 공유 컨텍스트 정보를 포함하는 구조)
    HRESULT hr = S_OK;
    HANDLE tokenHandle = NULL; // 액세스 토큰에 열린 프로세스에 대한 핸들
    DWORD  byteReturned = 0;   // 액세스 토큰에 대해 요청된 액세스 유형을 지정
    struct _tokenInfoBuffer {
        TOKEN_USER tokenUser;
        BYTE buffer[SECURITY_MAX_SID_SIZE];
    } tokenInfoBuffer;

    // 입력 ID 0으로 설정
    ZeroMemory(Identity, sizeof(WINBIO_IDENTITY));  // 구조체 0으로 초기화
    Identity->Type = WINBIO_ID_TYPE_NULL;

    // 현제 프로세스와 연결된 토큰 열기
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &tokenHandle)) {  // 실패 > 반환값 = 0 // GetCurrentProcess() : 현재 프로세스에 대한 핸들을 검색.
        DWORD win32Status = GetLastError();      // GetLastError() : 호출 스레드의 마지막 오류 코드 값을 검색.                            
        wprintf_s(L"Can not Open Token Handle : %d\n", win32Status);
        hr = HRESULT_FROM_WIN32(win32Status);   // HRESULT_FROM_WIN32 : 매핑 시스템 오류 코드에 대한 HRESULT 값
        goto E_Exit;
    }

    // 토큰버퍼 구조를 0으로 설정
    ZeroMemory(&tokenInfoBuffer, sizeof(tokenInfoBuffer));

    // 액세스 토큰에 대한 정보를 검색한다. (SID검색)
    if (!GetTokenInformation(
        tokenHandle,
        TokenUser,
        &tokenInfoBuffer.tokenUser,
        sizeof(tokenInfoBuffer),
        &byteReturned))
    {
        DWORD win32Satus = GetLastError();
        wprintf_s(L"Can not Query Token Information : %d\n", win32Satus);
        hr = HRESULT_FROM_WIN32(win32Satus);
        goto E_Exit;
    }

    //tokenInfoBuffer 에서 SID를 WINBIO_IDENTITY 구조로 복사? 구조 다음으로 복사?
    CopySid(SECURITY_MAX_SID_SIZE, Identity->Value.AccountSid.Data, tokenInfoBuffer.tokenUser.User.Sid);  // CopySid() : SID(security identifier)의 버퍼를 복사

    //SID 크기를 지정하고 WINB를 할당
    Identity->Value.AccountSid.Size = GetLengthSid(tokenInfoBuffer.tokenUser.User.Sid);
    Identity->Type = WINBIO_ID_TYPE_SID;

E_Exit:

    if (tokenHandle != NULL) {
        CloseHandle(tokenHandle);
    }
    return hr;
}


string DataBases() {
    HRESULT hr = S_OK;
    PWINBIO_STORAGE_SCHEMA storageSchemaArray = NULL;
    SIZE_T storageCount = 0;
    SIZE_T index = 0;
    string guid = "";

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


string SaveGuid(__in PWINBIO_UUID Guid) {
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
    string guid(buffer);

    return guid;
}


// 생체인식 센서
int Verify(WINBIO_BIOMETRIC_SUBTYPE subFactor) {

    HRESULT hr = S_OK;
    WINBIO_SESSION_HANDLE sessionHandle = NULL;
    WINBIO_UNIT_ID unitId = 0;
    WINBIO_REJECT_DETAIL rejectDetail = 0;
    WINBIO_IDENTITY identity = { 0 };
    BOOLEAN match = FALSE;
    
    int result = 2;
    int option = 2;
    string guid = "";

    // 사용자 identity 찾기
    hr = GetCurrentUserIdentity(&identity);
    if (FAILED(hr)) {
        wprintf_s(L"User Identity not found. hr = 0x%x\n", hr);
        goto E_Exit;
    }

    // 시스템에 연결
    hr = WinBioOpenSession(
        WINBIO_TYPE_FINGERPRINT,
        WINBIO_POOL_SYSTEM,
        WINBIO_FLAG_DEFAULT,
        NULL,
        0,
        NULL,
        &sessionHandle
    );
    if (FAILED(hr)) {
        wprintf_s(L"\nWinBioOpenSession failed. hr = 0x%x\n", hr);
        goto E_Exit;
    }

    // 생체 인식 확인
    wprintf_s(L"\n** Calling WinBioVerify - Swip finger on sersor ... **\n");
    hr = WinBioVerify(     // 생체 인식 샘플을 캡쳐하고 사용자 ID에 해당하는지 확인
        sessionHandle,
        &identity,
        subFactor,
        &unitId,
        &match,
        &rejectDetail
    );
    wprintf_s(L"\nSwipe Processed _ Unit ID : %d\n", unitId);

    if (FAILED(hr)) {
        if (hr == WINBIO_E_NO_MATCH) {
            wprintf_s(L"\nNO MATCH _ Identity verification failed... \n");
        }
        else if (hr == WINBIO_E_BAD_CAPTURE) {
            wprintf_s(L"\nBAD CAPTURE _ Reason : %d\n", rejectDetail);
        }
        else {
            wprintf_s(L"\nWinBioVerify failed ... hr = 0x%x\n", hr);
        }
        option = 2; // option 2는 지문인식 실패
        guid = DataBases();
        result = Finet(option, guid);

        goto E_Exit;
    }

    option = 1; // option 1은 지문인식 성공
    wprintf_s(L"\nFingerprint Verified : %d \n", unitId);
    guid = DataBases();
    result = Finet(option, guid);

    return result;


E_Exit:
    if (sessionHandle != NULL) {  // 세션이 이미 열려 있으면
        WinBioCloseSession(sessionHandle);   // 세션 닫고
        sessionHandle = NULL;  // 세션 초기화
    }
    wprintf_s(L"\nClose..... bye\n");

    return 2;
}