#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <vector>
#include <string>
#include <algorithm>
/*
Mkdir测试
step1：mkdir a\b\c\d
       mkdir abc>def
检测：root下有a没有abc>ade，a下有b....
step2：mkdir a
检测：返回应该失败
mkdir a\b\c\d
检测：返回应该失败
step3：
cd a
mkdir 测试目录1\测试目录2\测试目录3

检测：返回应该成功。从a目录下找"测试*"应该能找到3个目录

step4:
mkdir a a\b b
检测：返回应该成功。从c:\目录下能找到2个“？ ？”的目录
*/

TEST_F(common_test,mkdir1)
{
	//测试一次创建多个目录及名字错误的目录
	//mkdir a\b\c\d
	int v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c\\d");
	ASSERT_EQ(v,1);
	//mkdir abc>def 应返回0
	v=GetVDiskProxy()->ExecCommand("mkdir abc>def");
	ASSERT_EQ(v,0);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
	//c盘下应该找到4个目录，名字分别是a b c d
	std::vector<std::string> names;
	names.push_back("a");names.push_back("b");names.push_back("c");names.push_back("d");
	ASSERT_EQ(findres->GetCount(),4);
	for (int i=0;i<4;++i)
	{
		auto obj=findres->GetItemByIdx(i);
		std::string n=obj->GetName();
		auto itr=std::find(names.begin(),names.end(),n);
		ASSERT_NE(itr,names.end());
	}
	findres->Release();

	//重复创建目录，测试是否返回错误
	//mkdir a 返回应该失败
	v=GetVDiskProxy()->ExecCommand("mkdir a");
	ASSERT_EQ(v,0);
	
	//mkdir a/b/c 返回应该失败
	v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c");
	ASSERT_EQ(v,0);

	//测试中文路径
	v=GetVDiskProxy()->ExecCommand("cd a");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir 测试目录1\\测试目录2\\测试目录3");
	ASSERT_EQ(v,1);
	auto findres2=rootdir->Find("a",false);
	IDirProxy* a=(IDirProxy*)findres2->GetItemByIdx(0);
	EXPECT_EQ(a!=0,true);
    // ????
    a->Find("*",false)->display();
	ASSERT_EQ(a->Find("*",false)->GetCount(),2);
	findres2->Release();
	ASSERT_EQ(a->Find("测试*",true)->GetCount(),3);
	findres2->Release();

	//测试带空格的目录

	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a a\\b b\\");
	ASSERT_EQ(v,1);
	findres2=rootdir->Find("? ?",true);
	EXPECT_EQ(findres2->GetCount(),2);
	findres2->Release();

	//清空目录，不对下一个测试案例造成影响
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s");
	ASSERT_EQ(v,1);
	findres = rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres = NULL;
}