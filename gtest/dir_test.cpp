#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>
using namespace std;
/*
dir�������
׼������
mkdir a1\a2\a3
mkdir b
mkdir c
copy e:\������̼��\����Ŀ¼ a1
copy e:\������̼��\�����ļ�\a.txt a1\a2\\a.txt
copy e:\������̼��\ͨ��� c:\
step1��dir������Ŀ¼����
dir d
��飺Ӧ�÷���0
step2(dir����Ŀ¼��������·�������·��):
dir a1
dir c:\a1\a2
��飺��Ӧ����1�������Ŀ¼���ļ���Ŀ¼
step3��dir/s����
dir a1/s
��飺���Ŀ¼a1����������Ŀ¼���ļ�
step4(dir/ad):
dir/ad
��飺ֻ�����Ŀ¼������Ŀ¼
step5(dir/s/ad):
dir/ad/s
��飺ֻ�����Ŀ¼������Ŀ¼�µ�Ŀ¼
*/

TEST_F(common_test,dir)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir a1\\a2\\a3");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir b");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir c");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\����Ŀ¼ a1");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a1\\a2\\a.txt");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ��� c:\\");
	ASSERT_EQ(v,1);

	//������Ŀ¼����
	v=GetVDiskProxy()->ExecCommand("dir d");
	ASSERT_EQ(v,0);

	//����Ŀ¼����
	v=GetVDiskProxy()->ExecCommand("dir a1");
	ASSERT_EQ(v,1);
	cout <<"�˴���������ļ���Ŀ¼a2"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("dir c:\\a1\\a2");
	ASSERT_EQ(v,1);
	cout <<"�˴����1���ļ���Ŀ¼a3"<<endl;
	system("pause");

	v=GetVDiskProxy()->ExecCommand("dir c:\\a1\\a2\\a.txt");
	ASSERT_EQ(v,1);
	cout <<"�˴����a.txt���ļ�"<<endl;
	system("pause");

	//����/s����
	v=GetVDiskProxy()->ExecCommand("dir a1/s");
	ASSERT_EQ(v,1);
	cout <<"�˴����a1��6���ļ�a2��1���ļ�"<<endl;
	system("pause");

	//����/ad����
	v=GetVDiskProxy()->ExecCommand("dir/ad");
	ASSERT_EQ(v,1);
	cout <<"�˴����a1��b��c����Ŀ¼"<<endl;
	system("pause");

	//����/s/ad����
	v=GetVDiskProxy()->ExecCommand("dir/ad/s");
	ASSERT_EQ(v,1);
	cout <<"�˴������Ŀ¼������Ŀ¼��a1Ŀ¼��1��Ŀ¼��a2Ŀ¼��1��Ŀ¼��a3��b��c��û��Ŀ¼"<<endl;
	system("pause");

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