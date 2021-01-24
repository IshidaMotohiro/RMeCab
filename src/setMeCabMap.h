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


#ifndef _SETMECABMAP_
#define _SETMECABMAP_
////////////////////////////////////////////////////////////////

SEXP setMeCabMap(int typeSet, char input[], map<string, int> & ma0, 	map<string, int>  &  ma1,  	map<string, int>::iterator & pma0, map<string, int>::iterator & pma,  list <string> & strL, 	 list <string>::iterator & iter,  list <string> & hinsi, 	 list <string>::iterator & hinsi_it,  list <string> & saibun, 	 list <string>::iterator & saibun_it, vector<string> & Ppos2, int pos_n, int Ngram, int genkei, const char * dic );

#endif
