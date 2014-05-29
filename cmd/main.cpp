#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include "App.h"

#ifdef _DEBUG
#include <vld.h>
#endif

int _tmain()
{
    _tsetlocale(LC_ALL, _T("chs"));
    
    App a;
    a.run();

	return 0;
}