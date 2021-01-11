/*
  ver 1.00 2021 01 11

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
#include "setMeCabMap.h"

//////////////////////////////////////////////////////
/* 指定されたターゲットからテキストを読み込んでNgramを作成 */
extern "C" {
  SEXP docDF(SEXP directory,
			 SEXP origF,
			 SEXP fileN,
			 SEXP ft,
			 SEXP type,
			 SEXP pos,
			 SEXP posN,
			 SEXP minFreq,
			 SEXP N,
// 			 SEXP sym,
// 			 SEXP kigo,
			 SEXP Genkei,
			 SEXP nDF,
			 SEXP mydic  ){
    // Rprintf("BUF1  =  %i\n", BUF1); // 
    // Rprintf("BUF2  =  %i\n", BUF2); // 
    // Rprintf("BUF3  =  %i\n", BUF3); //
    // Rprintf("BUF4  =  %i\n", BUF4); //
    // Rprintf("FILEINPUT  =  %i\n", FILEINPUT); //
    
	int file = 0,  n0 = 0, i = 0, j = 0, pc = 0, xx = 1;
	const char* dic = CHAR(STRING_ELT(mydic, 0));//指定辞書
	
	int f_count = INTEGER_VALUE( fileN );//ファイル(行)数
	char* path = 0;// 2011 03 11  char* path;
	// 2011 03 10 //  char * f[f_count];
	vector <string> ff;
	//const char* KIGO = CHAR(STRING_ELT(kigo,0));

	int typeSet  = INTEGER_VALUE( type );// 形態素か，品詞か，文字か
	int Ngram  = INTEGER_VALUE( N );// N の数
	int mFreq  = INTEGER_VALUE( minFreq );// 最小頻度の数
	if(mFreq < 1){
	  mFreq = 1;
	}
	//int mSym  = INTEGER_VALUE( sym );// 記号を含めるか 0 含めない;1 含める
	int NDF  = INTEGER_VALUE( nDF );// データフレームの列数
		
	int genkei  = INTEGER_VALUE( Genkei );// 活用は原型か 0 表層形か 1 
	char file_name[FILEN];
	char  input[BUF4];
	char * p;
	string str;
	
	char buf1[BUF1];// [512];//入力された語形を記憶
	//	char buf2[1024];
	char buf3[BUF1];// [512];品詞チェック用
	char buf4[BUF1];// [1024];品詞チェック用

	SEXP tmp, row_names, mydf = R_NilValue, varlabels = R_NilValue;// 2011 03 11 //  SEXP mydf, tmp, row_names, varlabels;// SEXP ans, dim, dimnames, row_names, col_names;
	
			
	int mFt  = INTEGER_VALUE( ft );// ファイル 0 かディレクトリ 1 かデータフレーム列か2
	/////
		
	FILE *fp;// 2009 04 03	
	map<string, int> ma0;//, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	vector <map<string, int> > vecmap;// 2011 03 09 
	for (i = 0; i < f_count; i++) vecmap.push_back(map<string, int>() );
	map<string, int>::iterator pma0, pma;// マップ検索用
	list <string> hinsi, strL, saibun;
	list <string>::iterator hinsi_it, iter, saibun_it;// 2009 04 03

	/////
	// Rprintf("f_file  =  %i\n", f_count); // 2011 03 09
	PROTECT(directory = AS_CHARACTER(directory));pc++;
	PROTECT(origF = AS_CHARACTER(origF));pc++;//ファイル名//各列文字の処理
	
	if(mFt == 1 || mFt == 0 ){// ファイル 0 かディレクトリ 1
	  path = R_alloc(strlen(CHAR(STRING_ELT(directory, 0))), sizeof(char));//ディレクトリ名
	  strcpy(path, CHAR(STRING_ELT(directory, 0)));
	  
	  // 2011 03 10 // for(file = 0; file < f_count; file++){
		// 2011 03 10 // f[file]  =  R_alloc(strlen(CHAR(STRING_ELT(origF, file))), sizeof(char));
	  // 2011 03 10 // }
	  for(file = 0; file < f_count; file++){
		// 2011 03 10 // strcpy(f[file], CHAR(STRING_ELT(origF, file)));
		ff.push_back(CHAR(STRING_ELT(origF, file))); 
		// 2011 03 10 // Rprintf("f[file] = %s\n", f[file]); // 2011 03 09
		// 2011 03 10 // Rprintf("ff[file] = %s\n", ff[file].c_str()); // 2011 03 09
	  }
	}
	// Rprintf("after loop: f[1] = %s\n", f[1]); // 2011 03 09	
	int pos_n  = INTEGER_VALUE( posN );// pos の数 // 2005 06 3
// 	bool flag = 1;
// 	if(pos_n == 0){
// 	  pos_n = 1;
// 	  flag = 0;
// 	}
	// 2011 03 10 // char *Ppos[pos_n];
	vector <string> Ppos2; 
	SEXP myPos;  
	
	if(pos_n > 0){// if(flag){//if(pos_n > 0){}
	  PROTECT(myPos = AS_CHARACTER(pos));pc++;
	  // 2011 03 10 // for( i = 0; i < pos_n; i++){
		// 2011 03 10 // Ppos[i] = R_alloc(strlen(CHAR(STRING_ELT(myPos, i))), sizeof(char));
	  // 2011 03 10 // }
	  //   Rprintf("end myPos = AS_CHARACTER(pos) \n");
	  for( i = 0; i < pos_n; i++){
		// 2011 03 10 // strcpy(Ppos[i], CHAR(STRING_ELT(myPos, i)));
		Ppos2.push_back (CHAR(STRING_ELT(myPos, i)) ) ;// 2011 03 10 
		//	Rprintf("Pos[%d] =  %s\n", i, Ppos[i]);
	  }
	}// 2005 06 23
	else{
	  // 2011 03 10 // Ppos[pos_n] = '\0';
	  myPos = NULL;  
// 	  	  strcpy(buf3 , meisiCode());
// // 	  if (strcmp(buf3, "名詞") == 0){
// // 		Rprintf("%s\n", buf3);
// // 	  }
// 	  PROTECT(myPos = allocVector(STRSXP, 1));pc++;
// 	  SET_STRING_ELT(myPos, 0, mkCharCE(buf3,  (utf8locale)?CE_UTF8:CE_NATIVE ));
// 	  Ppos[0] = R_alloc(strlen(CHAR(STRING_ELT(myPos, 0))), sizeof(char));
// 	  strcpy(Ppos[0], CHAR(STRING_ELT(myPos, 0)));
	}
	
	
	// FILE *fp;

	
	// map<string, int> ma0, ma[f_count];     // ファイル数の数+登録単語用マップの数1
	// map<string, int>::iterator pma0, pma;// マップ検索用
	// list <string> hinsi, strL, saibun;
	// list <string>::iterator hinsi_it, iter, saibun_it;

	// Rprintf("after loop2: f[1] = %s\n", f[1]); // 2011 03 09	
	
	for(file = 0; file < f_count; file++) {	
	  // Rprintf("in for loop: file = %i :f[file] = %s\n", file, f[file] ); // 2011 03 09	
	
	  if(mFt == 2){//データフレームのベクトル
		
		if( strlen(CHAR(STRING_ELT(origF, file))) < 1 || STRING_ELT(origF, file) == NA_STRING ) {
		  // 		Rprintf("in ISNA\n");		
		  continue;
		}
		//input = (char []) R_alloc(strlen(CHAR(STRING_ELT(directory, file))), sizeof(char));
		strcpy(input , CHAR(STRING_ELT(origF, file)));
		//Rprintf("to setMeCabMap\n");
		pma0 = ma0.begin();
		pma = (vecmap.at(file)).begin();// ma[file].begin();
		strL.clear();
		hinsi.clear();
		saibun.clear();
		
		//setMeCabMap(typeSet, input, ma0,  ma[file], pma0, pma, strL, iter,   hinsi, hinsi_it, saibun, saibun_it,  Ppos, pos_n, mSym,  KIGO, Ngram, genkei);
		setMeCabMap(typeSet, input, ma0,  vecmap.at(file),     pma0, pma, strL, iter,   hinsi, hinsi_it, saibun, saibun_it,  Ppos2, pos_n, Ngram, genkei, dic);

		
		////////////////////////////////////////////////
	  }else if(mFt == 0 || mFt ==1){// ファイル 0 かディレクトリ 1
	    // Rprintf("file = %i: f[file] = %s\n", file, f[file]); // 2011 03 09	
	    // sprintf(file_name, "%s/%s", path, f[file]);
	    sprintf(file_name, "%s/%s", path, ff[file].c_str());
	    // Rprintf("file_name = %s not found\n",file_name);// 2011 03 09
	    if(strcmp(file_name, "") == 0){
	      continue;
	      
	    }
		
		if((fp = fopen(file_name, "r")) == NULL){
		  Rprintf("NULL! %s not found\n",file_name);
		  UNPROTECT(pc);
		  return(R_NilValue);
		}else{
		  //strL.clear();
		  Rprintf("file_name =  %s opened\n",  file_name );	
		  while(!feof(fp)){
			//Rprintf("fgets\n");
		    if(fgets(input, FILEINPUT, fp) != NULL){// 2011 03 11  if(fgets(input, 5120, fp) != NULL){

			  if(strlen(input) < 1){
				continue;
			  }
			  // Rprintf("to setMeCabMap\n");
			  pma0 = ma0.begin();
			  pma = (vecmap.at(file)).begin();// ma[file].begin();
			  strL.clear();
			  hinsi.clear();
			  saibun.clear();
			  
			  //setMeCabMap(typeSet, input, ma0,  ma[file], pma0, pma, strL, iter,  hinsi, hinsi_it, saibun, saibun_it, Ppos, pos_n, mSym, KIGO, Ngram, genkei);
			  setMeCabMap(typeSet, input, ma0,  vecmap.at(file),    pma0, pma, strL, iter,  hinsi, hinsi_it, saibun, saibun_it, Ppos2, pos_n, Ngram, genkei, dic);
			  ////////////////////////////////////////////////
			  
			}
		  }//while(feop)
		  fclose(fp);
		  
		} //else// 
		
	  }//else if(mFt == 0 || mFt ==1){// for(file);
	}//for 



	
	////////////// MeCab の処理終了


	
	// 最低頻度のチェック
	pma0 = ma0.begin();
	while( pma0 != ma0.end()  ){
	  if(pma0->second < mFreq){
		ma0.erase(pma0++);///// ma0.erase(pma0);// 2007 09 15 // ma0.erase(pma0++);
	  }else{
		++pma0;
	  }
	}
	  
	n0 = (int)ma0.size();// ターム数のチェック
	  
	if(n0 > OVERLINE ){ // 40000 -> OVERLINE  // 2016 12 27
	  Rprintf("Warning! number of extracted terms =  %d\n", n0);
	}else{
	  Rprintf("number of extracted terms = %d\n", n0);
	  Rprintf("now making a data frame. wait a while!\n");
	}
	  
	if(n0 < 1){
	  Rprintf("no terms extracted\n");
	  UNPROTECT(pc);
	  return(R_NilValue);
	}
	  


	//////////////////// データフレームの作成


		
			//		Rprintf("nn = %d\n", nn);

		if(typeSet == 0 || typeSet == 2){
		  
		  PROTECT(mydf = allocVector(VECSXP, 1 + f_count));pc++;
		  SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n0));//文字gram or 品詞gram
		  for(file = 0; file < f_count; file++){
			SET_VECTOR_ELT(mydf, file+1, allocVector(INTSXP, n0));// 頻度
	  }
		  //文字組 +  ファイル数のdata.frame   // 列数
		}else if(typeSet == 1){

		  if(NDF == 1){//名詞組を独立したデータフレーム列として返す場合
			i = Ngram + 2 + f_count;
		  	PROTECT(mydf = allocVector(VECSXP, i ));pc++;
			for(j = 0; j < i ; j++){
			  if(j < Ngram +2){
				SET_VECTOR_ELT(mydf, j, allocVector(STRSXP, n0));//単語列
			  }else{
				SET_VECTOR_ELT(mydf, j, allocVector(INTSXP, n0));// 頻度
			  }
			  
			}
		  }else{//名詞組-品詞組ｰ再分類1 +  ファイル数のdata.frame   // 列数
		  
			PROTECT(mydf = allocVector(VECSXP, 3 + f_count));pc++;//名詞組-品詞組ｰ再分類1 +  ファイル数のdata.frame   // 列数
			SET_VECTOR_ELT(mydf, 0, allocVector(STRSXP, n0));//単語列
			SET_VECTOR_ELT(mydf, 1, allocVector(STRSXP, n0));//品詞列
			SET_VECTOR_ELT(mydf, 2, allocVector(STRSXP, n0));//細目列
			for(file = 0; file < f_count; file++){
			  SET_VECTOR_ELT(mydf, file+3, allocVector(INTSXP, n0));// 頻度
			}
		  }
		}
		
		//Rprintf("data frame made\n");
		///各列の代入開始
		//Rprintf("data frame made\n");
		///各列の代入開始
		if(mydf == NULL){
		  Rprintf("NULL");
		}
		

		
		if(typeSet == 0){//文字の場合
		  pma0 = ma0.begin();
		  for (xx = 0; xx < n0 && pma0 != ma0.end(); xx++) {// n0 行のタームの数だけ繰り返す
			strcpy(buf3, (pma0->first).c_str());
			//Rprintf("before column");
			//先頭列の xx 行に 文字組をセット

			
// #if defined(WIN32)		
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, CE_NATIVE ));
// #else
// 			SET_VECTOR_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, CE_UTF8 ));
// #endif

			SET_STRING_ELT(VECTOR_ELT(mydf, 0), xx, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE ));// < 2006 04 18>
			
			//Rprintf("column 0 is finished");
			// 各ファイルから探し出してその頻度を新規列に追加
			for(file = 0; file < f_count && pma0 != ma0.end(); file++){
			  pma = (vecmap.at(file)).begin();// ma[file].begin();
			  
			  pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
			  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){// 見つかった
				INTEGER(VECTOR_ELT(mydf, 1+file))[xx] =   pma->second;// 新規列に追加
			  }
			  else{
				INTEGER(VECTOR_ELT(mydf, 1+file))[xx] = 0;// 新規列に追加
			  }
			}
			  //Rprintf("column %d is finished", (file+1));
			
			pma0++;
			//if(xx % 10 == 0) Rprintf("* ");// 2006 03 27
			
		  } //////////////////////////////
		}else if(typeSet == 1 ){//タームの場合
		  pma0 = ma0.begin();
		  buf3[0] = '\0';
		  for (xx = 0; xx < n0; xx++) {//n0 行のタームの数だけ繰り返す
			strcpy(buf4, (pma0->first).c_str());// 最初の要素の文字列を取得し
			p = strtok(buf4 , "\a" );//タームの内容を Ngramずつ区切る
			//	  Rprintf("buf4 = %s - ", buf4); 
			j = 0;
			i = 1;
			//str.erase();
			while(p != NULL){// _TYPE_1 

			  if(NDF == 1 && i <= Ngram ){//タームはデータフレーム形式で
				sprintf(buf3, "%s", p);
				
// #if defined(WIN32)		  
// 				SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE  ));//j列のxx行にセット
// #elif  defined(__MINGW32__)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE ));
// #else
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8  ));
// #endif

				SET_STRING_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE  ));// < 2006 04 18>
			  
			  //Rprintf("buf3 = %s  \n", buf3); 
			  i++;
			  p = strtok( NULL, "\a");
			  buf3[0] = '\0';
			  continue;
			  }
			  
			  if(  (i % Ngram)  == 0){
				//sprintf(buf3, "%s", str);
				strcat(buf3,p);
				// Rprintf("buf3 = %s  \n", buf3);

				
// #if defined(WIN32)		  
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE  ));//j列のxx行にセット
// #elif  defined(__MINGW32__)
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE ));
// #else
// 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8  ));
// #endif

				SET_STRING_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, (utf8locale)?CE_UTF8:CE_NATIVE  ));//j列のxx行にセット < 2006 04 18>

		
			  //str.erase();
			  buf3[0] = '\0';
			  //++i;
			  }else{
				
				strcat(buf3, p);
				strcat(buf3, "-");
				//str.append(p);
				//str.append("-");
				//++i;
			  }
// // // 			  sprintf(buf3, "%s", p);// 名詞組，品詞組,細分組の取得
// // // 			  //		Rprintf("buf3 = %s\n", buf3);
// // // #if defined(WIN32)		  
// // // 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE  ));//j列のxx行にセット
// // // #elif  defined(__MINGW32__)
// // // 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_NATIVE ));
// // // #else
// // // 			  SET_VECTOR_ELT(VECTOR_ELT(mydf, j++), xx, mkCharCE( buf3, CE_UTF8  ));
// // // #endif
			  
			  p = strtok( NULL, "\a");
			  ++i;
			  // if( j >= 2){
// 				continue;
// 			  }
			  

			}//////while(p != NULL) _TYPE_1 //////////////////////////////

			/////////////////////////////////////////////////	
		// 各ファイルから探し出してその頻度を新規列に追加
			for(file = 0; file < f_count && pma0 != ma0.end(); file++){
			  pma = (vecmap.at(file)).begin(); // ma[file].begin();
			  pma =  (vecmap.at(file)).find( (pma0->first).c_str() );// ma[file].find( (pma0->first).c_str() );
			  if(pma !=  (vecmap.at(file)).end()){// if(pma != ma[file].end()){// 見つかった
				if(NDF == 1){
				  INTEGER(VECTOR_ELT(mydf, Ngram+2+file))[xx] =   pma->second;// 新規列に追加
				}else{
				  INTEGER(VECTOR_ELT(mydf, 3+file))[xx] =   pma->second;//
				}
			  }
			  else{
				if(NDF == 1){
				  INTEGER(VECTOR_ELT(mydf, Ngram+2+file))[xx] = 0;// 新規列に追加
				}else{
				  INTEGER(VECTOR_ELT(mydf, 3+file))[xx] = 0;// 新規列に追加
				}
			  }
			}
			
			pma0++;
			// if(xx % 10 == 0) Rprintf("* ");// 2006 03 27 removed 2007 05
			
		  }// for (xx = 0; xx < n0; xx++) //n0 行のタームの数だけ繰り返す
		  
		}// else if(typeSet == 1 )//タームの場合
	//Rprintf("frequnecy made\n");
	//df 列ベクトルの名前を用意
	
	//  その単純な初期化
	if(typeSet == 0){//文字グラムの場合
	  PROTECT(varlabels = allocVector(STRSXP, 1+f_count)); pc++;
	  // Rprintf("col names allocated\n");

	  
// #if defined(WIN32)	  
// 	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  CE_NATIVE ));
// #else
// 	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  CE_UTF8 ));
// #endif

	  
	  SET_STRING_ELT(varlabels, 0, mkCharCE( "Ngram",  (utf8locale)?CE_UTF8:CE_NATIVE ));
	  
	  
	  //Rprintf("first col names set\n");
	  // 各ファイルあるいは行ごとの名前を設定
	  for(j = 0; j < f_count; j++){

		if(mFt == 2){//データフレームの場合
		  sprintf(buf4, "Row%d", j+1);//s

		  
// #if defined(WIN32)
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, CE_NATIVE ));
// #else		  
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, CE_UTF8 ));
// #endif

		  
		  SET_STRING_ELT(varlabels, j+1, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));
		  
		  
		}else{//ファイルの場合

		  
// #if defined(WIN32)
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], CE_NATIVE ));
// #elif  defined(__MINGW32__)		  
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], CE_NATIVE ));
// #else		  
// 		  SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], CE_UTF8 ));
// #endif

		  
		  SET_STRING_ELT(varlabels, j+1, mkCharCE(ff[j].c_str(), (utf8locale)?CE_UTF8:CE_NATIVE )); // 2011 03 10 SET_STRING_ELT(varlabels, j+1, mkCharCE(f[j], (utf8locale)?CE_UTF8:CE_NATIVE ));
		  
		}
	  }
	  
	}else if(typeSet == 1 ){//タームの場合
	  if(NDF == 1){
		
		PROTECT(varlabels = allocVector(STRSXP, Ngram + 2 + f_count)); pc++;
		for(i = 0; i< (Ngram +2); i++){
		  if(i < Ngram){
			sprintf(buf1, "N%d", i+1);

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  CE_UTF8 ));
// #endif

			SET_STRING_ELT(varlabels, i, mkCharCE( buf1,  (utf8locale)?CE_UTF8:CE_NATIVE ));
			
			
		  }else if (i ==  (Ngram)){
			
			
// #if defined(WIN32)			
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  CE_UTF8 ));
// #endif	

			SET_STRING_ELT(varlabels, i, mkCharCE( "POS1",  (utf8locale)?CE_UTF8:CE_NATIVE ));

			
		  }else if(i ==  (Ngram +1) ){
			

// #if defined(WIN32)			
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_UTF8 ));
// #endif			
// 			//SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  CE_NATIVE ));


			SET_STRING_ELT(varlabels, i, mkCharCE( "POS2",  (utf8locale)?CE_UTF8:CE_NATIVE ));
			
		  }
		}
	  }else{// if(NDF == 1)
		
		PROTECT(varlabels = allocVector(STRSXP, 3+f_count)); pc++;
		
// #if defined(WIN32)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  CE_NATIVE ));
// 		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  CE_NATIVE ));	
// #else		  
// 		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  CE_UTF8 ));
// 		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  CE_UTF8 ));
// 		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  CE_UTF8 ));
// #endif

		SET_STRING_ELT(varlabels, 0, mkCharCE( "TERM",  (utf8locale)?CE_UTF8:CE_NATIVE ));
		SET_STRING_ELT(varlabels, 1, mkCharCE( "POS1",  (utf8locale)?CE_UTF8:CE_NATIVE ));
		SET_STRING_ELT(varlabels, 2, mkCharCE( "POS2",  (utf8locale)?CE_UTF8:CE_NATIVE ));

		
	  }
	  //Rprintf("col names allocated\n");

	  
	  if(mFt == 0 || mFt == 1){//  各ファイル名を列名として設定
		for(j = 0; j < f_count; j++){
		  
		  sprintf(buf4, "%s", ff[j].c_str());// 2011 03 10 sprintf(buf4, "%s", f[j]);//s
		  
		  if(NDF == 1){// Ngram 本体はいちいち単独列

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  CE_NATIVE ));
// #else
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  CE_UTF8 ));
// #endif

		SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE( buf4,  (utf8locale)?CE_UTF8:CE_NATIVE ));

		
		  } else{// Ngram 本体は一つでまとまり


			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, 3+ j, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 		  SET_STRING_ELT(varlabels,  3 +j, mkCharCE(buf4, CE_NATIVE ));
// #else			  
// 		  SET_STRING_ELT(varlabels,  3 + j, mkCharCE(buf4, CE_UTF8 ));
// #endif

			SET_STRING_ELT(varlabels, 3+ j, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));

		  
		  }
		}
		//Rprintf("file names allocated\n");
		
	  }  else if(mFt == 2){//  行番号を列名として設定
		for(j = 0; j < f_count; j++){
		  sprintf(buf4, "Row%d", j+1);//
		  if(NDF == 1){// Ngram 本体はいちいち単独列

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, CE_NATIVE ));
// #else		  
// 			SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, CE_UTF8 ));
// #endif

			
	SET_STRING_ELT(varlabels, Ngram + 2 + j, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));
			
		  }else{

			
// #if defined(WIN32)
// 			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, CE_NATIVE ));
// #elif  defined(__MINGW32__)
// 			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, CE_NATIVE ));
// #else		  
// 			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, CE_UTF8 ));
// #endif

			
			SET_STRING_ELT(varlabels, 3 + j, mkCharCE(buf4, (utf8locale)?CE_UTF8:CE_NATIVE ));

			
		  }
		
		}
	  }
	}//else if(typeset ==1) //タームの場合
  
	
	Rprintf("\n");// 2006 03 27

	//Rprintf("row number n0  = %d\n", n0 );
		
	//Rprintf("after colunm names set\n" );
	///////////////////////// new_end 
	  // データフレームの行名を設定．必須
	PROTECT(row_names = allocVector(STRSXP, n0));pc++;
	//Rprintf("after row names set\n" );
	char  labelbuff[6];// char  labelbuff[5]; 2006 03
	for (xx = 0; xx < n0 ; xx++) {
	  sprintf(labelbuff, "%d", xx+1);

	  
// #if defined(WIN32)
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , CE_NATIVE));
// #elif  defined(__MINGW32__)
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , CE_NATIVE));
// #else	  
// 	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , CE_UTF8));
// #endif

	  SET_STRING_ELT(row_names, xx, mkCharCE(labelbuff , (utf8locale)?CE_UTF8:CE_NATIVE));
	  
	  //Rprintf("set row %d\n", xx+1 );
	}
	//Rprintf("before setAttr\n" );
	// データフレームオブジェクト mydf の属性設定
	
	// 	  	  // オブジェクトはデータフレームだと指定する
	PROTECT(tmp = mkString("data.frame")); pc++;//tmpにその属性を一時保存
	//Rprintf("data frame made");
	
	setAttrib(mydf, R_ClassSymbol, tmp);
	setAttrib(mydf, R_NamesSymbol, varlabels);
	setAttrib(mydf, R_RowNamesSymbol, row_names);
	
	//Rprintf("before UNPROTECT\n" );
	UNPROTECT(pc);
	//	Rprintf("UNPROTECT \n");
	//free(f);
	return (mydf);
	
	
	// 2006 03 05
	
	// # sym 引数は，抽出タームに句読点なので記号を含めるかを指定する．
	// ##            デフォルトでは sym = 0 とセットされており，
	// ##            記号はカウントされないが，
	// ##            sym = 1 とすると，記号を含めてカウントした結果が出力される
	// ##            pos 引数に記号が含まれた場合は自動的に sym = 1 とセットされる
	
	// 	///////////////////////////////////////////////////////
  }
}
// extern




/////////////////////////////////////////////////////////////
