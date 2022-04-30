
#include "RMeCab.h"


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
		// とりあえず空の要素を5000個持つリストを生成する
	  PROTECT(my_list = allocVector(VECSXP, 5000));  pa++; // pa++ 1; // 2019 06 10 1000 -> 5000
// Rprintf("allocVector\n");
	  while(!feof(fp)){
	    if(fgets(input2, FILEINPUT, fp) != NULL){// 2011 03 11 if(fgets(input2, 5120, fp) != NULL){
		  node = ( mecab_node_t * ) mecab_sparse_tonode(mecab, input2);
		  CHECK(node);
		  ////////// PROTECT(my_char=allocVector(STRSXP,15));
		  
				/// 解析結果のノードをなめる
		  for (;  node; node = node->next) {
            // printf("%d ", node->id);
			
			if (node->stat == MECAB_BOS_NODE)
			  //printf("BOS");
			  continue;
			else if (node->stat == MECAB_EOS_NODE)
			  //printf("EOS");
			  continue;
			else {
			  // 2010  buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);
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
 // Rprintf("allocVector2\n");
 //Rprintf("%s\n", my_char);			  

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

			  SET_STRING_ELT(  my_char  ,  0, mkCharCE( buf1 , CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE  ));

			  
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

				SET_STRING_ELT( my_char,  j, mkCharCE( p, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE  )); 
				
				p = strtok( NULL,"," );
				j++;
			  }
			  SET_VECTOR_ELT(my_list, i, my_char);
			  UNPROTECT(1);    pa--;       //pa-- 1;
			  i++;
			  if(i + 5000  > OVERLINE){// リストが10万(四万)を越えるならエラーを起こしやすい// 2019 06 10 1000 -> 5000
				Rprintf("i == %d stop\n", i);
				fclose(fp);mecab_destroy(mecab);
				//	my_list = my_char =  NULL;p = NULL;
				UNPROTECT(pa);
                return(my_list);
				/*
				UNPROTECT(1);pa--;
				PROTECT(my_list = lengthgets(my_list, 0));pa++;
				UNPROTECT(1);pa--;
				*/
				// return (R_NilValue); 
			  }
			  
			  

			  // もしもリストの空きがなくなっているなら，5000個新規スペースを足す
			  if(xlength (my_list) <=  (R_xlen_t) i) { // 2015 12 18 // if(length(my_list) <= i){
                             
				UNPROTECT(1);  pa--;   // pa--; 0
				PROTECT(my_list = lengthgets(my_list, i+5000)); pa++;// pa++ 1;// 2019 06 10 1000->5000
			  }
			  
			  // i++;; 2010 08 23
			}
			// memset(buf1,'\0',strlen(buf1));
			memset(buf1,0,strlen(buf1));
			memset(buf2,0,strlen(buf2));		
		  }// for
		}//if
	  }// while(!feof(fp));//while
	   // Rprintf("close fike\n");        
	  fclose(fp);
      //	  mecab_destroy(mecab);
	  //  UNPROTECT(2);
		// リスト末尾に空きがあるなら取り除く
	  UNPROTECT(1);  pa--;                  // pa-- 0;
	  if(xlength (my_list) >  (R_xlen_t) i) { // 2015 12 18 if(length(my_list) > i){
		PROTECT(my_list = lengthgets(my_list, i)); pa++; // pa++ 1;
	  }

      mecab_destroy(mecab);
	  UNPROTECT(pa); // UNPROTECT(1);                                  // pa-- 1;
	  return(my_list);
	  
	}//_else_end
	return(R_NilValue);//return(0);
}
}//_end_extern





