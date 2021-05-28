#include "pch.h"
#include "data.h"
#pragma warning(disable:4996)


// ���� ����� ID �˻� __inout : ǥ�⸸�� ���� ��ũ��
HRESULT GetCurrentUserIdentity(__inout PWINBIO_IDENTITY Identity) {  // PWINBIO_IDENTITY : WINBIO_PIPELINE(���� ��ü ���� ����, ����, ���� ����Ϳ� ���� ���Ǵ� ���� ���ؽ�Ʈ ������ �����ϴ� ����)
    HRESULT hr = S_OK;
    HANDLE tokenHandle = NULL; // �׼��� ��ū�� ���� ���μ����� ���� �ڵ�
    DWORD  byteReturned = 0;   // �׼��� ��ū�� ���� ��û�� �׼��� ������ ����
    struct _tokenInfoBuffer {
        TOKEN_USER tokenUser;
        BYTE buffer[SECURITY_MAX_SID_SIZE];
    } tokenInfoBuffer;

    // �Է� ID 0���� ����
    ZeroMemory(Identity, sizeof(WINBIO_IDENTITY));  // ����ü 0���� �ʱ�ȭ
    Identity->Type = WINBIO_ID_TYPE_NULL;

    // ���� ���μ����� ����� ��ū ����
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &tokenHandle)) {  // ���� > ��ȯ�� = 0 // GetCurrentProcess() : ���� ���μ����� ���� �ڵ��� �˻�.
        DWORD win32Status = GetLastError();      // GetLastError() : ȣ�� �������� ������ ���� �ڵ� ���� �˻�.                            
        wprintf_s(L"Can not Open Token Handle : %d\n", win32Status);
        hr = HRESULT_FROM_WIN32(win32Status);   // HRESULT_FROM_WIN32 : ���� �ý��� ���� �ڵ忡 ���� HRESULT ��
        goto E_Exit;
    }

    // ��ū���� ������ 0���� ����
    ZeroMemory(&tokenInfoBuffer, sizeof(tokenInfoBuffer));

    // �׼��� ��ū�� ���� ������ �˻��Ѵ�. (SID�˻�)
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

    //tokenInfoBuffer ���� SID�� WINBIO_IDENTITY ������ ����? ���� �������� ����?
    CopySid(SECURITY_MAX_SID_SIZE, Identity->Value.AccountSid.Data, tokenInfoBuffer.tokenUser.User.Sid);  // CopySid() : SID(security identifier)�� ���۸� ����

    //SID ũ�⸦ �����ϰ� WINB�� �Ҵ�
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


// ��ü�ν� ����
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

    // ����� identity ã��
    hr = GetCurrentUserIdentity(&identity);
    if (FAILED(hr)) {
        wprintf_s(L"User Identity not found. hr = 0x%x\n", hr);
        goto E_Exit;
    }

    // �ý��ۿ� ����
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

    // ��ü �ν� Ȯ��
    wprintf_s(L"\n** Calling WinBioVerify - Swip finger on sersor ... **\n");
    hr = WinBioVerify(     // ��ü �ν� ������ ĸ���ϰ� ����� ID�� �ش��ϴ��� Ȯ��
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
        option = 2; // option 2�� �����ν� ����
        guid = DataBases();
        result = Finet(option, guid);

        goto E_Exit;
    }

    option = 1; // option 1�� �����ν� ����
    wprintf_s(L"\nFingerprint Verified : %d \n", unitId);
    guid = DataBases();
    result = Finet(option, guid);

    return result;


E_Exit:
    if (sessionHandle != NULL) {  // ������ �̹� ���� ������
        WinBioCloseSession(sessionHandle);   // ���� �ݰ�
        sessionHandle = NULL;  // ���� �ʱ�ȭ
    }
    wprintf_s(L"\nClose..... bye\n");

    return 2;
}