/*
  ver 0.99995 2016 12 27

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
/* 文書行列作成関数 文字列と解析方法が引数 */
extern "C" {
  // 2005 03 28 第三引数 (入力文の長さ) を削った
  SEXP  RMeCabDoc(SEXP filename, SEXP mypref, SEXP kigo, SEXP mydic ){ 

	const char* input = CHAR(STRING_ELT(filename,0));//解析対象ファイル名
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	int sym = INTEGER_VALUE(kigo);// 記号を含むか含まない
		// const char* KIGO = CHAR(STRING_ELT(kigo,0));
	char KIGO[BUF1]  = {0};
	strcpy(KIGO, kigoCode());
	
// 	if(sym == 1 ){// 記号を含むので，「記号」をセットする
// 	  strcpy(KIGO, kigoCode());
// 	}

	char input2[BUF4] = {0};
	mecab_t *mecab;
	mecab_node_t *node;
	int pref = INTEGER_VALUE(mypref);// もとの形 0 か原型 1 か
	char buf1[BUF1] = {0};// [512];//入力された語形を記憶
	char buf2[BUF3] = {0};
	char buf3[BUF2] = {0};// [256]; 記号チェック用
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
				
				if(xlength(my_list) <= (R_xlen_t) i ) {  // 2015 12 28 if(length(my_list) <= i){
				  UNPROTECT(1);
				  PROTECT(my_list = lengthgets(my_list, i+1000));// pa++;  1
				}
				
				
				p = NULL;
				
				i++;
				if(i > OVERLINE ){// 40000 -> OVERLINE  // 2016 12 27
				  Rprintf("node numbers i == %d stop\n", i);
				  fclose(fp);
				  UNPROTECT(1);
				  PROTECT(my_list = lengthgets(my_list, 0));
				  UNPROTECT(1);
				  return (R_NilValue); 
				}
			  }
			}
			
			memset(buf1,0,strlen(buf1));// 2017 08 04 
			memset(buf2,0,strlen(buf2));// 2017 08 04 
		  }//_end_for_
		}//_fgets_end
	  }// _while_end
	  UNPROTECT(1);                                            // pa--;0
	  if(xlength(my_list) > (R_xlen_t) i ) { //2015 12 18 if(length(my_list) > i){
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

















