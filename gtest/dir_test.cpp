#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>
using namespace std;
/*
dir命令测试
准备工作
mkdir a1\a2\a3
mkdir b
mkdir c
copy e:\虚拟磁盘检查\整个目录 a1
copy e:\虚拟磁盘检查\单个文件\a.txt a1\a2\\a.txt
copy e:\虚拟磁盘检查\通配符 c:\
step1（dir不存在目录）：
dir d
检查：应该返回0
step2(dir存在目录包含绝对路径和相对路径):
dir a1
dir c:\a1\a2
检查：均应返回1，并输出目录下文件和目录
step3（dir/s）：
dir a1/s
检查：输出目录a1及其所有子目录下文件
step4(dir/ad):
dir/ad
检查：只输出根目录下所有目录
step5(dir/s/ad):
dir/ad/s
检查：只输出根目录及其子目录下的目录
*/

TEST_F(common_test,dir)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir a1\\a2\\a3");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir b");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir c");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\整个目录 a1");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a1\\a2\\a.txt");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符 c:\\");
	ASSERT_EQ(v,1);

	//不存在目录测试
	v=GetVDiskProxy()->ExecCommand("dir d");
	ASSERT_EQ(v,0);

	//存在目录测试
	v=GetVDiskProxy()->ExecCommand("dir a1");
	ASSERT_EQ(v,1);
	cout <<"此处输出六个文件和目录a2"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("dir c:\\a1\\a2");
	ASSERT_EQ(v,1);
	cout <<"此处输出1个文件和目录a3"<<endl;
	system("pause");

	v=GetVDiskProxy()->ExecCommand("dir c:\\a1\\a2\\a.txt");
	ASSERT_EQ(v,1);
	cout <<"此处输出a.txt个文件"<<endl;
	system("pause");

	//参数/s测试
	v=GetVDiskProxy()->ExecCommand("dir a1/s");
	ASSERT_EQ(v,1);
	cout <<"此处输出a1下6个文件a2下1个文件"<<endl;
	system("pause");

	//参数/ad测试
	v=GetVDiskProxy()->ExecCommand("dir/ad");
	ASSERT_EQ(v,1);
	cout <<"此处输出a1、b、c三个目录"<<endl;
	system("pause");

	//参数/s/ad测试
	v=GetVDiskProxy()->ExecCommand("dir/ad/s");
	ASSERT_EQ(v,1);
	cout <<"此处输出根目录下三个目录，a1目录下1个目录，a2目录下1个目录，a3、b、c下没有目录"<<endl;
	system("pause");

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