#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <algorithm>
#include <iostream>
using namespace std;
/*
Cd�������
׼������
mkdir a1\b1\c1
mkdir a2\b2\c2
mkdir a3\b3\c3

step1(�л���C1):
cd a1\b1\c1
copy e:\������̼��\�����ļ�\a.txt c1.txt
��飺��c1�����ҵ�c1.txt�����ҹ��ҲҪ������c:\a1\b1\c1>

step2(���˵��ϼ�Ŀ¼):
cd..
copy e:\������̼��\�����ļ�\a.txt b1.txt
��飺��b1�����ҵ�b1.txt�����ҹ��ҲҪ������c:\a1\b1>
step3(�˵���Ŀ¼):
cd\
copy e:\������̼��\�����ļ�\a.txt root.txt
��飺�ڸ�Ŀ¼�����ҵ�root.txt�����ҹ��ҲҪ������c:\>
step4(��ͬ��ʽ�л�):
(1)
cd.\a2\b2\c2
copy e:\������̼��\�����ļ�\a.txt c2.txt
��飺��c2�����ҵ�c2.txt�����ҹ��ҲҪ������c:\a2\b2\c2>
(2)
cd c:\a3\b3\c3
copy e:\������̼��\�����ļ�\a.txt c3.txt
��飺��c3�����ҵ�c3.txt�����ҹ��ҲҪ������c:\a3\b3\c3>
step5(�л��������ڵ�Ŀ¼):
cd c:\a4
��飺����0�����ҹ�겻�ı�
*/


TEST_F(common_test,cd)
{

	int v=GetVDiskProxy()->ExecCommand("mkdir a1\\b1\\c1");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a2\\b2\\c2");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a3\\b3\\c3");
	ASSERT_EQ(v,1);

	//�л���c1����
	v=GetVDiskProxy()->ExecCommand("cd a1\\b1\\c1");
	ASSERT_EQ(v,1);
	cout <<endl<<"(�˴����ӦΪc:\\a1\\b1\\c1\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt c1.txt");
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

	//cd..����
	v=GetVDiskProxy()->ExecCommand("cd..");
	ASSERT_EQ(v,1);
	cout <<endl<<"(�˴����ӦΪc:\\a1\\b1\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt b1.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("b1",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* b1;
    b1 = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = b1->Find("b1.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	
	//cd\����
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	cout <<endl<<"(�˴����ӦΪc:\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt root.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("root.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();

	//��ͬ��ʽ�л�
	//(1)
	v=GetVDiskProxy()->ExecCommand("cd.\\a2\\b2\\c2");
	ASSERT_EQ(v,1);
	cout <<endl<<"(�˴����ӦΪc:\\a2\\b2\\c2\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt c2.txt");
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
	cout <<endl<<"(�˴����ӦΪc:\\a3\\b3\\c3\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt c3.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("c3",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* c3;
    c3 = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c3->Find("c3.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();

	//�л������ڵ�Ŀ¼
	v=GetVDiskProxy()->ExecCommand("cd a4");
	ASSERT_EQ(v,0);
	cout <<endl<<"(�˴����ӦΪc:\\a3\\b3\\c3\\>)"<<endl;
	system("pause");

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