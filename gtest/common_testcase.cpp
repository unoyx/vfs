#include "common_testcase.h"
#include "VirtualDisk.h"

//HMODULE hdll=0;
//typedef IVirtualDiskProxy*(*lpGetVirtualDiskProxy)();
//lpGetVirtualDiskProxy GetVirtualDiskProxyFun; 
IVirtualDiskProxy* gIvdp=0;

extern const char* dll_path;

void load_dll()
{
	gIvdp = GetVirtualDiskProxy();
	if(!gIvdp)
		::MessageBoxA(0,"GetVirtualDiskProxyFun ß∞‹",0,0);
}

void unload_dll()
{
//	if(hdll)
//        FreeModule(hdll);
//		FreeLibrary(hdll);
    ReleaseVirtualDiskProxy();



}
void common_test::SetUpTestCase()
{
	load_dll();
}


void common_test::TearDownTestCase()
{
	unload_dll();
}

IVirtualDiskProxy* common_test::GetVDiskProxy()
{
	return gIvdp;
}