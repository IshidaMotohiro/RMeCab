/*
  ver 1.05 2020 04 28

    全ての関数を使われる前に消し去りたい


　 　 　 　 |＼　　 　 　 　 　 ／|
　 　 　 　 |＼＼　　 　 　 ／／|
　　　　 　 : 　,>　｀´￣｀´　<　 ′
.　　　　 　 Ｖ　 　 　 　 　 　 Ｖ
.　　　　 　 i{　●　 　 　 ●　}i
　　　　 　 八　 　 ､_,_, 　 　 八 　　　わけがわからないよ 
. 　 　 　 /　个 . ＿　 ＿ . 个 ',
　　　＿/ 　 il 　 ,'　　　 '.　 li　 ',＿_


    docDF関数で全てをまかなえるから
  
*/

#include "RMeCab.h"



//#define CHECK(eval) if (! eval) {				\
//    fprintf (stderr, "Exception:%s\n", mecab_strerror (mecab));	\
//    mecab_destroy(mecab);						\
//    return (SEXP) -1; }
///////////////////////////////////////////////////////////////

int BUF1 = 128 * 4 ;  // 2016/12/27 2 -> 4
int BUF2 = 64 * 4 ;   // 2016/12/27 2 -> 4
int BUF3 = 1024 * 20 ; // 2016/12/27  2 -> 20
int BUF4 = 5120 * 40 ; // 2016/12/27 2 -> 40
int FILEINPUT = 5120 * 40 ; //2016/12/27 2  ->  40
int OVERLINE  = 999999 ; //2017 08 03 99999 -> 999999
int FILEN  = 99999 ; //2017 08 03 99999 -> 999999


/////////////////////////////////////////////////////////////
char meisi[128];
/////////////////////////////////////////////////////////////

// ishida@ishida-ubuntu8:~$ echo -n '形容詞' | hexdump -C
// 00000000  e5 bd a2 e5 ae b9 e8 a9  9e                       |.........|
// 00000009
char keiyou[128];
///////////////

// ishida@ishida-ubuntu8:~/research/statistics/myRcode$  echo -n '記号' | hexdump -C
// 00000000  e8 a8 98 e5 8f b7     
/////////////////////////////////////////////////////////////
char kigo[128];
////////////////////////////////////////////////

// extern "C" {
  
char  * meisiCode(){
    const char  meisi_utf8[]  =  {static_cast<char>(0xe5),
				  static_cast<char>(0x90),
				  static_cast<char>(0x8d),
				  static_cast<char>(0xe8),
				  static_cast<char>(0xa9),
				  static_cast<char>(0x9e),
				  0}; //名詞
  {
	void *cd;
	const char *i_buf    = meisi_utf8;
	size_t      i_len    = strlen(meisi_utf8);
	char       *o_buf    = meisi;
	size_t      o_len    = sizeof(meisi);
		cd = Riconv_open("","UTF-8");
		Riconv(cd, &i_buf, &i_len, &o_buf, &o_len);
		Riconv_close(cd);
  }
  return meisi;
}
/////////////////////////////////////////////////////////////

// ishida@ishida-ubuntu8:~$ echo -n '形容詞' | hexdump -C
// 00000000  e5 bd a2 e5 ae b9 e8 a9  9e                       |.........|
// 00000009
char  * keiyouCode(){
  //    SEXP res;
  const char  keiyou_utf8[]  =  {static_cast<char>(0xe5),
				 static_cast<char>(0xbd),
				 static_cast<char>(0xa2),
				 static_cast<char>(0xe5),
				 static_cast<char>(0xae),
				 static_cast<char>(0xb9),
				 static_cast<char>(0xe8),
				 static_cast<char>(0xa9),
				 static_cast<char>(0x9e),
				 0}; //形容詞
  //  char        buf[128];
  {
	void *cd;
	const char *i_buf    = keiyou_utf8;
	size_t      i_len    = strlen(keiyou_utf8);
	char       *o_buf    = keiyou;
	size_t      o_len    = sizeof(keiyou);
		cd = Riconv_open("","UTF-8");
		Riconv(cd, &i_buf, &i_len, &o_buf, &o_len);
		Riconv_close(cd);
  }
  return keiyou;
}
///////////////////////////////////////////////////////////////////
char  * kigoCode(){
    const char  kigo_utf8[]  =  {static_cast<char>(0xe8),
				 static_cast<char>(0xa8),
				 static_cast<char>(0x98),
				 static_cast<char>(0xe5),
				 static_cast<char>(0x8f),
				 static_cast<char>(0xb7),
				 0}; //記号
  //  char        buf[128];
  {
	void *cd;
	const char *i_buf    = kigo_utf8;
	size_t      i_len    = strlen(kigo_utf8);
	char       *o_buf    = kigo;
	size_t      o_len    = sizeof(kigo);
		cd = Riconv_open("","UTF-8");
		Riconv(cd, &i_buf, &i_len, &o_buf, &o_len);
		Riconv_close(cd);
  }
  return kigo;
}

  // }
