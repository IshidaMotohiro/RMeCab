makeNgram <-
  function (filename, type = 1, N = 2, pos = "Default",  dic = "" , mecabrc = "", etc = ""   ) {# pos = ("名詞")

         
   ##  if( is.null(dic) || is.na(dic)){
   ##    dic = ""
   ##  } else if( (xl <- nchar(dic))  > 0 ) {
   ##    if (substring(dic, xl-3) != ".dic" || !(file.exists(dic)) )
   ##      {
   ##         cat ("specified dictionary file not found; result by default dictionary.\n") # 
   ##       dic = ""
   ##      }
   ##    else {
   ##      dic <- paste(" -u", dic)
   ##    }
   ## }
    
    dummy <- Ngram(filename, type, N, pos, dic, mecabrc, etc)
    dummy$Text <- rep(basename(filename), length(dummy$Freq))
     if(length(dummy) < 1){
       return(NULL)
     }
     else{
     return(dummy)
   }
  }


docNgram <-
  function( mydir,  type = 1, N = 2, pos = "Default" ,  dic = "" , mecabrc = "", etc = ""  ) {# pos = ("名詞")

    if( type > 0 &&  (any( pos == "" | is.na(pos)) )){
      stop("specify pos argument!")
    }
    
  dummy <- lapply( dir(mydir, full.names=TRUE), makeNgram, type, N, pos, dic, mecabrc, etc)
    
  if(length(dummy) == 0){
#    gc()
    stop("no doc-matrix returned.")
  }
  else if(length(dummy) == 1){
    return(dummy)
  }
  
  # dtm <- t(xtabs(Freq ~ ., data = do.call("rbind", dummy)))
  dtm <- xtabs(Freq ~ ., data = do.call("rbind", dummy))
   invisible( dtm)  
}
