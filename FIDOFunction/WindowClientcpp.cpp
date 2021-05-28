//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "data.h"

#define dheader 0xf1e7


int Finet(int option, string guid)

{
    string ip = findIP();
    string port = "51234";

    WSADATA wsaData;
    SOCKET hSock;
    SOCKADDR_IN servAddr;

    char rbuffer[136];
    char wbuffer[136];
    FINETP* rfinetp;
    FINETP* wfinetp;

    char* cMac = getMAC();
    //printf("%s", cMac);

    try {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    catch (exception& e) {
        //cout << e.what() << endl;
        return 2;
    }

    try {
        hSock = socket(PF_INET, SOCK_STREAM, 0);
    }
    catch (exception& e) {
        //cout << e.what() << endl;
        return 2;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    //servAddr.sin_addr.s_addr = inet_addr("192.168.0.98");
    servAddr.sin_port = htons(atoi(port.c_str()));

    try {
        connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
    }
    catch (exception& e) {
        //cout << e.what() << endl;
        return 2;
    }

    // send request packet
    memset(wbuffer, 0x00, sizeof(wbuffer));
    wfinetp = (FINETP*)wbuffer;
    wfinetp->header = htons(dheader);
    wfinetp->id = FINETP::request;
    wfinetp->option = htons(option);
    memset(wfinetp->senID, 0x00, sizeof(wfinetp->senID));
    memset(wfinetp->data, 0x00, sizeof(wfinetp->data));
    wfinetp->checksum = htons(cal_chksum(wfinetp));

    send(hSock, (char*)wfinetp, sizeof(FINETP), 0);
    printf("\n(1) Authentication Request - Send Success!\n");

    // recieve packet
    recv(hSock, rbuffer, sizeof(FINETP), 0);
    rfinetp = (FINETP*)rbuffer;
    if (ntohs(rfinetp->header) != dheader) {
        printf("Invalid header\n");
        closesocket(hSock);
        WSACleanup();
        return 2;
    }
    else {
        printf("valid header\n");
    }
    if (ntohs(rfinetp->checksum) != cal_chksum(rfinetp)) {
        printf("Invalid checksum\n");
        closesocket(hSock);
        WSACleanup();
        return 2;
    }
    else {
        printf("valid checksum\n");
    }
    // recieved packet is challenge packet
    if (rfinetp->id == FINETP::ID::challenges) {
        uint16_t challenge = ntohs(rfinetp->option);
        printf("(2) Challenge-Response Challenge - Recieve success!\n");

        // send cresponse packet - 1 : MAC/GUID
        memset(wbuffer, 0x00, sizeof(wbuffer));
        wfinetp = (FINETP*)wbuffer;
        wfinetp->header = htons(dheader);
        wfinetp->id = FINETP::cresponse;
        wfinetp->option = htons(FINETP::success_n_fido); // macguid
        strncpy(wfinetp->senID, sha256(cMac).c_str(), 64);
        free(cMac);
        string data_guid = guid;
        string data_guid_challenge = "";
        data_guid_challenge += sha256(data_guid);
        data_guid_challenge += to_string(challenge);
        strcpy(wfinetp->data, sha256(data_guid_challenge).c_str());
        wfinetp->checksum = htons(cal_chksum(wfinetp));
        send(hSock, (char*)wfinetp, sizeof(FINETP), 0);
        printf("(3) Challenge-Response Response - Send Success!\n");

        // recieve response packet
        recv(hSock, rbuffer, sizeof(FINETP), 0);
        rfinetp = (FINETP*)rbuffer;
        if (ntohs(rfinetp->header) != dheader) {
            printf("Invalid header\n");
            closesocket(hSock);
            WSACleanup();
            return 2;
        }
        else {
            printf("valid header\n");
        }
        if (ntohs(rfinetp->checksum) != cal_chksum(rfinetp)) {
            printf("Invalid header\n");
            closesocket(hSock);
            WSACleanup();
            return 2;
        }
        else {
            printf("valid checksum\n");
        }
        printf("(4) Authentication Response - Recieve success!\n");
        if (ntohs(rfinetp->option) == FINETP::Option::success_n_fido) {
            printf("Auth Success!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 1;
        }
        else if (ntohs(rfinetp->option) == FINETP::Option::fail_n_idpw) {
            printf("Auth FAIL!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 2;
        }
    }

    // recieved packet is response pacekt
    if (rfinetp->id == FINETP::ID::response) {
        printf("(4) Authentication Response - Recieve success!\n");
        if (ntohs(rfinetp->option) == FINETP::Option::success_n_fido) {
            printf("Auth Success!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 1;
        }
        else if (ntohs(rfinetp->option) == FINETP::Option::fail_n_idpw) {
            printf("Auth FAIL!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 2;
        }
    }
    //return 2;
}


int Finet(CString id, CString pw)

{
    string ip = findIP();
    string port = "51234";

    int option = 1;
    string guid;
    WSADATA wsaData;
    SOCKET hSock;
    SOCKADDR_IN servAddr;

    char rbuffer[136];
    char wbuffer[136];
    FINETP* rfinetp;
    FINETP* wfinetp;

    char* cMac = getMAC();
    //printf("%s", cMac);

    try {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    catch (exception& e) {
        //cout << e.what() << endl;
        return 2;
    }

    try {
        hSock = socket(PF_INET, SOCK_STREAM, 0);
    }
    catch (exception& e) {
        //cout << e.what() << endl;
        return 2;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    //servAddr.sin_addr.s_addr = inet_addr("192.168.0.98");
    servAddr.sin_port = htons(atoi(port.c_str()));

    try {
        connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
    }
    catch (exception& e) {
        //cout << e.what() << endl;
        return 2;
    }

    // send request packet
    memset(wbuffer, 0x00, sizeof(wbuffer));
    wfinetp = (FINETP*)wbuffer;
    wfinetp->header = htons(dheader);
    wfinetp->id = FINETP::request;
    wfinetp->option = htons(option);
    memset(wfinetp->senID, 0x00, sizeof(wfinetp->senID));
    memset(wfinetp->data, 0x00, sizeof(wfinetp->data));
    wfinetp->checksum = htons(cal_chksum(wfinetp));

    send(hSock, (char*)wfinetp, sizeof(FINETP), 0);
    printf("\n(1) Authentication Request - Send Success!\n");

    // recieve packet
    recv(hSock, rbuffer, sizeof(FINETP), 0);
    rfinetp = (FINETP*)rbuffer;
    if (ntohs(rfinetp->header) != dheader) {
        printf("Invalid header\n");
        closesocket(hSock);
        WSACleanup();
        return 0;
    }
    else {
        printf("valid header\n");
    }
    if (ntohs(rfinetp->checksum) != cal_chksum(rfinetp)) {
        printf("Invalid checksum\n");
        closesocket(hSock);
        WSACleanup();
        return 0;
    }
    else {
        printf("valid checksum\n");
    }
    // recieved packet is challenge packet
    if (rfinetp->id == FINETP::ID::challenges) {
        uint16_t challenge = ntohs(rfinetp->option);
        printf("(2) Challenge-Response Challenge - Recieve success!\n");

        // send cresponse packet - 2 : ID / PW
        memset(wbuffer, 0x00, sizeof(wbuffer));
        wfinetp = (FINETP*)wbuffer;
        wfinetp->header = htons(dheader);
        wfinetp->id = FINETP::cresponse;
        wfinetp->option = htons(FINETP::fail_n_idpw); // idpw
        //---------------------------------------------
        string subid = string(CT2CA(id.operator LPCWSTR()));
        strncpy(wfinetp->senID, subid.c_str(), 64);
        free(cMac);
        //---------------------------------------------
        string data_pw = CT2CA(pw.operator LPCWSTR());
        string data_pw_challenge = "";
        data_pw_challenge += sha256(data_pw);
        data_pw_challenge += to_string(challenge);
        strcpy(wfinetp->data, sha256(data_pw_challenge).c_str());
        wfinetp->checksum = htons(cal_chksum(wfinetp));
        send(hSock, (char*)wfinetp, sizeof(FINETP), 0);
        printf("(3) Challenge-Response Response - Send Success!\n");

        // recieve response packet
        recv(hSock, rbuffer, sizeof(FINETP), 0);
        rfinetp = (FINETP*)rbuffer;
        if (ntohs(rfinetp->header) != dheader) {
            printf("Invalid header\n");
            closesocket(hSock);
            WSACleanup();
            return 0;
        }
        else {
            printf("valid header\n");
        }
        if (ntohs(rfinetp->checksum) != cal_chksum(rfinetp)) {
            printf("Invalid header\n");
            closesocket(hSock);
            WSACleanup();
            return 0;
        }
        else {
            printf("valid checksum\n");
        }
        printf("(4) Authentication Response - Recieve success!\n");
        if (ntohs(rfinetp->option) == FINETP::Option::success_n_fido) {
            printf("Auth Success!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 1;
        }
        else if (ntohs(rfinetp->option) == FINETP::Option::fail_n_idpw) {
            printf("Auth FAIL!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 2;
        }
    }

    // recieved packet is response pacekt
    if (rfinetp->id == FINETP::ID::response) {
        printf("(4) Authentication Response - Recieve success!\n");
        if (ntohs(rfinetp->option) == FINETP::Option::success_n_fido) {
            printf("Auth Success!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 1;
        }
        else if (ntohs(rfinetp->option) == FINETP::Option::fail_n_idpw) {
            printf("Auth FAIL!!! \n");
            closesocket(hSock);
            WSACleanup();
            return 2;
        }
    }
    //return 2;
}


string findIP() {
    system("ipconfig > ./routerIP.txt");
    ifstream file;
    string line;
    int offset;
    string rip;
    file.open("routerIP.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file, line);
            if ((offset = line.find("무선 LAN 어댑터 Wi-Fi:", 0)) != string::npos) break;
        }

        while (!file.eof()) {
            getline(file, line);
            if ((offset = line.find("기본 게이트웨이 . . . . . . : ", 0)) != string::npos) {
                rip = line.substr(33, 12);
                cout << rip << endl;
                break;
            }
        }
    }
    return rip;
}

uint16_t cal_chksum(FINETP* finetp) {
    uint16_t res = 0x0000;
    res += finetp->header;
    res += finetp->id;
    res += finetp->option;
    for (int i = 0; i < 64; i++) {
        res += finetp->senID[i];
    }
    for (int i = 0; i < 64; i++) {
        res += finetp->data[i];
    }
    res = (~res & 0x0000FFFF);
    return res;
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


void ErrorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
