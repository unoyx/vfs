#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>

using namespace std;

/*
Del����
׼��������
mkdir a\b\c
step1(ɾ�������ڵ��ļ�):
del a\b\c\1.txt
��飺Ӧ�÷���ʧ��
step2(ɾ�������ļ�)��
copy e:\������̼��\�����ļ�\a.txt a\b\c\*
del a\b\c\1.txt
��飺Ӧ�÷��سɹ���������1.txt������
step3(ɾ������Ŀ¼)��
(1)copy e:\������̼��\����Ŀ¼ a\b\del*
del a\\b\\*
(2)copy e:\������̼��\����Ŀ¼ a\b\del*
del c:\a\b
��飺(1)���سɹ��������ҽ��Ϊ��(2)�����ʾ�Ƿ�ɾ��Ŀ¼�������ļ�������Y���سɹ�������N����ʧ��
step4(ʹ��ͨ���ɾ��)��
��1��copy e:\������̼��\ͨ���\*.txt a\
del a\*.txt
��2��copy e:\������̼��\ͨ���\?.cc a\
del a\?.cc
��飺��1����2��������������سɹ��������ҽ��Ϊ��
step5(��������)��
cd\
rmdir a/s
mkdir a\b\c
copy e:\������̼��\ͨ���\*.txt a\
copy e:\������̼��\ͨ���\*.txt a\b\
copy e:\������̼��\ͨ���\*.txt a\b\c
del c:\a\*.txt/s
��飺���سɹ��������ҡ�*.txt�����Ϊ��
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
	
	//ɾ�������ڵ��ļ�
	v=GetVDiskProxy()->ExecCommand("del a\\b\\c\\1.txt");
	ASSERT_EQ(v,0);

	//ɾ�������ļ�
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a\\b\\c\\*");
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
	
	//ɾ������Ŀ¼
	//(1)
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\����Ŀ¼ a\\b\\del*");
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
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\����Ŀ¼ a\\b\\del*");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
    //����N
	cout<<"(�����������N)"<<endl;
	v=GetVDiskProxy()->ExecCommand("del a\\b");
	ASSERT_EQ(v,0);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
	//����Y
	cout<<"(�����������Y)"<<endl;
	v=GetVDiskProxy()->ExecCommand("del a\\b");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("del*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	
	
	//ͨ���ɾ��
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\*.txt a\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
    findres->display();
	ASSERT_EQ(findres->GetCount(),5);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\?.cc a\\");
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

	//��������
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s \\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\*.txt a");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\*.txt a\\b\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\*.txt a\\b\\c");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
	ASSERT_EQ(findres->GetCount(),15);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("del c:\\a\\*.txt/s");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*.txt",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();


	//���Ŀ¼��������һ�����԰������Ӱ��
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s \\");
	ASSERT_EQ(v,1);
	findres = rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres = NULL;
}