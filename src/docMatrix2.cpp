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
		ma0.erase(pma0++);/////ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
			  
	
	n0 = (int)ma0.size();// ターム数のチェック
			
	if(n0 > OVERLINE ){ // 40000 -> OVERLINE  // 2016 12 27
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









