#pragma once
#include "pch.h"
#include "sha256.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h> // HRESULT 
#include <winbio.h>
#include <conio.h>    // _getch()
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <Assert.h>
#include <fstream>
#pragma comment(lib, "Winbio.lib")
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

#pragma pack(push, 1)
typedef struct _FINETP
{
    uint16_t header;
    uint8_t id;
    uint16_t checksum;
    uint16_t option;
    char senID[64];
    char data[64];

    typedef enum
    {
        request = 1,
        challenges = 2,
        cresponse = 3,
        response = 4
    }ID;

    typedef enum
    {
        success_n_fido = 1,
        fail_n_idpw = 2
    }Option;

}FINETP;
#pragma pack(pop)


HRESULT GetCurrentUserIdentity(__inout PWINBIO_IDENTITY Identity);

string DataBases();

string SaveGuid(__in PWINBIO_UUID Guid);

int Verify(WINBIO_BIOMETRIC_SUBTYPE subFactor);

int Finet(int option, string guid);

int Finet(CString id, CString pw);

uint16_t cal_chksum(FINETP* finetp);

string findIP();

char* getMAC();

void ErrorHandling(const char* message);