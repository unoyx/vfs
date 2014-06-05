#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>
using namespace std;
/*
compare命令测试
准备工作
mkdir compare
copy e:\虚拟磁盘检查\compare\A.txt compare

step1(开头不一致比较):
compare e:\虚拟磁盘检查\compare\A(changeTop).txt compare\A.txt
检查：返回1，观察输出是否正确
step2(中间不一致比较)：
compare e:\虚拟磁盘检查\compare\A(changeMiddle).txt compare\A.txt
检查：返回1，观察输出是否正确
step3(结尾不一致比较)：
compare e:\虚拟磁盘检查\compare\A(changeEnd).txt compare\A.txt
检查：返回1，观察输出是否正确
step4(文件一致比较)：
compare e:\虚拟磁盘检查\compare\A.txt compare\A.txt
检查：返回1观察输出是否正确，应输出一致
step5(磁盘上不存在的文件):
compare e:\虚拟磁盘检查\compare\不存在.txt compare\A.txt
检查：返回0
step6(虚拟磁盘上不存在的文件):
compare e:\虚拟磁盘检查\compare\A.txt compare\不存在.txt
检查：返回0


附：
A文件内容：            A(changeTop)：                A(changeMiddle)：                   A(changeEnd)：
          abcdefg                 defg                               abcdefg                          abcdefg
          hijk   lmn              hijk   lmn                         hijmn                            hijk   lmn
          opqrstuvwxyz            opqrstuvwxyz                       opqrstuvwxyz                     opqrstuv
*/

TEST_F(common_test,compare)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir compare");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\虚拟磁盘检查\\compare\\A.txt compare");
	ASSERT_EQ(v,1);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),2);
	findres->Release();

	//开头不一致测试
	cout<<endl<<"（开头不一致）"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\虚拟磁盘检查\\compare\\A(changeTop).txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//中间不一致测试
	cout<<endl<<"（中间不一致）"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\虚拟磁盘检查\\compare\\A(changeMiddle).txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//结尾不一致测试
	cout<<endl<<"（结尾不一致）"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\虚拟磁盘检查\\compare\\A(changeEnd).txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//文件比较一致测试
	cout<<endl<<"（应输出：内容一致）"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\虚拟磁盘检查\\compare\\A.txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//磁盘文件不存在
	v=GetVDiskProxy()->ExecCommand("compare e:\\虚拟磁盘检查\\compare\\不存在.txt compare\\A.txt");
	ASSERT_EQ(v,0);

	//虚拟磁盘文件不存在
	v=GetVDiskProxy()->ExecCommand("compare e:\\虚拟磁盘检查\\compare\\A.txt compare\\不存在.txt");
	ASSERT_EQ(v,0);

	//清空目录，不对下一个测试案例造成影响
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s");
	ASSERT_EQ(v,1);
	rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	findres = rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres = NULL;
}