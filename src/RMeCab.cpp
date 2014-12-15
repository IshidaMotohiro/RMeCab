/*
  ver 0.9998 2014 12 03

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

//#define WIN 0

#include <mecab.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <R_ext/Riconv.h>

/////////////////////extern "C" {
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
////////////////////}
extern int utf8locale;

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

/////////////////////////////////////////////////////////////
char meisi[128];
char  * meisiCode(){
  //    SEXP res;
  const char  meisi_utf8[]  =  {0xe5, 0x90, 0x8d, 0xe8, 0xa9, 0x9e, 0}; //名詞
  //  char        buf[128];
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
char keiyou[128];
char  * keiyouCode(){
  //    SEXP res;
  const char  keiyou_utf8[]  =  {0xe5,
								 0xbd,
								 0xa2,
								 0xe5,
								 0xae,
								 0xb9,
								 0xe8,
								 0xa9,
								 0x9e,
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
///////////////


// ishida@ishida-ubuntu8:~/research/statistics/myRcode$  echo -n '記号' | hexdump -C
// 00000000  e8 a8 98 e5 8f b7     
/////////////////////////////////////////////////////////////
char kigo[128];
char  * kigoCode(){
  //    SEXP res;
  const char  kigo_utf8[]  =  {0xe8, 0xa8, 0x98, 0xe5, 0x8f, 0xb7, 0}; //記号
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

////////////////////////////////////////////////




#define CHECK(eval) if (! eval) { \
    fprintf (stderr, "Exception:%s\n", mecab_strerror (mecab)); \
    mecab_destroy(mecab); \
    return (SEXP) -1; }
///////////////////////////////////////////////////////////////

const int BUF1 = 128 * 2 ;
const int BUF2 = 64 * 2 ;
const int BUF3 = 1024 * 2 ;
const int BUF4 = 5120 * 2 ;
const int FILEINPUT = 5120 * 2 ;
  
////////////////////////////////////////////////////////////////

SEXP setMeCabMap(int typeSet, char input[], map<string, int> & ma0, 	map<string, int>  &  ma1,  	map<string, int>::iterator & pma0, map<string, int>::iterator & pma,  list <string> & strL, 	 list <string>::iterator & iter,  list <string> & hinsi, 	 list <string>::iterator & hinsi_it,  list <string> & saibun, 	 list <string>::iterator & saibun_it, vector<string> & Ppos2, int pos_n, int Ngram, int genkei, const char * dic ){// map<string, int> &  ma1, char *Ppos[], 
  
  mecab_t *mecab;
  mecab_node_t *node;
	
 int i, j , posC = 0, xx =0; 	
 char buf1 [BUF1];// 2010 12 17 //[128];// [512];//入力された語形を記憶
 char buf2[BUF3];
 char buf3[BUF2];// 2010 12 17 //[64];// [512];記号チェック用
 char buf4[BUF2];// 2010 12 17 //[64];// [1024];記号チェック用
 string str;
 char *p;
	
	
  wchar_t  wbuf [BUF4] = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
  unsigned int  wz = 0;
	
  string target;
  char target2[BUF3];
			
				

  //	http://mecab.sourceforge.net/mecab.html
  mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");mecab_new2(" -d mecab\dic\ipadic -O ruby");
  CHECK(mecab);


	//Rprintf("%s strlen of input= %d\n", input, strlen(input)); 
			
  if(typeSet == 0){// 文字単位なら
	//			  Rprintf("in typeSet == 0 %s \n",  file_name );	
	//		  Rprintf("%s\n", input);			  
	p = strchr( input, '\n' );
	/* 改行文字があった場合 */
	if ( p != NULL )
	  {
		/* 改行文字を終端文字に置き換える */
		*p = '\0';
	  }
		
	//			Rprintf("strlen of input= %d\n", strlen(input));  
	if(strlen(input) > 0){
	  //Rprintf("%s\n", input);
				
	  //		  Rprintf("in strlen(input) > 0  %s \n",  file_name );				
	  mbstowcs(wbuf, input,  strlen(input));/* マルチバイト文字列をワイド文字列に変換*/

	  //for(int z = 0; z <  (wcslen(wbuf) - Ngram); z++){
	  for( wz = 0; wz <  wcslen(wbuf) ; wz++){

		// 2005 07 22
		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif
		
		//				Rprintf("target2 = %s\n", target2);
		if(strlen(target2) < 1){
		  break;
		}
		//エスケープ記号類
		//strcpy(buf1, *target2);
		if( *target2 > 0x00 && *target2 < 0x21 ){//エスケープ記号類0x0e
		  continue;
		}//
		//////////// windows では wsprintf(str[ys], "%lc", wbuf[z+ys + yw]);

		//	if( strcmp(target2, " ") == 0 ||  strcmp(target2, "　")==0){
		if( strcmp((char *) target2, " ") == 0 ||  strcmp((char *) target2, "　")==0){
		  //				  printf("found\n");
		  continue;
		} else{
				  
		  /////////////// new_begin //////////////// ここは文字単位
 
		  // target = target2;   

		  strL.push_back( target2);
					
		  if(strL.size() >= (unsigned int) Ngram){
			//					Rprintf("in if(strL.size) \n");					
			target.erase();
			//target.append("[");
			xx = 1;
			for ( iter = strL.begin(); iter != strL.end(); iter++){
			  //						Rprintf("in for\n");
			  //						Rprintf("str %s\n", * iter);
			  target.append( *iter);
			  if(xx < Ngram){
				 target.append(" ");//target.append("-");
			  }
			  xx++;
			  //					  Rprintf("xx = %d\n", xx);
						
			}
			xx = 1;
			//target.append("]");
			//					Rprintf("target %s\n", target);
			//					Rprintf("before m1.find \n");
			//出てきた形態素原型は既に全体マップにあるか？
			pma = ma0.find(target);
			//出てきた形態素原型は既にマップにあるか？
			if(pma != ma0.end()){
			  pma->second =  pma->second + 1;
			  //二つ目の数値を加算
			}					
			else{// マップにないなら，新規にマップに追加
			  //					  Rprintf("add map \n");
			  ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
			}
			// 同じ処理を個別マップにも行う
			pma = ma1.find(target);//出てきた形態素原型は既に個別マップにあるか？
			if(pma != ma1.end()){
			  pma->second =  pma->second + 1;
			  //二つ目の数値を加算
			}
			else{// マップにないなら，新規にマップに追加
			  ma1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
			}
					  
			strL.pop_front();
		  }//_if strSize>= Ngram
		}// _else_end
				  
		////////////////////////////////////// new _end ////
	  }//_for2_< wcslen
	  
	}// if_strlen_>_0_end




			  
			  
  } else {// if_type_set 形態素あるいは品詞の場合



	////////////////////////////////////////////////////////////////
	//			Rprintf("after fgets input =  %s\n",input );
	node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
	CHECK(node);
	//			Rprintf("node check" );		
	/// 解析結果のノードをなめる
	for (;  node; node = node->next) {// node とはその文の形態素ごとに設定される
	  //			  		printf("%d ", node->id);
			  
	  if (node->stat == MECAB_BOS_NODE)
		//printf("BOS");
		continue;
	  else if (node->stat == MECAB_EOS_NODE)
		//printf("EOS");
		continue;
	  else {// BOS, EOS 以外
	    // 2010  		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
		strncpy(buf1, node->surface, node->length) ;//元の語形
		
		buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
		// strlen関数はstringの文字数を返します。この長さには、終端のNULL文字('\0')は含まれません。
		if(strlen(buf1) < 1){// 2006 06 移動		
		  continue;
		}
		
		//< 2005 11 07> //Rprintf("%s\n", buf1);			
		//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){// if( atoi(buf1)  == 0x0e){//エスケープ記号類
		if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類0x0e // strlen(buf1) == 1 &&
		  continue;
		}// </ 2005 11 07>
		
		//		buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動				 
// 		if(strlen(buf1) < 1){// 2006 06 移動		
// 		  continue;
// 		}
		//				Rprintf("buf1 = %s\n", buf1);
				
		strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
		if(strlen(buf2) < 1){
		  continue;
		}
		//				Rprintf("buf2 = %s\n", buf2);

		//////////////

				
		p = strtok(buf2, "," );//取得情報の分割
		// 品詞の判定
		j = 1;
		////////////////////////////////////////////////////////////////////

					
		if(typeSet == 2){// 品詞情報で数える
		  if( j == 1 && p != NULL ){//品詞情報1
			strL.push_back(p);
			//						Rprintf("typeSet == = %d; p = %s\n", typeSet, p);					 
			p = NULL;
		  }


					
		}else if(typeSet == 1){// 形態素原形で数える
						  
		  //////////////////////////////////////////////


		  if(j == 1 &&  p != NULL){
			sprintf(buf3, "%s", p);
// 			// if(mSym < 1 && strcmp(buf3, "記号") == 0){
// 			if(mSym < 1 && strcmp(buf3, KIGO) == 0){						
// 			  p = NULL;
// 			  //j = 9;
// 			  continue;// 記号は一切省き，総計にも加えない
// 			}
// 			//	
//			Rprintf("buf3 %s\n", buf3);
			if(pos_n == 0){
			  hinsi.push_back(buf3);
			  posC = 1;
			}else{
			  for(i = 0; i < pos_n; i++){
			    sprintf(buf4, "%s", Ppos2[i].c_str());	// 2011 03 10 sprintf(buf4, "%s", Ppos[i]);				
				//					Rprintf("buf4 %s\n", buf4);
				
				if(strcmp(buf3, buf4) == 0){
				  posC = 1;
				  hinsi.push_back(buf3);
				  break;
				}
			  }
			}
			if(posC != 1){
			  p = NULL;
			  posC = 0;
			  continue;
			}
		  }
				  
				
		  while ( p != NULL ) {
					
			// if(j == 1){//品詞情報1
			// 					str = p;
			// 					// str.append(",");
			// 				  }else
			 if(j == 2){//品詞第2情報
			   saibun.push_back(p);
			 } else if( j == 7){
			  if(genkei == 1 || p == NULL || strcmp(p, "*") == 0){
				// strL.push_back(p);//原型str = buf1;// str.append(buf1);//元の語形
				strL.push_back(buf1);//元の語形
				//Rprintf("in str = buf1\n");
			  }
			  else{
				strL.push_back(p);//原型 strL.push_back(buf1);
				//Rprintf("in str = p\n");
			  }
			}
			p = strtok( NULL,"," );
			j++;
			if(j > 7){
			  p = NULL;
			}

				  
		  }// while(P!= NULL)
		  posC = 0;
		} // else if typset = 1

				  
	  }  //////else // BOS, EOS 以外

	  ////////////// 抽出終了
	  if(strL.size() >= (unsigned int) Ngram){// リストのサイズが指定通りであるなら，保存を始める
		//				  Rprintf("type = %d, strL size =  %d\n", typeSet, strL.size() );
		target.erase();//保存のための文字列を初期化
		target.append("");
		xx = 1;
		for ( iter = strL.begin(); iter != strL.end(); iter++){
		  // Rprintf("in for\n");
		  //sprintf(buf3, "%s", *iter);
		  //Rprintf("str %s\n", *iter);
		  //Rprintf("after Rprintf in for\n");
		  target.append( *iter);// target.append( buf3); //target.append( *iter);
		  //					Rprintf("target append\n");
		  if(xx < Ngram){
			 target.append(" ");//target.append("-");
		  }
		  xx++;
		} // for 
		xx = 1;
		if(typeSet == 1){
		  target.append(" ");
		  for ( hinsi_it = hinsi.begin(); hinsi_it != hinsi.end(); hinsi_it++){
		  // Rprintf("in for\n");
			//sprintf(buf3, "%s", *iter);
			//Rprintf("str %s\n", *iter);
			//Rprintf("after Rprintf in for\n");
			target.append( *hinsi_it);// target.append( buf3); //target.append( *iter);
		  //					Rprintf("target append\n");
			if(xx < Ngram){
			   target.append(" ");//target.append("-");
			}
			xx++;
		  } // for
		
		  xx = 1;
		
		  target.append(" ");
		  for ( saibun_it = saibun.begin(); saibun_it != saibun.end(); saibun_it++){
			// Rprintf("in for\n");
			//sprintf(buf3, "%s", *iter);
		  //Rprintf("str %s\n", *iter);
		  //Rprintf("after Rprintf in for\n");
			target.append( *saibun_it);// target.append( buf3); //target.append( *iter);
			//					Rprintf("target append\n");
		  if(xx < Ngram){
			 target.append(" ");//target.append("-");
		  }
		  xx++;
		  } // for
		
		  xx = 1;
		}//if(typeSet == 1){
		
		
		pma0 = ma0.find(target);//出てきた形態素原型は既に全体マップにあるか？
		if(pma0 != ma0.end()){
		  pma0->second =  pma0->second + 1;
		  //二つ目の数値を加算
		}
		else{// マップにないなら，新規にマップに追加
		  ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
		}
				  
		pma = ma1.find(target);// str 出てきた形態素原型は既に個別マップにあるか？
		if(pma != ma1.end()){
		  pma->second =  pma->second + 1;
		  //二つ目の数値を加算
		}
		else{// マップにないなら，新規にマップに追加
		  ma1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
		}
				  
		strL.pop_front();// 最初の要素を取り除く
		
		if(typeSet == 1){
		  hinsi.pop_front();
		  saibun.pop_front();
		}
	  }				  // if(strL.size() >= Ngram)
				
	}//for(;node;)// Rprintf("node check ended\n");
			  
  }
  	mecab_destroy(mecab);


	return (R_NilValue);// return 0;


}



//////////////////////

/////////////////////////////////////////////////////////
/* 文字列と解析方法が引数 */
extern "C" {
  // 2005 03 28 第三引数 (入力文の長さ) を削った
  SEXP  RMeCabC(SEXP str, SEXP mypref, SEXP mydic){ 
	const char* input = CHAR(STRING_ELT(str, 0));//解析対象文字列
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定オプション(辞書だけではない)

	// Rprintf(dic); 
	mecab_t *mecab;
	mecab_node_t *node;
	//	const char *result;
	int pref = INTEGER_VALUE(mypref);// もとの形 0 か原型 1 か
	int z = 0;int pa = 0;
	char buf1[BUF1];//入力された語形を記憶
	char buf2[BUF3];
 	char *p;
	SEXP my_list, my_char,my_char2;
	// int pa = 0; // PROTECT

	int i   = 0;//ループ用
	
	if(pref != 0 && pref != 1){ pref = 0;}

	// mecabによる解析を行う	
	mecab = mecab_new2 (dic);//mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");
	CHECK(mecab);	
	node = (mecab_node_t *) mecab_sparse_tonode(mecab, input);
	CHECK(node);
	// node の総数がチェックできれば，良いのだが
	
	// とりあえず空の要素を1000個持つリストを生成する
	PROTECT(my_list = allocVector(VECSXP, 1000));pa++;// pa++; 1


	for (;  node; node = node->next) {
	
		if (node->stat == MECAB_BOS_NODE)
			//printf("BOS");
			continue;
		else if (node->stat == MECAB_EOS_NODE)
			//printf("EOS");
			continue;
		else {
			//// node->length はノード全体ではなく，形態素の長さ
			// 元の単語を buf1 に記録
		  // 2010  		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);
		  strncpy(buf1, node->surface, node->length) ;

		  buf1[node->length] = '\0';// NULL-Terminateしておく // 2000 6 月移動
		  
		  if(strlen(buf1) < 1){// 2000 6 月移動
			continue;
		  }

		  
		  // < 2005 11 07>
		  // if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//エスケープ記号類
		  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
			continue;
		  }// </ 2005 11 07>
		  
// 		  buf1[node->length] = '\0';// NULL-Terminateしておく// 2000 6 月移動
// 		  //< 2005 11 07> //Rprintf("%s\n", buf1);			
		  
// 		  if(strlen(buf1) < 1){// 2000 6 月移動
// 			continue;
// 		  }
		  
		  strcpy(buf2, node->feature);// CSV で表記された形態素ごと(node)の素性情報
		  if(strlen(buf2) < 1){
			continue;
		  }

		  p = strtok(buf2, "," );
		  
		  PROTECT(my_char2 = allocVector(STRSXP,1));pa++;// pa++; 2

// 		  // < 2005 11 08>
// #if defined(WIN32)
// 		  SET_STRING_ELT(my_char2, 0, mkCharCE(p, CE_NATIVE)); 		
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(my_char2, 0, mkCharCE(p, CE_NATIVE));  // 品詞情報をセット		
// #else
// 		  // Rprintf("CE_UTF8\n");
// 		SET_STRING_ELT(my_char2, 0, mkCharCE(p, CE_UTF8));  // 品詞情報をセット		
// #endif
// 		//SET_STRING_ELT(my_char2, 0, mkChar(p));  // 形態素情報をセット
// 		// </ 2005 11 08>

		 SET_STRING_ELT(my_char2, 0, mkCharCE(p, (utf8locale)?CE_UTF8:CE_NATIVE));

		 
		  PROTECT(my_char = allocVector(STRSXP,1));pa++;// pa++; 3
			//文字型のベクトル一個を生成
			
		  if(pref == 1){// 原型を取るため，六個目の要素を取る
			 z = 0;
			while(z < 6 &&  p!= NULL){
			  p = strtok(NULL, "," );
			  z++;
			}
			if(p == NULL || strcmp(p, "*") == 0){

// 			  // < 2005 11 08>
// #if defined(WIN32)
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE)); 		
// #elif  defined(__MINGW32__)
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));  // 形態素情報をセット		
// #else
// 			  // Rprintf("CE_UTF8\n");
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_UTF8));  // 形態素情報をセット		
// #endif		  
// 			  // SET_STRING_ELT(my_char, 0, mkChar(buf1));//もとの語形をセット
// 			  // </ 2005 11 08>

		SET_STRING_ELT(my_char, 0, mkCharCE(buf1, (utf8locale)?CE_UTF8:CE_NATIVE)); 		  
			  
			}else if( p != NULL){

			  
			  // < 2005 11 08>
// #if defined(WIN32)
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(p, CE_NATIVE)); 		
// #elif  defined(__MINGW32__)
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(p, CE_NATIVE)); 	
// #else
// 			  //Rprintf("CE_UTF8\n");
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(p, CE_UTF8));		
// #endif
// 			  // SET_STRING_ELT(my_char, 0, mkChar(p));//原型をセット
// 			  // </ 2005 11 08>

			  SET_STRING_ELT(my_char, 0, mkCharCE(p, (utf8locale)?CE_UTF8:CE_NATIVE)); 

			  
			}
//////			z = 1;
		  }
		  else{

			
//  // < 2005 11 08>
// #if defined(WIN32)
// 			SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE)); 		
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));		
// #else
// 			  SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_UTF8));		
// #endif	
// 			// SET_STRING_ELT(my_char, 0, mkChar(buf1));//もとの単語をセット
// 			// </ 2005 11 08>

		SET_STRING_ELT(my_char, 0, mkCharCE(buf1, (utf8locale)?CE_UTF8:CE_NATIVE)); 			  
		  }
		  //p  = NULL;
		  //	SET_NAMED(my_char, 1);
		  SET_NAMES(my_char, my_char2);// 
		  //setAttrib(my_char, R_NamesSymbol, my_char2); //もと単語の名前に形態素情報
		  //UNPROTECT(2);// pa = pa - 1; 1
		  SET_VECTOR_ELT(my_list, i, my_char);// リストに追加
		  UNPROTECT(2);pa--;pa--;// pa = pa - 1; 1
		  i++;
		  if(i + 1000  > 199999){// リストが20万（四万）を越えるならエラーを起こしやすい
		    Rprintf("i == %d stop\n", i);
		    mecab_destroy(mecab);
		    //	my_list = my_char =  NULL;p = NULL;
		    UNPROTECT(pa);return(my_list);
		    /*
		      UNPROTECT(1);pa--;
		      PROTECT(my_list = lengthgets(my_list, 0));pa++;
		      UNPROTECT(1);pa--;
		    */
		    return (R_NilValue); 
		  }
			  

		// もしもリストの空きがなくなっているなら，1000個新規スペースを足す
		  if(length(my_list) <= i){
			 UNPROTECT(1);pa--;
			PROTECT(my_list = lengthgets(my_list, i+1000));pa++;// pa++;  1
		  }
		  p = NULL;
		  
		  // i++; 2010 08 23
		  
		}
		//memset(buf1,'\0',strlen(buf1));
		//memset(buf1,0,strlen(buf1));
		//memset(buf2,0,strlen(buf2));
	}

	mecab_destroy(mecab);
		
	UNPROTECT(1);  pa--;                                // pa--;0
	if(length(my_list) > i){
		PROTECT(my_list = lengthgets(my_list, i));pa++;// pa++;1
	}
	UNPROTECT(pa); //   UNPROTECT(1);   // pa--;                               // pa--;0

	return(my_list); 
}
}//_end_extern





///////////////////////////////////////////////////////////
/* テキストから読む */
extern "C" {
  SEXP RMeCabText(SEXP filename, SEXP mydic){ 

	const char* file = CHAR(STRING_ELT(filename, 0));
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	char input2[BUF4];
	mecab_t *mecab;
	mecab_node_t *node;
	int i = 0;
	int j = 0; int pa = 0;
	//	int console = 0;
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char *p;
	SEXP my_list, my_char;

	FILE *fp;// 2009 04 03
	
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");
	CHECK(mecab);

	// FILE *fp; 
	
// 	if(strlen(file) < 1){
// 	  Rprintf("file not found\n");
// 	  return(R_NilValue);	  
// 	}

	if((fp = fopen(file, "r")) == NULL){
	  Rprintf("file not found\n");
	  return(R_NilValue);
	}else{
	  
	  Rprintf("file = %s \n", file);
		// とりあえず空の要素を1000個持つリストを生成する
	  PROTECT(my_list = allocVector(VECSXP, 1000));  pa++; // pa++ 1;

	  while(!feof(fp)){
	    if(fgets(input2, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input2, 5120, fp) != NULL){
		  node = ( mecab_node_t * ) mecab_sparse_tonode(mecab, input2);
		  CHECK(node);
		  ////////// PROTECT(my_char=allocVector(STRSXP,15));
		  
				/// 解析結果のノードをなめる
		  for (;  node; node = node->next) {
			//		printf("%d ", node->id);
			
			if (node->stat == MECAB_BOS_NODE)
			  //printf("BOS");
			  continue;
			else if (node->stat == MECAB_EOS_NODE)
			  //printf("EOS");
			  continue;
			else {
			  // 2010 		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);
			  strncpy(buf1, node->surface, node->length) ;//元の語形

			  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			  
			  /////////	strncpy(buf2, node->feature, node->length) ;
			  if(strlen(buf1) < 1){// 2006 06 移動
				continue;
			  }

			  //
			  //< 2005 11 07> //Rprintf("%s\n", buf1);			
			  // if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
			  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
				continue;
			  }// </ 2005 11 07>
			  
// 			  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			  
// 			  /////////	strncpy(buf2, node->feature, node->length) ;
// 			  if(strlen(buf1) < 1){// 2006 06 移動
// 				continue;
// 			  }

			  PROTECT(my_char=allocVector(STRSXP,10));pa++;
			  // トークン解析結果用ベクトル生成  // pa++ 2;

			  

//  // < 2005 11 08>
// #if defined(WIN32)
// 			  SET_STRING_ELT(  my_char  ,  0, mkCharCE( buf1 , CE_NATIVE  )); 		
// #elif  defined(__MINGW32__)
// 			  SET_STRING_ELT(  my_char  ,  0, mkCharCE( buf1 , CE_NATIVE ));		
// #else
// 			  SET_STRING_ELT(  my_char  , 0, mkCharCE(  buf1  , CE_UTF8));		
// #endif			  
// 			  //			  SET_STRING_ELT(my_char, 0, mkChar(buf1));
// // </ 2005 11 08>

			  SET_STRING_ELT(  my_char  ,  0, mkCharCE( buf1 , (utf8locale)?CE_UTF8:CE_NATIVE  ));

			  
			  //元トークンを冒頭にセット
			  strcpy(buf2, node->feature);//解析情報の取得.要素数は 9
			  if(strlen(buf2) < 1){
				continue;
			  }

			  p = strtok(buf2, "," );//取得情報の分割
			  j = 1;
			  while ( p != NULL && j < 10) {



//  // < 2005 11 08>
// #if defined(WIN32)
// 			  SET_STRING_ELT( my_char   ,  j, mkCharCE( p, CE_NATIVE  )); 		
// #elif  defined(__MINGW32__)
// 			  SET_STRING_ELT(  my_char  ,  j, mkCharCE( p , CE_NATIVE ));		
// #else
// 			  SET_STRING_ELT(  my_char  , j, mkCharCE( p   , CE_UTF8));		
// #endif				
// 			  // SET_STRING_ELT(my_char, j, mkChar(p));//元トークンを冒頭にセット
// // </ 2005 11 08>

				SET_STRING_ELT( my_char,  j, mkCharCE( p, (utf8locale)?CE_UTF8:CE_NATIVE  )); 
				
				p = strtok( NULL,"," );
				j++;
			  }
			  SET_VECTOR_ELT(my_list, i, my_char);
			  UNPROTECT(1);    pa--;       //pa-- 1;
			  i++;
			  if(i + 1000  > 199999){// リストが10万(四万)を越えるならエラーを起こしやすい
				Rprintf("i == %d stop\n", i);
				fclose(fp);mecab_destroy(mecab);
				//	my_list = my_char =  NULL;p = NULL;
				UNPROTECT(pa);return(my_list);
				/*
				UNPROTECT(1);pa--;
				PROTECT(my_list = lengthgets(my_list, 0));pa++;
				UNPROTECT(1);pa--;
				*/
				return (R_NilValue); 
			  }
			  
			  

			  // もしもリストの空きがなくなっているなら，1000個新規スペースを足す
			  if(length(my_list) <= i){
				UNPROTECT(1);  pa--;   // pa--; 0
				PROTECT(my_list = lengthgets(my_list, i+1000)); pa++;// pa++ 1;
			  }
			  
			  // i++;; 2010 08 23
			}
			//memset(buf1,'\0',strlen(buf1));
			//memset(buf1,0,strlen(buf1));
			//memset(buf2,0,strlen(buf2));		
		  }// for
		}//if
	  }// while(!feof(fp));//while
	  
	  fclose(fp);
	  mecab_destroy(mecab);
	  //  UNPROTECT(2);
		// リスト末尾に空きがあるなら取り除く
	  UNPROTECT(1);  pa--;                  // pa-- 0;
	  if(length(my_list) > i){
		PROTECT(my_list = lengthgets(my_list, i)); pa++; // pa++ 1;
	  }

	  
	  UNPROTECT(pa); // UNPROTECT(1);                                  // pa-- 1;
	  return(my_list);
	  
	}//_else_end
	return(R_NilValue);//return(0);
}
}//_end_extern






//////////////////////////////////////////////////////
/* データフレームから指定された列の文字列を読む */
extern "C" {

  SEXP RMeCabFreq(SEXP dfCol, SEXP mydic ){ 

  const char* input = CHAR(STRING_ELT(dfCol,0));
  const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
  
  
  //  const int * flag = INTEGER(dfY);
  char input2[BUF4];//char input2[5120];
  mecab_t *mecab;
  mecab_node_t *node;
  int j = 0, n = 0, z = 0;
  int pc = 0;
  //  int console = 0;
  char buf1[BUF1];// [512];//入力された語形を記憶
  char buf2[BUF3];
  char *p;
  //SEXP mydf, my_list, my_char, tmp, varlabels,row_names;
  SEXP mydf, tmp, varlabels, row_names;
    
  map<string, int> m1;
  map<string, int>::iterator pa; 
  //  pair<string, int> pa;
  
  string str;// 2009 04 03
  FILE *fp; // 2009 04 03

  mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");
  CHECK(mecab);

  // string str;

   
  // FILE *fp;


  if((fp = fopen(input, "r")) == NULL){
	Rprintf("no file found\n");
	return(R_NilValue);
  }else{
	Rprintf("file = %s \n", input);
	// とりあえず空の要素を1000個持つリストを生成する
	//// for_list//PROTECT(my_list = allocVector(VECSXP, 1000));
	while(!feof(fp)){
	  if(fgets(input2, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input2, 5120, fp) != NULL){
		node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input2);
		CHECK(node);
		
		/// 解析結果のノードをなめる
		for (;  node; node = node->next) {
		  //		printf("%d ", node->id);
		  
		  if (node->stat == MECAB_BOS_NODE)
			//printf("BOS");
			continue;
		  else if (node->stat == MECAB_EOS_NODE)
			//printf("EOS");
			continue;
		  else {
		    // 2010 		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);
 			strncpy(buf1, node->surface, node->length) ;//元の語形

			buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			
			if(strlen(buf1) < 1){// 2006 06 移動
				continue;
			}
			
			//< 2005 11 07> //Rprintf("%s\n", buf1);			
			//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
			if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
			  continue;
			}// </ 2005 11 07>
			
//  			buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			
// 			if(strlen(buf1) < 1){// 2006 06 移動
// 				continue;
// 			}
		
			strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
			if(strlen(buf2) < 1){
				continue;
			}

			p = strtok(buf2, "," );//取得情報の分割
			j = 1;
			while ( p != NULL ) {
			  if(j == 1){//品詞情報1
				str = p;
				str.append(" ");
			  }
			  else if(j == 2){//品詞情報2
				str.append(p);
				str.append(" ");
			  }
			  else if( j == 7){
				if(p == NULL || strcmp(p, "*") == 0){
				  str.append(buf1);//元の語形
				}
				else{
				  str.append(p);
				}
				pa = m1.find(str);//出てきた形態素原型は既にマップにあるか？
				if(pa != m1.end()){
				  pa->second =  pa->second + 1;
				  //二つ目の数値を加算
				}
				else{// マップにないなら，新規にマップに追加
				  m1.insert(make_pair(str, 1));// 1 は 1個目と言う意味
				}
			  }
			  p = strtok( NULL,"," );
			  j++;
			}						
		  }
		  //memset(buf1,'\0',strlen(buf1));
		  //memset(buf1,0,strlen(buf1));
		  //memset(buf2,0,strlen(buf2));		
		}// for
	  }//if
	}// while(!feof(fp));//while
	
	fclose(fp);
	mecab_destroy(mecab);
	
	// map の内容を data.frame にして返す
	n = (int)m1.size();
	if(n < 1){
	  Rprintf("empty results\n");
	  UNPROTECT(pc);
	  return (R_NilValue);
	}

	
	PROTECT(mydf = allocVector(VECSXP, 4));//4 列のdata.frame
	pc++;
	SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n));//形態素原型
	SET_VECTOR_ELT(mydf, 1, allocVector(STRSXP, n));//品詞情報1
	SET_VECTOR_ELT(mydf, 2, allocVector(STRSXP, n));//品詞情報2	
	SET_VECTOR_ELT(mydf, 3, allocVector(INTSXP, n));// 頻度
	// オブジェクトはデータフレームだと指定する
	PROTECT(tmp = mkString("data.frame"));
	pc++;
	//df 内ベクトルの名前を用意
	PROTECT(varlabels = allocVector(STRSXP, 4));
	pc++;
	//  その単純な初期化

//  // < 2005 11 08>
// #if defined(WIN32)
// 	SET_STRING_ELT(varlabels, 0, mkCharCE("Term", CE_NATIVE));
// 	SET_STRING_ELT(varlabels, 1, mkCharCE("Info1", CE_NATIVE));
// 	SET_STRING_ELT(varlabels, 2, mkCharCE("Info2", CE_NATIVE));
// 	SET_STRING_ELT(varlabels, 3, mkCharCE("Freq", CE_NATIVE));			  		
// #elif  defined(__MINGW32__)
// 	SET_STRING_ELT(varlabels, 0, mkCharCE("Term", CE_NATIVE));
// 	SET_STRING_ELT(varlabels, 1, mkCharCE("Info1", CE_NATIVE));
// 	SET_STRING_ELT(varlabels, 2, mkCharCE("Info2", CE_NATIVE));
// 	SET_STRING_ELT(varlabels, 3, mkCharCE("Freq", CE_NATIVE));			  	
// #else
// 	SET_STRING_ELT(varlabels, 0, mkCharCE("Term" , CE_UTF8 ));
// 	SET_STRING_ELT(varlabels, 1, mkCharCE("Info1", CE_UTF8 ));
// 	SET_STRING_ELT(varlabels, 2, mkCharCE("Info2", CE_UTF8 ));
// 	SET_STRING_ELT(varlabels, 3, mkCharCE("Freq", CE_UTF8 ));			 	
// #endif	
// // </ 2005 11 08>

	SET_STRING_ELT(varlabels, 0, mkCharCE("Term", (utf8locale)?CE_UTF8:CE_NATIVE));
	SET_STRING_ELT(varlabels, 1, mkCharCE("Info1", (utf8locale)?CE_UTF8:CE_NATIVE));
	SET_STRING_ELT(varlabels, 2, mkCharCE("Info2", (utf8locale)?CE_UTF8:CE_NATIVE));
	SET_STRING_ELT(varlabels, 3, mkCharCE("Freq", (utf8locale)?CE_UTF8:CE_NATIVE));

	
	Rprintf("length = %d \n", m1.size());
	pa = m1.begin();

	for ( z = 0; z < n; z++) {
	  char s [256];
	  char *token;
	  strcpy(s, (pa->first).c_str());
	  //	  Rprintf("%s\n", s);
	  token = strtok(s, " ");// 最初の要素は品詞情報1

// 	 // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 1), z, mkCharCE(token, CE_NATIVE));		 		
// #elif  defined(__MINGW32__)
// 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 1), z, mkCharCE(token, CE_NATIVE));		 	
// #else
// 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 1), z, mkCharCE(token, CE_UTF8));
// #endif	  
// 	  //	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 1), z, mkChar(token));
// // </ 2005 11 08>

	  SET_STRING_ELT(VECTOR_ELT(mydf, 1), z, mkCharCE(token, (utf8locale)?CE_UTF8:CE_NATIVE));///////<2006 04 18>
	  
	  token = strtok(NULL, " ");// 次の要素は品詞情報2

  
 // // < 2005 11 08>
// #if defined(WIN32)
// 		SET_VECTOR_ELT(VECTOR_ELT(mydf, 2), z, mkCharCE(token, CE_NATIVE));	 	
// #elif  defined(__MINGW32__)
// 		SET_VECTOR_ELT(VECTOR_ELT(mydf, 2), z, mkCharCE(token, CE_NATIVE));	 	
// #else
// 		SET_VECTOR_ELT(VECTOR_ELT(mydf, 2), z, mkCharCE(token, CE_UTF8));	 	
// #endif	  
// 		// SET_VECTOR_ELT(VECTOR_ELT(mydf, 2), z, mkChar(token));
// // </ 2005 11 08>
	  
	  SET_STRING_ELT(VECTOR_ELT(mydf, 2), z, mkCharCE(token, (utf8locale)?CE_UTF8:CE_NATIVE));	 ///////<2006 04 18>
	  


	  
	  token = strtok( NULL, " ");// 次が形態素原型

//  // < 2005 11 08>
// #if defined(WIN32)
// 		 SET_VECTOR_ELT(VECTOR_ELT(mydf,0), z, mkCharCE(token, CE_NATIVE));	 	
// #elif  defined(__MINGW32__)
// 		 SET_VECTOR_ELT(VECTOR_ELT(mydf,0), z, mkCharCE(token, CE_NATIVE));	 	
// #else
// 		 SET_VECTOR_ELT(VECTOR_ELT(mydf,0), z, mkCharCE(token, CE_UTF8));	 	
// #endif	  
// 		 //	  SET_VECTOR_ELT(VECTOR_ELT(mydf,0), z, mkChar(token));
//  // </ 2005 11 08>

	  SET_STRING_ELT(VECTOR_ELT(mydf,0), z, mkCharCE(token, (utf8locale)?CE_UTF8:CE_NATIVE)); ///////<2006 04 18>

	  

	  INTEGER(VECTOR_ELT(mydf,3))[z] =   pa->second;// 最後に頻度情報

	  // Rprintf("map %d ; word = %s: numb = %d\n", i, s, pa->second);
	  pa++;
	}

	// データフレームオブジェクト mydf の属性設定 
	setAttrib(mydf, R_ClassSymbol, tmp);
	setAttrib(mydf, R_NamesSymbol, varlabels);
	
	// 行名を指定．必須
	PROTECT(row_names = allocVector(STRSXP, n));
	pc++;
	char labelbuff[6];//char labelbuff[n]; 2006 03
	for ( z = 0; z < n; z++) {
	  sprintf(labelbuff, "%d", z+1);

  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));		 	
// #else
// 	  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));	 	
// #endif
 //	  SET_STRING_ELT(row_names, z, mkChar(labelbuff));
  // </ 2005 11 08>


	   SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));
	   
	  
	}
	setAttrib(mydf, R_RowNamesSymbol, row_names);

	UNPROTECT(pc);
	return (mydf);
  }
  return(R_NilValue);
}
}// end_extern
///////////////////////////////////////////////
//}// end_extern 








/////////////////////////////////////////////////////////
/* 文書行列作成関数 文字列と解析方法が引数 */
extern "C" {
  // 2005 03 28 第三引数 (入力文の長さ) を削った
  SEXP  RMeCabDoc(SEXP filename, SEXP mypref, SEXP kigo, SEXP mydic ){ 

	const char* input = CHAR(STRING_ELT(filename,0));//解析対象ファイル名
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	int sym = INTEGER_VALUE(kigo);// 記号を含むか含まない
		// const char* KIGO = CHAR(STRING_ELT(kigo,0));
	char KIGO[BUF1] ;
	strcpy(KIGO, kigoCode());
	
// 	if(sym == 1 ){// 記号を含むので，「記号」をセットする
// 	  strcpy(KIGO, kigoCode());
// 	}

	char input2[BUF4];
	mecab_t *mecab;
	mecab_node_t *node;
	int pref = INTEGER_VALUE(mypref);// もとの形 0 か原型 1 か
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char buf3[BUF2];// [256]; 記号チェック用
 	char *p;
	SEXP my_list, my_char,my_char2;

	
 	
	FILE *fp;
	  
	int i = 0;//ループ用
	int z = 0;

	
	if(pref != 0 && pref != 1){ pref = 0;}
	
	if((fp = fopen(input, "r")) == NULL){
	  Rprintf("file not found\n");
	  return(R_NilValue);
	}else{
	  Rprintf("file = %s \n", input);
	  // mecabによる解析準備	
	  mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	  CHECK(mecab);	
	  // とりあえず空の要素を1000個持つリストを生成する
	  PROTECT(my_list = allocVector(VECSXP, 2000));                //pa++; 1
	  //ファイル読み込み開始

	  
	  while(!feof(fp)){
	    if(fgets(input2, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input2, 5120, fp) != NULL){
		  node = (mecab_node_t *) mecab_sparse_tonode(mecab, input2);
		  CHECK(node);
		  for (;  node; node = node->next) {
			
			if (node->stat == MECAB_BOS_NODE)
			  //printf("BOS");
			  continue;
			else if (node->stat == MECAB_EOS_NODE)
			  //printf("EOS");
			  continue;
			else {
			  //// node->length はノード全体ではなく，形態素の長さ
			  // 元の語形を buf1 に記録
			  // 2010 		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);
			  strncpy(buf1, node->surface, node->length) ;

			  buf1[node->length] = '\0';// NULL-Terminateしておく// 2006 06 移動
			  
			  if(strlen(buf1) < 1){// 2006 06 移動
				continue;
			  }
			  
			  //< 2005 11 07> //Rprintf("%s\n", buf1);			
			  //if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
			  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
				continue;
			  }// </ 2005 11 07>
			  
// 			  buf1[node->length] = '\0';// NULL-Terminateしておく// 2006 06 移動
			  
// 			  if(strlen(buf1) < 1){// 2006 06 移動
// 				continue;
// 			  }

			  strcpy(buf2, node->feature);// CSV で表記された形態素ごと(node)の素性情報
			  if(strlen(buf2) < 1){
				continue;
			  }
			  
			  p = strtok(buf2, "," );
			  if(p != NULL){
				sprintf(buf3, "%s", p);
				if( sym == 0 && strcmp(buf3, KIGO) == 0){
				  continue;// 記号はカウントしない
				}
				PROTECT(my_char2 = allocVector(STRSXP,1));         // pa++; 2


  
//  // < 2005 11 08>
// #if defined(WIN32)
// 				SET_STRING_ELT(my_char2, 0, mkCharCE(p, CE_NATIVE));  // 形態素情報をセット
// #elif  defined(__MINGW32__)
// 				SET_STRING_ELT(my_char2, 0, mkCharCE(p, CE_NATIVE));  // 形態素情報をセット
// #else
// 			 	SET_STRING_ELT(my_char2, 0, mkCharCE(p, CE_UTF8));  // 形態素情報をセット	
// #endif	
// 				//				SET_STRING_ELT(my_char2, 0, mkChar(p));  // 形態素情報をセット
// // </ 2005 11 08>
				

			SET_STRING_ELT(my_char2, 0, mkCharCE(p, (utf8locale)?CE_UTF8:CE_NATIVE));  // 形態素情報をセット	
				
				
				PROTECT(my_char = allocVector(STRSXP,1));          // pa++; 3
				//文字型のベクトル一個を生成
				
				if(pref == 1){// 原型を取るため，六個目の要素を取る
				  z = 0;
				  while(z < 6 &&  p!= NULL){
					
					p = strtok(NULL, "," );
					z++;
				  }
				  
				  if (p==NULL || strcmp(p, "*") == 0){//

 // // < 2005 11 08>
// #if defined(WIN32)
// 					SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));	
// #elif  defined(__MINGW32__)
// 					SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));	
// #else
// 			 		SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_UTF8));
// #endif					
// 					//					SET_STRING_ELT(my_char, 0, mkChar(buf1));//buf1//入力語形をセット
//   // </ 2005 11 08>
					
					SET_STRING_ELT(my_char, 0, mkCharCE(buf1, (utf8locale)?CE_UTF8:CE_NATIVE));
					

				  }else if (p!=NULL){
					

 // // < 2005 11 08>
// #if defined(WIN32)
// 			 		SET_STRING_ELT(my_char, 0, mkCharCE(p, CE_NATIVE));//原型をセット
// #elif  defined(__MINGW32__)
// 			 		SET_STRING_ELT(my_char, 0, mkCharCE(p, CE_NATIVE));//原型をセット
// #else
// 			 		SET_STRING_ELT(my_char, 0, mkCharCE(p, CE_UTF8));//原型をセット
// #endif					
// 					// SET_STRING_ELT(my_char, 0, mkChar(p));//原型をセット
//   // </ 2005 11 08>

					
				SET_STRING_ELT(my_char, 0, mkCharCE(p, (utf8locale)?CE_UTF8:CE_NATIVE));//原型をセット
				
					
				  }
				  else{

//  // < 2005 11 08>
// #if defined(WIN32)
// 			 	SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));
// #elif  defined(__MINGW32__)
// 			 	SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));
// #else
// 			 	SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_UTF8));
// #endif	
// 				//SET_STRING_ELT(my_char, 0, mkChar(buf1));
// 					// </ 2005 11 08>

	SET_STRING_ELT(my_char, 0, mkCharCE(buf1, (utf8locale)?CE_UTF8:CE_NATIVE));
				
				  }
				  
				}
				else{
				  
//  // < 2005 11 08>
// #if defined(WIN32)
// 			 	 SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));//
// #elif  defined(__MINGW32__)
// 			 	 SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_NATIVE));//
// #else
// 			 	 SET_STRING_ELT(my_char, 0, mkCharCE(buf1, CE_UTF8));//
// #endif	  
// 				 //SET_STRING_ELT(my_char, 0, mkChar(buf1));//もとの単語をセット
// // </ 2005 11 08>


				   SET_STRING_ELT(my_char, 0, mkCharCE(buf1, (utf8locale)?CE_UTF8:CE_NATIVE));//
				  
				}
				SET_NAMES(my_char, my_char2);//
				//UNPROTECT(2);                              // pa = pa - 2; 1
				
				SET_VECTOR_ELT(my_list, i, my_char);// リストに追加
				UNPROTECT(2);
				
				// もしもリストの空きがなくなっているなら，1000個新規スペースを足す
				if(length(my_list) <= i){
				  UNPROTECT(1);
				  PROTECT(my_list = lengthgets(my_list, i+1000));// pa++;  1
				}
				
				
				p = NULL;
				
				i++;
				if(i > 40000){
				  Rprintf("node numbers i == %d stop\n", i);
				  fclose(fp);
				  UNPROTECT(1);
				  PROTECT(my_list = lengthgets(my_list, 0));
				  UNPROTECT(1);
				  return (R_NilValue); 
				}
			  }
			}
			
			//memset(buf1,0,strlen(buf1));
			//memset(buf2,0,strlen(buf2));
		  }//_end_for_
		}//_fgets_end
	  }// _while_end
	  UNPROTECT(1);                                            // pa--;0
	  if(length(my_list) > i){
		PROTECT(my_list = lengthgets(my_list, i));             // pa++;1
	  }
	  UNPROTECT(1);
                                       // pa--;0
	  mecab_destroy(mecab);

	  fclose(fp); 
	  return(my_list);
	}//end_else
  }//end_RMeCabDoc
}//_end_extern


















//////////////////////////////////////////////////////
/* データフレームから指定された列の文字列を読む */
extern "C" {

  SEXP RMeCabMx(SEXP filename, SEXP pos, SEXP posN, SEXP minFreq, SEXP kigo , SEXP mydic  ){ // SEXP sym, SEXP kigo

	//	Rprintf("in RMeCabMX\n");

  const char* input = CHAR(STRING_ELT(filename,0));
  const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
  
  char KIGO[BUF1]; // = CHAR(STRING_ELT(kigo,0));
  strcpy(KIGO, kigoCode());
  //strcpy(KIGO, "記号");
  //  Rprintf("before minFreq\n");
  int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
 
  int mSym  = INTEGER_VALUE( kigo );// INTEGER_VALUE( sym ) 記号を含めるか 0 含めない;1 含める

  int mF  = 0;// map オブジェクト内の最小頻度以下の個数を記憶

  int pos_n  = INTEGER_VALUE( posN );// pos の数
  bool flag = 1;
  if(pos_n < 1 ){
	pos_n = 2;// = 1
	flag = 0;
  }
  
  // char *Ppos[pos_n]; // 2011 03 10 
  vector<string> Ppos2;  

  int totalM = 0;
  char input2[BUF4];//char input2[5120];char input2[5120];
  mecab_t *mecab;
  mecab_node_t *node;

  int i, j, n, z, zz  = 0;
  int posC = 0;
  int pc = 0;
  //  int console = 0;
  char buf1[BUF1];// [512];//入力された語形を記憶
  char buf2[BUF3];
  char buf3[BUF2];// [128];記号チェック用
  char buf4[BUF2];// [128];  記号チェック用
  char *p;
  //  SEXP mydf, tmp, varlables, row_names;
  SEXP vecInt, vecName, myPos;    
  map<string, int> m1;
  map<string, int>::iterator pa; 

  //Rprintf("before MeCab\n");

  string str;// 2009 04 03
  FILE *fp; // 2009 04 03


  mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
  CHECK(mecab);

 
  //Rprintf("before Pos set\n");
	
  if(pos_n > 0 && flag){
    PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 for( i = 0; i < pos_n; i++){
	  // Ppos[i] = R_alloc( (unsigned int ) strlen (CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // }
  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
    for( i = 0; i < pos_n; i++){
	  // 2011 03 10 strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
      Ppos2.push_back(CHAR(STRING_ELT(myPos, i)));  // 2011 03 10 
	//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
      if(strcmp(Ppos2[i].c_str() , KIGO) == 0){// if(strcmp(Ppos[i], KIGO) == 0){
	mSym = 1;
      }
    }
  }else{
	PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());

	
	// 2011 03 10 // Ppos[0] = R_alloc(  (unsigned int )  strlen( meisiCode()), sizeof(char));
	// 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	Ppos2.push_back( meisiCode() ); // 2011 03 10 
	// 2011 03 10 //  Ppos[1] = R_alloc( (unsigned int )  strlen( keiyouCode() ), sizeof(char));
	// 2011 03 10 //  strcpy(Ppos[1], keiyouCode() );
	Ppos2.push_back( keiyouCode() ); // 2011 03 10 

// 	char * tmp;
// 	strcpy(tmp, (const char*) meisiCode());
// 	strcpy(Ppos[0], tmp );
	  
// 	strcpy(tmp, (const char*) keiyouCode());
// 	strcpy(Ppos[0], tmp );
	  


  }

  //Rprintf("before file to open\n");
  if((fp = fopen(input, "r")) == NULL){
	Rprintf("no file found\n");
	return(R_NilValue);
  }else{
	
	Rprintf("file = %s\n",input );	
	
	while(!feof(fp)){
	  if(fgets(input2, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input2, 5120, fp) != NULL){
		node = ( mecab_node_t * ) mecab_sparse_tonode(mecab, input2);
		CHECK(node);
		
		/// 解析結果のノードをなめる
		for (;  node; node = node->next) {
		  //		printf("%d ", node->id);
		  
		  if (node->stat == MECAB_BOS_NODE)
			//printf("BOS");
			continue;
		  else if (node->stat == MECAB_EOS_NODE)
			//printf("EOS");
			continue;
		  else {
		    // 2010 		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);
 			strncpy(buf1, node->surface, node->length) ;//元の語形

			buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			
			if(strlen(buf1) < 1){
				continue;// 2006 06 移動
			}
			
			//< 2005 11 07> //Rprintf("%s\n", buf1);			
			//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
			if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
			  continue;
			}// </ 2005 11 07>
			
//  			buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			
// 			if(strlen(buf1) < 1){// 2006 06 移動
// 				continue;
// 			}
		
			strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
			if(strlen(buf2) < 1){
				continue;
			}

			p = strtok(buf2, "," );//取得情報の分割
			if( p != NULL){
			  sprintf(buf3, "%s", p);
			  //Rprintf("%s\n", buf3);
			  //if(mSym < 1 && strcmp(buf3, "記号") == 0){
			  if(mSym < 1 && strcmp(buf3, KIGO) == 0){	
				continue;
			  }
			  totalM++;
			  for( i = 0; i < pos_n; i++){
			    sprintf(buf4, "%s", Ppos2[i].c_str());// 2011 03 10 // sprintf(buf4, "%s", Ppos[i]);				
				//				Rprintf("buf4 %s\n", buf4);
				if(strcmp(buf3, buf4) == 0){
				  posC = 1;
				}
			  }
			  if(posC != 1){
				//				mFreq++;
				//				mF++;
				p = NULL;
				posC = 0;
				continue;
			  }
			}
			j = 1;
			while ( p != NULL ) {
			  if( j == 1){//品詞情報1
				str = p;
				//				str.append(",");
			  }
			  else if( j == 7){
				if(p == NULL || strcmp(p, "*") == 0){
				  str = buf1;//元の語形
			   
				  //				  str.append(buf1);//元の語形
				}
				else{
				  //				  str.append(p);
				  str = p;
				 
				}
				pa = m1.find(str);//出てきた形態素原型は既にマップにあるか？
				if(pa != m1.end()){
				  pa->second =  pa->second + 1;
				  //二つ目の数値を加算
				}
				else{// マップにないなら，新規にマップに追加
				  m1.insert(make_pair(str, 1));// 1 は 1個目と言う意味
				}
			  }
			  p = strtok( NULL,"," );
			  posC = 0;
			  j++;
			}						

		  }
		  //memset(buf1,'\0',strlen(buf1));
		  //memset(buf1,0,strlen(buf1));
		  //memset(buf2,0,strlen(buf2));		
		}// for
	  }//if
	}// while(!feof(fp));//while
	
	UNPROTECT(pc);//この段階では PC は １のはず
	pc--;// ゼロにする
	fclose(fp);
	mecab_destroy(mecab);

	//	  return(R_NilValue);
	
	sprintf(buf3, "[[LESS-THAN-%d]]",mFreq); 

	
	n = (int)m1.size();
	if(n < 1){
	  Rprintf("empty results\n");
	  UNPROTECT(pc);
	  return (R_NilValue);
	}

	
	PROTECT(vecName = allocVector(STRSXP, n));
	pc++;
	PROTECT(vecInt = allocVector(INTSXP, n));
	pc++;



	
	// Rprintf("morphem numbers  = %d \n", m1.size());

	if(n < 1){
	  
	  //	  Rprintf("(000) map length =0\n");

	  UNPROTECT(pc);
  	  PROTECT(vecInt = lengthgets(vecInt, 2));
  	  PROTECT(vecName = lengthgets(vecName, 2));

 
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_NATIVE)); 
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_NATIVE)); 
// #else
// 	  SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_UTF8)); 
// #endif
// 	  //  SET_STRING_ELT(vecName, 0, mkChar(buf3));  // 規程頻度以下のトークン数
//  // </ 2005 11 08>

	SET_STRING_ELT(vecName, 0, mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE));   
	  
	  INTEGER(vecInt)[0] =  0;


//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_NATIVE ));	
// #else
// 	  SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_UTF8 )); 	
// #endif	  
// 			// SET_STRING_ELT(vecName, 1, mkChar( "[[TOTAL-TOKENS]]" ));
//  // </ 2005 11 08>

	  SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE ));
	  
	  // テキスト総トークン数をセット
	  INTEGER(vecInt)[1] =  totalM;
	  SET_NAMES(vecInt, vecName);// 
	  UNPROTECT(pc);
	  //	  Rprintf("(000) OUT \n");
	  return (vecInt);	  
	}
	
	pa = m1.begin();
	

	
// // 	PROTECT(mydf = allocVector(VECSXP, 2));//2 列のdata.frame
// // 	pc++;
// // 	SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n));//形態素原型
// // 	SET_VECTOR_ELT(mydf, 1, allocVector(INTSXP, n));// 頻度
// // 	PROTECT(tmp = mkString("data.frame"));
// // 	pc++;
// // 	//df 内ベクトルの名前を用意
// // 	PROTECT(varlabels = allocVector(STRSXP, 2));
// // 	pc++;
// // 	//  その単純な初期化
// // 	SET_STRING_ELT(varlabels, 0, mkChar("Term"));
// // 	SET_STRING_ELT(varlabels, 1, mkChar("Freq"));

	//	 Rprintf("(start) length(vecInt) = %d \n",length(vecInt) );

	zz = 0;// 実際にベクトルに収納された要素数
	// map の内容を vector にして返す
	for ( z = 0; z < n; z++) {
	  //	  Rprintf("mFreq - %d ; pa->second = %d \n", mFreq, pa->second );

// // 	  char s [256];
// // 	  strcpy(s, (pa->first).c_str());
// // 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkChar((pa->first).c_str()));
// // 	  INTEGER(VECTOR_ELT(mydf,1))[z] =   pa->second;// 最後に頻度情報

 	  if(   (pa->second)  <  mFreq){
		mF = mF + (pa->second);
 	  }
 	  else{

//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(vecName, zz, mkCharCE((pa->first).c_str(), CE_NATIVE )); 	 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(vecName, zz, mkCharCE((pa->first).c_str() , CE_NATIVE)); 	 	
// #else
// 		SET_STRING_ELT(vecName, zz, mkCharCE((pa->first).c_str() , CE_UTF8)); 	 	
// #endif		
// 		//SET_STRING_ELT(vecName, zz, mkChar((pa->first).c_str() ));  // 形態素情報をセット
// // </ 2005 11 08>

		SET_STRING_ELT(vecName, zz, mkCharCE((pa->first).c_str(), (utf8locale)?CE_UTF8:CE_NATIVE ));
		
		
		INTEGER(vecInt)[zz] =   (pa->second);
		zz++;
	  }
	  
	  pa++;
	}//_end_for

	/////	sprintf(buf3, "[[LESS-THAN-%d]]",mFreq); 
	
	//	Rprintf("n = %d :  zz = %d\n",n ,zz  );
	if(zz < 1 ){
	  //	  Rprintf("(0) zz = %d \n",zz );

//  // < 2005 11 08>
// #if defined(WIN32)
// 	SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_NATIVE)); 		 	
// #elif  defined(__MINGW32__)
// 	SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_NATIVE)); 		 	
// #else
// 	SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_UTF8)); 		 	
// #endif	  
// 	//SET_STRING_ELT(vecName, 0, mkChar(buf3));  // 規程頻度以下のトークン数
//  // </ 2005 11 08>

	  SET_STRING_ELT(vecName, 0, mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE)); 
	  
	  INTEGER(vecInt)[0] =  mF;

 // // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_NATIVE ));	 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_NATIVE ));	 	
// #else
// 		SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_UTF8 ));	 	
// #endif	  
// 		//SET_STRING_ELT(vecName, 1, mkChar( "[[TOTAL-TOKENS]]" ));
//  // </ 2005 11 08>

	  	SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE ));	 	
	  
	  // テキスト総トークン数をセット
	  INTEGER(vecInt)[1] =  totalM;

	  UNPROTECT(pc);
  	  PROTECT(vecInt = lengthgets(vecInt, 2));
  	  PROTECT(vecName = lengthgets(vecName, 2));
	  //  	  UNPROTECT(pc);
//  	  return(R_NilValue);
	}else if(zz  ==  n){// 二つ足す
	  //	  Rprintf("(1) zz = %d \n",zz ); 
	  UNPROTECT(pc);
 	  PROTECT(vecInt = lengthgets(vecInt, zz+2));
 	  PROTECT(vecName = lengthgets(vecName, zz+2));
	  //	  Rprintf("(2) zz = %d \n", zz );

//  // < 2005 11 08>
// #if defined(WIN32)
// 		 SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_NATIVE));	 	
// #elif  defined(__MINGW32__)
// 		 SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_NATIVE));	 	
// #else
// 		 SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_UTF8));	 	
// #endif	  
// 		 //SET_STRING_ELT(vecName, zz  , mkChar(buf3));  // 規程頻度以下のトークン数
//  // </ 2005 11 08>

	  SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE));	

	  
	  INTEGER(vecInt)[zz] =  mF;

  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	 SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_NATIVE  )); 		 	
// #elif  defined(__MINGW32__)
// 	 SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_NATIVE  )); 		 	
// #else
// 	 SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_UTF8  )); 
// #endif	  
// 	 // SET_STRING_ELT(vecName, zz+1  , mkChar(  "[[TOTAL-TOKENS]]"  ));  //// テキスト総トークン数をセット
//  // </ 2005 11 08>

	   SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE  )); 	
	  
	  INTEGER(vecInt)[zz+1] =  totalM;
	  
	}else if(zz+1  ==  n){// 二つ足す
	  //	   Rprintf("(3) zz = %d \n", zz );
	  UNPROTECT(pc);
 	  PROTECT(vecInt = lengthgets(vecInt, zz+2));
 	  PROTECT(vecName = lengthgets(vecName, zz+2));
	  //	  Rprintf("(2) zz = %d \n", zz );

//  // < 2005 11 08>
// #if defined(WIN32)
// 	SET_STRING_ELT(vecName, zz-1 , mkCharCE(buf3, CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 	SET_STRING_ELT(vecName, zz-1 , mkCharCE(buf3, CE_NATIVE));		 	
// #else
// 	SET_STRING_ELT(vecName, zz-1 , mkCharCE(buf3, CE_UTF8));		 	
// #endif	  
// 	//SET_STRING_ELT(vecName, zz-1 , mkChar(buf3));  // 規程頻度以下のトークン数
//   // </ 2005 11 08>

	  SET_STRING_ELT(vecName, zz-1 , mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE));	
	  
	  INTEGER(vecInt)[zz-1] =  mF;

  
//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(vecName, zz , mkCharCE(  "[[TOTAL-TOKENS]]", CE_NATIVE  ));	 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(vecName, zz , mkCharCE(  "[[TOTAL-TOKENS]]", CE_NATIVE  ));	 	
// #else
// 		SET_STRING_ELT(vecName, zz , mkCharCE(  "[[TOTAL-TOKENS]]", CE_UTF8 ));	 
// #endif	  
// 		//SET_STRING_ELT(vecName, zz , mkChar(  "[[TOTAL-TOKENS]]"  ));  //// テキスト総トークン数をセット
//  // </ 2005 11 08>

	  SET_STRING_ELT(vecName, zz , mkCharCE(  "[[TOTAL-TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE  ));	 	
	  
	  
	  INTEGER(vecInt)[zz] =  totalM;
	  
	}else if(zz+2  ==  n){// 一つ足す
	  //	  Rprintf("(4) zz = %d \n", zz );
	  UNPROTECT(pc);
 	  PROTECT(vecInt = lengthgets(vecInt, zz+2));
 	  PROTECT(vecName = lengthgets(vecName, zz+2));
	  
 // // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_NATIVE));	 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_NATIVE));	 	
// #else
// 		SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_UTF8));	 	
// #endif	  
// 		// SET_STRING_ELT(vecName, zz  , mkChar(buf3));   // 規程頻度以下のトークン数
//  // </ 2005 11 08>
	  
	  SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE));	 	
	  
	  
	  INTEGER(vecInt)[zz] =  mF;

//  // < 2005 11 08>
// #if defined(WIN32)
// 	 SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_NATIVE  ));  		 	
// #elif  defined(__MINGW32__)
// 	 SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_NATIVE  ));  		 	
// #else
// 	 SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_UTF8  ));	 	
// #endif	  
// 	 //SET_STRING_ELT(vecName, zz+1  , mkChar(  "[[TOTAL-TOKENS]]"  ));  //// テキスト総トークン数をセット
//   // </ 2005 11 08>

	  SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE  ));  

	  
	  INTEGER(vecInt)[zz+1] =  totalM;

 	}else if(zz +2 < n){

	  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	SET_STRING_ELT(vecName, zz, mkCharCE(buf3, CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 	SET_STRING_ELT(vecName, zz, mkCharCE(buf3, CE_NATIVE));		 	
// #else
// 	SET_STRING_ELT(vecName, zz, mkCharCE(buf3, CE_UTF8));		 	
// #endif
// 	// SET_STRING_ELT(vecName, zz, mkChar(buf3));  // 規程頻度以下のトークン数
// // </ 2005 11 08>

	  
	  SET_STRING_ELT(vecName, zz, mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE));
	  
	  INTEGER(vecInt)[zz] =  mF;
	  
 // // < 2005 11 08>
// #if defined(WIN32)
// 	 SET_STRING_ELT(vecName, zz+1, mkCharCE( "[[TOTAL-TOKENS]]", CE_NATIVE )); 	 
// #elif  defined(__MINGW32__)
// 	 SET_STRING_ELT(vecName, zz+1, mkCharCE( "[[TOTAL-TOKENS]]", CE_NATIVE )); 	 
// #else
// 	 SET_STRING_ELT(vecName, zz+1, mkCharCE( "[[TOTAL-TOKENS]]", CE_UTF8 ));   	
// #endif
// 	 //  SET_STRING_ELT(vecName, zz+1, mkChar( "[[TOTAL-TOKENS]]" ));  // テキスト総トークン数をセット
//   // </ 2005 11 08>

	 SET_STRING_ELT(vecName, zz+1, mkCharCE( "[[TOTAL-TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE )); 		 

	 
	  INTEGER(vecInt)[zz+1] =  totalM;


	  UNPROTECT(pc);
 	  PROTECT(vecInt = lengthgets(vecInt, zz+2));
 	  PROTECT(vecName = lengthgets(vecName, zz+2));
	  
	  
	}

	zz = 0;
	mF = 0;
	totalM = 0;
	//	Rprintf("* ");
	// 	// データフレームオブジェクト mydf の属性設定 
// // 	setAttrib(mydf, R_ClassSymbol, tmp);
// // 	setAttrib(mydf, R_NamesSymbol, varlabels);
	
// // 	// 行名を指定．必須
// // 	PROTECT(row_names = allocVector(STRSXP, n));
// // 	pc++;
// // 	char labelbuff[n];
// // 	for (int z = 0; z < n; z++) {
// // 	  sprintf(labelbuff, "%d", z+1);
// // 	  SET_STRING_ELT(row_names, z, mkChar(labelbuff));
// // 	}
// // 	setAttrib(mydf, R_RowNamesSymbol, row_names);
	SET_NAMES(vecInt, vecName);// 
	UNPROTECT(pc);
	
	return (vecInt);
  }
  return(R_NilValue);
 }
}// end_extern
///////////////////////////////////////////////






//////////////////////////////////////////////////////
/* N-gram を計算 */
extern "C" {
  SEXP Ngram(SEXP filename, SEXP type, SEXP N, SEXP pos, SEXP posN  , SEXP mydic ){
	const char* file = CHAR(STRING_ELT(filename,0));
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int typeSet  = INTEGER_VALUE( type );// pos の数
	int Ngram  = INTEGER_VALUE( N );// 最小頻度の数
	int mFreq = 2;
	char  input[BUF4] = { 0 };//char  input[5120] = { 0 };
	//	char  input2[5120] = { 0 };
	
	wchar_t  wbuf [BUF4] = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	/* マルチバイトUTF-8 で漢字は3バイト，一文字取るなら 3 + 1 */
// 	//	char str[Ngram][4]  = { 0 };
// // 	char str1[4] = { 0 };
// // 	char str2[4] = { 0 };
// //	char gram2[7]  = { 0 };
// //	int test = 0;
	FILE *fp;
	char * p;

	int pc = 0;
	int i, z , j = 0,  n = 0, nn = 0, xx = 0;
	unsigned int  wz = 0;
	
	//////////////////////// < 2005 06 13 >
	char buf3[BUF2];// [1024]; 品詞チェック用
	char buf4[BUF2];// [1024];品詞チェック用
	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 13

	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2; // = 1
	  flag = 0;
	}
	
	// 2011 03 10 //  char *Ppos[pos_n];
	vector <string> Ppos2;
	SEXP myPos;  

	if(pos_n > 0 && flag){
	  PROTECT(myPos = AS_CHARACTER(pos));
	  pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 //  strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back( CHAR(STRING_ELT(myPos, i)) ); // 2011 03 10 
	  }
	}else{
// 	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());
	  
	  // 2011 03 10 //  Ppos[0] = R_alloc( (unsigned int )  strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 //  strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back( meisiCode() );// 2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc( (unsigned int )  strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 //  strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back(keiyouCode() );// 2011 03 10 	
	}
	//////////////////////// </ 2005 06 13>
	
// 	char str[6][6*3 + 1] =  { 0 };
// 	char gram[6*3 + 1] = {0};


	SEXP mydf, tmp, varlabels, row_names;
	
	map<string, int> m1, m2;
	map<string, int>::iterator pa; 
	//  pair<string, int> pa;

	list <string> strL;
	list <string>::iterator iter;
	
	string target;
	char target2[BUF3];
				
	mecab_t *mecab;
	mecab_node_t *node;

	char buf1[BUF1];// [512];//入力された語形を記憶 // 2009 04 03
	char buf2[BUF3];// 2009 04 03
	int totalN = 0;// 2009 04 03

				  
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");
	CHECK(mecab);

	
	// char buf1[128];// [512];//入力された語形を記憶
	// char buf2[1024];
	
	// //	char buf3[32];

	// int totalN = 0;

		
// 	if(strlen(file) < 1){
// 	  //	  Rprintf("strlen(file) = %d\n", strlen(file));
// 	  Rprintf("file not found\n");
// 	  return(R_NilValue);	  
// 	}

	if(typeSet == 0){
	  setlocale(LC_ALL,"");
	}
	
	if(Ngram < 2){
	  Ngram = 2;
	}


	Rprintf("file = %s Ngram = %d \n", file, Ngram);


	if((fp = fopen(file, "r")) == NULL){
	  Rprintf("file not found\n");
	  return((SEXP) 1);
	}else{
	  strL.clear();
	  
	  while(!feof(fp)){
	    if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input, 5120, fp) != NULL){		  

		  if(typeSet == 0){// 文字単位なら
			//		  Rprintf("%s\n", input);			  
			p = strchr( input, '\n' );
			/* 改行文字があった場合 */
			if ( p != NULL )
			  {
				/* 改行文字を終端文字に置き換える */
				*p = '\0';
			  }
		
			//			Rprintf("strlen of input= %d\n", strlen(input));  
			if(strlen(input) > 0){
			  //Rprintf("%s\n", input);

			  
			  mbstowcs(wbuf, input,  strlen(input));/* マルチバイト文字列をワイド文字列に変換*/
// 			  //			  Rprintf("strlen of wbuf = %d\n", wcslen(wbuf));  
// 			  //				  while(wbuf[test] != '\0'){
// 			  //			  while(test <= (wcslen(wbuf) - Ngram)  ){
// 			  //
// 			  //for(int z = 0; z <  (wcslen(wbuf) - Ngram); z++){
			  for( wz = 0; wz <  wcslen(wbuf) ; wz++){	// 2005 07 22
				//				for(int y = 0; y < Ngram; y++){

// // 				// < 2005 11 06>
// // 				if(WIN){
// // 				  //wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// // 				}else{
// // 				   sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// // 				}
// // 				// < /2005 11 06>
				
// #if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
// 		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// #elif  defined(__MINGW32__) || defined(__WINGW64__)
// 		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
// #else
// 				sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// #endif

		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif
	
				
				//				Rprintf("target2 = %s\n", target2);
				if(strlen(target2) < 1){
				  break;;
				}
				//エスケープ記号類
				//strcpy(buf1, *target2);
				if( *target2 > 0x00 && *target2 < 0x21 ){//エスケープ記号類0x0e
				  continue;
				}//
				
				//				strcpy(target2 , test;
				//	if( strcmp(target2, " ") == 0 ||  strcmp(target2, "　")==0){
				if( strcmp((char *) target2, " ") == 0 ||  strcmp((char *) target2, "　")==0){
				//				  printf("found\n");
				  continue;
				}
// 				//< 2005 11 08>
// 				// if( atoi(target2) >  0x00 &&  atoi(target2) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
// 				if( target2[0] >  0x00 &&  target2[0] < 0x21 ){//if( atoi(buf1) == 0x0e)
// 				  continue;
// 				}// </ 2005 11 08>

				else{
				  
		/////////////// new_begin ////////////////

				  // target = target2;

				  strL.push_back( target2 );

				  if(strL.size() >= (unsigned int) Ngram){
					//					Rprintf("in if(strL.size) \n");					
					target.erase();
					target.append("[");
					xx = 1;
					for ( iter = strL.begin(); iter != strL.end(); iter++){
					  //					  Rprintf("in for\n");
					  //x 				  Rprintf("str %s\n", *iter);
					  target.append( *iter);
					  if(xx < Ngram){
						target.append("-");
					  }
					  xx++;
					  //					  Rprintf("xx = %d\n", xx);
										  
					}
					xx = 1;
					target.append("]");
					//					Rprintf("target %s\n", target);
					//					Rprintf("before m1.find \n");					
					pa = m1.find(target);
					//出てきた形態素原型は既にマップにあるか？
					if(pa != m1.end()){
					  pa->second =  pa->second + 1;
					  //二つ目の数値を加算
					}					
					else{// マップにないなら，新規にマップに追加
					  //					  Rprintf("add map \n");
					  m1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
					}
					strL.pop_front();
				  }//_if strSize>= Ngram
				}// _else_end
				
				
				////////////////////////////////////// new _end ////
			  }//_for2_z < wcslen
			  // gram[0] = '\0';
// 			  for(int z = 0; < z strlen(gram); z++){
// 				gram[z] = 0
// 			  }
// 			  sprintf(gram, "%s", "");//memset(gram,'\0',strlen(gram));			  
			}// if_strlen_>_0_end
			
		  }	else {// if_type_set 形態素あるいは品詞の場合
			///////////////////////////////////////////////////
			//			Rprintf("before mecab start\n");
			  
// 			mecab = mecab_new2 ("めかぶ");
// 			CHECK(mecab);
			node = ( mecab_node_t * ) mecab_sparse_tonode(mecab, input);
			CHECK(node);
			//			Rprintf("after check node\n");
			/// 解析結果のノードをなめる
			
			for (;  node; node = node->next) {
			  
			  //		printf("%d ", node->id);
			  
			  if (node->stat == MECAB_BOS_NODE)
				//printf("BOS");
				continue;
			  else if (node->stat == MECAB_EOS_NODE)
				//printf("EOS");
				continue;
			  else {//必要な情報ノード
				totalN++;
				// 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
				strncpy(buf1, node->surface, node->length) ;//元の語形
				
				buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				
				if(strlen(buf1) < 1){// 2006 06 移動
				  continue;
				}//if_end

				
				//< 2005 11 07> //Rprintf("%s\n", buf1);			
				//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
				if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
				  continue;
				}// </ 2005 11 07>
				
// 				buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				
// 				if(strlen(buf1) < 1){// 2006 06 移動
// 				  continue;
// 				}//if_end
				
				strcpy(buf2, node->feature);
				//ノードごとに解析情報の取得.要素数は 9
				if(strlen(buf2) < 1){
				  continue;
				}//if_end
				//				if(typeSet == 1){// 形態素原形で数える
				p = strtok(buf2, "," );//取得情報の分割
				if(p != NULL){
				  sprintf(buf3, "%s", p);// 品詞情報の取得
				}
				j = 1;
				while ( p != NULL ) {
				 if(typeSet == 2){// 品詞情報で数える
				   if( j == 1){//品詞情報1
					 strL.push_back(p);
					 //  Rprintf("typeSet == = %d; p = %s\n", typeSet, p);					 
					 p = NULL;
					 continue;
				   }
				 }else if(typeSet == 1){// 形態素原形で数える
				   

				   if( j == 7){// p には形態素原型がセットされている状態
					 //////// < 2005 06 13>
					 if(pos_n != 0){// 指定品詞があるなら
					   for( i = 0; i < pos_n; i++){
					     sprintf(buf4, "%s", Ppos2[i].c_str());// 2011 03 10 //	sprintf(buf4, "%s", Ppos[i]);				
						 //				Rprintf("buf4 %s\n", buf4);
						 if(strcmp(buf3, buf4) == 0){ // 指定品詞と一致
						   if( p!=NULL &&  strcmp(p, "*") != 0){
							 strL.push_back(p);//原型
						   } else{
							 strL.push_back(buf1);//元の語形
						   }
						 }
					   } 
					   ////////// </ 2005 06 13>
					 }
					 else if(p == NULL || strcmp(p, "*") == 0){
					   strL.push_back(buf1);//元の語形
					   // Rprintf("typeSet == = %d; p = %s\n", typeSet, p);
					 }
					 else{
					   strL.push_back(p);
					   // Rprintf("typeSet == = %d; p = %s\n", typeSet, p) ;
					 }
				   }
				 }// _else if(typeSet == 1){形態素原形で数える_end
				 p = strtok( NULL,"," );
				 j++;
				}//_while_( p != NULL )_end
			  }//if_typSet_end// 形態素原形で数える_end
			  // 次のノードに移る直前

			  if(strL.size() >= (unsigned int) Ngram){
				//				Rprintf("strL size =  %d\n", strL.size() );
				target.erase();
				target.append("[");
				xx = 1;
				for ( iter = strL.begin(); iter != strL.end(); iter++){
				  //				  Rprintf("in for\n");
// 				  Rprintf("str %s\n", *iter);
// 				  Rprintf("after Rprintf in for\n");
				  target.append( *iter);
				  if(xx < Ngram){
					target.append("-");
				  }
				  xx++;
				}
				xx = 1;
				target.append("]");
				//				Rprintf("target  =  %s\n", target );
				//			  	Rprintf("before map find \n");	
				pa = m1.find(target);//出てきた N -gramは既にマップにあるか？
				if(pa != m1.end()){
				  pa->second =  pa->second + 1;
				  //二つ目の数値を加算
				}
				else{// マップにないなら，新規にマップに追加
				  m1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
				}
				strL.pop_front();
				//	Rprintf(" strL.pop_front() \n");
				

			  
			  
			  
			  }//(str.size() >= Ngram)_end
			  // 次のノードに移る
			} //else_必要な情報ノード_end

			
		  } //_for_node_end
		  ////////////////
// 		  if(m1.size() > 40000){
// 			Rprintf("Ngram size exceeds 40000: stop\n");
// 			return(R_NilValue);
// 		  }
// 		  ////////////////////
		}//else_if_type_set_end
		
	  }//while_fopen_
	  mecab_destroy(mecab);
	  
	  	// map の内容を data.frame にして返す
	  n = (int)m1.size();
	  if(n < 1){
		Rprintf("empty results\n");
		UNPROTECT(pc);
		return (R_NilValue);
	  }

		  ////////////////////

	  
	  if(n > 40000){
		Rprintf("Ngram exceeds 40000\n");
		nn = 0;
		
		pa = m1.begin();
		while(pa != m1.end()){
		  if(   (pa->second)  <  mFreq){
			nn ++;
		  }else{
			m2.insert(make_pair( (pa->first).c_str() , pa->second));
			
		  }
		  pa++;
		}
		//		Rprintf("m2 finished\n");
		
		m2.insert(make_pair("[LESS-THAN-2]", nn));
		nn = m2.size();
		if(nn < 1){
		  Rprintf("empty results\n");
		  UNPROTECT(pc);
		  return (R_NilValue);
		}


		//		Rprintf("nn = %d\n", nn);		
		PROTECT(mydf = allocVector(VECSXP, 2));//2 列のdata.frame
		pc++;
		SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, nn));// N-gram セット
		SET_VECTOR_ELT(mydf, 1, allocVector(INTSXP, nn));// 頻度	  
		//		Rprintf("length = %d \n", m1.size());

		////////////////// orjinal /////////////
		pa = m2.begin();
		for (z = 0; z < nn; z++) {
		  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), CE_NATIVE ));		 	
// #elif  defined(__MINGW32__)
// 	SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), CE_NATIVE ));		 	
// #else
// 	SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), CE_UTF8 ));		 	
// #endif
// 	// SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkChar( (pa->first).c_str() ));
// // </ 2005 11 08>

		  SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), (utf8locale)?CE_UTF8:CE_NATIVE ));			  ///<2006 04 18>
		  
		  INTEGER(VECTOR_ELT(mydf,1))[z] =   pa->second;// 最後に頻度情報
		  pa++;
		  // if(z % 10 == 0)	  Rprintf("* ");// 2006 03 removed 2007 05
		}
		
		Rprintf("\n");// 2006 03
			  // 	  	  // オブジェクトはデータフレームだと指定する
		PROTECT(tmp = mkString("data.frame"));
		pc++;
		// 	  //df 内ベクトルの名前を用意
		PROTECT(varlabels = allocVector(STRSXP, 2));
		pc++;
		//  その単純な初期化

		
 // // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_NATIVE));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_NATIVE));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_NATIVE));		 	
// #else
// 		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_UTF8));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_UTF8));
// #endif		
// // 		SET_STRING_ELT(varlabels, 0, mkChar("Ngram"));
// // 		SET_STRING_ELT(varlabels, 1, mkChar("Freq"));
// 		// </ 2005 11 08>
		

		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", (utf8locale)?CE_UTF8:CE_NATIVE));
		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", (utf8locale)?CE_UTF8:CE_NATIVE));		

	///////////////////////// new_end 
	  // データフレームオブジェクト mydf の属性設定 
		setAttrib(mydf, R_ClassSymbol, tmp);
		setAttrib(mydf, R_NamesSymbol, varlabels);
	  
	  // 行名を指定．必須
		PROTECT(row_names = allocVector(STRSXP, nn));
		pc++;
		char labelbuff[6];// char labelbuff[nn]; 2006 03
		for ( z = 0; z < nn  ; z++) {
		  sprintf(labelbuff, "%d", z+1);

		  
//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));	 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));	 	
// #else
// 		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));	 	
// #endif
// 		//SET_STRING_ELT(row_names, z, mkChar(labelbuff));
// 		    // </ 2005 11 08>

		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));
		  
		
		}
		setAttrib(mydf, R_RowNamesSymbol, row_names);
		
		UNPROTECT(pc);
		
		////////////////// orjinal _end /////////////
		
		
	  }else{// else_n
		//		Rprintf("n < 40000:\n");
		PROTECT(mydf = allocVector(VECSXP, 2));//2 列のdata.frame
		pc++;
		SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n));// N-gram セット
		SET_VECTOR_ELT(mydf, 1, allocVector(INTSXP, n));// 頻度	  
		Rprintf("length = %d \n", m1.size());

		pa = m1.begin();
		
		////////////////// orjinal /////////////
		for ( z = 0; z < n; z++) {
		  //		  Rprintf("map %d = %s\n", z, (pa->first).c_str()   );
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	 SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str() , CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 	 SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), CE_NATIVE ));		 	
// #else
// 	 SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), CE_UTF8 ));		 	
// #endif		  
// 	 //SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkChar( (pa->first).c_str() ));
// 		    // </ 2005 11 08>


		  SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str() , (utf8locale)?CE_UTF8:CE_NATIVE));///<2006 04 18>
		  
		  
		INTEGER(VECTOR_ELT(mydf,1))[z] =   pa->second;// 最後に頻度情報
		pa++;
		// if(z % 10 == 0)  Rprintf("* ");// 2006 03 removed 2007 05
		}
		Rprintf("\n");// 2006 03
		
			  // 	  	  // オブジェクトはデータフレームだと指定する
		PROTECT(tmp = mkString("data.frame"));
		pc++;
	  // 	  //df 内ベクトルの名前を用意
		PROTECT(varlabels = allocVector(STRSXP, 2));
		pc++;
		//  その単純な初期化

		
// 		// < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_NATIVE));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_NATIVE));
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_NATIVE));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_NATIVE));	
// #else
// 		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_UTF8));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_UTF8));
// #endif	  
// 	  //  	  SET_STRING_ELT(varlabels, 0, mkChar("Ngram"));
// 	  //  	  SET_STRING_ELT(varlabels, 1, mkChar("Freq"));
// 	  // </ 2005 11 08>


		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", (utf8locale)?CE_UTF8:CE_NATIVE));
		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", (utf8locale)?CE_UTF8:CE_NATIVE));

		
	///////////////////////// new_end 
	  // データフレームオブジェクト mydf の属性設定 
		setAttrib(mydf, R_ClassSymbol, tmp);
		setAttrib(mydf, R_NamesSymbol, varlabels);
		
	  // 行名を指定．必須
		PROTECT(row_names = allocVector(STRSXP, n));
		pc++;
		char labelbuff[6];// char labelbuff[n];2006 03 
		for ( z = 0; z < n  ; z++) {
		  sprintf(labelbuff, "%d", z+1);
		  
		  
// 		  // < 2005 11 08>
// #if defined(WIN32)
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			 
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			
// #else
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));			
// #endif
// 		  // SET_STRING_ELT(row_names, z, mkChar(labelbuff));
// 		  // </ 2005 11 08>
		  
		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));

		
		}
		setAttrib(mydf, R_RowNamesSymbol, row_names);
		
		UNPROTECT(pc);
		
	  }// else_n_end
	  return (mydf);
	  
	}//_else
  }//_SEXP_Ngram
}// Extern


///////////////////////////////////////////////////////////////


extern "C"{

  SEXP collocate(SEXP filename, SEXP target, SEXP span, SEXP mydic ){
	int spa  = INTEGER_VALUE(span );// 最小頻度の数
	//Rprintf("span = %d\n", spa);
	const char* file = CHAR(STRING_ELT(filename,0));
	const char* word = CHAR(STRING_ELT(target,0));
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	char  input[BUF4] = { 0 };//	char  input[5120] = { 0 };
	
	mecab_t *mecab;
	mecab_node_t *node;
	
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char *p;
	//SEXP mydf, my_list, my_char, tmp, varlabels,row_names;
	SEXP mydf, tmp, varlabels, row_names;
	
	// mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");
	// CHECK(mecab);
	
	int j = 0, z = 0, y = 0, n = 0, count = 0;
	int pc = 0;
	string str;

	int totalN = 0;
	
	vector<string> vec1;
	vector<string>::iterator it1;

	map<string, int> m0;//, m1, m2;// m1 はnodeの前の辞書  m2 はnodeの後の辞書
	map<string, int>::iterator pa0,pa;// , pa, pa2;

	map<string, pair<int,int> > mP;// nodeに対するあるspan語の前後における語数
	map<string, pair<int,int> >::iterator pM;

	int beforeN = 0;
	int afterN = 0;

	FILE *fp;

	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");// 2009 04 03 
	CHECK(mecab);// 2009 04 03 

		
// 	if(strlen(file) < 1){
// 	  Rprintf("file not found\n");
// 	  return(R_NilValue);	  
// 	}


	if(strlen(word) < 1){
	  Rprintf("please input a target morphem\n");
	  return (R_NilValue);
	}
	
	if((fp = fopen(file, "r")) == NULL){
	  Rprintf("no file found\n");
	  return(R_NilValue);
	}else{
	  Rprintf("file = %s \n", file);
	  while(!feof(fp)){
	    if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input, 5120, fp) != NULL){
		  node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
		  CHECK(node);
		  
		  /// 解析結果のノードをなめる
		  for (;  node; node = node->next) {
			//		printf("%d ", node->id);
			
			if (node->stat == MECAB_BOS_NODE)
			  //printf("BOS");
			  continue;
			else if (node->stat == MECAB_EOS_NODE)
			  //printf("EOS");
			  continue;
			else {

			  totalN++;
			  // 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
			  strncpy(buf1, node->surface, node->length) ;//元の語形
			  
			  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			  
			  if(strlen(buf1) < 1){// 2006 06 移動
				continue;
			  }
			  
			  	//< 2005 11 07> //Rprintf("%s\n", buf1);			
			  //if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
			  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
				continue;
			  }// </ 2005 11 07>
			  
// 			  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			  
// 			  if(strlen(buf1) < 1){// 2006 06 移動
// 				continue;
// 			  }
			  
			  strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
			  if(strlen(buf2) < 1){
				continue;
			  }
			  
			  p = strtok(buf2, "," );//取得情報の分割
			  j = 1;
			  while ( p != NULL ) {
				if( j == 7){
				  if(p == NULL || strcmp(p, "*") == 0){
					//Rprintf("p == NULL or *\n");
					vec1.push_back(buf1);//元の語形
					pa0 = m0.find(buf1);
					if(pa0 != m0.end()){
					  //Rprintf("found  buf1\n");
					  pa0->second += 1;
					}else{
					  m0.insert(make_pair(buf1, 1));// 1 は 1個目と言う意味
					  //Rprintf("m0.insert  buf1\n");
					}
					
				  }
				  else{
					//Rprintf("p else\n");					
					//Rprintf("j==7 , p\n");
					vec1.push_back(p);
					pa0 = m0.find(p);
					if(pa0 != m0.end()){
					  
					  //Rprintf("found  p\n");
					  pa0->second += 1;

					}else{
					  m0.insert(make_pair(p, 1));// 1 は 1個目と言う意味
					  //Rprintf("m0.insert  p\n");
					}
					
				  }
				}
				p = strtok( NULL,"," );
				j++;
			  }						
			}//_else
		  }// for node 
		}//if_fgets_end
	  }// while(!feof(fp));//while
	  
	  fclose(fp);
	  mecab_destroy(mecab);

	  //	  Rprintf("mecab destroyed\n");
	  
	  //	  Rprintf("vec1 size = %d\n", (int) vec1.size());
	  z = 1;
	  string coll;
	  
	  // Rprintf("vec 1 size  = %d :  \n",  vec1.size()   );
	  // sprintf(buf1, "%s", *(it1));
	  
	  // printf("found  = %s :  \n",  buf1 );
	  // Vectorに貯めた語の列から，node を発掘し，その前と後ろを登録する
	  it1 = vec1.begin();
	  // Rprintf("found  = %s :  \n",  *it1  );
	  while(it1 != vec1.end()){
		y = spa;
		if( word == *it1){
		  //		  Rprintf("word = %s : z = %d \n", word, z);	  
		  while((z - y)  <= 0){
			y--;
			continue;
			
		  }
		  for( ; y >= 0; y--){
// 			if( (z-y)  < 0){
// 			  break;
// 			}

			if(y <0) break;
			//			Rprintf("y = %d :try to find\n", y);
			pM = mP.find( *(it1-y)) ; // node前の語の処理
			//			Rprintf("found  = %s : y = %d \n",  *(it1-y)   , y);
			if(pM != mP.end()){
			  (pM->second).first =  (pM->second).first + 1;
			  //二つ目の数値を加算
			}
			else{// マップにないなら，新規にマップに追加
			  mP.insert(make_pair( *(it1-y ) , pair<int,int>(1,0))) ;// 1 は 1個目と言う意味
						beforeN++;
			}
		  }
		  y = 1;
		  //		  Rprintf("y is setted\n");
		  while( (z + y) <= vec1.size()){// node後の語の処理
			
			pM = mP.find( *(it1+y )) ;
			if(pM != mP.end()){
			  (pM->second).second  =  (pM->second).second + 1;
			  //二つ目の数値を加算
			}
			else{// マップにないなら，新規にマップに追加
			  mP.insert(make_pair( *(it1+y ) , pair<int,int>(0,1))) ;// 1 は 1個目と言う意味
						afterN++;
			}
			//			cout << *(it1+y ) << " ";
			y++;
			if(y > spa){
			  break;
			}
		  }
		}//if_end

		it1++;
		z++;
	  }//_while(it1 != ) _ end

	  	// map の内容を data.frame にして返す
	  n = (int)mP.size();
	  //Rprintf("mP.size() = %d \n",  n);

	  if(n < 1){
		Rprintf("%s not found\n", word);
		UNPROTECT(pc);
		return (R_NilValue);
	  }
	  n = n + 2;// for totalN, total Morphemes
	
	  PROTECT(mydf = allocVector(VECSXP, 5));//4 列のdata.frame
	  pc++;
	  SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n));//形態素原型
	  SET_VECTOR_ELT(mydf, 1, allocVector(INTSXP, n));// 頻度 Before
	  SET_VECTOR_ELT(mydf, 2, allocVector(INTSXP, n));// 頻度 After
	  SET_VECTOR_ELT(mydf, 3, allocVector(INTSXP, n));// /SPAN 頻度

	  SET_VECTOR_ELT(mydf, 4, allocVector(INTSXP, n));// total頻度	  
	  // オブジェクトはデータフレームだと指定する
	  PROTECT(tmp = mkString("data.frame"));
	  pc++;
	  //df 内ベクトルの名前を用意
	  PROTECT(varlabels = allocVector(STRSXP, 5));
	  pc++;
	//  その単純な初期化



	  SET_STRING_ELT(varlabels, 0, mkCharCE("Term", (utf8locale)?CE_UTF8:CE_NATIVE));
	  SET_STRING_ELT(varlabels, 1, mkCharCE("Before", (utf8locale)?CE_UTF8:CE_NATIVE));
	  SET_STRING_ELT(varlabels, 2, mkCharCE("After", (utf8locale)?CE_UTF8:CE_NATIVE));
	  SET_STRING_ELT(varlabels, 3, mkCharCE("Span", (utf8locale)?CE_UTF8:CE_NATIVE));
	  
	  SET_STRING_ELT(varlabels, 4, mkCharCE("Total", (utf8locale)?CE_UTF8:CE_NATIVE));		 		  


	  	  	  ///// 
	  SET_STRING_ELT(VECTOR_ELT(mydf, 0), n-2, mkCharCE( "[[MORPHEMS]]", (utf8locale)?CE_UTF8:CE_NATIVE));//
	  INTEGER(VECTOR_ELT(mydf,1))[n-2] =   beforeN  ; // Before 内タイプ数
	  INTEGER(VECTOR_ELT(mydf,2))[n-2] =   afterN ; // After 内タイプ数
	  INTEGER(VECTOR_ELT(mydf,3))[n-2] =   beforeN + afterN ; // 総タイプ数
	  INTEGER(VECTOR_ELT(mydf,4))[n-2] =   m0.size();// テキスト内タイプ数

	  beforeN = afterN = 0;
	  
	  Rprintf("length = %d \n", m0.size());// 総タイプ数
	 
	  // pa0 = m0.begin();

	  count = 0;

	  pM = mP.begin();
	  
	  for( z = 0; pM != mP.end(); z++, pM++){

		SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pM->first).c_str(), (utf8locale)?CE_UTF8:CE_NATIVE ));	//<2006 04 18> ターム名の登録
		
		//	Rprintf("found  = %s : z = %d \n",  (pM->first).c_str()   , z);
			
		INTEGER(VECTOR_ELT(mydf,1))[z] =   (pM->second).first ;// /Before頻度
		count += (pM->second).first ;
		beforeN = beforeN + (pM->second).first ;// /Beforeトークン数
		
		INTEGER(VECTOR_ELT(mydf,2))[z] =   (pM->second).second ;// /After頻度
		count +=  (pM->second).second ;
		afterN = afterN + (pM->second).second ;// /Afterトークン数

		INTEGER(VECTOR_ELT(mydf,3))[z] =   (pM->second).first + (pM->second).second ;// SPAN 頻度
	  pa0 = m0.begin();

		pa0 = m0.find( (pM->first).c_str());
		if(pa0 != m0.end()){
		  INTEGER(VECTOR_ELT(mydf,4))[z] =   pa0->second;// 最後に頻度情報
		}
		else{
		  INTEGER(VECTOR_ELT(mydf,4))[z] =   0;// 最後に頻度情報
		}
		  pa0++;
	  }
	  // for( z = 0; pa2 != m2.end(); z++){// After Word の検索と

	  // 	SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa2->first).c_str(), (utf8locale)?CE_UTF8:CE_NATIVE ));	//<2006 04 18>
		
		
	  // 	INTEGER(VECTOR_ELT(mydf,2))[z] =   pa2->second;// 最後に頻度情報
	  // 	count += pa2->second;
		
	  // 	pa0 = m0.find( (pa2->first).c_str());

	  // 	if(pa0 != m0.end()){
	  // 	  INTEGER(VECTOR_ELT(mydf,2))[z] =   pa0->second;// 最後に頻度情報
	  // 	}
	  // 	else{
	  // 	  INTEGER(VECTOR_ELT(mydf,2))[z] =   0;// 最後に頻度情報
	  // 	}
	  // 	  pa2++;
	  // }


//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), n-1, mkCharCE( "[[TOKENS]]", CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), n-1, mkCharCE( "[[TOKENS]]", CE_NATIVE));			 	
// #else
// 	  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), n-1, mkCharCE( "[[TOKENS]]", CE_UTF8)); 		 	
// #endif	  
// 	  //SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), n-1, mkChar( "[[TOKENS]]"));
//  // </ 2005 11 08>


	  SET_STRING_ELT(VECTOR_ELT(mydf, 0), n-1, mkCharCE( "[[TOKENS]]", (utf8locale)?CE_UTF8:CE_NATIVE)); //<2006 04 18>
	  INTEGER(VECTOR_ELT(mydf,1))[n-1] =   beforeN;//  before 総トークン数
	  INTEGER(VECTOR_ELT(mydf,2))[n-1] =   afterN ;// after テキスト総トークン数
	  INTEGER(VECTOR_ELT(mydf,3))[n-1] =   count ;// テキスト総トークン数
	  INTEGER(VECTOR_ELT(mydf,4))[n-1] =   vec1.size();//totalN;// テキスト総トークン数
	  // データフレームオブジェクト mydf の属性設定 
	  setAttrib(mydf, R_ClassSymbol, tmp);
	  setAttrib(mydf, R_NamesSymbol, varlabels);
	  
	// 行名を指定．必須
	  PROTECT(row_names = allocVector(STRSXP, n));
	  pc++;
	  char labelbuff[6];//char labelbuff[n]; 2006 03
	  for (z = 0; z < n; z++) {
		sprintf(labelbuff, "%d", z+1);

		
//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			 
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			 
// #else
// 		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));			 	
// #endif		
// 		// SET_STRING_ELT(row_names, z, mkChar(labelbuff));
//   // </ 2005 11 08>
		
		SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));		

		
	  }
	  setAttrib(mydf, R_RowNamesSymbol, row_names);
	  
	  UNPROTECT(pc);
	  return (mydf);
	}
	return(R_NilValue);
  }

}//_extern_end


/////////////////

/////////////////////// 2005 06 14 ///////////////////////////////

/* N-gram の結果をデータフレームで返す */
extern "C" {
  SEXP NgramDF(SEXP filename, SEXP type, SEXP N, SEXP pos, SEXP posN , SEXP mydic  ){
	const char* file = CHAR(STRING_ELT(filename,0));
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int typeSet  = INTEGER_VALUE( type );// pos の数
	int Ngram  = INTEGER_VALUE( N );// 最小頻度の数
	int mFreq = 2;
	char  input[10240];// 2010 12 17 ;// [5120] = { 0 };//char  input[5120] = { 0 };
	//	char  input2[5120] = { 0 };
	
	wchar_t  wbuf [10240];// 2010 12 17 ;// [5120] = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	FILE *fp;
	char * p;
	int i, j, n, nn, z, xx ;
	unsigned int  wz = 0;
	int pc = 0;
	
	//////////////////////// < 2005 06 13 >
	char buf3[BUF3];// 2010 12 17 ;// [64]; // [1024];品詞チェック用
	char buf4[BUF3];// 2010 12 17 ;[64]; // [1024];品詞チェック用
	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 13
	
	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2;// = 1
	  flag = 0;
	}

	// 2011 03 10 //  char *Ppos[pos_n];
	vector <string> Ppos2;
	SEXP myPos;  

	SEXP mydf, tmp, varlabels, row_names;// 2009 04 03
	
	map<string, int> m1, m2;
	map<string, int>::iterator pa; 
	
	list <string> strL;
	list <string>::iterator iter;
	
	string target;
	char target2[BUF3];
				
	mecab_t *mecab;
	mecab_node_t *node;
	
	char buf1[BUF3];// 2010 12 17 ;[128];// [512];//入力された語形を記憶
	char buf2[BUF3];
	
	int totalN = 0;// 2009 04 03

	
	
	if(pos_n > 0 && flag){
	  PROTECT(myPos = AS_CHARACTER(pos));
	  pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back ( CHAR(STRING_ELT(myPos, i)) ) ;// 2011 03 10 
	  }
	}else{
	//   PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());
	  	  
	  // 2011 03 10 // Ppos[0] = R_alloc(strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back ( meisiCode() ) ;// 2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc(strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back ( keiyouCode() ) ;// 2011 03 10 
	}
	//////////////////////// </ 2005 06 13>

	// SEXP mydf, tmp, varlabels, row_names;
	
	// map<string, int> m1, m2;
	// map<string, int>::iterator pa; 
	// //  pair<string, int> pa;

	// list <string> strL;
	// list <string>::iterator iter;
	
	// string target;
	// char target2[1024];
				
	// mecab_t *mecab;
	// mecab_node_t *node;
	
	// char buf1[128];// [512];//入力された語形を記憶
	// char buf2[1024];
	

	// int totalN = 0;

			  
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	CHECK(mecab);


	if(typeSet == 0){
	  setlocale(LC_ALL,"");
	}
	
	if(Ngram < 2){
	  Ngram = 2;
	}


	Rprintf("file = %s Ngram = %d \n", file, Ngram);


	if((fp = fopen(file, "r")) == NULL){
	  Rprintf("file not found\n");
	  return((SEXP) 1);
	}else{
	  strL.clear();
	  
	  while(!feof(fp)){
	    if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11  if(fgets(input, 5120, fp) != NULL){		  

		  if(typeSet == 0){// 文字単位なら
			//		  Rprintf("%s\n", input);			  
			p = strchr( input, '\n' );
			/* 改行文字があった場合 */
			if ( p != NULL )
			  {
				/* 改行文字を終端文字に置き換える */
				*p = '\0';
			  }
		
			//			Rprintf("strlen of input= %d\n", strlen(input));  
			if(strlen(input) > 0){
			  //Rprintf("%s\n", input);

			  
			  mbstowcs(wbuf, input,  strlen(input));/* マルチバイト文字列をワイド文字列に変換*/
// 			  //			  Rprintf("strlen of wbuf = %d\n", wcslen(wbuf));  
// 			  //				  while(wbuf[test] != '\0'){
// 			  //			  while(test <= (wcslen(wbuf) - Ngram)  ){
// 			  //
// 			  //for(int z = 0; z <  (wcslen(wbuf) - Ngram); z++){
			  for( wz = 0; wz <  wcslen(wbuf) ; wz++){//2005 07 22
				//				for(int y = 0; y < Ngram; y++){

// 				// <2005 11 06>
// // 				if(WIN){
// // 				  //wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// // 				}else{
// // 				   sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// // 				}
// 				// </ 2005 11 06>

				
// #if defined(WIN32)
// 				wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// #elif  defined(__MINGW32__)
// 				wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
// #else
// 				sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// #endif
		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif
					
				//				Rprintf("target2 = %s\n", target2);
				if(strlen(target2) < 1){
				  break;;
				}
				//エスケープ記号類
				//strcpy(buf1, *target2);
				if( *target2 > 0x00 && *target2 < 0x21 ){//エスケープ記号類0x0e
				  continue;
				}//
				//				strcpy(target2 , test;
				//	if( strcmp(target2, " ") == 0 ||  strcmp(target2, "　")==0){
				if( strcmp((char *) target2, " ") == 0 ||  strcmp((char *) target2, "　")==0){
				//				  printf("found\n");
				  continue;
				}
				else{
				  
		/////////////// new_begin ////////////////

				  // target = target2;

				  strL.push_back( target2);

				  if(strL.size() >= (unsigned int) Ngram){
					//					Rprintf("in if(strL.size) \n");					
					target.erase();
					// target.append("["); // 2005 06 14 
					xx = 1;
					for ( iter = strL.begin(); iter != strL.end(); iter++){
					  //					  Rprintf("in for\n");
					  //x 				  Rprintf("str %s\n", *iter);
					  target.append( *iter);
					  if(xx < Ngram){
						target.append(" ");	// target.append("-");// 2005 06 14 
					  }
					  xx++;
					  //					  Rprintf("xx = %d\n", xx);
										  
					}
					xx = 1;
					// target.append("]"););// 2005 06 14 
					//					Rprintf("target %s\n", target);
					//					Rprintf("before m1.find \n");					
					pa = m1.find(target);
					//出てきた形態素原型は既にマップにあるか？
					if(pa != m1.end()){
					  pa->second =  pa->second + 1;
					  //二つ目の数値を加算
					}					
					else{// マップにないなら，新規にマップに追加
					  //					  Rprintf("add map \n");
					  m1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
					}
					strL.pop_front();
				  }//_if strSize>= Ngram
				}// _else_end
				
				
				////////////////////////////////////// new _end ////
			  }//_for2_< wcslen
			  // gram[0] = '\0'; 
			}// if_strlen_>_0_end
			
		  }	else {// if_type_set 形態素あるいは品詞の場合
			///////////////////////////////////////////////////
			//			Rprintf("before mecab start\n");
			node = ( mecab_node_t * ) mecab_sparse_tonode(mecab, input);
			CHECK(node);
			//			Rprintf("after check node\n");
			/// 解析結果のノードをなめる
			
			for (;  node; node = node->next) {
			  
			  //		printf("%d ", node->id);
			  
			  if (node->stat == MECAB_BOS_NODE)
				//printf("BOS");
				continue;
			  else if (node->stat == MECAB_EOS_NODE)
				//printf("EOS");
				continue;
			  else {//必要な情報ノード
				totalN++;
				// 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
				strncpy(buf1, node->surface, node->length) ;//元の語形

				buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				
				if(strlen(buf1) < 1){// 2006 06 移動
				  continue;
				}//if_end
				
				//< 2005 11 07> //Rprintf("%s\n", buf1);			
				//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
				if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
				  continue;
				}// </ 2005 11 07>
				
// 				buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				
// 				if(strlen(buf1) < 1){// 2006 06 移動
// 				  continue;
// 				}//if_end
				
				strcpy(buf2, node->feature);
				//ノードごとに解析情報の取得.要素数は 9
				if(strlen(buf2) < 1){
				  continue;
				}//if_end
				//				if(typeSet == 1){// 形態素原形で数える
				p = strtok(buf2, "," );//取得情報の分割
				if(p != NULL){
				  sprintf(buf3, "%s", p);// 品詞情報の取得
				}
				j = 1;
				while ( p != NULL ) {
				 if(typeSet == 2){// 品詞情報で数える
				   if( j == 1){//品詞情報1
					 strL.push_back(p);
					 //  Rprintf("typeSet == = %d; p = %s\n", typeSet, p);					 
					 p = NULL;
					 continue;
				   }
				 }else if(typeSet == 1){// 形態素原形で数える
				   

				   if( j == 7){// p には形態素原型がセットされている状態
					 //////// < 2005 06 13>
					 if(pos_n != 0){// 指定品詞があるなら
					   for( i = 0; i < pos_n; i++){
					     sprintf(buf4, "%s", Ppos2[i].c_str()); // 2011 03 10 sprintf(buf4, "%s", Ppos[i]);				
						 //				Rprintf("buf4 %s\n", buf4);
						 if(strcmp(buf3, buf4) == 0){ // 指定品詞と一致
						   if( p!=NULL &&  strcmp(p, "*") != 0){
							 strL.push_back(p);//原型
						   } else{
							 strL.push_back(buf1);//元の語形
						   }
						 }
					   } 
					   ////////// </ 2005 06 13>
					 }
					 else if(p == NULL || strcmp(p, "*") == 0){
					   strL.push_back(buf1);//元の語形
					   // Rprintf("typeSet == = %d; p = %s\n", typeSet, p);
					 }
					 else{
					   strL.push_back(p);
					   // Rprintf("typeSet == = %d; p = %s\n", typeSet, p) ;
					 }
				   }
				 }// _else if(typeSet == 1){形態素原形で数える_end
				 p = strtok( NULL,"," );
				 j++;
				}//_while_( p != NULL )_end
			  }//if_typSet_end// 形態素原形で数える_end
			  // 次のノードに移る直前

			  if(strL.size() >= (unsigned int) Ngram){
				//				Rprintf("strL size =  %d\n", strL.size() );
				target.erase();
				//target.append("[");
				xx = 1;
				for ( iter = strL.begin(); iter != strL.end(); iter++){
				  //				  Rprintf("in for\n");
// 				  Rprintf("str %s\n", *iter);
// 				  Rprintf("after Rprintf in for\n");
				  target.append( *iter);
				  if(xx < Ngram){
					target.append(" ");//	target.append("-");
				  }
				  xx++;
				}
				xx = 1;
				//target.append("]");
				//				Rprintf("target  =  %s\n", target );
				//			  	Rprintf("before map find \n");	
				pa = m1.find(target);//出てきた N -gramは既にマップにあるか？
				if(pa != m1.end()){
				  pa->second =  pa->second + 1;
				  //二つ目の数値を加算
				}
				else{// マップにないなら，新規にマップに追加
				  m1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
				}
				strL.pop_front();
				//	Rprintf(" strL.pop_front() \n");
				

			  
			  
			  
			  }//(str.size() >= Ngram)_end
			  // 次のノードに移る
			} //else_必要な情報ノード_end

			
		  } //_for_node_end
// 		  ////////////////////
		}//else_if_type_set_end
		
	  }//while_fopen_
	  mecab_destroy(mecab);
	  
	  	// map の内容を data.frame にして返す
	  n = (int)m1.size();
	  if(n < 1){
		Rprintf("empty results\n");
		UNPROTECT(pc);
		return (R_NilValue);
	  }

		  ////////////////////

	  
	  if(n > 40000){
		Rprintf("Ngram exceeds 40000\n");
		nn = 0;
		
		pa = m1.begin();
		while(pa != m1.end()){
		  if(   (pa->second)  <  mFreq){
			nn ++;
		  }else{
			m2.insert(make_pair( (pa->first).c_str() , pa->second));
			
		  }
		  pa++;
		}
		//		Rprintf("m2 finished\n");
		
		m2.insert(make_pair("LESSTHAN2", nn));
		nn = m2.size();
		if(nn < 1){
		  Rprintf("empty results\n");
		  UNPROTECT(pc);
		  return (R_NilValue);
		}


		//		Rprintf("nn = %d\n", nn);		
		PROTECT(mydf = allocVector(VECSXP, Ngram+1));//Ngram +  頻度列 のdata.frame
		pc++;
		for( z = 0; z < Ngram; z++){
		  SET_VECTOR_ELT(mydf, z, allocVector(STRSXP, nn));// N-gram セット
		}
		SET_VECTOR_ELT(mydf, Ngram, allocVector(INTSXP, nn));// 頻度	  
		//		Rprintf("length = %d \n", m1.size());

		////////////////// orjinal /////////////
		pa = m2.begin();
		for ( z = 0; z < nn; z++) {
		  strcpy(buf4, (pa->first).c_str());
		  p = strtok(buf4 , " " );//取得情報の分割
		  j = 0;
		  while(p != NULL){
			sprintf(buf3, "%s", p);// 品詞情報の取得
			if(strcmp(buf3, "LESSTHAN2") == 0){
//  // < 2005 11 08>
// #if defined(WIN32)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( buf3 , CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( buf3 , CE_NATIVE));			 	
// #else
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( buf3, CE_UTF8 )); 	
// #endif			  
// 			  // SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkChar( buf3 ));
//   // </ 2005 11 08>

			  SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));	///<2006 04 18>		 

			  
			  for( i = 1; i < Ngram; i++){

				
//  // < 2005 11 08>
// #if defined(WIN32)
// 				SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( " " , CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 				SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( " " , CE_NATIVE));			 	
// #else
// 				SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( " ", CE_UTF8 ));			 	
// #endif				
// 				//SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkChar( " " ));
//   // </ 2005 11 08>

		SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( " " ,  (utf8locale)?CE_UTF8:CE_NATIVE));///<2006 04 18>		 


		
			  }
			  p = NULL;
			}
			else{
			  
//  // < 2005 11 08>
// #if defined(WIN32)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, CE_NATIVE ));			 	
// #elif  defined(__MINGW32__)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, CE_NATIVE ));			 	
// #else
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif			  
// 			  // SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkChar( buf3 ));
//   // </ 2005 11 08>

			  SET_STRING_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));	///<2006 04 18>
			  
			  
			  p = strtok( NULL, " "); 
			}
		  }
		  INTEGER(VECTOR_ELT(mydf,Ngram))[z] =   pa->second;// 最後に頻度情報
		  pa++;
		  // if(z % 10 == 0) Rprintf("* ");// 2006 03 removed 2007 05
		}

		
		Rprintf("\n");// 2006 03
			  // 	  	  // オブジェクトはデータフレームだと指定する
		PROTECT(tmp = mkString("data.frame"));
		pc++;
		// 	  //df 内ベクトルの名前を用意
		PROTECT(varlabels = allocVector(STRSXP, Ngram+1));
		pc++;
		//  その単純な初期化
		for( z = 0; z < Ngram; z++){
		  sprintf(buf3, "Ngram%d",z+1);

		  
//  // < 2005 11 08>
// #if defined(WIN32)
// 		  SET_STRING_ELT(varlabels, z, mkCharCE( buf3, CE_NATIVE ));			 
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(varlabels, z, mkCharCE( buf3 , CE_NATIVE));		   
// #else
// 		  SET_STRING_ELT(varlabels, z, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif		  
// 		  // SET_STRING_ELT(varlabels, z, mkChar( buf3 ));
//   // </ 2005 11 08>

		  SET_STRING_ELT(varlabels, z, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));
		  
		}

  
//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", CE_NATIVE));			 
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", CE_NATIVE));			 
// #else
// 		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", CE_UTF8));			 
// #endif		
// 		// SET_STRING_ELT(varlabels, Ngram, mkChar("Freq"));
//   // </ 2005 11 08>

		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", (utf8locale)?CE_UTF8:CE_NATIVE));	
		

	///////////////////////// new_end 
	  // データフレームオブジェクト mydf の属性設定 
		setAttrib(mydf, R_ClassSymbol, tmp);
		setAttrib(mydf, R_NamesSymbol, varlabels);
	  
	  // 行名を指定．必須
		PROTECT(row_names = allocVector(STRSXP, nn));
		pc++;
		char labelbuff[6];//char labelbuff[nn]; 2006 03
		for ( z = 0; z < nn  ; z++) {
		  sprintf(labelbuff, "%d", z+1);
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			 
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			 
// #else
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));			 
// #endif		  
// 		  //SET_STRING_ELT(row_names, z, mkChar(labelbuff));
//   // </ 2005 11 08>

		   SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));	
		  
		}
		setAttrib(mydf, R_RowNamesSymbol, row_names);
		
		UNPROTECT(pc);
		
		////////////////// orjinal _end /////////////
		
		
	  }else{// else_n
		//		Rprintf("n < 40000:\n");
				//		Rprintf("nn = %d\n", nn);		
		PROTECT(mydf = allocVector(VECSXP, Ngram+1));//Ngram 列のdata.frame
		pc++;
		for( z = 0; z < Ngram; z++){
		  SET_VECTOR_ELT(mydf, z, allocVector(STRSXP, n));// N-gram セット
		}
		SET_VECTOR_ELT(mydf, Ngram, allocVector(INTSXP, n));// 頻度	  
		//		Rprintf("length = %d \n", m1.size());
		
		//Rprintf("length = %d \n", m1.size());

		pa = m1.begin();

		for (z = 0; z < n; z++) {
		  strcpy(buf4, (pa->first).c_str());
		  p = strtok(buf4 , " " );//取得情報の分割
		  j = 0;
		  while(p != NULL){
			sprintf(buf3, "%s", p);//
//  // < 2005 11 08>
// #if defined(WIN32)
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, CE_NATIVE ));			 	
// #elif  defined(__MINGW32__)
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, CE_NATIVE ));			 	
// #else
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif
			//SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), z, mkChar( buf3 ));
  // </ 2005 11 08>
			
			SET_STRING_ELT(VECTOR_ELT(mydf, j++), z, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));			 //<2006 04 18>
			

			
			p = strtok( NULL, " "); 
		  }
		  INTEGER(VECTOR_ELT(mydf,Ngram))[z] =   pa->second;// 最後に頻度情報
		  pa++;
		}
			  // 	  	  /
		// オブジェクトはデータフレームだと指定する
		PROTECT(tmp = mkString("data.frame"));
		pc++;
		// 	  //df 内ベクトルの名前を用意
		PROTECT(varlabels = allocVector(STRSXP, Ngram+1));
		pc++;
		//  その単純な初期化
		for( z = 0; z < Ngram; z++){
		  sprintf(buf3, "Ngram%d",z+1);

		  
//  // < 2005 11 08>
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, z, mkCharCE( buf3, CE_NATIVE ));		 	
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, z, mkCharCE( buf3, CE_NATIVE ));		  	
// #else
// 			SET_STRING_ELT(varlabels, z, mkCharCE( buf3, CE_UTF8 ));			 
// #endif		  
// 			// SET_STRING_ELT(varlabels, z, mkChar( buf3 ));
//   // </ 2005 11 08>

		  SET_STRING_ELT(varlabels, z, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));	
			
		}

		
//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", CE_NATIVE));			 
// #else
// 		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", CE_UTF8));			 
// #endif		
// 		// SET_STRING_ELT(varlabels, Ngram, mkChar("Freq"));
//   // </ 2005 11 08>	
		
		SET_STRING_ELT(varlabels, Ngram, mkCharCE("Freq", (utf8locale)?CE_UTF8:CE_NATIVE));	
		
	  
	///////////////////////// new_end 
	  // データフレームオブジェクト mydf の属性設定 
		setAttrib(mydf, R_ClassSymbol, tmp);
		setAttrib(mydf, R_NamesSymbol, varlabels);
	  
		// 行名を指定．必須
		PROTECT(row_names = allocVector(STRSXP, n));
		pc++;
		char labelbuff[6];//char labelbuff[n]; 2006 03
		for ( z = 0; z < n  ; z++) {
		  sprintf(labelbuff, "%d", z+1);

		  
//  // < 2005 11 08>
// #if defined(WIN32)
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));		   	
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_NATIVE));			 
// #else
// 		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));		 
// #endif		  
// 		  // SET_STRING_ELT(row_names, z, mkChar(labelbuff));
//   // </ 2005 11 08>
		  
		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));	

		  
		}
		setAttrib(mydf, R_RowNamesSymbol, row_names);
		
		UNPROTECT(pc);
		
	  }// else_n_end
	  return (mydf);
	  
	}//_else
  }//_SEXP_Ngram
}// Extern


////////////////////////
//////////////////////////////////////////////////////
/* 指定されたデータフレーム列からテキストを読み込んでMatrixを作成 */
extern "C" {

  SEXP docMatrixDF(SEXP charVec, SEXP charLeng, SEXP pos, SEXP posN, SEXP minFreq, SEXP mydic  ){ 
	
	char *input;// [5120];
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	string str;
	
	mecab_t *mecab;
	mecab_node_t *node;
	int file = 0, n0 = 0, i = 0, j = 0, pc = 0, posC = 0, z = 0, *rans;
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char buf3[BUF1]; // [512];品詞チェック用
	char buf4[BUF1]; // [1024];品詞チェック用
	char *p;
	SEXP ans, dim, dimnames, row_names, col_names;// 2009 04 03 
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
	PROTECT(charLeng = AS_INTEGER(charLeng));pc++;
	int file_num = INTEGER_POINTER(charLeng)[0];
	PROTECT(charVec = AS_CHARACTER(charVec));pc++;
		  	
	map<string, int> ma0;//, ma[file_num];     // 全要素用のマップと要素ごとのマップ // 2009 04 03
	vector<map<string, int> > vecmap; // 2011 03 09 
	for (i = 0; i < file_num; i++) vecmap.push_back(map<string, int>() );
	
	map<string, int>::iterator pma0, pma;// マップ検索用// 2009 04 03



	// SEXP ans, dim, dimnames, row_names, col_names;
	
	// PROTECT(posN = AS_INTEGER(posN));pc++;
	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3
	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2;// = 1
	  flag = 0;
	}
	
	// 2011 03 10 // char *Ppos[pos_n];
	vector <string> Ppos2; 
	SEXP myPos;  


	if(pos_n > 0 && flag ){
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
	// 2011 03 10 // 	Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back ( CHAR(STRING_ELT(myPos, i)) ); // 2011 03 10 
		
	  }
	}// 2005 06 23
	else{
// 	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());
	  	  	  
	  // 2011 03 10 // Ppos[0] = R_alloc(strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back ( meisiCode() ); // 2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc(strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back ( keiyouCode() ); // 2011 03 10 
	}
	
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	CHECK(mecab);

		  	
	// map<string, int> ma0, ma[file_num];     // 全要素用のマップと要素ごとのマップ
	// map<string, int>::iterator pma0, pma;// マップ検索用

	for(file = 0; file < file_num; file++){
	  //	  Rprintf("Row %d check\n", file);
 	  if( strlen(CHAR(STRING_ELT(charVec, file))) < 1 || STRING_ELT(charVec, file) == NA_STRING ) {
// 		Rprintf("in ISNA\n");		
//  		ma[file].insert(make_pair("NA", 0));// 1 は 1個目と言う意
// 		ma0.insert(make_pair("NA", 0));// 1 は 1個目と言う意
 		continue;
 	  }
	  input = R_alloc(strlen(CHAR(STRING_ELT(charVec, file))), sizeof(char));

	  strcpy(input , CHAR(STRING_ELT(charVec, file)));

	  node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
	  CHECK(node);
		
	  /// 解析結果のノードをなめる
	  for (;  node; node = node->next) {
		//		printf("%d ", node->id);
			  
		// Rprintf("node check %d times\n", file );
		
		if (node->stat == MECAB_BOS_NODE)
		  //printf("BOS");
		  continue;
		else if (node->stat == MECAB_EOS_NODE)
		  //printf("EOS");
		  continue;
		else {
		  // 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
		  strncpy(buf1, node->surface, node->length) ;//元の語形

		  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
		  
		  if(strlen(buf1) < 1){// 2006 06 移動
			continue;
		  }
		  
		  //< 2005 11 07> //Rprintf("%s\n", buf1);			
		  //if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
		  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
			continue;
		  }// </ 2005 11 07>
		  
// 		  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
		  
// 		  if(strlen(buf1) < 1){// 2006 06 移動
// 			continue;
// 		  }
		  //Rprintf("buf1 = %s\n", buf1);
		  
		  strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
		  if(strlen(buf2) < 1){
			continue;
		  }
		  //Rprintf("buf2 = %s\n", buf2);
		  
		  p = strtok(buf2, "," );//取得情報の分割
		  j = 1;
		  while ( p != NULL ) {
			//Rprintf("in p != NULL\n");
			if(j == 1){//品詞情報1
			  str = p;
			  //Rprintf("in str = p\n");
			  // str.append(",");
			  //Rprintf("str = %s\n", p);
			  //Rprintf("in str = p\n");			  
			  for(z = 0; z < pos_n; z++){
			    sprintf(buf4, "%s", Ppos2[z].c_str());// 2011 03 10 // sprintf(buf4, "%s", Ppos[z]);
				sprintf(buf3, "%s", p);
				//Rprintf("buf3 %s\n", buf3);
				if(strcmp(buf3, buf4) == 0){
				  posC = 1;
				}
			  }
			  if(posC != 1){
				p = NULL;
				posC = 0;
				continue;
			  }
			}else if( j == 7){
			  //			  Rprintf("in j == 7\n");
			  if(p == NULL || strcmp(p, "*") == 0){
				str = buf1;// str.append(buf1);//元の語形
			  }
			  else{
				str = p;// str.append(p);// 形態素原型
				//				Rprintf("in j == 7 + str = p\n");				
			  }
			  
			  pma0 = ma0.find(str);//出てきた形態素原型は既に全体マップにあるか？
			  if(pma0 != ma0.end()){
				//				Rprintf("incremnet\n");
				pma0->second =  pma0->second + 1;
				//二つ目の数値を加算
			  }
			  else{// マップにないなら，新規にマップに追加
				ma0.insert(make_pair(str, 1));// 1 は 1個目と言う意味
				//				Rprintf("insert \n");
			  }
			  
			  // pma = ma[file].find(str);//出てきた形態素原型は既に個別マップにあるか？
			  pma = (vecmap.at (file)).find(str);
			  if(pma != (vecmap.at(file)).end()){// if(pma != ma[file].end()){
				pma->second =  pma->second + 1;
				//二つ目の数値を加算
			  }
			  else{// マップにないなら，新規にマップに追加
			    (vecmap.at (file)).insert(make_pair(str, 1));// ma[file].insert(make_pair(str, 1));// 1 は 1個目と言う意味
			  }
			}
			p = strtok( NULL,"," );
			j++;
			posC = 0;
		  }// while(P!= NULL) 						 
		}// else
	  }// for (; node;)
	  
// 	  if(ma[file].size() < 1){
// 		ma[file].insert(make_pair("NA", 0));// 1 は 1個目と言う意
// 	  }
	
	}// for(file < file_num)
	
	mecab_destroy(mecab);
	
	//	Rprintf("term check\n");

	// 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		//// http://www.geocities.jp/ky_webid/cpp/library/009.html
		// // なお、erase() は、削除された要素を指しているイテレータを無効な状態にしてしまうので注意が 必要です。vector、deque、string の場合には、erase() の戻り値を受け取るようにして、必ず有効なイテレータ を取得し直すように書きますが、set や multiset の場合は（また、第１０章で紹介 する map や multimap の場合は）erase() に戻り値がありません。そこで、次のように使います。
		// // nums.erase( it++ );    // 要素を削除し、インクリメントを１つ先へ進める
		ma0.erase(pma0++);///// ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
			  
	
	n0 = (int)ma0.size();// ターム数のチェック
	if(n0 < 1){
	  Rprintf("no terms larger than  minFreq = %d\n", mFreq);
	  UNPROTECT(pc);
	  return (R_NilValue); 
	}
	PROTECT (ans = allocMatrix(INTSXP, file_num, n0));pc++;// docMatrixDF
	rans = INTEGER(ans);
	

  // 各要素から取得したマップを，全要素ファイルから取得したマップ ma[0]と照合して
  // 行列に登録する．
  // 方法としては ma[0] の要素ごとに，各マップに登録があるか確認し，あれば，その頻度を，なければ 0 を代入する
	for(file = 0; file < file_num ; file++){// ベクトルの長さだけ繰り返す
	  //Rprintf("ma0 and ma  check\n");
	  pma0 = ma0.begin();
	  pma = (vecmap.at(file)).begin(); // ma[file].begin();
	  j = 0;// Term 数のチェック
	  while( j < n0 && pma0 != ma0.end()  ){

	    pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
	    if(pma != (vecmap.at(file)).end()){ // if(pma != ma[file].end()){// 見つかった
		  rans[ file  + file_num * j] = pma->second;
		}
		else{
		  rans[file + file_num * j  ] = 0;		  
		}
		pma0++;
		j++;
	  }
	  
	}
	
    PROTECT(dim = allocVector(INTSXP, 2));pc++;
    INTEGER(dim)[0] = file_num; INTEGER(dim)[1] = n0;
    setAttrib(ans, R_DimSymbol, dim);
   
	Rprintf("to make data frame\n");
  // 列名のセット；これはターム
	PROTECT(col_names = allocVector(STRSXP, n0));pc++;
	pma0 = ma0.begin();
	for(i = 0; i < n0 && pma0 != ma0.end() ; i++){
	  strcpy(buf3, (pma0->first).c_str());
	  //Rprintf("col names = %s\n", (pma0->first).c_str());
	  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_NATIVE ));			 	
// #else
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_UTF8 ));			 	
// #endif
// 	  // SET_STRING_ELT(col_names, i, mkChar(buf3 ));
//  // </ 2005 11 08>

	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));
	  
	  pma0++;
	  // if(i % 10 == 0) Rprintf("* ");// 2006 03 27 removed 2007 05
	}
	Rprintf("\n");// 2006 03 27
  // 行名のセット// これは要素番号
	PROTECT(row_names = allocVector(STRSXP, file_num));pc++;
	i = 0;
	for(i = 0; i < file_num ; i++){
	  //Rprintf("row names = %s\n", *pv);
	  sprintf(buf3, "OBS.%d", i+1);

	  
 // // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, i, mkCharCE( buf3, CE_NATIVE ));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, i, mkCharCE( buf3, CE_NATIVE ));			 	
// #else
// 	  SET_STRING_ELT(row_names, i, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif	  
// 	  // SET_STRING_ELT(row_names, i, mkChar( buf3 ));
//   // </ 2005 11 08>
	  
	  SET_STRING_ELT(row_names, i, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));	

	  
	}
	
	PROTECT(dimnames = allocVector(VECSXP, 2));pc++;
	SET_VECTOR_ELT(dimnames, 0, row_names); // 行の名前をセット
	SET_VECTOR_ELT(dimnames, 1, col_names);//列名をセット
	setAttrib(ans, R_DimNamesSymbol, dimnames);
	
	UNPROTECT(pc);
	

	return (ans);
	
	// 2005 06 17 
	///////////////////////////////////////////////////////
  }
} // extern


////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/* 指定されたディレクトリからファイルを読み込んでMatrixを作成 */
extern "C" {

    
  SEXP docMatrix2(SEXP directory, SEXP origF, SEXP fileN, SEXP ft, SEXP pos, SEXP posN, SEXP minFreq, SEXP kigo , SEXP mydic  ){// SEXP sym, SEXP kigo 

	int file = 0,  n0 = 0, i = 0, j = 0, posC = 0, pc = 0, *rans;
	char* path;
	int f_count = INTEGER_VALUE( fileN );// 最小頻度の数
	// const char* KIGO = CHAR(STRING_ELT(kigo,0));
	char KIGO[BUF1] ;
	 strcpy(KIGO, kigoCode());
	int mSym  = INTEGER_VALUE( kigo );// INTEGER_VALUE( sym );// 記号を含めるか 0 含めない;1 含める
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
	if(mFreq < 1){
	  mFreq = 1;
	}
	//	int mFt  = INTEGER_VALUE( ft );// ファイル 0 かディレクトリ 1 か最小頻度の数

	char file_name[512];
	char input[BUF4];
	string str;
	
	mecab_t *mecab;
	mecab_node_t *node;
	
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char buf3[BUF1];// 品詞チェック用
	char buf4[BUF1];//[1024];
	char *p;
	SEXP ans, dim, dimnames, row_names, col_names;
	
	FILE *fp; // 2009 04 03
	map<string, int> ma0;// , ma[f_count];     // ファイル数の数+登録単語用マップの数1 // 2009 04 03
	vector <map<string, int> > vecmap;// 2011 03 09 
	for (i = 0; i < f_count; i++) vecmap.push_back(map<string, int>() );
	map<string, int>::iterator pma0, pma;// マップ検索用// 2009 04 03



	PROTECT(directory = AS_CHARACTER(directory)); pc++;
	path = R_alloc(strlen(CHAR(STRING_ELT(directory, 0))), sizeof(char));
	strcpy(path, CHAR(STRING_ELT(directory, 0)));


	// 2011 03 10 // char * f[f_count];
	vector <string> ff;
	PROTECT(origF = AS_CHARACTER(origF));pc++;
	// 2011 03 10 //  for(file = 0; file < f_count; file ++){
 		// 2011 03 10 // f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(origF, file))), sizeof(char));
	// 2011 03 10 // }
	for(file = 0; file < f_count; file ++){
  		// 2011 03 10 //  strcpy(f[file], CHAR(STRING_ELT(origF, file)));
		ff.push_back( CHAR(STRING_ELT(origF, file))) ; // 2011 03 10 
	}

	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3

	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2;// = 1
	  flag = 0;
	}

	// 2011 03 10 //  char *Ppos[pos_n];
	vector <string> Ppos2; 
	SEXP myPos;  

	if(pos_n > 0 && flag){
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back( CHAR(STRING_ELT(myPos, i)) ); // 2011 03 10 
		//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
		if(strcmp(Ppos2[i].c_str(), KIGO) == 0){// if(strcmp(Ppos[i], KIGO) == 0){
		  mSym = 1;
		}
	  }
	}// 2005 06 23
	else{
// 	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));

	  // 2011 03 10 // Ppos[0] = R_alloc(strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back (meisiCode() );//2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc(strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back (  keiyouCode() ); // 2011 03 10 
	}

	
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	CHECK(mecab);
	
	// FILE *fp;





	// map<string, int> ma0, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	// map<string, int>::iterator pma0, pma;// マップ検索用


	for(file = 0; file < f_count; file++) {


	  sprintf(file_name, "%s/%s", path, ff[file].c_str());// 2011 03 10  // sprintf(file_name, "%s/%s", path, f[file]);
	  if(strcmp(file_name, "") == 0){
		continue;
		
	  }
	  ////////////////
	  if((fp = fopen(file_name, "r")) == NULL){
		Rprintf("NULL! %s not found\n",file_name);
		UNPROTECT(pc);
		return(R_NilValue);
	  }else{
		Rprintf("file_name =  %s opened\n",  file_name );	
		while(!feof(fp)){
		  
		  if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11  if(fgets(input, 5120, fp) != NULL){
			if(strlen(input) < 1){
			  continue;
			}
			//			Rprintf("after fgets input =  %s\n",input );
			node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
			CHECK(node);
			//			Rprintf("node check" );		
			/// 解析結果のノードをなめる
			for (;  node; node = node->next) {
			  //			  		printf("%d ", node->id);
			  
			  if (node->stat == MECAB_BOS_NODE)
				//printf("BOS");
				continue;
			  else if (node->stat == MECAB_EOS_NODE)
				//printf("EOS");
				continue;
			  else {
				// 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
				strncpy(buf1, node->surface, node->length) ;//元の語形

								
				buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				
				if(strlen(buf1) < 1){// 2006 06 移動
				  continue;
				}
				//				Rprintf("buf1 = %s\n", buf1);
				
					//< 2005 11 07> //Rprintf("%s\n", buf1);			
				//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){//if( atoi(buf1) == 0x0e){//エスケープ記号類
				if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
				  continue;
				}// </ 2005 11 07>
				
// 				buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				
// 				if(strlen(buf1) < 1){// 2006 06 移動
// 				  continue;
// 				}
// 				//				Rprintf("buf1 = %s\n", buf1);
				
				strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
				if(strlen(buf2) < 1){
				  continue;
				}
				//				Rprintf("buf2 = %s\n", buf2);
				
				p = strtok(buf2, "," );//取得情報の分割
				// 品詞の判定
				if( p != NULL){
				  sprintf(buf3, "%s", p);
				  //   Rprintf("buf3 %s\n", buf3);
				  // if(mSym < 1 && strcmp(buf3, "記号") == 0){				  
				  if(mSym < 1 && strcmp(buf3, KIGO) == 0){
					//  Rprintf("記号\n");
					p = NULL;
					// j = 9;
					continue;// 記号は一切省き，総計にも加えない
				  }
				  for(i = 0; i < pos_n; i++){
				    sprintf(buf4, "%s", Ppos2[i].c_str());// 2011 03 10		sprintf(buf4, "%s", Ppos[i]);				
					//					Rprintf("buf4 %s\n", buf4);
					if(strcmp(buf3, buf4) == 0){
					  posC = 1;
					  break;
					}
				  }
				  if(posC != 1){
					p = NULL;
					posC = 0;
					continue;
				  }
				}
				//////////////
				j = 1;
				while ( p != NULL ) {
				  //				  Rprintf("in p!=NULL j = %d p = %s\n",j, p);
				  
				  // if(j == 1){//品詞情報1
// 					str = p;
// 					// str.append(",");
// 				  }else
				  
				  if( j == 7){
					if(p == NULL || strcmp(p, "*") == 0){
					  str = buf1;// str.append(buf1);//元の語形
					  //Rprintf("in str = buf1\n");
					}
					else{
					  str = p;// str.append(p);
					  //Rprintf("in str = p\n");
					}
					
					pma0 = ma0.find(str);//出てきた形態素原型は既に全体マップにあるか？
					if(pma0 != ma0.end()){
					  pma0->second =  pma0->second + 1;
					  //二つ目の数値を加算
					}
					else{// マップにないなら，新規にマップに追加
					  ma0.insert(make_pair(str, 1));// 1 は 1個目と言う意味
					}
					
					pma = (vecmap.at(file)).find(str);// ma[file].find(str);//出てきた形態素原型は既に個別マップにあるか？
					if(pma != (vecmap.at(file)).end()){// if(pma != ma[file].end()){
					  pma->second =  pma->second + 1;
					  //二つ目の数値を加算
					}
					else{// マップにないなら，新規にマップに追加
					   (vecmap.at(file)).insert(make_pair(str, 1));// ma[file].insert(make_pair(str, 1));// 1 は 1個目と言う意味
					}
				  }
				  
				  j++;
				  if(j > 7){
					p = NULL;
					j = 1;
				  }else{
					p = strtok( NULL,"," );
				  }
				  
				  posC = 0;
				}// while(P!= NULL) 						 
			  }// else
			}// for (; node;)
			
			//Rprintf("node check ended\n");
			
		  }//if(fgets(input, 5120, fp) != NULL){
		  // 		  file++;
// 		  if(file >= f_count){
// 			break;
//		  }
		}// while(!feof(fp));//while

		fclose(fp);


	  }//else
	  //Rprintf("increment \n");
	  if(file >= f_count){
		break;
	  }
	}// for(int z = 0; //while pv != v.end();

	mecab_destroy(mecab);
	//Rprintf("mecab destroyed\n");
	// 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		//// http://www	.geocities.jp/ky_webid/cpp/library/009.html
		ma0.erase(pma0++);/////ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
			  
	
	n0 = (int)ma0.size();// ターム数のチェック
			
	if(n0 > 40000){
	  Rprintf("Warning! number of extracted terms = %d\n", n0);
	}else{
	
	  Rprintf("number of extracted terms = %d\n", n0);
	}
	PROTECT (ans = allocMatrix(INTSXP, f_count, n0));pc++;//docMatrix2
	rans = INTEGER(ans);
	

  // 各ファイルから取得したマップを，全ファイルから取得したマップ ma[0]と照合して
  // 行列に登録する．
  // 方法としては ma[0] の要素ごとに，各マップに登録があるか確認し，あれば，その頻度を，なければ 0 を代入する
	for(file = 0; file < f_count ; file++){// ファイルの数だけ繰り返す
	  //Rprintf("ma0 and ma  check\n");
	  pma0 = ma0.begin();
	  pma = (vecmap.at(file)).begin();// ma[file].begin();
	  j = 0;// Term 数のチェック
	  while( j < n0 && pma0 != ma0.end()  ){
	    pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
	    if(pma != (vecmap.at(file)).end()){// if(pma != ma[file].end()){// 見つかった
		  rans[ file  + f_count * j] = pma->second;
		}
		else{
		  rans[file + f_count * j  ] = 0;		  
		}
		pma0++;
		j++;
	  }
	  
	}
	
    PROTECT(dim = allocVector(INTSXP, 2));pc++;
    INTEGER(dim)[0] = f_count; INTEGER(dim)[1] = n0;
    setAttrib(ans, R_DimSymbol, dim);
   
	Rprintf("to make matrix now\n");
  // 列名のセット；これはターム
	PROTECT(col_names = allocVector(STRSXP, n0));pc++;
	pma0 = ma0.begin();
	for(i = 0; i < n0 && pma0 != ma0.end() ; i++){
	  strcpy(buf3, (pma0->first).c_str());
	  //Rprintf("col names = %s\n", (pma0->first).c_str());
  
 // // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_NATIVE ));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_NATIVE ));			 	
// #else
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_UTF8 ));			 	
// #endif	  
// 	  // SET_STRING_ELT(col_names, i, mkChar(buf3 ));
//   // </ 2005 11 08>

	   SET_STRING_ELT(col_names, i, mkCharCE(buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));

	   
	  pma0++;
	  // if(i % 10 == 0) Rprintf("* ");// 2006 03 27 removed 2007 05
	}
	
	Rprintf("\n");// 2006 03 27
  // 行名のセット// これはファイル名
	// pv = v.begin();
	PROTECT(row_names = allocVector(STRSXP, f_count));pc++;
	for(file = 0; file < f_count;file++){ 
	  //	  Rprintf("row names = %s\n", f[file]);
	  strcpy(buf3, ff[file].c_str() );// 2011 03 10 // strcpy(buf3, f[file]);// strcpy(buf3, *pv);
	  //	  if(mFt == 1){

  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3 , CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3 , CE_NATIVE));			 	
// #else
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif	  
// 	  // SET_STRING_ELT(row_names, file, mkChar( buf3 ));
//   // </ 2005 11 08>

	  SET_STRING_ELT(row_names, file, mkCharCE( buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));		
	  
		//	  }else {
		//		SET_STRING_ELT(row_names, file, mkChar("texts" ));
		//	  }
	}

// 	free(f);
// 	Rprintf("free \n");


//	Rprintf("set dimnames \n");
	PROTECT(dimnames = allocVector(VECSXP, 2));pc++;
	SET_VECTOR_ELT(dimnames, 0, row_names); // 行の名前をセット
	SET_VECTOR_ELT(dimnames, 1, col_names);//列名をセット
	setAttrib(ans, R_DimNamesSymbol, dimnames);

	UNPROTECT(pc);
	//	Rprintf("UNPROTECT \n");
	//free(f);
	return (ans);
	
	// 2005 06 17 
	///////////////////////////////////////////////////////
  }
} // extern

/////////////////////////////////////////////////////////////









/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/* 指定されたディレクトリからファイルを読み込んでMatrixを作成 */
extern "C" {

  
  SEXP docNgram2(SEXP directory, SEXP origF, SEXP fileN, SEXP ft, SEXP type, SEXP pos, SEXP posN, SEXP minFreq, SEXP N, SEXP kigo , SEXP mydic  ){// SEXP sym, SEXP kigo
	
	int file = 0,  n0 = 0, i = 0, j = 0, posC = 0, xx =0, pc = 0, *rans;
	unsigned int  wz = 0;
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int f_count = INTEGER_VALUE( fileN );// 最小頻度の数
	char* path;
	// 2011 03 10 //  char * f[f_count];
	vector <string> ff;
	// const char* KIGO = CHAR(STRING_ELT(kigo,0));
	char KIGO[BUF1] ;
	strcpy(KIGO, kigoCode());
	
	PROTECT(directory = AS_CHARACTER(directory));pc++;
	path = R_alloc(strlen(CHAR(STRING_ELT(directory, 0))), sizeof(char));
	strcpy(path, CHAR(STRING_ELT(directory, 0)));

	PROTECT(origF = AS_CHARACTER(origF));pc++;
	// 2011 03 10 // for(file = 0; file < f_count; file ++){
	 // 2011 03 10 // f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(origF, file))), sizeof(char));
        // 2011 03 10 // }
	for(file = 0; file < f_count; file ++){
	  // 2011 03 10 //  strcpy(f[file], CHAR(STRING_ELT(origF, file)));
	  ff.push_back (CHAR(STRING_ELT(origF, file)) ) ; // 2011 03 10 
       }


	int typeSet  = INTEGER_VALUE( type );// 形態素か，品詞か，文字か
	int Ngram  = INTEGER_VALUE( N );// N の数
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
	if(mFreq < 1){
	  mFreq = 1;
	}
	int mSym  = INTEGER_VALUE( kigo );//INTEGER_VALUE( sym );// 記号を含めるか 0 含めない;1 含める

	//	int mFt  = INTEGER_VALUE( ft );// ファイル 0 かディレクトリ 1 か最小頻度の数
	

	char file_name[512];
	char input[BUF4];
	string str;
	
	mecab_t *mecab;
	mecab_node_t *node;
	
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char buf3[BUF1]; // [512]; 品詞チェック用
 	char buf4[BUF1]; // [1024]; 品詞チェック用

	char *p;
	SEXP ans, dim, dimnames, row_names, col_names;
	
	
	wchar_t  wbuf [BUF4] = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	list <string> strL;
	list <string>::iterator iter;
	
	string target;
	char target2[BUF3];

	FILE *fp;// 2009 04 03
	map<string, int> ma0;// , ma[f_count];     // ファイル数の数+登録単語用マップの数1 // 2009 04 03
	vector <map<string, int> > vecmap;// 2011 03 09 
	for (i = 0; i < f_count; i++) vecmap.push_back(map<string, int>() );
	map<string, int>::iterator pma0, pma;// マップ検索用// 2009 04 03


	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3
	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2;// = 1
	  flag = 0;
	}
	
	// 2011 03 10 // char *Ppos[pos_n];
	vector<string> Ppos2;
	SEXP myPos;  

	if(pos_n > 0 && flag ){
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back (  CHAR(STRING_ELT(myPos, i))) ; // 2011 03 10  
		//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
		if(strcmp(Ppos2[i].c_str(), KIGO) == 0){// if(strcmp(Ppos[i], KIGO) == 0){
		  mSym = 1;
		}
	  }
	}// 2005 06 23
	else{
// 	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());

	  // 2011 03 10 // Ppos[0] = R_alloc(strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back ( meisiCode()  ) ; // 2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc(strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back ( keiyouCode()  ) ; // 2011 03 10 	
	}
	

	
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	CHECK(mecab);



	
	// FILE *fp;

	
	// map<string, int> ma0, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	// map<string, int>::iterator pma0, pma;// マップ検索用


	for(file = 0; file < f_count; file++) {

	  sprintf(file_name, "%s/%s", path, ff[file].c_str());// sprintf(file_name, "%s/%s", path, f[file]);
	  if(strcmp(file_name, "") == 0){
		continue;
		
	  }
	  
	  if((fp = fopen(file_name, "r")) == NULL){
		Rprintf("NULL! %s not found\n",file_name);
		UNPROTECT(pc);
		return(R_NilValue);
	  }else{
		strL.clear();
		Rprintf("file_name =  %s opened\n",  file_name );	
		while(!feof(fp)){
		  
		  if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11  if(fgets(input, 5120, fp) != NULL){
			if(strlen(input) < 1){
			  continue;
			}

			////////////////////////////////////////////////////////////////




			if(typeSet == 0){// 文字単位なら
			  //			  Rprintf("in typeSet == 0 %s \n",  file_name );	
			  //		  Rprintf("%s\n", input);			  
			  p = strchr( input, '\n' );
			  /* 改行文字があった場合 */
			  if ( p != NULL )
				{
				  /* 改行文字を終端文字に置き換える */
				  *p = '\0';
				}
		
			  //			Rprintf("strlen of input= %d\n", strlen(input));  
			  if(strlen(input) > 0){
				//Rprintf("%s\n", input);
				
				//		  Rprintf("in strlen(input) > 0  %s \n",  file_name );				
				mbstowcs(wbuf, input,  strlen(input));/* マルチバイト文字列をワイド文字列に変換*/

				//for(int z = 0; z <  (wcslen(wbuf) - Ngram); z++){
				for( wz = 0; wz <  wcslen(wbuf) ; wz++){	// 2005 07 22
				  //				for(int y = 0; y < Ngram; y++){
				  // < 2005 11 06>
// 				if(WIN){
// 				  //wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// 				}else{
// 				    sprintf(target2, "%lc", wbuf[wz]);// Linux では sprintf
// 				}
				  // </ 2005 11 06>
// #if defined(WIN32)
// 				wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// #elif  defined(__MINGW32__)
// 				wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
// #else
// 				sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// #endif

		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif
			
				  //				Rprintf("target2 = %s\n", target2);
				  if(strlen(target2) < 1){
					break;;
				  }
				  //エスケープ記号類
				  //strcpy(buf1, *target2);
				  if( *target2 > 0x00 && *target2 < 0x21 ){//エスケープ記号類0x0e
					continue;
				  }//
				  //////////// windows では wsprintf(str[ys], "%lc", wbuf[z+ys + yw]);

				  //	if( strcmp(target2, " ") == 0 ||  strcmp(target2, "　")==0){
				  if( strcmp((char *) target2, " ") == 0 ||  strcmp((char *) target2, "　")==0){
					//				  printf("found\n");
					continue;
				  } else{
				  
					/////////////// new_begin //////////////// ここは文字単位
 
					// target = target2;   

					strL.push_back( target2);
					
					if(strL.size() >= (unsigned int) Ngram){
					  //					Rprintf("in if(strL.size) \n");					
					  target.erase();
					  target.append("[");
					  xx = 1;
					  for ( iter = strL.begin(); iter != strL.end(); iter++){
						//						Rprintf("in for\n");
						//						Rprintf("str %s\n", * iter);
						target.append( *iter);
						if(xx < Ngram){
						  target.append("-");
						}
						xx++;
						//					  Rprintf("xx = %d\n", xx);
						
					  }
					  xx = 1;
					  target.append("]");
					  //					Rprintf("target %s\n", target);
					  //					Rprintf("before m1.find \n");
					  //出てきた形態素原型は既に全体マップにあるか？
					  pma = ma0.find(target);
					  //出てきた形態素原型は既にマップにあるか？
					  if(pma != ma0.end()){
						pma->second =  pma->second + 1;
						//二つ目の数値を加算
					  }					
					  else{// マップにないなら，新規にマップに追加
						//					  Rprintf("add map \n");
						ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
					  }
					  // 同じ処理を個別マップにも行う
					  pma = (vecmap.at(file)).find(target);// ma[file].find(target);//出てきた形態素原型は既に個別マップにあるか？
					  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){
						pma->second =  pma->second + 1;
						//二つ目の数値を加算
					  }
					  else{// マップにないなら，新規にマップに追加
					    (vecmap.at(file)).insert(make_pair(target, 1));// ma[file].insert(make_pair(target, 1));// 1 は 1個目と言う意味
					  }
					  
					  strL.pop_front();
					}//_if strSize>= Ngram
				  }// _else_end
				  
				  ////////////////////////////////////// new _end ////
				}//_for2_< wcslen
				// gram[0] = '\0';
				// 			  for(int z = 0; < z strlen(gram); z++){
				// 				gram[z] = 0
				// 			  }
				// 			  sprintf(gram, "%s", "");//memset(gram,'\0',strlen(gram));			  
			  }// if_strlen_>_0_end




			  
			  
			} else {// if_type_set 形態素あるいは品詞の場合




	
// 			  mecab = mecab_new2 ("めかぶ");
// 			  CHECK(mecab);





			  ////////////////////////////////////////////////////////////////
			  //			Rprintf("after fgets input =  %s\n",input );
			  node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
			  CHECK(node);
			  //			Rprintf("node check" );		
			  /// 解析結果のノードをなめる
			  for (;  node; node = node->next) {// node とはその文の形態素ごとに設定される
				//			  		printf("%d ", node->id);
			  
				if (node->stat == MECAB_BOS_NODE)
				  //printf("BOS");
				  continue;
				else if (node->stat == MECAB_EOS_NODE)
				  //printf("EOS");
				  continue;
				else {// BOS, EOS 以外
				// 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
				  strncpy(buf1, node->surface, node->length) ;//元の語形

				  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				 
				  if(strlen(buf1) < 1){// 2006 06 移動
					continue;
				  }
				  
				   	//< 2005 11 07> //Rprintf("%s\n", buf1);			
				  //if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){// if( atoi(buf1)  == 0x0e){//エスケープ記号類
				  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
					continue;
				  }// </ 2005 11 07>
				  
// 				  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				 
// 				  if(strlen(buf1) < 1){// 2006 06 移動
// 					continue;
// 				  }
				  //				Rprintf("buf1 = %s\n", buf1);
				
				  strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
				  if(strlen(buf2) < 1){
					continue;
				  }
				  //				Rprintf("buf2 = %s\n", buf2);

				  //////////////

				
				  p = strtok(buf2, "," );//取得情報の分割
				  // 品詞の判定
				  j = 1;
				  ////////////////////////////////////////////////////////////////////

					
				  if(typeSet == 2){// 品詞情報で数える
					if( j == 1 && p != NULL ){//品詞情報1
					  strL.push_back(p);
					  //						Rprintf("typeSet == = %d; p = %s\n", typeSet, p);					 
					  p = NULL;
					}


					
				  }else if(typeSet == 1){// 形態素原形で数える
						  
					//////////////////////////////////////////////


					if(j == 1 &&  p != NULL){
					  sprintf(buf3, "%s", p);
					  // if(mSym < 1 && strcmp(buf3, "記号") == 0){
					  if(mSym < 1 && strcmp(buf3, KIGO) == 0){						
						p = NULL;
						//j = 9;
						continue;// 記号は一切省き，総計にも加えない
					  }
					  //				  Rprintf("buf3 %s\n", buf3);
					  for(i = 0; i < pos_n; i++){
					    sprintf(buf4, "%s", Ppos2[i].c_str());// 2011 03 10 	sprintf(buf4, "%s", Ppos[i]);				
						//					Rprintf("buf4 %s\n", buf4);
						if(strcmp(buf3, buf4) == 0){
						  posC = 1;
						  break;
						}
					  }
					  if(posC != 1){
						p = NULL;
						posC = 0;
						continue;
					  }
					}
				  
				
					while ( p != NULL ) {
					  //				  Rprintf("in p!=NULL j = %d p = %s\n",j, p);


					
					  // if(j == 1){//品詞情報1
					  // 					str = p;
					  // 					// str.append(",");
					  // 				  }else
				  
					  if( j == 7){
						if(p == NULL || strcmp(p, "*") == 0){
						  // strL.push_back(p);//原型str = buf1;// str.append(buf1);//元の語形
						  strL.push_back(buf1);//元の語形
						  //Rprintf("in str = buf1\n");
						}
						else{
						  strL.push_back(p);//原型 strL.push_back(buf1);
						  //Rprintf("in str = p\n");
						}
					  }
					  p = strtok( NULL,"," );
					  j++;
					  if(j > 7){
						p = NULL;
					  }

				  
					}// while(P!= NULL)
					posC = 0;
				  } // else if typset = 1

				  
				}  //////else // BOS, EOS 以外

				////////////// 抽出終了
				if(strL.size() >= (unsigned int) Ngram){// リストのサイズが指定通りであるなら，保存を始める
				  //				  Rprintf("type = %d, strL size =  %d\n", typeSet, strL.size() );
				  target.erase();//保存のための文字列を初期化
				  target.append("[");
				  xx = 1;
				  for ( iter = strL.begin(); iter != strL.end(); iter++){
					// Rprintf("in for\n");
					//sprintf(buf3, "%s", *iter);
					//Rprintf("str %s\n", *iter);
					//Rprintf("after Rprintf in for\n");
					target.append( *iter);// target.append( buf3); //target.append( *iter);
					//					Rprintf("target append\n");
					if(xx < Ngram){
					  target.append("-");
					}
					xx++;
				  } // for 
				  xx = 1;
				  target.append("]");	
				  pma0 = ma0.find(target);//出てきた形態素原型は既に全体マップにあるか？
				  if(pma0 != ma0.end()){
					pma0->second =  pma0->second + 1;
					//二つ目の数値を加算
				  }
				  else{// マップにないなら，新規にマップに追加
					ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
				  }
				  
				  pma = (vecmap.at(file)).find(target);// ma[file].find(target);// str 2006 08 25 出てきた形態素原型は既に個別マップにあるか？
				  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){
					pma->second =  pma->second + 1;
					//二つ目の数値を加算
				  }
				  else{// マップにないなら，新規にマップに追加
				   (vecmap.at(file)).insert(make_pair(target, 1));//  // ma[file].insert(make_pair(target, 1));// 1 は 1個目と言う意味
				  }
				  
				  strL.pop_front();// 最初の要素を取り除く
				}				  // if(strL.size() >= Ngram)
				
			  }//for(;node;)// Rprintf("node check ended\n");
			  
			} 
		  } //if(fgets) //////
		}//while(feop)
		fclose(fp);

	  } //else// 

	}//for(file);
//	free(f);
	mecab_destroy(mecab);
	//Rprintf("mecab destroyed\n");
			  
	// 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		//// http://www.	geocities.jp/ky_webid/cpp/library/009.html
		ma0.erase(pma0++);///// ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
	
	n0 = (int)ma0.size();// ターム数のチェック
		
	if(n0 > 40000){
	  Rprintf("Warning! number of extracted terms =  %d\n", n0);
	}else{
	  Rprintf("number of extracted terms = %d\n", n0);
	}

	if(n0 < 1){
	  Rprintf("no terms extracted\n");
	  UNPROTECT(pc);
	  return(R_NilValue);
	}
	

	PROTECT (ans = allocMatrix(INTSXP, f_count, n0));pc++;//docNgram2
	rans = INTEGER(ans);
	

	// 各ファイルから取得したマップを，全ファイルから取得したマップ ma[0]と照合して
	// 行列に登録する．
	// 方法としては ma[0] の要素ごとに，各マップに登録があるか確認し，あれば，その頻度を，なければ 0 を代入する
	for(file = 0; file < f_count ; file++){// ファイルの数だけ繰り返す
	  //Rprintf("ma0 and ma  check\n");
	  pma0 = ma0.begin();
	  pma = (vecmap.at(file)).begin();//  ma[file].begin();
	  j = 0;// Term 数のチェック
	  while( j < n0 && pma0 != ma0.end()  ){
	    pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
	   if(pma !=  (vecmap.at(file)).end()){ // if(pma != ma[file].end()){// 見つかった
		  rans[ file  + f_count * j] = pma->second;
		}
		else{
		  rans[file + f_count * j  ] = 0;		  
		}
		pma0++;
		j++;
	  }
	  
	}
	
    PROTECT(dim = allocVector(INTSXP, 2));pc++;
    INTEGER(dim)[0] = f_count; INTEGER(dim)[1] = n0;
    setAttrib(ans, R_DimSymbol, dim);
   
	Rprintf("to make matrix now\n");
	// 列名のセット；これはターム
	PROTECT(col_names = allocVector(STRSXP, n0));pc++;
	pma0 = ma0.begin();
	for(i = 0; i < n0 && pma0 != ma0.end() ; i++){
	  strcpy(buf3, (pma0->first).c_str());
	  //Rprintf("col names = %s\n", (pma0->first).c_str());
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , CE_NATIVE));			 	
// #else
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_UTF8 ));			 	
// #endif	  
// 	  // SET_STRING_ELT(col_names, i, mkChar(buf3 ));
//   // </ 2005 11 08>

	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));	
	  
	  pma0++;
	  // if(i % 10 == 0) Rprintf("* ");// 2006 03 27 removed 2007 05
	}
	
	Rprintf("\n");// 2006 03 27
	// 行名のセット// これはファイル名
	// pv = v.begin();
	PROTECT(row_names = allocVector(STRSXP, f_count));pc++;
	for(file = 0; file < f_count;file++){ 
	  //	  Rprintf("row names = %s\n", f[file]);
	  strcpy(buf3, ff[file].c_str());// 2011 03 10 strcpy(buf3, f[file]);// strcpy(buf3, *pv);
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_NATIVE ));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_NATIVE ));			 	
// #else
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif
// 	  // SET_STRING_ELT(row_names, file, mkChar( buf3 ));
//   // </ 2005 11 08>

	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));	
	  
	}

	// 	free(f);
	// 	Rprintf("free \n");


	//	Rprintf("set dimnames \n");
	PROTECT(dimnames = allocVector(VECSXP, 2));pc++;
	SET_VECTOR_ELT(dimnames, 0, row_names); // 行の名前をセット
	SET_VECTOR_ELT(dimnames, 1, col_names);//列名をセット
	setAttrib(ans, R_DimNamesSymbol, dimnames);

	UNPROTECT(pc);
	//	Rprintf("UNPROTECT \n");
	//free(f);
	return (ans);
	
	// 2005 06 17 
	///////////////////////////////////////////////////////
  }
} // extern


/////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
/* 指定されたディレクトリからファイルを読み込んでMatrixを作成 */
extern "C" {

  
  SEXP NgramDF2(SEXP directory, SEXP origF, SEXP fileN, SEXP ft, SEXP type, SEXP pos, SEXP posN, SEXP minFreq, SEXP N, SEXP kigo , SEXP mydic ){ // SEXP sym ,SEXP kigo 
	
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int file = 0,  n0 = 0, i = 0, j = 0, posC = 0, pc = 0, xx =0;
	char* path;
	unsigned int wz = 0;

	int f_count = INTEGER_VALUE( fileN );// 最小頻度の数
	// const char* KIGO = CHAR(STRING_ELT(kigo,0));
	char KIGO[BUF1] ;
	strcpy(KIGO, kigoCode());
	
	// 2011 03 10 //  char * f[f_count];
	vector <string> ff; 


       // 	PROTECT(directory = AS_CHARACTER(directory));pc++;
       // 	path = R_alloc(strlen(CHAR(STRING_ELT(directory, 0))), sizeof(char));
       // 	strcpy(path, CHAR(STRING_ELT(directory, 0)));


       // 	PROTECT(origF = AS_CHARACTER(origF));pc++;
       // 	for(file = 0; file < f_count; file ++){
       // 	 f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(origF, file))), sizeof(char));
       //  }
       // 	for(file = 0; file < f_count; file ++){
       // 	  strcpy(f[file], CHAR(STRING_ELT(origF, file)));
       // }


	int typeSet  = INTEGER_VALUE( type );// 形態素か，品詞か，文字か
	int Ngram  = INTEGER_VALUE( N );// N の数
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数

	int mSym  = INTEGER_VALUE( kigo );// = INTEGER_VALUE( sym );// 記号を含めるか 0 含めない;1 含める

	//	int mFt  = INTEGER_VALUE( ft );// ファイル 0 かディレクトリ 1 か最小頻度の数
	
	char file_name[512];
	char input[BUF4];
	string str;
	
	mecab_t *mecab;
	mecab_node_t *node;
	
	char buf1[BUF1];// [512];//入力された語形を記憶
	char buf2[BUF3];
	char buf3[BUF1];// [512]; 品詞チェック用
	char buf4[BUF1];// [1024];品詞チェック用

	char *p;
	SEXP mydf, tmp, row_names, varlabels;
	
	
	wchar_t  wbuf [BUF4] = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	list <string> strL;
	list <string>::iterator iter;
	
	string target;
	char target2[BUF3];

	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3

	FILE *fp;// 2009 04 03
	map<string, int> ma0;// , ma[f_count];     // ファイル数の数+登録単語用マップの数1
	vector <map<string, int> > vecmap; // 2011 03 09
	for (i = 0; i < f_count; i++) vecmap.push_back(map<string, int>() );
	map<string, int>::iterator pma0, pma;// マップ検索用



	PROTECT(directory = AS_CHARACTER(directory));pc++;// 2009 04 03
	path = R_alloc(strlen(CHAR(STRING_ELT(directory, 0))), sizeof(char));
	strcpy(path, CHAR(STRING_ELT(directory, 0)));


	PROTECT(origF = AS_CHARACTER(origF));pc++;
	// 2011 03 10 // for(file = 0; file < f_count; file ++){
	 // 2011 03 10 // f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(origF, file))), sizeof(char));
        // 2011 03 10 // }
	for(file = 0; file < f_count; file ++){
	  // 2011 03 10 // strcpy(f[file], CHAR(STRING_ELT(origF, file)));
	  ff.push_back ( CHAR(STRING_ELT(origF, file)) ) ; // 2011 03 10 
       }// 2009 04 03
	
	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2;// = 1
	  flag = 0;
	}
 
	// 2011 03 10 // char *Ppos[pos_n];
	vector <string> Ppos2;
	SEXP myPos;  
	
	if(pos_n > 0 && flag){
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back ( CHAR(STRING_ELT(myPos, i)) ); // 2011 03 10 
		//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
		if(strcmp(Ppos2[i].c_str(), KIGO) == 0){// if(strcmp(Ppos[i], KIGO) == 0){
		  mSym = 1;
		}
	  }
	}// 2005 06 23
	else{
// 	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());
	  // 2011 03 10 // Ppos[0] = R_alloc(strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back (meisiCode()  ) ; // 2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc(strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back( keiyouCode() ) ; // 2011 03 10 

	}
	

	
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	CHECK(mecab);



	// FILE *fp;


	// map<string, int> ma0, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	// map<string, int>::iterator pma0, pma;// マップ検索用


	for(file = 0; file < f_count; file++) {
	 //  Rprintf("file =%s\n", f[file]);
// 	  if(strcmp(f[file], "") == 0){
// 		continue;

	  sprintf(file_name, "%s/%s", path, ff[file].c_str() );// sprintf(file_name, "%s/%s", path, f[file]);

	  if(strcmp(file_name, "") == 0){
		continue;
	  }
	  //	  Rprintf("file_name =  %s ready to open\n",  file_name );
	  
	  if((fp = fopen(file_name, "r")) == NULL){
		Rprintf("NULL! %s not found\n",file_name);
		UNPROTECT(pc);
		return(R_NilValue);
	  }else{
		strL.clear();
		Rprintf("file_name =  %s opened\n",  file_name );	
		while(!feof(fp)){
		  
		  if(fgets(input, FILEINPUT, fp) != NULL){// 2010 03 11  if(fgets(input, 5120, fp) != NULL){
			if(strlen(input) < 1){
			  continue;
			}

			////////////////////////////////////////////////////////////////




			if(typeSet == 0){// 文字単位なら
			  //			  Rprintf("in typeSet == 0 %s \n",  file_name );	
			  //		  Rprintf("%s\n", input);			  
			  p = strchr( input, '\n' );
			  /* 改行文字があった場合 */
			  if ( p != NULL )
				{
				  /* 改行文字を終端文字に置き換える */
				  *p = '\0';
				}
		
			  //			Rprintf("strlen of input= %d\n", strlen(input));  
			  if(strlen(input) > 0){
				//Rprintf("%s\n", input);
				
				//		  Rprintf("in strlen(input) > 0  %s \n",  file_name );				
				mbstowcs(wbuf, input,  strlen(input));/* マルチバイト文字列をワイド文字列に変換*/
// 				//			  Rprintf("strlen of wbuf = %d\n", wcslen(wbuf));  
// 				//				  while(wbuf[test] != '\0'){
// 				//			  while(test <= (wcslen(wbuf) - Ngram)  ){
// 				//
// 				//for(int z = 0; z <  (wcslen(wbuf) - Ngram); z++){
				for( wz = 0; wz <  wcslen(wbuf) ; wz++){	// 2005 07 22
				  //				for(int y = 0; y < Ngram; y++){

// // 				if(WIN){
// // 				  //wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// // 				}else{
// // 				    sprintf(target2, "%lc", wbuf[wz]);// Linux では sprintf
// // 				}

// #if defined(WIN32)
// 				wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// #elif  defined(__MINGW32__)
// 				wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
// #else
// 				sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// #endif
		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif
			   
				
				  if(strlen(target2) < 1){
					break;;
				  }
				  //エスケープ記号類
				  //strcpy(buf1, *target2);
				  if( *target2 > 0x00 && *target2 < 0x21 ){//エスケープ記号類0x0e
					continue;
				  }//
				  /////////////// windows では wsprintf(str[ys], "%lc", wbuf[z+ys + yw]);

				  //	if( strcmp(target2, " ") == 0 ||  strcmp(target2, "　")==0){
				  if( strcmp((char *) target2, " ") == 0 ||  strcmp((char *) target2, "　")==0){
					//				  printf("found\n");
					continue;
				  } else{
				  
					/////////////// new_begin //////////////// ここは文字単位
 
					// target = target2;   

					strL.push_back( target2);
					
					if(strL.size() >= (unsigned int) Ngram){
					  //					Rprintf("in if(strL.size) \n");					
					  target.erase();
					  //					  target.append("[");
					  xx = 1;
					  for ( iter = strL.begin(); iter != strL.end(); iter++){
						//						Rprintf("in for\n");
						//						Rprintf("str %s\n", * iter);
						target.append( *iter);
						if(xx < Ngram){
						  target.append(" ");// target.append("-");
						}
						xx++;
						//					  Rprintf("xx = %d\n", xx);
						
					  }
					  xx = 1;
					  //					  target.append("]");
					  //					Rprintf("target %s\n", target);
					  //					Rprintf("before m1.find \n");
					  //出てきた形態素原型は既に全体マップにあるか？
					  pma = ma0.find(target);
					  //出てきた形態素原型は既にマップにあるか？
					  if(pma != ma0.end()){
						pma->second =  pma->second + 1;
						//二つ目の数値を加算
					  }					
					  else{// マップにないなら，新規にマップに追加
						//					  Rprintf("add map \n");
						ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
					  }
					  // 同じ処理を個別マップにも行う
					  pma = (vecmap.at(file)).find(target);// ma[file].find(target);//出てきた形態素原型は既に個別マップにあるか？
					  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){
						pma->second =  pma->second + 1;
						//二つ目の数値を加算
					  }
					  else{// マップにないなら，新規にマップに追加
					    (vecmap.at(file)).insert(make_pair(target, 1));// ma[file].insert(make_pair(target, 1));// 1 は 1個目と言う意味
					  }
					  
					  strL.pop_front();
					}//_if strSize>= Ngram
				  }// _else_end
				  
				  ////////////////////////////////////// new _end ////
				}//_for2_< wcslen
				// gram[0] = '\0';
				// 			  for(int z = 0; < z strlen(gram); z++){
				// 				gram[z] = 0
				// 			  }
				// 			  sprintf(gram, "%s", "");//memset(gram,'\0',strlen(gram));			  
			  }// if_strlen_>_0_end




			  
			  
			} else {// if_type_set 形態素あるいは品詞の場合




	
// 			  mecab = mecab_new2 ("めかぶ");
// 			  CHECK(mecab);





			  ////////////////////////////////////////////////////////////////
			  //			Rprintf("after fgets input =  %s\n",input );
			  node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
			  CHECK(node);
			  //			Rprintf("node check" );		
			  /// 解析結果のノードをなめる
			  for (;  node; node = node->next) {// node とはその文の形態素ごとに設定される
				//			  		printf("%d ", node->id);
			  
				if (node->stat == MECAB_BOS_NODE)
				  //printf("BOS");
				  continue;
				else if (node->stat == MECAB_EOS_NODE)
				  //printf("EOS");
				  continue;
				else {// BOS, EOS 以外
				// 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
				  strncpy(buf1, node->surface, node->length) ;//元の語形

				  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				  
				  if(strlen(buf1) < 1){// 2006 06 移動
					continue;
				  }
				  //
				  
				  	//< 2005 11 07> //Rprintf("%s\n", buf1);			
				  //if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){// if( atoi(buf1)  == 0x0e){//エスケープ記号類
				  if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
					continue;
				  }// </ 2005 11 07>
				  
// 				  buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
				  
// 				  if(strlen(buf1) < 1){// 2006 06 移動
// 					continue;
// 				  }
// 				  //		
//				  // Rprintf("buf1 = %s\n", buf1);
				
				  strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
				  if(strlen(buf2) < 1){
					continue;
				  }
				  //				Rprintf("buf2 = %s\n", buf2);

				  //////////////

				
				  p = strtok(buf2, "," );//取得情報の分割
				  // 品詞の判定
				  j = 1;
				  ////////////////////////////////////////////////////////////////////

					
				  if(typeSet == 2){// 品詞情報で数える
					if( j == 1 && p != NULL ){//品詞情報1
					  strL.push_back(p);
					  //						Rprintf("typeSet == = %d; p = %s\n", typeSet, p);					 
					  p = NULL;
					}


					
				  }else if(typeSet == 1){// 形態素原形で数える
						  
					//////////////////////////////////////////////


					if(j == 1 &&  p != NULL){
					  sprintf(buf3, "%s", p);
					  // if(mSym < 1 && strcmp(buf3, "記号") == 0){
					  if(mSym < 1 && strcmp(buf3, KIGO) == 0){						
						p = NULL;
						//j = 9;
						continue;// 記号は一切省き，総計にも加えない
					  }
					  //				  Rprintf("buf3 %s\n", buf3);
					  for(i = 0; i < pos_n; i++){
					    sprintf(buf4, "%s", Ppos2[i].c_str());// sprintf(buf4, "%s", Ppos[i]);				
						//					Rprintf("buf4 %s\n", buf4);
						if(strcmp(buf3, buf4) == 0){
						  posC = 1;
						  break;
						}
					  }
					  if(posC != 1){
						p = NULL;
						posC = 0;
						continue;
					  }
					}
				  
				
					while ( p != NULL ) {
					  //				  Rprintf("in p!=NULL j = %d p = %s\n",j, p);


					
					  // if(j == 1){//品詞情報1
					  // 					str = p;
					  // 					// str.append(",");
					  // 				  }else
				  
					  if( j == 7){
						if(p == NULL || strcmp(p, "*") == 0){
						  //strL.push_back(p);//原型str = buf1;// str.append(buf1);//元の語形
						  strL.push_back(buf1);//元の語形
						  //Rprintf("in str = buf1\n");
						}
						else{
						  strL.push_back(p);//原形;
						  //Rprintf("in str = p\n");
						}
					  }
					  p = strtok( NULL,"," );// 2006 03 27 ","
					  j++;
					  if(j > 7){
						p = NULL;
					  }

				  
					}// while(P!= NULL)
					posC = 0;
				  } // else if typset = 1

				  
				}  //////else // BOS, EOS 以外

				////////////// 抽出終了
				if(strL.size() >= (unsigned int) Ngram){// リストのサイズが指定通りであるなら，保存を始める
				  //				  Rprintf("type = %d, strL size =  %d\n", typeSet, strL.size() );
				  target.erase();//保存のための文字列を初期化
				  // target.append("[");
				  xx = 1;
				  for ( iter = strL.begin(); iter != strL.end(); iter++){
					// Rprintf("in for\n");
					//sprintf(buf3, "%s", *iter);
				// 	char bufXX[256];
// 					sprintf(bufXX, "%s", *iter);
// 					Rprintf("str %s\n", bufXX);
					//Rprintf("after Rprintf in for\n");
					target.append( *iter);// target.append( buf3); //target.append( *iter);
					//Rprintf("target append %s\n", target.c_str());
					if(xx < Ngram){
					  target.append(" ");//  target.append("-");
					}
					xx++;
				  } // for 
				  xx = 1;
				  // target.append("]");	
				  pma0 = ma0.find(target);//出てきた形態素原型は既に全体マップにあるか？
				  if(pma0 != ma0.end()){
					pma0->second =  pma0->second + 1;
					// Rprintf("target + 1  %s\n", target.c_str());
					//二つ目の数値を加算
				  }
				  else{// マップにないなら，新規にマップに追加
					ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
					//Rprintf("target new add  %s\n", target.c_str());
				  }
				  
				  pma = (vecmap.at(file)).find(target);// ma[file].find(target);//str 出てきた形態素原型は既に個別マップにあるか？
				   if(pma != (vecmap.at(file)).end()){// if(pma != ma[file].end()){
					pma->second =  pma->second + 1;
					//Rprintf("each file target + 1  %s\n", target.c_str());
					//二つ目の数値を加算
				  }
				  else{// マップにないなら，新規にマップに追加
				    (vecmap.at(file)).insert(make_pair(target, 1));// ma[file].insert(make_pair(target, 1));// 1 は 1個目と言う意味
					//Rprintf("each file target new add   %s\n", target.c_str());
				  }
				  
				  strL.pop_front();// 最初の要素を取り除く
				}				  // if(strL.size() >= Ngram)
				
			  }//for(;node;)// Rprintf("node check ended\n");
			  
			} 
		  } //if(fgets) //////
		}//while(feop)
		fclose(fp);

	  } //else// 

	}//for(file);
//	free(f);
	mecab_destroy(mecab);
	//Rprintf("mecab destroyed\n");
		
	// 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		//// http://www	.geocities.jp/ky_webid/cpp/library/009.html
		ma0.erase(pma0++);/////	ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
		  
	
	n0 = (int)ma0.size();// ターム数のチェック

			
	if(n0 > 40000){
	  Rprintf("Warning! number of extracted terms =   %d\n", n0);
	}else{
	  Rprintf("number of extracted terms = %d\n", n0);
	  
	}

	
	if(n0 < 1){
	  Rprintf("no terms extracted\n");
	  UNPROTECT(pc);
	  return(R_NilValue);
	}
	

	///////////////////////////////
	// データフレームの生成
			//		Rprintf("nn = %d\n", nn);		
	//	PROTECT(mydf = allocVector(VECSXP, Ngram+1));pc++;//Ngram +  頻度列 のdata.frame
	PROTECT(mydf = allocVector(VECSXP, Ngram + f_count));pc++;//Ngram +  ファイル数のdata.frame   
	for( xx = 0; xx < Ngram; xx++){
	  SET_VECTOR_ELT(mydf, xx, allocVector(STRSXP, n0));// 各Ngram 列のメモリ n0 行分用意
	}
	//	SET_VECTOR_ELT(mydf, Ngram, allocVector(INTSXP, n0));// 頻度列のメモリ n0 行分を用意
	for( xx = Ngram; xx < Ngram+f_count; xx++){
	  SET_VECTOR_ELT(mydf, xx, allocVector(INTSXP, n0));// 頻度列のメモリ n0 行分を用意
	}	
	//Rprintf("df allocated\n" );

	///各列の代入開始
		///各列の代入開始
	
	pma0 = ma0.begin();
	for (xx = 0; xx < n0; xx++) {//n0 個のタームの数だけ繰り返す
	  strcpy(buf4, (pma0->first).c_str());// 最初の要素の文字列を取得し
	  p = strtok(buf4 , " " );//取得情報の分割
	  //	  Rprintf("buf4 = %s - ", buf4); 
	  j = 0;
	  while(p != NULL){
		sprintf(buf3, "%s", p);// 品詞情報の取得
		//		Rprintf("buf3 = %s\n", buf3);
 
//  // < 2005 11 08>
// #if defined(WIN32)
// 		SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3 , CE_NATIVE));	 
// #elif  defined(__MINGW32__)
// 		SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3 , CE_NATIVE));		 	
// #else
// 		SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8 ));  	
// #endif		
// 		// SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkChar( buf3 ));//j列のxx行にセット
//   // </ 2005 11 08>


		SET_STRING_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));// < 2006 04 18>
		
		
		p = strtok( NULL, " ");
		if( j >= Ngram){
		  continue;
		}
	  }
	  pma0++;
	}
	////////////////
	//		Rprintf("next loop\n");
// 	pma0 = ma0.begin();
// 	while(pma0 != ma0.end()){
	for(file = 0; file < f_count; file ++){
	  pma0 = ma0.begin();
	  pma = (vecmap.at(file)).begin();// ma[file].begin();
	  j = 0;
	  while(j < n0 && pma0 != ma0.end()){
	    pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
	    if(pma != (vecmap.at(file)).end()){// if(pma != ma[file].end()){// 見つかった
		  INTEGER(VECTOR_ELT(mydf, Ngram+file))[j] =   pma->second;// Ngram 列の最後に頻度情報
		}
		else{
		  INTEGER(VECTOR_ELT(mydf, Ngram+file))[j] = 0;
		}
		pma0++;
		j++;

	  }
	  // Rprintf("* ");// 2006 03 27 removed 2007 05
	}

	Rprintf("\n");// 2006 03 27
	//	Rprintf("before tmp set\n" );
	// 	  	  // オブジェクトはデータフレームだと指定する
	PROTECT(tmp = mkString("data.frame")); pc++;//tmpにその属性を一時保存
	
// 		// 	  //df 内ベクトルの名前を用意
// 	PROTECT(varlabels = allocVector(STRSXP, Ngram+1)); pc++;
	
// 	//  その単純な初期化
// 	for(xx= 0; xx < Ngram; xx++){
// 	  sprintf(buf3, "Ngram%d", xx+1);
// 	  SET_STRING_ELT(varlabels, xx, mkChar( buf3 ));
// 	}

		// 	  //df 内ベクトルの名前を用意
	//	PROTECT(varlabels = allocVector(STRSXP, Ngram+1)); pc++;
	PROTECT(varlabels = allocVector(STRSXP, Ngram+f_count)); pc++;	
	//  その単純な初期化
	for(xx= 0; xx < Ngram; xx++){
	  sprintf(buf3, "Ngram%d", xx+1);

  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(varlabels, xx, mkCharCE( buf3 , CE_NATIVE));		 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(varlabels, xx, mkCharCE( buf3 , CE_NATIVE));			 	
// #else
// 	  SET_STRING_ELT(varlabels, xx, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif
// 	  // SET_STRING_ELT(varlabels, xx, mkChar( buf3 ));
//   // </ 2005 11 08>

	  SET_STRING_ELT(varlabels, xx, mkCharCE( buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));		 	
	  
	}
	
	j = 0;
	for(xx = Ngram; xx < Ngram+f_count; xx++){
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(varlabels, xx, mkCharCE(f[j++], CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(varlabels, xx, mkCharCE(f[j++], CE_NATIVE));			 	
// #else
// 	  SET_STRING_ELT(varlabels, xx, mkCharCE(f[j++], CE_UTF8));			 	
// #endif 
// 	  //SET_STRING_ELT(varlabels, xx, mkChar(f[j++]));
//   // </ 2005 11 08>
	  
	  SET_STRING_ELT(varlabels, xx, mkCharCE(ff[j++].c_str() , (utf8locale)?CE_UTF8:CE_NATIVE));// 2011 03 10 SET_STRING_ELT(varlabels, xx, mkCharCE(f[j++], (utf8locale)?CE_UTF8:CE_NATIVE));
	  
	}
		
		
	//	Rprintf("after colunm names set\n" );
	///////////////////////// new_end 
	  // データフレームオブジェクト mydf の属性設定 
	setAttrib(mydf, R_ClassSymbol, tmp);
	setAttrib(mydf, R_NamesSymbol, varlabels);
	  
	  // 行名を指定．必須
	PROTECT(row_names = allocVector(STRSXP, n0));pc++;
	char labelbuff[6];// char labelbuff[2]; 2006 03
	for (xx = 0; xx < n0 ; xx++) {
	  sprintf(labelbuff, "%d", xx+1);
 
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff, CE_NATIVE));			 
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff, CE_NATIVE));			 
// #else
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff, CE_UTF8));		 	
// #endif	  
// 	  // SET_STRING_ELT(row_names, xx, mkChar(labelbuff));
//   // </ 2005 11 08>

	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff, (utf8locale)?CE_UTF8:CE_NATIVE));

	  
	}
	setAttrib(mydf, R_RowNamesSymbol, row_names);
		
	//	Rprintf("before UNPROTECT\n" );
	UNPROTECT(pc);
	//	Rprintf("UNPROTECT \n");
	//free(f);
	return (mydf);
	
	// 2005 08 20 
	///////////////////////////////////////////////////////
  }
} // extern


















//////////////////////////////////////////////////////
/* 指定されたターゲットからテキストを読み込んでNgramを作成 */
extern "C" {
  SEXP docDF(SEXP directory,
			 SEXP origF,
			 SEXP fileN,
			 SEXP ft,
			 SEXP type,
			 SEXP pos,
			 SEXP posN,
			 SEXP minFreq,
			 SEXP N,
// 			 SEXP sym,
// 			 SEXP kigo,
			 SEXP Genkei,
			 SEXP nDF,
			 SEXP mydic  ){
	
	int file = 0,  n0 = 0, i = 0, j = 0, pc = 0, xx = 1;
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int f_count = INTEGER_VALUE( fileN );//ファイル(行)数
	char* path = 0;// 2011 03 11  char* path;
	// 2011 03 10 //  char * f[f_count];
	vector <string> ff;
	//const char* KIGO = CHAR(STRING_ELT(kigo,0));

	int typeSet  = INTEGER_VALUE( type );// 形態素か，品詞か，文字か
	int Ngram  = INTEGER_VALUE( N );// N の数
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
	if(mFreq < 1){
	  mFreq = 1;
	}
	//int mSym  = INTEGER_VALUE( sym );// 記号を含めるか 0 含めない;1 含める
	int NDF  = INTEGER_VALUE( nDF );// データフレームの列数
		
	int genkei  = INTEGER_VALUE( Genkei );// 活用は原型か 0 表層形か 1 
	char file_name[512];
	char  input[BUF4];
	char * p;
	string str;
	
	char buf1[BUF1];// [512];//入力された語形を記憶
	//	char buf2[1024];
	char buf3[BUF1];// [512];品詞チェック用
	char buf4[BUF1];// [1024];品詞チェック用

	SEXP tmp, row_names, mydf = R_NilValue, varlabels = R_NilValue;// 2011 03 11 //  SEXP mydf, tmp, row_names, varlabels;// SEXP ans, dim, dimnames, row_names, col_names;
	
			
	int mFt  = INTEGER_VALUE( ft );// ファイル 0 かディレクトリ 1 かデータフレーム列か2
	/////
		
	FILE *fp;// 2009 04 03	
	map<string, int> ma0;//, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	vector <map<string, int> > vecmap;// 2011 03 09 
	for (i = 0; i < f_count; i++) vecmap.push_back(map<string, int>() );
	map<string, int>::iterator pma0, pma;// マップ検索用
	list <string> hinsi, strL, saibun;
	list <string>::iterator hinsi_it, iter, saibun_it;// 2009 04 03

	/////
	// Rprintf("f_file  =  %i\n", f_count); // 2011 03 09
	PROTECT(directory = AS_CHARACTER(directory));pc++;
	PROTECT(origF = AS_CHARACTER(origF));pc++;//ファイル名//各列文字の処理
	
	if(mFt == 1 || mFt == 0 ){// ファイル 0 かディレクトリ 1
	  path = R_alloc(strlen(CHAR(STRING_ELT(directory, 0))), sizeof(char));//ディレクトリ名
	  strcpy(path, CHAR(STRING_ELT(directory, 0)));
	  
	  // 2011 03 10 // for(file = 0; file < f_count; file++){
		// 2011 03 10 // f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(origF, file))), sizeof(char));
	  // 2011 03 10 // }
	  for(file = 0; file < f_count; file++){
		// 2011 03 10 // strcpy(f[file], CHAR(STRING_ELT(origF, file)));
		ff.push_back(CHAR(STRING_ELT(origF, file))); 
		// 2011 03 10 // Rprintf("f[file] = %s\n", f[file]); // 2011 03 09
		// 2011 03 10 // Rprintf("ff[file] = %s\n", ff[file].c_str()); // 2011 03 09
	  }
	}
	// Rprintf("after loop: f[1] = %s\n", f[1]); // 2011 03 09	
	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3
// 	bool flag = 1;
// 	if(pos_n == 0){
// 	  pos_n = 1;
// 	  flag = 0;
// 	}
	// 2011 03 10 // char *Ppos[pos_n];
	vector <string> Ppos2; 
	SEXP myPos;  
	
	if(pos_n > 0){// if(flag){//if(pos_n > 0){}
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back (CHAR(STRING_ELT(myPos, i)) ) ;// 2011 03 10 
		//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
	  }
	}// 2005 06 23
	else{
	  // 2011 03 10 // Ppos[pos_n] = '\0';
	  myPos = NULL;  
// 	  	  strcpy(buf3 , meisiCode());
// // 	  if (strcmp(buf3, "名詞") == 0){
// // 		Rprintf("%s\n", buf3);
// // 	  }
// 	  PROTECT(myPos = allocVector(STRSXP, 1));pc++;
// 	  SET_STRING_ELT(myPos, 0, mkCharCE(buf3,  (utf8locale)?CE_UTF8:CE_NATIVE ));
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], CHAR(STRING_ELT(myPos, 0)));
	}
	
	
	// FILE *fp;

	
	// map<string, int> ma0, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	// map<string, int>::iterator pma0, pma;// マップ検索用
	// list <string> hinsi, strL, saibun;
	// list <string>::iterator hinsi_it, iter, saibun_it;

	// Rprintf("after loop2: f[1] = %s\n", f[1]); // 2011 03 09	
	
	for(file = 0; file < f_count; file++) {	
	  // Rprintf("in for loop: file = %i :f[file] = %s\n", file, f[file] ); // 2011 03 09	
	
	  if(mFt == 2){//データフレームのベクトル
		
		if( strlen(CHAR(STRING_ELT(origF, file))) < 1 || STRING_ELT(origF, file) == NA_STRING ) {
		  // 		Rprintf("in ISNA\n");		
		  continue;
		}
		//input = (char []) R_alloc(strlen(CHAR(STRING_ELT(directory, file))), sizeof(char));
		strcpy(input , CHAR(STRING_ELT(origF, file)));
		//Rprintf("to setMeCabMap\n");
		pma0 = ma0.begin();
		pma = (vecmap.at(file)).begin();// ma[file].begin();
		strL.clear();
		hinsi.clear();
		saibun.clear();
		
		//setMeCabMap(typeSet, input, ma0,  ma[file], pma0, pma, strL, iter,   hinsi, hinsi_it, saibun, saibun_it,  Ppos, pos_n, mSym,  KIGO, Ngram, genkei);
		setMeCabMap(typeSet, input, ma0,  vecmap.at(file),     pma0, pma, strL, iter,   hinsi, hinsi_it, saibun, saibun_it,  Ppos2, pos_n, Ngram, genkei, dic);

		
		////////////////////////////////////////////////
	  }else if(mFt == 0 || mFt ==1){// ファイル 0 かディレクトリ 1
	    // Rprintf("file = %i: f[file] = %s\n", file, f[file]); // 2011 03 09	
	    // sprintf(file_name, "%s/%s", path, f[file]);
	    sprintf(file_name, "%s/%s", path, ff[file].c_str());
	    // Rprintf("file_name = %s not found\n",file_name);// 2011 03 09
	    if(strcmp(file_name, "") == 0){
	      continue;
	      
	    }
		
		if((fp = fopen(file_name, "r")) == NULL){
		  Rprintf("NULL! %s not found\n",file_name);
		  UNPROTECT(pc);
		  return(R_NilValue);
		}else{
		  //strL.clear();
		  Rprintf("file_name =  %s opened\n",  file_name );	
		  while(!feof(fp)){
			//Rprintf("fgets\n");
		    if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11  if(fgets(input, 5120, fp) != NULL){

			  if(strlen(input) < 1){
				continue;
			  }
			  // Rprintf("to setMeCabMap\n");
			  pma0 = ma0.begin();
			  pma = (vecmap.at(file)).begin();// ma[file].begin();
			  strL.clear();
			  hinsi.clear();
			  saibun.clear();
			  
			  //setMeCabMap(typeSet, input, ma0,  ma[file], pma0, pma, strL, iter,  hinsi, hinsi_it, saibun, saibun_it, Ppos, pos_n, mSym, KIGO, Ngram, genkei);
			  setMeCabMap(typeSet, input, ma0,  vecmap.at(file),    pma0, pma, strL, iter,  hinsi, hinsi_it, saibun, saibun_it, Ppos2, pos_n, Ngram, genkei, dic);
			  ////////////////////////////////////////////////
			  
			}
		  }//while(feop)
		  fclose(fp);
		  
		} //else// 
		
	  }//else if(mFt == 0 || mFt ==1){// for(file);
	}//for 



	
	////////////// MeCab の処理終了


	
	// 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		//// http://www	.geocities.jp/ky_webid/cpp/library/009.html
		ma0.erase(pma0++);///// ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
	  
	n0 = (int)ma0.size();// ターム数のチェック
	  
	if(n0 > 40000){
	  Rprintf("Warning! number of extracted terms =  %d\n", n0);
	}else{
	  Rprintf("number of extracted terms = %d\n", n0);
	  Rprintf("now making a data frame. wait a while!\n");
	}
	  
	if(n0 < 1){
	  Rprintf("no terms extracted\n");
	  UNPROTECT(pc);
	  return(R_NilValue);
	}
	  


	//////////////////// データフレームの作成


		
			//		Rprintf("nn = %d\n", nn);

		if(typeSet == 0 || typeSet == 2){
		  
		  PROTECT(mydf = allocVector(VECSXP, 1 + f_count));pc++;
		  SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n0));//文字gram or 品詞gram
		  for(file = 0; file < f_count; file++){
			SET_VECTOR_ELT(mydf, file+1, allocVector(INTSXP, n0));// 頻度
	  }
		  //文字組 +  ファイル数のdata.frame   // 列数
		}else if(typeSet == 1){

		  if(NDF == 1){//名詞組を独立したデータフレーム列として返す場合
			i = Ngram + 2 + f_count;
		  	PROTECT(mydf = allocVector(VECSXP, i ));pc++;
			for(j = 0; j < i ; j++){
			  if(j < Ngram +2){
				SET_VECTOR_ELT(mydf, j, allocVector(STRSXP, n0));//単語列
			  }else{
				SET_VECTOR_ELT(mydf, j, allocVector(INTSXP, n0));// 頻度
			  }
			  
			}
		  }else{//名詞組-品詞組ｰ再分類1 +  ファイル数のdata.frame   // 列数
		  
			PROTECT(mydf = allocVector(VECSXP, 3 + f_count));pc++;//名詞組-品詞組ｰ再分類1 +  ファイル数のdata.frame   // 列数
			SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n0));//単語列
			SET_VECTOR_ELT(mydf, 1, allocVector(STRSXP, n0));//品詞列
			SET_VECTOR_ELT(mydf, 2, allocVector(STRSXP, n0));//細目列
			for(file = 0; file < f_count; file++){
			  SET_VECTOR_ELT(mydf, file+3, allocVector(INTSXP, n0));// 頻度
			}
		  }
		}
		
		//Rprintf("data frame made\n");
		///各列の代入開始
		//Rprintf("data frame made\n");
		///各列の代入開始
		if(mydf == NULL){
		  Rprintf("NULL");
		}
		

		
		if(typeSet == 0){//文字の場合
		  pma0 = ma0.begin();
		  for (xx = 0; xx < n0 && pma0 != ma0.end(); xx++) {// n0 行のタームの数だけ繰り返す
			strcpy(buf3, (pma0->first).c_str());
			//Rprintf("before column");
			//先頭列の xx 行に 文字組をセット

			
// #if defined(WIN32)		
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, CE_NATIVE ));
// #else
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, CE_UTF8 ));
// #endif

			SET_STRING_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));// < 2006 04 18>
			
			//Rprintf("column 0 is finished");
			// 各ファイルから探し出してその頻度を新規列に追加
			for(file = 0; file < f_count && pma0 != ma0.end(); file++){
			  pma = (vecmap.at(file)).begin();// ma[file].begin();
			  
			  pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
			  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){// 見つかった
				INTEGER(VECTOR_ELT(mydf, 1+file))[xx] =   pma->second;// 新規列に追加
			  }
			  else{
				INTEGER(VECTOR_ELT(mydf, 1+file))[xx] = 0;// 新規列に追加
			  }
			}
			  //Rprintf("column %d is finished", (file+1));
			
			pma0++;
			//if(xx % 10 == 0) Rprintf("* ");// 2006 03 27
			
		  } //////////////////////////////
		}else if(typeSet == 1 ){//タームの場合
		  pma0 = ma0.begin();
		  buf3[0] = '\0';
		  for (xx = 0; xx < n0; xx++) {//n0 行のタームの数だけ繰り返す
			strcpy(buf4, (pma0->first).c_str());// 最初の要素の文字列を取得し
			p = strtok(buf4 , " " );//タームの内容を Ngramずつ区切る
			//	  Rprintf("buf4 = %s - ", buf4); 
			j = 0;
			i = 1;
			//str.erase();
			while(p != NULL){// _TYPE_1 

			  if(NDF == 1 && i <= Ngram ){//タームはデータフレーム形式で
				sprintf(buf3, "%s", p);
				
// #if defined(WIN32)		  
// 				SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE  ));//j列のxx行にセット
// #elif  defined(__MINGW32__)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE ));
// #else
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8  ));
// #endif

				SET_STRING_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE  ));// < 2006 04 18>
			  
			  //Rprintf("buf3 = %s  \n", buf3); 
			  i++;
			  p = strtok( NULL, " ");
			  buf3[0] = '\0';
			  continue;
			  }
			  
			  if(  (i % Ngram)  == 0){
				//sprintf(buf3, "%s", str);
				strcat(buf3,p);
				// Rprintf("buf3 = %s  \n", buf3);

				
// #if defined(WIN32)		  
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE  ));//j列のxx行にセット
// #elif  defined(__MINGW32__)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE ));
// #else
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8  ));
// #endif

				SET_STRING_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE  ));//j列のxx行にセット < 2006 04 18>

		
			  //str.erase();
			  buf3[0] = '\0';
			  //++i;
			  }else{
				
				strcat(buf3, p);
				strcat(buf3, "-");
				//str.append(p);
				//str.append("-");
				//++i;
			  }
// // // 			  sprintf(buf3, "%s", p);// 名詞組，品詞組,細分組の取得
// // // 			  //		Rprintf("buf3 = %s\n", buf3);
// // // #if defined(WIN32)		  
// // // 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE  ));//j列のxx行にセット
// // // #elif  defined(__MINGW32__)
// // // 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE ));
// // // #else
// // // 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8  ));
// // // #endif
			  
			  p = strtok( NULL, " ");
			  ++i;
			  // if( j >= 2){
// 				continue;
// 			  }
			  

			}//////while(p != NULL) _TYPE_1 //////////////////////////////

			/////////////////////////////////////////////////	
		// 各ファイルから探し出してその頻度を新規列に追加
			for(file = 0; file < f_count && pma0 != ma0.end(); file++){
			  pma = (vecmap.at(file)).begin(); // ma[file].begin();
			  pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
			  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){// 見つかった
				if(NDF == 1){
				  INTEGER(VECTOR_ELT(mydf, Ngram+2+file))[xx] =   pma->second;// 新規列に追加
				}else{
				  INTEGER(VECTOR_ELT(mydf, 3+file))[xx] =   pma->second;//
				}
			  }
			  else{
				if(NDF == 1){
				  INTEGER(VECTOR_ELT(mydf, Ngram+2+file))[xx] = 0;// 新規列に追加
				}else{
				  INTEGER(VECTOR_ELT(mydf, 3+file))[xx] = 0;// 新規列に追加
				}
			  }
			}
			
			pma0++;
			// if(xx % 10 == 0) Rprintf("* ");// 2006 03 27 removed 2007 05
			
		  }// for (xx = 0; xx < n0; xx++) //n0 行のタームの数だけ繰り返す
		  
		}// else if(typeSet == 1 )//タームの場合
	//Rprintf("frequnecy made\n");
	//df 列ベクトルの名前を用意
	
	//  その単純な初期化
	if(typeSet == 0){//文字グラムの場合
	  PROTECT(varlabels = allocVector(STRSXP, 1+f_count)); pc++;
	  // Rprintf("col names allocated\n");

	  
// #if defined(WIN32)	  
// 	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  CE_NATIVE ));
// #else
// 	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  CE_UTF8 ));
// #endif

	  
	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  (utf8locale)?CE_UTF8:CE_NATIVE ));
	  
	  
	  //Rprintf("first col names set\n");
	  // 各ファイルあるいは行ごとの名前を設定
	  for(j = 0; j < f_count; j++){

		if(mFt == 2){//データフレームの場合
		  sprintf(buf4, "Row%d", j+1);//s

		  
// #if defined(WIN32)
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, CE_NATIVE ));
// #else		  
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, CE_UTF8 ));
// #endif

		  
		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));
		  
		  
		}else{//ファイルの場合

		  
// #if defined(WIN32)
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], CE_NATIVE ));
// #elif  defined(__MINGW32__)		  
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], CE_NATIVE ));
// #else		  
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], CE_UTF8 ));
// #endif

		  
		  SET_STRING_ELT(varlabels, j+1, mkCharCE(ff[j].c_str(), (utf8locale)?CE_UTF8:CE_NATIVE )); // 2011 03 10 SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], (utf8locale)?CE_UTF8:CE_NATIVE ));
		  
		}
	  }
	  
	}else if(typeSet == 1 ){//タームの場合
	  if(NDF == 1){
		
		PROTECT(varlabels = allocVector(STRSXP, Ngram + 2 + f_count)); pc++;
		for(i = 0; i< (Ngram +2); i++){
		  if(i < Ngram){
			sprintf(buf1, "N%d", i+1);

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  CE_UTF8 ));
// #endif

			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  (utf8locale)?CE_UTF8:CE_NATIVE ));
			
			
		  }else if (i ==  (Ngram)){
			
			
// #if defined(WIN32)			
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  CE_UTF8 ));
// #endif	

			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  (utf8locale)?CE_UTF8:CE_NATIVE ));

			
		  }else if(i ==  (Ngram +1) ){
			

// #if defined(WIN32)			
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_UTF8 ));
// #endif			
// 			//SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_NATIVE ));


			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  (utf8locale)?CE_UTF8:CE_NATIVE ));
			
		  }
		}
	  }else{// if(NDF == 1)
		
		PROTECT(varlabels = allocVector(STRSXP, 3+f_count)); pc++;
		
// #if defined(WIN32)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  CE_NATIVE ));	
// #else		  
// 		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  CE_UTF8 ));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  CE_UTF8 ));
// 		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  CE_UTF8 ));
// #endif

		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  (utf8locale)?CE_UTF8:CE_NATIVE ));
		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  (utf8locale)?CE_UTF8:CE_NATIVE ));
		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  (utf8locale)?CE_UTF8:CE_NATIVE ));

		
	  }
	  //Rprintf("col names allocated\n");

	  
	  if(mFt == 0 || mFt == 1){//  各ファイル名を列名として設定
		for(j = 0; j < f_count; j++){
		  
		  sprintf(buf4, "%s", ff[j].c_str());// 2011 03 10 sprintf(buf4, "%s", f[j]);//s
		  
		  if(NDF == 1){// Ngram 本体はいちいち単独列

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  CE_UTF8 ));
// #endif

		SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  (utf8locale)?CE_UTF8:CE_NATIVE ));

		
		  } else{// Ngram 本体は一つでまとまり


			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, 3+ j, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(varlabels,  3 +j, mkCharCE(buf4, CE_NATIVE ));
// #else			  
// 		  SET_STRING_ELT(varlabels,  3 + j, mkCharCE(buf4, CE_UTF8 ));
// #endif

			SET_STRING_ELT(varlabels, 3+ j, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));

		  
		  }
		}
		//Rprintf("file names allocated\n");
		
	  }  else if(mFt == 2){//  行番号を列名として設定
		for(j = 0; j < f_count; j++){
		  sprintf(buf4, "Row%d", j+1);//
		  if(NDF == 1){// Ngram 本体はいちいち単独列

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, CE_NATIVE ));
// #else		  
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, CE_UTF8 ));
// #endif

			
	SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));
			
		  }else{

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, CE_NATIVE ));
// #else		  
// 			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, CE_UTF8 ));
// #endif

			
			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));

			
		  }
		
		}
	  }
	}//else if(typeset ==1) //タームの場合
  
	
	Rprintf("\n");// 2006 03 27

	//Rprintf("row number n0  = %d\n", n0 );
		
	//Rprintf("after colunm names set\n" );
	///////////////////////// new_end 
	  // データフレームの行名を設定．必須
	PROTECT(row_names = allocVector(STRSXP, n0));pc++;
	//Rprintf("after row names set\n" );
	char  labelbuff[6];// char  labelbuff[5]; 2006 03
	for (xx = 0; xx < n0 ; xx++) {
	  sprintf(labelbuff, "%d", xx+1);

	  
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , CE_NATIVE));
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , CE_NATIVE));
// #else	  
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , CE_UTF8));
// #endif

	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , (utf8locale)?CE_UTF8:CE_NATIVE));
	  
	  //Rprintf("set row %d\n", xx+1 );
	}
	//Rprintf("before setAttr\n" );
	// データフレームオブジェクト mydf の属性設定
	
	// 	  	  // オブジェクトはデータフレームだと指定する
	PROTECT(tmp = mkString("data.frame")); pc++;//tmpにその属性を一時保存
	//Rprintf("data frame made");
	
	setAttrib(mydf, R_ClassSymbol, tmp);
	setAttrib(mydf, R_NamesSymbol, varlabels);
	setAttrib(mydf, R_RowNamesSymbol, row_names);
	
	//Rprintf("before UNPROTECT\n" );
	UNPROTECT(pc);
	//	Rprintf("UNPROTECT \n");
	//free(f);
	return (mydf);
	
	
	// 2006 03 05
	
	// # sym 引数は，抽出タームに句読点なので記号を含めるかを指定する．
	// ##            デフォルトでは sym = 0 とセットされており，
	// ##            記号はカウントされないが，
	// ##            sym = 1 とすると，記号を含めてカウントした結果が出力される
	// ##            pos 引数に記号が含まれた場合は自動的に sym = 1 とセットされる
	
	// 	///////////////////////////////////////////////////////
  }
}
// extern




/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/* 指定されたデータフレームを読み込んでMatrixを作成 */
extern "C" {

  
  SEXP docNgramDF(SEXP mojiVec, SEXP rowN, SEXP type, SEXP pos, SEXP posN, SEXP minFreq, SEXP N,  SEXP kigo , SEXP mydic ){// SEXP sym, SEXP kigo
	
	int file = 0,  n0 = 0, i = 0, j = 0, posC = 0, xx =0, pc = 0, *rans;/////
	unsigned int  wz = 0;
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	// const char* KIGO = CHAR(STRING_ELT(kigo,0));

	char KIGO[BUF1];
	strcpy(KIGO, kigoCode());
	
	PROTECT(rowN = AS_INTEGER(rowN));pc++;
	int row_num = INTEGER_POINTER(rowN)[0];
	PROTECT(mojiVec = AS_CHARACTER(mojiVec));pc++;

// 	for(file = 0; file < row_num; file ++){
// 	 f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(mojiVec, file))), sizeof(char));
//         }
// 	for(file = 0; file < row_num; file ++){
// 	  strcpy(f[file], CHAR(STRING_ELT(mojiVec, file)));
//        }


	int typeSet  = INTEGER_VALUE( type );// 形態素か，品詞か，文字か
	int Ngram  = INTEGER_VALUE( N );// N の数
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
	if(mFreq < 1){
	  mFreq = 1;
	}
	int mSym  = INTEGER_VALUE( kigo );//INTEGER_VALUE( sym );// 記号を含めるか 0 含めない;1 含める// 
	char * input;
	string str;
	
	mecab_t *mecab;
	mecab_node_t *node;
	
	char buf1[BUF1];//入力された語形を記憶
	char buf2[BUF3];
	char buf3[BUF1];// [512];品詞チェック
	char buf4[BUF1];// [1024];

	char *p;
	SEXP ans, dim, dimnames, row_names, col_names;
	
	
	wchar_t  wbuf [BUF4] = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	list <string> strL;
	list <string>::iterator iter;
	
	string target;
	char target2[BUF3];

	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3
	bool flag = 1;
	if(pos_n < 1 ){
	  pos_n = 2;// = 1
	  flag = 0;
	}
	
	// 2011 03 10 // char *Ppos[pos_n];
	vector <string> Ppos2;
	SEXP myPos;
	
	map<string, int> ma0;// , ma[row_num];     // ファイル数の数+登録単語用マップの数1 // 2009 04 03
	vector <map<string, int>  > vecmap;// 2011 03 09 
	for (i = 0; i < row_num; i++) vecmap.push_back(map<string, int>() );
	map<string, int>::iterator pma0, pma;// マップ検索用 // 2009 04 03


	if(pos_n > 0 && flag){
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back ( CHAR(STRING_ELT(myPos, i))); // 2011 03 10 
		//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
		if(strcmp(Ppos2[i].c_str(), KIGO) == 0){// if(strcmp(Ppos[i], KIGO) == 0){// 2011 03 10 
		  mSym = 1;
		}
	  }
	}// 2005 06 23
	else{
// 	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], meisiCode());

	  // 2011 03 10 // Ppos[0] = R_alloc(strlen( meisiCode()), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[0], meisiCode() );
	  Ppos2.push_back (meisiCode() ) ; // 2011 03 10 
	  // 2011 03 10 // Ppos[1] = R_alloc(strlen( keiyouCode() ), sizeof(char));
	  // 2011 03 10 // strcpy(Ppos[1], keiyouCode() );
	  Ppos2.push_back ( keiyouCode() ) ; // 2011 03 10 	
	}
	

	
	mecab = mecab_new2 (dic);// mecab = mecab_new2 ("MeCab");// mecab_new2 (" -u user.dic");
	CHECK(mecab);


	
	// map<string, int> ma0, ma[row_num];     // ファイル数の数+登録単語用マップの数1
	// map<string, int>::iterator pma0, pma;// マップ検索用


	for(file = 0; file < row_num; file++){
	   strL.clear(); // 2006 11 12 ;
	  //Rprintf("file = %d\n", file);		
	  if( strlen(CHAR(STRING_ELT(mojiVec, file))) < 1 || STRING_ELT(mojiVec, file) == NA_STRING ) {
		//Rprintf("in ISNA\n");	
		continue;
		// 		Rprintf("in ISNA\n");		
// 		Rprintf("something wrong\n");
// 		UNPROTECT(pc);
// 		return(R_NilValue);
	  }
	  input = R_alloc(strlen(CHAR(STRING_ELT(mojiVec, file))), sizeof(char));

	  strcpy(input , CHAR(STRING_ELT(mojiVec, file)));
	  
	  
	  
	  
	  if(typeSet == 0){// 文字単位なら
		//			  Rprintf("in typeSet == 0 %s \n",  file_name );	
		//		  Rprintf("%s\n", input);			  
		p = strchr( input, '\n' );
		/* 改行文字があった場合 */
		if ( p != NULL )
		  {
			/* 改行文字を終端文字に置き換える */
			*p = '\0';
		  }
		
		//			Rprintf("strlen of input= %d\n", strlen(input));  
		if(strlen(input) > 0){
		  //Rprintf("%s\n", input);
		  
		  //		  Rprintf("in strlen(input) > 0  %s \n",  file_name );				
		  mbstowcs(wbuf, input,  strlen(input));/* マルチバイト文字列をワイド文字列に変換*/
		  
		  //		  //for(int z = 0; z <  (wcslen(wbuf) - Ngram); z++){
		  for( wz = 0; wz <  wcslen(wbuf) ; wz++){	// 2005 07 22

// #if defined(WIN32)
// 			wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
// #elif  defined(__MINGW32__)
// 			wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
// #else
// 			sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
// #endif
		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif			
			//				Rprintf("target2 = %s\n", target2);
			if(strlen(target2) < 1){
			  break;;
			}
			//エスケープ記号類
			//strcpy(buf1, *target2);
			if( *target2 > 0x00 && *target2 < 0x21 ){//エスケープ記号類0x0e
			  continue;
			}//
			//////////// windows では wsprintf(str[ys], "%lc", wbuf[z+ys + yw]);
			
			//	if( strcmp(target2, " ") == 0 ||  strcmp(target2, "　")==0){
			if( strcmp((char *) target2, " ") == 0 ||  strcmp((char *) target2, "　")==0){
			  //				  printf("found\n");
			  continue;
			} else {// IN character
			  
			  /////////////// new_begin //////////////// ここは文字単位
			  
			  // target = target2;   
			  
			  strL.push_back( target2);
			  
			  if(strL.size() >= (unsigned int) Ngram){
				//					Rprintf("in if(strL.size) \n");					
				target.erase();
				target.append("[");
				xx = 1;
				for ( iter = strL.begin(); iter != strL.end(); iter++){
				  //						Rprintf("in for\n");
				  //						Rprintf("str %s\n", * iter);
				  target.append( *iter);
				  if(xx < Ngram){
					target.append("-");
				  }
				  xx++;
				  //					  Rprintf("xx = %d\n", xx);
				  
				}
				xx = 1;
				target.append("]");
				//					Rprintf("target %s\n", target);
				//					Rprintf("before m1.find \n");
				//出てきた形態素原型は既に全体マップにあるか？
				pma = ma0.find(target);
				//出てきた形態素原型は既にマップにあるか？
				if(pma != ma0.end()){
				  pma->second =  pma->second + 1;
				  //二つ目の数値を加算
				}					
				else{// マップにないなら，新規にマップに追加
				  //					  Rprintf("add map \n");
				  ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
				}
				// 同じ処理を個別マップにも行う
				pma = (vecmap.at(file)).find(target);// ma[file].find(target);//出てきた形態素原型は既に個別マップにあるか？
				if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){
				  pma->second =  pma->second + 1;
				  //二つ目の数値を加算
				}
				else{// マップにないなら，新規にマップに追加
				  (vecmap.at(file)).insert(make_pair(target, 1));// ma[file].insert(make_pair(target, 1));// 1 は 1個目と言う意味
				}
				
				strL.pop_front();
			  }//_if strSize>= Ngram
			}// _else_end
			
			////////////////////////////////////// new _end ////
		  }//_for2_< wcslen
		  
		}// if_strlen_>_0_end
		
		
	  } else {// if_type_set 形態素あるいは品詞の場合
		
		
			  ////////////////////////////////////////////////////////////////
			  //			Rprintf("after fgets input =  %s\n",input );
		node = (	mecab_node_t * ) mecab_sparse_tonode(mecab, input);
		CHECK(node);
		//			Rprintf("node check" );		
		/// 解析結果のノードをなめる
		for (;  node; node = node->next) {// node とはその文の形態素ごとに設定される
		  //			  		printf("%d ", node->id);
		
		  if (node->stat == MECAB_BOS_NODE)
			//printf("BOS");
			continue;
		  else if (node->stat == MECAB_EOS_NODE)
			//printf("EOS");
			continue;
		  else {// BOS, EOS 以外
			// 2010buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
			strncpy(buf1, node->surface, node->length) ;//元の語形

			buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			
			if(strlen(buf1) < 1){// 2006 06 移動
			  continue;
			}

			
			//< 2005 11 07> //Rprintf("%s\n", buf1);			
			//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){// if( atoi(buf1)  == 0x0e){//エスケープ記号類
			if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類
			  continue;
			}// </ 2005 11 07>
			
// 			buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
			
// 			if(strlen(buf1) < 1){// 2006 06 移動
// 			  continue;
// 			}
			//				Rprintf("buf1 = %s\n", buf1);
			
			strcpy(buf2, node->feature);//ノードごとに解析情報の取得.要素数は 9
			if(strlen(buf2) < 1){
			  continue;
			}
			//				Rprintf("buf2 = %s\n", buf2);
			
			//////////////
			
			
			p = strtok(buf2, "," );//取得情報の分割
		  // 品詞の判定
			j = 1;
			////////////////////////////////////////////////////////////////////
			
			
			if(typeSet == 2){// 品詞情報で数える
			  if( j == 1 && p != NULL ){//品詞情報1
				strL.push_back(p);
				//						Rprintf("typeSet == = %d; p = %s\n", typeSet, p);					 
				p = NULL;
			  }
			  
			  
			  
			}else if(typeSet == 1){// 形態素原形で数える
			  
			  //////////////////////////////////////////////
			  
			  
			  if(j == 1 &&  p != NULL){
				sprintf(buf3, "%s", p);
				// if(mSym < 1 && strcmp(buf3, "記号") == 0){
				if(mSym < 1 && strcmp(buf3, KIGO) == 0){						
				  p = NULL;
				  //j = 9;
				  continue;// 記号は一切省き，総計にも加えない
				}
				//				  Rprintf("buf3 %s\n", buf3);
				for(i = 0; i < pos_n; i++){
				  sprintf(buf4, "%s", Ppos2[i].c_str());// 2011 03 10 sprintf(buf4, "%s", Ppos[i]);				
				  //					Rprintf("buf4 %s\n", buf4);
				  if(strcmp(buf3, buf4) == 0){
					posC = 1;
					break;
				  }
				}
				if(posC != 1){
				  p = NULL;
				  posC = 0;
				  continue;
				}
			  }
			  
			  
			  while ( p != NULL ) {
				//				  Rprintf("in p!=NULL j = %d p = %s\n",j, p);
				
			  
				
				// if(j == 1){//品詞情報1
				// 					str = p;
				// 					// str.append(",");
					  // 				  }else
				
				if( j == 7){
				  if(p == NULL || strcmp(p, "*") == 0){
					// strL.push_back(p);//原型str = buf1;// str.append(buf1);//元の語形
					strL.push_back(buf1);//元の語形
					//Rprintf("in str = buf1\n");
				  }
				  else{
					strL.push_back(p);//原型 strL.push_back(buf1);
					//Rprintf("in str = p\n");
				  }
				}
				p = strtok( NULL,"," );
				j++;
				if(j > 7){
				  p = NULL;
				}
				
			  
			  }// while(P!= NULL)
			  posC = 0;
			} // else if typset = 1
			
			
		  }  //////else // BOS, EOS 以外
		  
		  ////////////// 抽出終了
		  if(strL.size() >= (unsigned int) Ngram){// リストのサイズが指定通りであるなら，保存を始める
			//				  Rprintf("type = %d, strL size =  %d\n", typeSet, strL.size() );
			target.erase();//保存のための文字列を初期化
			target.append("[");
			xx = 1;
			for ( iter = strL.begin(); iter != strL.end(); iter++){
			  // Rprintf("in for\n");
			  //sprintf(buf3, "%s", *iter);
			  //Rprintf("str %s\n", *iter);
			  //Rprintf("after Rprintf in for\n");
			  target.append( *iter);// target.append( buf3); //target.append( *iter);
			  //					Rprintf("target append\n");
			  if(xx < Ngram){
				target.append("-");
			  }
			  xx++;
			} // for 
			xx = 1;
			target.append("]");	
			pma0 = ma0.find(target);//出てきた形態素原型は既に全体マップにあるか？
			if(pma0 != ma0.end()){
			  pma0->second =  pma0->second + 1;
			  //二つ目の数値を加算
			}
			else{// マップにないなら，新規にマップに追加
			  ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
			}
			
			pma = (vecmap.at(file)).find(target ); // ma[file].find(target );// str 2006 08 25 出てきた形態素原型は既に個別マップにあるか？
			 if(pma != (vecmap.at(file)).end()){// if(pma != ma[file].end()){
			pma->second =  pma->second + 1;
			//二つ目の数値を加算
		  }
		  else{// マップにないなら，新規にマップに追加
		    (vecmap.at(file)).insert(make_pair(target, 1));// ma[file].insert(make_pair(target, 1));// 1 は 1個目と言う意味
		  }
		  
		  strL.pop_front();// 最初の要素を取り除く
		  }				  // if(strL.size() >= Ngram)
		  
		}//for(;node;)// Rprintf("node check ended\n");
		
	  }
	  // if(file % 10 == 0) Rprintf("* ");// 2006 03 27 removed 2007 05
	 
	}//for (file++)

	Rprintf("\n");// 2006 03 27
		
	mecab_destroy(mecab);
	  //Rprintf("mecab destroyed\n");
	  
	  // 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		//// http://www	.geocities.jp/ky_webid/cpp/library/009.html
		ma0.erase(pma0++);/////	ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
	
	n0 = (int)ma0.size();// ターム数のチェック
	
	if(n0 > 40000){
	  Rprintf("Warning! number of extracted terms =  %d\n", n0);
	}else{
	  Rprintf("number of extracted terms = %d\n", n0);
	}
	
	if(n0 < 1){
	  Rprintf("no terms extracted\n");
	  UNPROTECT(pc);
	  return(R_NilValue);
	}
	
	
	PROTECT (ans = allocMatrix(INTSXP, row_num, n0));pc++;//docNgram2
	rans = INTEGER(ans);
	

	// 各ファイルから取得したマップを，全ファイルから取得したマップ ma[0]と照合して
	// 行列に登録する．
	// 方法としては ma[0] の要素ごとに，各マップに登録があるか確認し，あれば，その頻度を，なければ 0 を代入する
	for(file = 0; file < row_num ; file++){// ファイルの数だけ繰り返す
	  //Rprintf("ma0 and ma  check\n");
	  pma0 = ma0.begin();
	  pma = (vecmap.at(file)).begin();// ma[file].begin();
	  j = 0;// Term 数のチェック
	  while( j < n0 && pma0 != ma0.end()  ){
	    pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
	    if(pma != (vecmap.at(file)).end() ) { // if(pma != ma[file].end()){// 見つかった
		  rans[ file  + row_num * j] = pma->second;
		}
		else{
		  rans[file + row_num * j  ] = 0;		  
		}
		pma0++;
		j++;
	  }
	  
	}
	
    PROTECT(dim = allocVector(INTSXP, 2));pc++;
    INTEGER(dim)[0] = row_num; INTEGER(dim)[1] = n0;
    setAttrib(ans, R_DimSymbol, dim);
   
	Rprintf("to make matrix now\n");
	// 列名のセット；これはターム
	PROTECT(col_names = allocVector(STRSXP, n0));pc++;
	pma0 = ma0.begin();
	for(i = 0; i < n0 && pma0 != ma0.end() ; i++){
	  strcpy(buf3, (pma0->first).c_str());
	  //Rprintf("col names = %s\n", (pma0->first).c_str());
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , CE_NATIVE));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , CE_NATIVE));			 	
// #else
// 	  SET_STRING_ELT(col_names, i, mkCharCE(buf3, CE_UTF8 ));			 	
// #endif	  
// 	  // SET_STRING_ELT(col_names, i, mkChar(buf3 ));
//   // </ 2005 11 08>

	  SET_STRING_ELT(col_names, i, mkCharCE(buf3 , (utf8locale)?CE_UTF8:CE_NATIVE));		
	  
	  pma0++;
	}
	

	// 行名のセット// これはファイル名
	// pv = v.begin();
	PROTECT(row_names = allocVector(STRSXP, row_num));pc++;
	for(file = 0; file < row_num;file++){ 
	  //	  Rprintf("row names = %s\n", f[file]);
	  // strcpy(buf3, f[file]);// strcpy(buf3, *pv);
	  sprintf(buf3, "Row%d", file+1);//
  
//  // < 2005 11 08>
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_NATIVE ));			 	
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_NATIVE ));			 	
// #else
// 	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, CE_UTF8 ));			 	
// #endif
// 	  // SET_STRING_ELT(row_names, file, mkChar( buf3 ));
//   // </ 2005 11 08>

	  SET_STRING_ELT(row_names, file, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));	
	  
	}

	// 	free(f);
	// 	Rprintf("free \n");


	//	Rprintf("set dimnames \n");
	PROTECT(dimnames = allocVector(VECSXP, 2));pc++;
	SET_VECTOR_ELT(dimnames, 0, row_names); // 行の名前をセット
	SET_VECTOR_ELT(dimnames, 1, col_names);//列名をセット
	setAttrib(ans, R_DimNamesSymbol, dimnames);

	UNPROTECT(pc);
	//	Rprintf("UNPROTECT \n");
	//free(f);
	return (ans);
	
	// 2005 06 17 
	///////////////////////////////////////////////////////
  }
} // extern


/////////////////////////////////////////////////////////////

/* START2008Y-3
   
  ver 0.9998 2014 12 03
  
  ティロ・フィナーレ
  
 */
