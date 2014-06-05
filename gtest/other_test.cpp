#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>

/*
准备工作
mkdir abc
step1(错误命令测试):
aaa bbb
检测：返回0,
step2(命令大小写):
dir
Dir
DIR
diR
dIr
检测：返回1，都能正确识别
step3(参数测试):
dir/s/ad
dir c:\/s/ad
dir/s/ad .\abc
dir.\abc/ad/s
检测：返回1
step4(空格测试):
      dir      /s
检测：返回1
*/

TEST_F(common_test,other)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir abc");
	ASSERT_EQ(v,1);
	//错误命令测试
	v=GetVDiskProxy()->ExecCommand("aaa bbb");
	ASSERT_EQ(v,0);

	//大小写测试
	v=GetVDiskProxy()->ExecCommand("dir");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("Dir");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("DIR");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("diR");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dIr");
	ASSERT_EQ(v,1);
	//参数测试
	v=GetVDiskProxy()->ExecCommand("dir/s/ad");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dir c:\\/s/ad");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dir/s/ad .\\abc");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dir.\\abc/ad/s");
	ASSERT_EQ(v,1);
	//空格测试
	v=GetVDiskProxy()->ExecCommand("   dir   /s");
	ASSERT_EQ(v,1);

	//清空目录，不对下一个测试案例造成影响
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s \\");
	ASSERT_EQ(v,1);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres = rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres = NULL;
}