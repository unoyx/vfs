#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>
using namespace std;
/*
compare�������
׼������
mkdir compare
copy e:\������̼��\compare\A.txt compare

step1(��ͷ��һ�±Ƚ�):
compare e:\������̼��\compare\A(changeTop).txt compare\A.txt
��飺����1���۲�����Ƿ���ȷ
step2(�м䲻һ�±Ƚ�)��
compare e:\������̼��\compare\A(changeMiddle).txt compare\A.txt
��飺����1���۲�����Ƿ���ȷ
step3(��β��һ�±Ƚ�)��
compare e:\������̼��\compare\A(changeEnd).txt compare\A.txt
��飺����1���۲�����Ƿ���ȷ
step4(�ļ�һ�±Ƚ�)��
compare e:\������̼��\compare\A.txt compare\A.txt
��飺����1�۲�����Ƿ���ȷ��Ӧ���һ��
step5(�����ϲ����ڵ��ļ�):
compare e:\������̼��\compare\������.txt compare\A.txt
��飺����0
step6(��������ϲ����ڵ��ļ�):
compare e:\������̼��\compare\A.txt compare\������.txt
��飺����0


����
A�ļ����ݣ�            A(changeTop)��                A(changeMiddle)��                   A(changeEnd)��
          abcdefg                 defg                               abcdefg                          abcdefg
          hijk   lmn              hijk   lmn                         hijmn                            hijk   lmn
          opqrstuvwxyz            opqrstuvwxyz                       opqrstuvwxyz                     opqrstuv
*/

TEST_F(common_test,compare)
{
	int v=GetVDiskProxy()->ExecCommand("mkdir compare");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\compare\\A.txt compare");
	ASSERT_EQ(v,1);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),2);
	findres->Release();

	//��ͷ��һ�²���
	cout<<endl<<"����ͷ��һ�£�"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\������̼��\\compare\\A(changeTop).txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//�м䲻һ�²���
	cout<<endl<<"���м䲻һ�£�"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\������̼��\\compare\\A(changeMiddle).txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//��β��һ�²���
	cout<<endl<<"����β��һ�£�"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\������̼��\\compare\\A(changeEnd).txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//�ļ��Ƚ�һ�²���
	cout<<endl<<"��Ӧ���������һ�£�"<<endl;
	v=GetVDiskProxy()->ExecCommand("compare e:\\������̼��\\compare\\A.txt compare\\a.txt");
	ASSERT_EQ(v,1);
	
	system("pause");

	//�����ļ�������
	v=GetVDiskProxy()->ExecCommand("compare e:\\������̼��\\compare\\������.txt compare\\A.txt");
	ASSERT_EQ(v,0);

	//��������ļ�������
	v=GetVDiskProxy()->ExecCommand("compare e:\\������̼��\\compare\\A.txt compare\\������.txt");
	ASSERT_EQ(v,0);

	//���Ŀ¼��������һ�����԰������Ӱ��
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