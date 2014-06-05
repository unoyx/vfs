#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>

using namespace std;

/*
Del测试
准备工作：
mkdir a\b\c
step1(删除不存在的文件):
del a\b\c\1.txt
检查：应该返回失败
step2(删除单个文件)：
copy e:\虚拟磁盘检查\单个文件\a.txt a\b\c\*
del a\b\c\1.txt
检查：应该返回成功，并查找1.txt不存在
step3(删除整个目录)：
(1)copy e:\虚拟磁盘检查\整个目录 a\b\del*
del a\\b\\*
(2)copy e:\虚拟磁盘检查\整个目录 a\b\del*
del c:\a\b
检查：(1)返回成功，并查找结果为空(2)输出提示是否删除目录下所有文件，输入Y返回成功，输入N返回失败
step4(使用通配符删除)：
（1）copy e:\虚拟磁盘检查\通配符\*.txt a\
del a\*.txt
（2）copy e:\虚拟磁盘检查\通配符\?.cc a\
del a\?.cc
检查：（1）（2）两组命令均返回成功，并查找结果为空
step5(参数测试)：
cd\
rmdir a/s
mkdir a\b\c
copy e:\虚拟磁盘检查\通配符\*.txt a\
copy e:\虚拟磁盘检查\通配符\*.txt a\b\
copy e:\虚拟磁盘检查\通配符\*.txt a\b\c
del c:\a\*.txt/s
检查：返回成功，并查找“*.txt”结果为空
*/


TEST_F(common_test,del)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c");
	ASSERT_EQ(v,1);
//    ASSERT_EQ(v, 0);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
    findres->display();
	ASSERT_EQ(findres->GetCount(),3);
	findres->Release();
	
	//删除不存在的文件
	v=GetVDiskProxy()->ExecCommand("del a\\b\\c\\1.txt");
	ASSERT_EQ(v,0);

	//删除单个文件
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a\\b\\c\\*");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("a.txt",true);
    findres->display();
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("del a\\b\\c\\a.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("a.txt",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	
	//删除整个目录
	//(1)
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\整个目录 a\\b\\del*");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("del a\\b\\*");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	//(2)
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\整个目录 a\\b\\del*");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
    //输入N
	cout<<"(请测试者输入N)"<<endl;
	v=GetVDiskProxy()->ExecCommand("del a\\b");
	ASSERT_EQ(v,0);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
	//输入Y
	cout<<"(请测试者输入Y)"<<endl;
	v=GetVDiskProxy()->ExecCommand("del a\\b");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	
	
	//通配符删除
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\*.txt a\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
    findres->display();
	ASSERT_EQ(findres->GetCount(),5);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\?.cc a\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("?.cc",true);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("del a\\*.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("del a\\?.cc");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("?.cc",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();

	//参数测试
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s \\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\*.txt a");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\*.txt a\\b\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\*.txt a\\b\\c");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
	ASSERT_EQ(findres->GetCount(),15);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("del c:\\a\\*.txt/s");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();


	//清空目录，不对下一个测试案例造成影响
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s \\");
	ASSERT_EQ(v,1);
	findres = rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres = NULL;
}