#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include "common_testcase.h"
#include <vector>
#include <string>
#include <algorithm>
/*
Mkdir����
step1��mkdir a\b\c\d
       mkdir abc>def
��⣺root����aû��abc>ade��a����b....
step2��mkdir a
��⣺����Ӧ��ʧ��
mkdir a\b\c\d
��⣺����Ӧ��ʧ��
step3��
cd a
mkdir ����Ŀ¼1\����Ŀ¼2\����Ŀ¼3

��⣺����Ӧ�óɹ�����aĿ¼����"����*"Ӧ�����ҵ�3��Ŀ¼

step4:
mkdir a a\b b
��⣺����Ӧ�óɹ�����c:\Ŀ¼�����ҵ�2������ ������Ŀ¼
*/

TEST_F(common_test,mkdir1)
{
	//����һ�δ������Ŀ¼�����ִ����Ŀ¼
	//mkdir a\b\c\d
	int v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c\\d");
	ASSERT_EQ(v,1);
	//mkdir abc>def Ӧ����0
	v=GetVDiskProxy()->ExecCommand("mkdir abc>def");
	ASSERT_EQ(v,0);
	auto rootdir=GetVDiskProxy()->GetVolumnByIdx(0)->GetRootDir();
	auto findres=rootdir->Find("*",true);
	//c����Ӧ���ҵ�4��Ŀ¼�����ֱַ���a b c d
	std::vector<std::string> names;
	names.push_back("a");names.push_back("b");names.push_back("c");names.push_back("d");
	ASSERT_EQ(findres->GetCount(),4);
	for (int i=0;i<4;++i)
	{
		auto obj=findres->GetItemByIdx(i);
		std::string n=obj->GetName();
		auto itr=std::find(names.begin(),names.end(),n);
		ASSERT_NE(itr,names.end());
	}
	findres->Release();

	//�ظ�����Ŀ¼�������Ƿ񷵻ش���
	//mkdir a ����Ӧ��ʧ��
	v=GetVDiskProxy()->ExecCommand("mkdir a");
	ASSERT_EQ(v,0);
	
	//mkdir a/b/c ����Ӧ��ʧ��
	v=GetVDiskProxy()->ExecCommand("mkdir a\\b\\c");
	ASSERT_EQ(v,0);

	//��������·��
	v=GetVDiskProxy()->ExecCommand("cd a");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir ����Ŀ¼1\\����Ŀ¼2\\����Ŀ¼3");
	ASSERT_EQ(v,1);
	auto findres2=rootdir->Find("a",false);
	IDirProxy* a=(IDirProxy*)findres2->GetItemByIdx(0);
	EXPECT_EQ(a!=0,true);
    // ????
    a->Find("*",false)->display();
	ASSERT_EQ(a->Find("*",false)->GetCount(),2);
	findres2->Release();
	ASSERT_EQ(a->Find("����*",true)->GetCount(),3);
	findres2->Release();

	//���Դ��ո��Ŀ¼

	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("mkdir a a\\b b\\");
	ASSERT_EQ(v,1);
	findres2=rootdir->Find("? ?",true);
	EXPECT_EQ(findres2->GetCount(),2);
	findres2->Release();

	//���Ŀ¼��������һ�����԰������Ӱ��
	v=GetVDiskProxy()->ExecCommand("cd\\");
	ASSERT_EQ(v,1);
	v=GetVDiskProxy()->ExecCommand("rmdir/s");
	ASSERT_EQ(v,1);
	findres = rootdir->Find("*",true);
	ASSERT_EQ(findres->GetCount(),0);
	findres->Release();
	findres = NULL;
}