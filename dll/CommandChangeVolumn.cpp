#include "CommandChangeVolumn.h"
#include "Vector.h"
#include "path.h"
#include "CommandException.h"
#include "VirtualDiskNode.h"

CommandChangeVolumn::CommandChangeVolumn(void)
{
}


CommandChangeVolumn::~CommandChangeVolumn(void)
{
}

void CommandChangeVolumn::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() != 1)
    {
        throw CommandException(_T("�޷�ʶ�������"));
    }
    m_volumn = pathes[0];
}

void CommandChangeVolumn::setSwitches(const Vector<MyString>& switches)
{
    if (!switches.isEmpty())
    {
        throw CommandException(_T("�޷�ʶ�������\n"));
    }
}

void CommandChangeVolumn::exec(VirtualDiskNode* vfs)
{
    if (!isNormalizedPath(m_volumn))
    {
        m_volumn = vfs->pathNormalize(m_volumn);
    }
    if (vfs->findVolumn(m_volumn.toLower()) == -1)
        throw CommandException(_T("�����ڵľ��\n"));
    vfs->changeVolumn(m_volumn.substr(0, 2));
}
