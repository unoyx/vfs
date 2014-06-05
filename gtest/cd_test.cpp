#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>
using namespace std;
/*
Cd命令测试
准备工作
mkdir a1\b1\c1
mkdir a2\b2\c2
mkdir a3\b3\c3

step1(切换到C1):
cd a1\b1\c1
copy e:\虚拟磁盘检查\单个文件\a.txt c1.txt
检查：在c1下能找到c1.txt，并且光标也要调整到c:\a1\b1\c1>

step2(回退到上级目录):
cd..
copy e:\虚拟磁盘检查\单个文件\a.txt b1.txt
检查：在b1下能找到b1.txt，并且光标也要调整到c:\a1\b1>
step3(退到根目录):
cd\
copy e:\虚拟磁盘检查\单个文件\a.txt root.txt
检查：在根目录下能找到root.txt，并且光标也要调整到c:\>
step4(不同方式切换):
(1)
cd.\a2\b2\c2
copy e:\虚拟磁盘检查\单个文件\a.txt c2.txt
检查：在c2下能找到c2.txt，并且光标也要调整到c:\a2\b2\c2>
(2)
cd c:\a3\b3\c3
copy e:\虚拟磁盘检查\单个文件\a.txt c3.txt
检查：在c3下能找到c3.txt，并且光标也要调整到c:\a3\b3\c3>
step5(切换到不存在的目录):
cd c:\a4
检查：返回0，并且光标不改变
*/


TEST_F(common_test,cd)
{

	int v=GetVDiskProxy()->ExecCommand("mkdir a1\\b1\\c1");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a2\\b2\\c2");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a3\\b3\\c3");
	ASSERT_EQ(v,1);

	//切换到c1测试
	v=GetVDiskProxy()->ExecCommand("cd a1\\b1\\c1");
	ASSERT_EQ(v,1);
	cout <<endl<<"(此处光标应为c:\\a1\\b1\\c1\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt c1.txt");
	ASSERT_EQ(v,1);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("c1",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* c1;
    c1 = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c1->Find("c1.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();

	//cd..测试
	v=GetVDiskProxy()->ExecCommand("cd..");
	ASSERT_EQ(v,1);
	cout <<endl<<"(此处光标应为c:\\a1\\b1\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt b1.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("b1",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* b1;
    b1 = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = b1->Find("b1.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	
	//cd\测试
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	cout <<endl<<"(此处光标应为c:\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt root.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("root.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();

	//不同方式切换
	//(1)
	v=GetVDiskProxy()->ExecCommand("cd.\\a2\\b2\\c2");
	ASSERT_EQ(v,1);
	cout <<endl<<"(此处光标应为c:\\a2\\b2\\c2\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt c2.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("c2",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* c2;
    c2 = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c2->Find("c2.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	//(2)
	v=GetVDiskProxy()->ExecCommand("cd c:\\a3\\b3\\c3");
	ASSERT_EQ(v,1);
	cout <<endl<<"(此处光标应为c:\\a3\\b3\\c3\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt c3.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("c3",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* c3;
    c3 = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c3->Find("c3.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();

	//切换不存在的目录
	v=GetVDiskProxy()->ExecCommand("cd a4");
	ASSERT_EQ(v,0);
	cout <<endl<<"(此处光标应为c:\\a3\\b3\\c3\\>)"<<endl;
	system("pause");

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