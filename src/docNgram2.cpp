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
#include "RMeCab.h"

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
	
	
	wchar_t  wbuf [BUF4] ;// = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	memset (wbuf, 0, sizeof wbuf); // 2015 12 18
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

