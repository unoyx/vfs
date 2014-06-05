/********************************************************************
	created:	2014/03/12
	created:	12:3:2014   17:17
	filename: 	F:\ĞéÄâ´ÅÅÌ´úÂë¼ì²é\Checker\VDiskChecker\VDiskChecker\common_testcase.h
	file path:	F:\ĞéÄâ´ÅÅÌ´úÂë¼ì²é\Checker\VDiskChecker\VDiskChecker
	file base:	common_testcase
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <gtest/gtest.h>
#include "IVirtualDiskProxy.h"
#include <Windows.h>

class common_test:public testing::Test
{
public:
	static void SetUpTestCase() ;
	static void TearDownTestCase();
	IVirtualDiskProxy* GetVDiskProxy();
};