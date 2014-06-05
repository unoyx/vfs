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
Copy����
(�ڴ�����׼�������ļ�:e:\������̼��\�����ļ�\a.txt
                      e:\������̼��\ͨ���\*.txt 
					  e:\������̼��\����Ŀ¼
					  e:\������̼��\���ļ�
)
step1(�����ļ�����ʧ�ܲ���):
copy e:\������̼��\�����ļ�\a.txt a\b\c\d\a.txt
��飺Ӧ�÷���ʧ��
step2(����ļ�����ʧ�ܲ���):
copy e:\�����ڵ�Ŀ¼\*.txt c:\a\b\
copy e:\�����ڵ�Ŀ¼\1.txt c:\a\b\
copy e:\������̼��\ͨ���\*.txt c:\�����Ŀ¼\b\
copy e:\������̼��\ͨ���\*.txt c:\a\�����ڵ�Ŀ¼\
copy e:\������̼��\ͨ���\������.txt c:\a\b\
��飺
     ����ÿһ��command������Ӧ�ö���0
     rootĿ¼������*���õ��Ľ���ǿյ�
step3(�����ļ������ɹ����Լ��ļ������󿽱�ʧ�ܲ���):
mkdir a\b\c\d\e
copy e:\������̼��\�����ļ�\a.txt a\b\c\d\a.txt
copy e:\������̼��\�����ļ�\a.txt a\b\c\d\a>.txt
��飺�ҵ�dĿ¼�£�����*�����Ӧ��ֻ��a.txt��Ŀ¼e
step4(����Ŀ¼���������ɹ�����):
copy e:\������̼��\����Ŀ¼ c:\a\b\c\
��飺�ҵ�cĿ¼�£�����*�����Ӧ��ֻ��Ŀ¼�е��ļ���Ŀ¼d
step5(��ͨ�����������������):
copy e:\������̼��\ͨ���\*.txt c:\a\b\
��飺�ҵ�bĿ¼�£�����*�����Ӧ��ֻ��Ŀ¼�е��ļ���Ŀ¼c
step6(���ļ��Ŀ�������):
copy e:\\������̼��\\���ļ�\\1.28G.zip c:\\
copy e:\\������̼��\\���ļ�\\1.9G.zip c:\\
��飺���ܳ��ֱ�������
rmdir����
step7(ɾ������Ŀ¼ʧ�ܲ���):
cd\
rmdir a
��飺����Ӧ����0
step8(������Ŀ¼ɾ��ʧ�ܲ���)��
rmdir c:\a\b\c\d\e
��飺����1��Ӧ���Ҳ���Ŀ¼e
step9(ɾ����ǰĿ¼�ɹ�����):
cd a\\b\\c\\d
rmdir /s
copy e:\\������̼��\\�����ļ�\\a.txt ��ǰĿ¼.txt
��飺����1���Ҳ���Ŀ¼d��������Ŀ¼c�����ҵ�"��ǰĿ¼.txt"
step10(Ŀ¼ɾ���ɹ�����)��
rmdir c:\a/s
��飺����1����c:\a�Ҳ���
step11(ɾ�������ڵ�Ŀ¼ʧ�ܲ���):
rmdir c:\a
��飺����0
*/

TEST_F(common_test,copyAndRmdir)
{
	//�����ڲ����ڵ�Ŀ¼�Ͽ��������ļ�
	int v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,0);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	//���Զ���ļ�����ʧ��
	
	v=GetVDiskProxy()->ExecCommand("mkdir a\\b");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),2);
	findres->Release();
	std::vector<std::string> copyCommand;
	copyCommand.push_back("copy e:\\������̼��\\�����ڵ�Ŀ¼\\*.txt c:\\a\\b\\");
	copyCommand.push_back("copy e:\\������̼��\\�����ڵ�Ŀ¼\\1.txt c:\\a\\b\\");
	copyCommand.push_back("copy e:\\������̼��\\ͨ���\\*.txt c:\\�����ڵ�Ŀ¼\\b\\");
	copyCommand.push_back("copy e:\\������̼��\\ͨ���\\*.txt c:\\a\\�����ڵ�Ŀ¼\\");
	copyCommand.push_back("copy e:\\������̼��\\ͨ���\\������.txt c:\\a\\b\\");
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

	// �����ļ������ɹ�����
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a\\b\\c\\d\\a>.txt");
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
	//�����Ѿ����ڵ��ļ�
	//������
	cout<<"(�����������N)"<<endl;
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,0);
	//����
	cout<<"(�����������Y)"<<endl;
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt a\\b\\c\\d\\a.txt");
	ASSERT_EQ(v,1);

	//����Ŀ¼�����ɹ�����
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\����Ŀ¼ a\\b\\c\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("C",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* c = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c->Find("*",false);
	ASSERT_EQ(findres->GetCount(),7);
	findres->Release();
	c = NULL;

	//ͨ�������
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\*.txt c:\\a\\b\\");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("b",true);
	ASSERT_EQ(findres->GetCount(),1);
	IDirProxy* b = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = b->Find("*",false);

	ASSERT_EQ(findres->GetCount(),6);
	findres->Release();
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\ͨ���\\?.cc c:\\a\\b\\");
	findres = b->Find("*",false);

	ASSERT_EQ(findres->GetCount(),7);
	findres->Release();
	b = NULL;

	//���ļ�����
//	ASSERT_NO_THROW(GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\���ļ�\\1.28G.zip c:\\"));
//	ASSERT_NO_THROW(GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\���ļ�\\1.9G.zip c:\\"));

	//ɾ��Ŀ¼ʧ�ܲ���

	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir a");
	ASSERT_EQ(v,0);

	//ɾ����Ŀ¼�ɹ�����

	v=GetVDiskProxy()->ExecCommand("rmdir a\\b\\c\\d\\e");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("cd a\\b\\c\\d\\e");
	ASSERT_EQ(v,0);
	//ɾ����ǰĿ¼�ɹ����ԣ����ҹ���ƶ����ϼ�Ŀ¼
	v=GetVDiskProxy()->ExecCommand("cd a\\b\\c\\d");
	ASSERT_EQ(v,1);	
	v=GetVDiskProxy()->ExecCommand("rmdir /s");
	ASSERT_EQ(v,1);
	cout <<endl<<"(�˴����ӦΪc:\\a\\b\\c\\>)"<<endl;
	system("pause");
	v=GetVDiskProxy()->ExecCommand("copy e:\\������̼��\\�����ļ�\\a.txt ��ǰĿ¼.txt");
	ASSERT_EQ(v,1);
	findres=rootdir->Find("d",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres=rootdir->Find("C",true);
	ASSERT_EQ(findres->GetCount(),1);
    c = dynamic_cast <IDirProxy*>(findres->GetItemByIdx(0));
	findres->Release();
	findres = c->Find("��ǰĿ¼.txt",false);
	ASSERT_EQ(findres->GetCount(),1);
	findres->Release();
	c = NULL;

	//ɾ���ǿ�Ŀ¼�ɹ�����

	v=GetVDiskProxy()->ExecCommand("rmdir c:\\a/s");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("cd c:\\a");
	ASSERT_EQ(v,0);

	//ɾ�������ڵ�Ŀ¼ʧ�ܲ���

	v=GetVDiskProxy()->ExecCommand("rmdir c:\\a");
	ASSERT_EQ(v,0);

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