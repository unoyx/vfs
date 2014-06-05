#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"

int main(int argc, char* argv[])
{
//    ::testing::GTEST_FLAG(filter)="common_test.mkdir1";
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
    return 0;
}