#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include "App.h"

int _tmain()
{
    _tsetlocale(LC_ALL, _T("chs"));
    
    App a;
    a.run();

	return 0;
}