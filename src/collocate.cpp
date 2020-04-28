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

///////////////////////////////////////////////////////////////


extern "C"{

  SEXP collocate(SEXP filename, SEXP target, SEXP span, SEXP mydic ){
	int spa  = INTEGER_VALUE(span );// 最小頻度の数
	//Rprintf("span = %d\n", spa);
	const char* file = CHAR(STRING_ELT(filename,0));
	const char* word = CHAR(STRING_ELT(target,0));
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	char  input[BUF4] ;//= { 0 };//	char  input[5120] = { 0 };
	memset (input, 0, sizeof input) ; // 2015 12 18
	
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
					  m0.insert(pair<char*,int>(buf1, 1));// 1 は 1個目と言う意味
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

