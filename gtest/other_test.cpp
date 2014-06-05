#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>

/*
׼������
mkdir abc
step1(�����������):
aaa bbb
��⣺����0,
step2(�����Сд):
dir
Dir
DIR
diR
dIr
��⣺����1��������ȷʶ��
step3(��������):
dir/s/ad
dir c:\/s/ad
dir/s/ad .\abc
dir.\abc/ad/s
��⣺����1
step4(�ո����):
      dir      /s
��⣺����1
*/

TEST_F(common_test,other)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir abc");
	ASSERT_EQ(v,1);
	//�����������
	v=GetVDiskProxy()->ExecCommand("aaa bbb");
	ASSERT_EQ(v,0);

	//��Сд����
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
	//��������
	v=GetVDiskProxy()->ExecCommand("dir/s/ad");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dir c:\\/s/ad");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dir/s/ad .\\abc");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("dir.\\abc/ad/s");
	ASSERT_EQ(v,1);
	//�ո����
	v=GetVDiskProxy()->ExecCommand("   dir   /s");
	ASSERT_EQ(v,1);

	//���Ŀ¼��������һ�����԰������Ӱ��
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