
#include "RMeCab.h"


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
				str.append("\a");
			  }
			  else if(j == 2){//品詞情報2
				str.append(p);
				str.append("\a");
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
		  memset(buf1,0,strlen(buf1));// 2017 08 04 
		  memset(buf2,0,strlen(buf2));// 2017 08 04 		
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
	  token = strtok(s, "\a");// 最初の要素は品詞情報1

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
	  
	  token = strtok(NULL, "\a");// 次の要素は品詞情報2

  
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
	  


	  
	  token = strtok( NULL, "\a");// 次が形態素原型

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







