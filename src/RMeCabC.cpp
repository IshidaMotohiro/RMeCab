/*


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
	char buf1[BUF1] = {0};//入力された語形を記憶
	char buf2[BUF3] = {0};
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
		  if(i + 1000  > OVERLINE){// リストが20万（四万）を越えるならエラーを起こしやすい
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
		  
		  if(xlength(my_list) <= (R_xlen_t) i ) { // 2015 12 28 if(length(my_list) <= i){
			 UNPROTECT(1);pa--;
			PROTECT(my_list = lengthgets(my_list, i+1000));pa++;// pa++;  1
		  }
		  p = NULL;
		  
		  // i++; 2010 08 23
		  
		}
		//memset(buf1,'\0',strlen(buf1));
		memset(buf1,0,strlen(buf1));
		memset(buf2,0,strlen(buf2));
	}

	mecab_destroy(mecab);
		
	UNPROTECT(1);  pa--;                                // pa--;0
       if(xlength(my_list) >  (R_xlen_t) i){//2015 12 18 //  if(length(my_list) > i){
		PROTECT(my_list = lengthgets(my_list, i));pa++;// pa++;1
	}
	UNPROTECT(pa); //   UNPROTECT(1);   // pa--;                               // pa--;0

	return(my_list); 
}
}//_end_extern

