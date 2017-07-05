#ifndef _RMECAB_H
#define _RMECAB_H

#include <mecab.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <R_ext/Riconv.h>

//#include <dirent.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#include <string>
#include <map>
#include <list>
#include <vector>
// #include <functional>
// #include <algorithm>
#include <utility>
#include <locale.h>

using namespace std;


/*
vector <string> split(string str, string delim)
{
    vector <string> result;
    int cutAt;
    while( (cutAt = str.find_first_of(delim)) != str.npos )
    {
        if(cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.size() > 0)
    {
        result.push_back(str);
    }
return result;
}
*/

 /* ワイド文字列 <-> マルチバイト文字列
void  multi (const wstring  * &src, string &target ){  
        char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	target = mbs;
	delete [] mbs; 
}

void wide (const string &src, wstring &target) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	target = wcs;
	delete [] wcs;
}
*/

extern int utf8locale;



extern  int BUF1 ;
extern  int BUF2 ;
extern  int BUF3 ;
extern  int BUF4 ;
extern  int FILEINPUT  ;
extern  int OVERLINE  ;


/////////////////////////////////////////////////////////////
extern char meisi[128];
extern char  * meisiCode();

/////////////////////////////////////////////////////////////

// ishida@ishida-ubuntu8:~$ echo -n '形容詞' | hexdump -C
// 00000000  e5 bd a2 e5 ae b9 e8 a9  9e                       |.........|
// 00000009
extern char keiyou[128];
extern char  * keiyouCode(); 
///////////////

// ishida@ishida-ubuntu8:~/research/statistics/myRcode$  echo -n '記号' | hexdump -C
// 00000000  e8 a8 98 e5 8f b7     
/////////////////////////////////////////////////////////////
extern char kigo[128];
extern char  * kigoCode();
////////////////////////////////////////////////


#define CHECK(eval) if (! eval) { \
    fprintf (stderr, "Exception:%s\n", mecab_strerror (mecab)); \
    mecab_destroy(mecab); \
    return (SEXP) -1; }
///////////////////////////////////////////////////////////////

// extern "C" {
// }
#endif
