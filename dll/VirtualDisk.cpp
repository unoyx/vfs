#include "VirtualDisk.h"
#include "VirtualDiskNode.h"

DLLAPI IVirtualDiskProxy* GetVirtualDiskProxy(void)
{
    VirtualDiskNode* ret = new VirtualDiskNode;
    return ret;
}

DLLAPI void ReleaseVirtualDiskProxy(IVirtualDiskProxy* disk)
{
    assert(disk);
    delete dynamic_cast<VirtualDiskNode*>(disk);
}
