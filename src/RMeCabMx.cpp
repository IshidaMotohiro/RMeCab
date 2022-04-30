
#include "RMeCab.h"


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
		  memset(buf1,0,strlen(buf1));// 2017 08 04 
		  memset(buf2,0,strlen(buf2));	// 2017 08 04 	
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

	SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));   
	  
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

	  SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE ));
	  
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

		SET_STRING_ELT(vecName, zz, mkCharCE((pa->first).c_str(), CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE ));
		
		
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

	  SET_STRING_ELT(vecName, 0, mkCharCE(buf3, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE)); 
	  
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

	  	SET_STRING_ELT(vecName, 1, mkCharCE( "[[TOTAL-TOKENS]]", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE ));	 	
	  
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

	  SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));	

	  
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

	   SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE  )); 	
	  
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

	  SET_STRING_ELT(vecName, zz-1 , mkCharCE(buf3, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));	
	  
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

	  SET_STRING_ELT(vecName, zz , mkCharCE(  "[[TOTAL-TOKENS]]", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE  ));	 	
	  
	  
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
	  
	  SET_STRING_ELT(vecName, zz  , mkCharCE(buf3, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));	 	
	  
	  
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

	  SET_STRING_ELT(vecName, zz+1  , mkCharCE(  "[[TOTAL-TOKENS]]", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE  ));  

	  
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

	  
	  SET_STRING_ELT(vecName, zz, mkCharCE(buf3, CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE));
	  
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

	 SET_STRING_ELT(vecName, zz+1, mkCharCE( "[[TOTAL-TOKENS]]", CE_UTF8));// (utf8locale)?CE_UTF8:CE_NATIVE )); 		 

	 
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



