#pragma once

#ifdef _DLL_FIDO_ 
#define DLL_FIDO _declspec(dllexport) 
#else 
#define DLL_FIDO __declspec(dllimport) 
#endif 

DLL_FIDO int fido(CString id, CString pw, int check);