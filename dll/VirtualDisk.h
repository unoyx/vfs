#ifndef _VIRTUAL_DISK_H_
#define _VIRTUAL_DISK_H_
#include "IVirtualDiskProxy.h"

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#ifdef DLL_EXPORTS
#define DLLAPI DLLEXPORT
#else
#define DLLAPI DLLIMPORT
#endif

extern "C"
{
    DLLAPI IVirtualDiskProxy* GetVirtualDiskProxy(void);
    DLLAPI void ReleaseVirtualDiskProxy(void);
};

#endif // _VIRTUAL_DISK_H_
