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
