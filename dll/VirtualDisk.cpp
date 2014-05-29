#include "VirtualDisk.h"
#include "VirtualDiskNode.h"

DLLAPI IVirtualDiskProxy* GetVirtualDiskProxy(void)
{
    IVirtualDiskProxy* ret = new VirtualDiskNode;
    return ret;
}

DLLAPI void ReleaseVirtualDiskProxy(IVirtualDiskProxy* disk)
{
    assert(disk);
    delete disk;
}
