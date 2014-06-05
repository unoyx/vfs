#include "VirtualDisk.h"
#include "VirtualDiskNode.h"

static VirtualDiskNode* disk = new VirtualDiskNode;

DLLAPI IVirtualDiskProxy* GetVirtualDiskProxy(void)
{
    return disk;
}

DLLAPI void ReleaseVirtualDiskProxy(void)
{
    assert(disk);
    delete dynamic_cast<VirtualDiskNode*>(disk);
    disk = nullptr;
}
