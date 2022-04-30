/*   ver 1.08 2022 04 30 */
/*
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

//#define WIN 0



#include "RMeCab.h"
#include "setMeCabMap.h"

////////////////////////////////////////////////////////////////

SEXP setMeCabMap(int typeSet, char input[], map<string, int> & ma0, 	map<string, int>  &  ma1,  	map<string, int>::iterator & pma0, map<string, int>::iterator & pma,  list <string> & strL, 	 list <string>::iterator & iter,  list <string> & hinsi, 	 list <string>::iterator & hinsi_it,  list <string> & saibun, 	 list <string>::iterator & saibun_it, vector<string> & Ppos2, int pos_n, int Ngram, int genkei, const char * dic ){// map<string, int> &  ma1, char *Ppos[], 
  
  mecab_t *mecab;
  mecab_node_t *node;
	
 int i, j , posC = 0, xx =0; 	
 char buf1 [BUF1];// 2010 12 17 //[128];// [512];//入力された語形を記憶
 char buf2[BUF3];
 char buf3[BUF2];// 2010 12 17 //[64];// [512];記号チェック用
 char buf4[BUF2];// 2010 12 17 //[64];// [1024];記号チェック用
 string str;
 char *p;
	
	
 wchar_t  wbuf [BUF4] ;// = { 0 }; //wchar_t  wbuf [5120] = { 0 }; /* ワイド文字列 : 日本語文字数  + 1 */
 memset (wbuf, 0, sizeof wbuf); // 2015 12 18
  unsigned int  wz = 0;
	
  string target;
  char target2[BUF3];
			
				

  //	http://mecab.sourceforge.net/mecab.html
  mecab = mecab_new2 (dic);// mecab = mecab_new2 ("めかぶ");// mecab_new2 (" -u user.dic");mecab_new2(" -d mecab\dic\ipadic -O ruby");
  CHECK(mecab);


	//Rprintf("%s strlen of input= %d\n", input, strlen(input)); 
			
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
	  for( wz = 0; wz <  wcslen(wbuf) ; wz++){

		// 2005 07 22
		// 2008 04 05 #if defined(_WIN64) || !defined(_WIN32)
		// defined(__MINGW32__) || defined(__MINGW64__
	    /* 2022 04 30
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
		wsprintf(target2, "%lc", wbuf[wz]);// windows では wsprintf
#elif  defined(__MINGW32__) || defined(__WINGW64__)
		wsprintf(target2, "%lc", wbuf[wz]);//  windows では wsprintf
#else
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
#endif
	    */
		sprintf(target2, "%lc", wbuf[wz]);// Linux  では sprintf
		//				Rprintf("target2 = %s\n", target2);
		if(strlen(target2) < 1){
		  break;
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
			//target.append("[");
			xx = 1;
			for ( iter = strL.begin(); iter != strL.end(); iter++){
			  //						Rprintf("in for\n");
			  //						Rprintf("str %s\n", * iter);
			  target.append( *iter);
			  if(xx < Ngram){
				 target.append("\a");//target.append("-");
			  }
			  xx++;
			  //					  Rprintf("xx = %d\n", xx);
						
			}
			xx = 1;
			//target.append("]");
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
			pma = ma1.find(target);//出てきた形態素原型は既に個別マップにあるか？
			if(pma != ma1.end()){
			  pma->second =  pma->second + 1;
			  //二つ目の数値を加算
			}
			else{// マップにないなら，新規にマップに追加
			  ma1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
			}
					  
			strL.pop_front();
		  }//_if strSize>= Ngram
		}// _else_end
				  
		////////////////////////////////////// new _end ////
	  }//_for2_< wcslen
	  
	}// if_strlen_>_0_end




			  
			  
  } else {// if_type_set 形態素あるいは品詞の場合



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
	    // 2010  		buf1 = (char *)malloc( node->length * MB_CUR_MAX+ 1);	
		strncpy(buf1, node->surface, node->length) ;//元の語形
		
		buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動
		// strlen関数はstringの文字数を返します。この長さには、終端のNULL文字('\0')は含まれません。
		if(strlen(buf1) < 1){// 2006 06 移動		
		  continue;
		}
		
		//< 2005 11 07> //Rprintf("%s\n", buf1);			
		//if( atoi(buf1) >  0x00 &&  atoi(buf1) < 0x0e ){// if( atoi(buf1)  == 0x0e){//エスケープ記号類
		if( buf1[0] > 0x00 && buf1[0] < 0x21 ){//エスケープ記号類0x0e // strlen(buf1) == 1 &&
		  continue;
		}// </ 2005 11 07>
		
		//		buf1[node->length] = '\0';// 末尾にNULLを加える// 2006 06 移動				 
// 		if(strlen(buf1) < 1){// 2006 06 移動		
// 		  continue;
// 		}
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
// 			// if(mSym < 1 && strcmp(buf3, "記号") == 0){
// 			if(mSym < 1 && strcmp(buf3, KIGO) == 0){						
// 			  p = NULL;
// 			  //j = 9;
// 			  continue;// 記号は一切省き，総計にも加えない
// 			}
// 			//	
//			Rprintf("buf3 %s\n", buf3);
			if(pos_n == 0){
			  hinsi.push_back(buf3);
			  posC = 1;
			}else{
			  for(i = 0; i < pos_n; i++){
			    sprintf(buf4, "%s", Ppos2[i].c_str());	// 2011 03 10 sprintf(buf4, "%s", Ppos[i]);				
				//					Rprintf("buf4 %s\n", buf4);
				
				if(strcmp(buf3, buf4) == 0){
				  posC = 1;
				  hinsi.push_back(buf3);
				  break;
				}
			  }
			}
			if(posC != 1){
			  p = NULL;
			  posC = 0;
			  continue;
			}
		  }
				  
				
		  while ( p != NULL ) {
					
			// if(j == 1){//品詞情報1
			// 					str = p;
			// 					// str.append(",");
			// 				  }else
			 if(j == 2){//品詞第2情報
			   saibun.push_back(p);
			 } else if( j == 7){
			  if(genkei == 1 || p == NULL || strcmp(p, "*") == 0){
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
		target.append("");
		xx = 1;
		for ( iter = strL.begin(); iter != strL.end(); iter++){
		  // Rprintf("in for\n");
		  //sprintf(buf3, "%s", *iter);
		  //Rprintf("str %s\n", *iter);
		  //Rprintf("after Rprintf in for\n");
		  target.append( *iter);// target.append( buf3); //target.append( *iter);
		  //					Rprintf("target append\n");
		  if(xx < Ngram){
			 target.append("\a");//target.append("-");
		  }
		  xx++;
		} // for 
		xx = 1;
		if(typeSet == 1){
		  target.append("\a");
		  for ( hinsi_it = hinsi.begin(); hinsi_it != hinsi.end(); hinsi_it++){
		  // Rprintf("in for\n");
			//sprintf(buf3, "%s", *iter);
			//Rprintf("str %s\n", *iter);
			//Rprintf("after Rprintf in for\n");
			target.append( *hinsi_it);// target.append( buf3); //target.append( *iter);
		  //					Rprintf("target append\n");
			if(xx < Ngram){
			   target.append("\a");//target.append("-");
			}
			xx++;
		  } // for
		
		  xx = 1;
		
		  target.append("\a");
		  for ( saibun_it = saibun.begin(); saibun_it != saibun.end(); saibun_it++){
			// Rprintf("in for\n");
			//sprintf(buf3, "%s", *iter);
		  //Rprintf("str %s\n", *iter);
		  //Rprintf("after Rprintf in for\n");
			target.append( *saibun_it);// target.append( buf3); //target.append( *iter);
			//					Rprintf("target append\n");
		  if(xx < Ngram){
			 target.append("\a");//target.append("-");
		  }
		  xx++;
		  } // for
		
		  xx = 1;
		}//if(typeSet == 1){
		
		
		pma0 = ma0.find(target);//出てきた形態素原型は既に全体マップにあるか？
		if(pma0 != ma0.end()){
		  pma0->second =  pma0->second + 1;
		  //二つ目の数値を加算
		}
		else{// マップにないなら，新規にマップに追加
		  ma0.insert(make_pair(target, 1));// 1 は 1個目と言う意味
		}
				  
		pma = ma1.find(target);// str 出てきた形態素原型は既に個別マップにあるか？
		if(pma != ma1.end()){
		  pma->second =  pma->second + 1;
		  //二つ目の数値を加算
		}
		else{// マップにないなら，新規にマップに追加
		  ma1.insert(make_pair(target, 1));// 1 は 1個目と言う意味
		}
				  
		strL.pop_front();// 最初の要素を取り除く
		
		if(typeSet == 1){
		  hinsi.pop_front();
		  saibun.pop_front();
		}
	  }				  // if(strL.size() >= Ngram)
				
	}//for(;node;)// Rprintf("node check ended\n");
			  
  }
  	mecab_destroy(mecab);


	return (R_NilValue);// return 0;


}



//////////////////////





















