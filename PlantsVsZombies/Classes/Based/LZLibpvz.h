#pragma

#ifndef  _libpvz
#define PVZ_API _declspec(dllexport) 
#else
#define PVZ_API _declspec(dllimport) 
#endif // ! _libpvz

#if __cplusplus
extern "C" {
#endif
	PVZ_API bool __stdcall decryption(const char* _str, char* buf);
	PVZ_API bool __stdcall encryption(const char* _str, char* buf);
#if __cplusplus
}
#endif 