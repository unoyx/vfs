#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
using namespace std;

/*
Copy测试
(在磁盘上准备以下文件:e:\虚拟磁盘检查\单个文件\a.txt
                      e:\虚拟磁盘检查\通配符\*.txt 
					  e:\虚拟磁盘检查\整个目录
					  e:\虚拟磁盘检查\大文件
)
step1(单个文件拷贝失败测试):
copy e:\虚拟磁盘检查\单个文件\a.txt a\b\c\d\a.txt
检查：应该返回失败
step2(多个文件拷贝失败测试):
copy e:\不存在的目录\*.txt c:\a\b\
copy e:\不存在的目录\1.txt c:\a\b\
copy e:\虚拟磁盘检查\通配符\*.txt c:\不存的目录\b\
copy e:\虚拟磁盘检查\通配符\*.txt c:\a\不存在的目录\
copy e:\虚拟磁盘检查\通配符\不存在.txt c:\a\b\
检查：
     以上每一个command都返回应该都是0
     root目录下搜索*，得到的结果是空的
step3(单个文件拷贝成功测试及文件名错误拷贝失败测试):
mkdir a\b\c\d\e
copy e:\虚拟磁盘检查\单个文件\a.txt a\b\c\d\a.txt
copy e:\虚拟磁盘检查\单个文件\a.txt a\b\c\d\a>.txt
检查：找到d目录下，查找*，结果应该只有a.txt和目录e
step4(整个目录批量拷贝成功测试):
copy e:\虚拟磁盘检查\整个目录 c:\a\b\c\
检查：找到c目录下，查找*，结果应该只有目录中的文件和目录d
step5(含通配符的批量拷贝测试):
copy e:\虚拟磁盘检查\通配符\*.txt c:\a\b\
检查：找到b目录下，查找*，结果应该只有目录中的文件和目录c
step6(大文件的拷贝测试):
copy e:\\虚拟磁盘检查\\大文件\\1.28G.zip c:\\
copy e:\\虚拟磁盘检查\\大文件\\1.9G.zip c:\\
检查：不能出现崩溃现象
rmdir测试
step7(删除存在目录失败测试):
cd\
rmdir a
检查：返回应该是0
step8(不存在目录删除失败测试)：
rmdir c:\a\b\c\d\e
检查：返回1；应该找不到目录e
step9(删除当前目录成功测试):
cd a\\b\\c\\d
rmdir /s
copy e:\\虚拟磁盘检查\\单个文件\\a.txt 当前目录.txt
检查：返回1；找不到目录d，并且在目录c下能找到"当前目录.txt"
step10(目录删除成功测试)：
rmdir c:\a/s
检查：返回1，且c:\a找不到
step11(删除不存在的目录失败测试):
rmdir c:\a
检查：返回0
*/

TEST_F(common_test,copyAndRmdir)
{
	//测试在不存在的目录上拷贝单个文件
	int v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,0);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	//测试多个文件拷贝失败
	
	v=GetVDiskProxy()->ExecCommand("mkdir a\\b");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),2);
	findres->Release();
	std::vector<std::string> copyCommand;
	copyCommand.push_back("copy e:\\虚拟磁盘检查\\不存在的目录\\*.txt c:\\a\\b\\");
	copyCommand.push_back("copy e:\\虚拟磁盘检查\\不存在的目录\\1.txt c:\\a\\b\\");
	copyCommand.push_back("copy e:\\虚拟磁盘检查\\通配符\\*.txt c:\\不存在的目录\\b\\");
	copyCommand.push_back("copy e:\\虚拟磁盘检查\\通配符\\*.txt c:\\a\\不存在的目录\\");
	copyCommand.push_back("copy e:\\虚拟磁盘检查\\通配符\\不存在.txt c:\\a\\b\\");
	for(unsigned int i =0;i<copyCommand.size();i++)
	{
		v=GetVDiskProxy()->ExecCommand(copyCommand[i].c_str());
		ASSERT_EQ(v,0);
		findres=rootdir->Find("*",true);
		ASSERT_EQ(findres->GetCount(),2);
		findres->Release();
	}

	v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c\\d\\e\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),5);
	findres->Release();

	// 单个文件拷贝成功测试
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a\\b\\c\\d\\a>.txt");
	ASSERT_EQ(v,0);
	findres=rootdir->Find("d",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* d = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = d->Find("*",false);
    // TODO FIXME
	ASSERT_EQ(findres->GetCount(),2);
	findres->Release();
	d = NULL;
	//拷贝已经存在的文件
	//不覆盖
	cout<<"(请测试者输入N)"<<endl;
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,0);
	//覆盖
	cout<<"(请测试者输入Y)"<<endl;
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,1);

	//整个目录拷贝成功测试
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\整个目录 a\\b\\c\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("C",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* c = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c->Find("*",false);
	ASSERT_EQ(findres->GetCount(),7);
	findres->Release();
	c = NULL;

	//通配符拷贝
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\*.txt c:\\a\\b\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("b",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* b = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = b->Find("*",false);

	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\通配符\\?.cc c:\\a\\b\\");
	findres = b->Find("*",false);

	ASSERT_EQ(findres->GetCount(),7);
	findres->Release();
	b = NULL;

	//大文件测试
//	ASSERT_NO_THROW(GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\大文件\\1.28G.zip c:\\"));
//	ASSERT_NO_THROW(GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\大文件\\1.9G.zip c:\\"));

	//删除目录失败测试

	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir a");
	ASSERT_EQ(v,0);

	//删除空目录成功测试

	v=GetVDiskProxy()->ExecCommand("rmdir a\\b\\c\\d\\e");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("cd a\\b\\c\\d\\e");
	ASSERT_EQ(v,0);
	//删除当前目录成功测试，并且光标移动到上级目录
	v=GetVDiskProxy()->ExecCommand("cd a\\b\\c\\d");
	ASSERT_EQ(v,1);	
	v=GetVDiskProxy()->ExecCommand("rmdir /s");
	ASSERT_EQ(v,1);
	cout <<endl<<"(此处光标应为c:\\a\\b\\c\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\单个文件\\a.txt 当前目录.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("d",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres=rootdir->Find("C",true);
	ASSERT_EQ(findres->GetCount(),1);
    c = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c->Find("当前目录.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	c = NULL;

	//删除非空目录成功测试

	v=GetVDiskProxy()->ExecCommand("rmdir c:\\a/s");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("cd c:\\a");
	ASSERT_EQ(v,0);

	//删除不存在的目录失败测试

	v=GetVDiskProxy()->ExecCommand("rmdir c:\\a");
	ASSERT_EQ(v,0);

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