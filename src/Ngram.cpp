/*   ver 1.08 2022 04 30 */

#include "RMeCab.h"

//////////////////////////////////////////////////////

extern "C" {
  SEXP Ngram(SEXP filename, SEXP type, SEXP N, SEXP pos, SEXP posN  , SEXP mydic ){
	const char* file = CHAR(STRING_ELT(filename,0));
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int typeSet  = INTEGER_VALUE( type );// pos の数
	int Ngram  = INTEGER_VALUE( N );// 最小頻度の数
	int mFreq = 2;
	char  input[BUF4];// = { 0 };//char  input[5120] = { 0 };
	memset (input, 0, sizeof input); // 2015 12 18
	//	char  input2[5120] = { 0 };
	
	wchar_t  wbuf [BUF4] ;// = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
	memset (wbuf, 0, sizeof wbuf);// 2015 12 18
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
			    /* 2022 04 30
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) // 2022 04 30
 		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
 		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
 		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif // 2022 04 30
			    */
			    sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
	
				
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

	  
	  if(n > OVERLINE ){ // 40000 -> OVERLINE  // 2016 12 27
		Rprintf("Ngram exceeds 99999 \n"); // 40000 -> 99999  // 2016 12 27
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

		  SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str(), CE_UTF8)); // (utf8locale)?CE_UTF8:CE_NATIVE ));			  ///<2006 04 18>
		  
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
		

		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_UTF8)); // (utf8locale)?CE_UTF8:CE_NATIVE));
		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_UTF8)); // (utf8locale)?CE_UTF8:CE_NATIVE));		

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

		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));
		  
		
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


		  SET_STRING_ELT(VECTOR_ELT(mydf, 0), z, mkCharCE( (pa->first).c_str() , CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));///<2006 04 18>
		  
		  
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


		SET_STRING_ELT(varlabels, 0, mkCharCE("Ngram", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));
		SET_STRING_ELT(varlabels, 1, mkCharCE("Freq", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));

		
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
		  
		  SET_STRING_ELT(row_names, z, mkCharCE(labelbuff, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));

		
		}
		setAttrib(mydf, R_RowNamesSymbol, row_names);
		
		UNPROTECT(pc);
		
	  }// else_n_end
	  return (mydf);
	  
	}//_else
  }//_SEXP_Ngram
}// Extern


