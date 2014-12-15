# 2008 03 28 # the third arg omitted  pos = c("名詞","形容詞"),sym = 0, kigo = "記号", 
docNgram2 <-
  function(directory, type = 0, pos = "Default", minFreq = 1, N = 2, kigo = 0, weight = "no" ,  dic = "", mecabrc = "", etc = ""   ){
#    posN <- length(pos)
                          
#    gc()
    if(any(suppressWarnings(dir(directory) ) > 0)){
      ft <- 1 ##ディレクトリが指定された
	file <- dir(directory)
    } else if (file.exists(directory)){
      ft <- 0 # 単独ファイル
      file <- basename(directory)
      directory <- dirname(directory)

    } else{
      stop("specify directory or a file!")
    }
    fileN = length(file)  

    if(type != 0 && type != 1 && type != 2){
      stop("type must be 0 or 1 or 2")
    }
    if( type > 0 &&  (any( pos == ""  | is.na(pos))) ){
      stop("specify pos argument!")
    }
    if( length(pos) == 1 && pos == "Default" ){
      posN <- 0
    }else{
      posN <- length(pos)
    }
##     if(type > 0){# 形態素 or 品詞情報
##       if( posN < 1){
##         stop("specify pos argument")
##       } else if("記号" %in% pos)
##         sym = 1# 記号を頻度に含めて出力する
##     }
    if(minFreq < 1){
      stop("minFreq argument must be equal to or larger than 1 ")      
    }
    if(N < 2){
      stop("N argument  must be equal to or larger than 2")  
    }


    if( is.null(dic) || is.na(dic)){
      dic = ""
    } else if( (xl <- nchar(dic))  > 0 ) {
      if (substring(dic, xl-3) != ".dic" || !(file.exists(dic)) )
        {
          cat ("specified dictionary file not found; result by default dictionary.\n") # cat ("dictionary file not found; no dic file specified.\n")
         dic = ""
        }
      else {
        dic <- paste(" -u", dic)
      }
    }
    #
   if(  is.null(mecabrc) || is.na(mecabrc) || (nchar(mecabrc)) < 2  ){
     mecabrc = ""
   } else {
     if ( !(file.exists(mecabrc)) )
       {
         cat ("specified mecabrc not found; result by default mecabrc.\n")
         mecabrc = ""
       }
     else {
       mecabrc <- paste("-r", mecabrc)
     }
   }
   #
   opt <- paste(dic, mecabrc, etc)
    
    dtm <- .Call("docNgram2", as.character(directory), as.character(file), as.numeric(fileN), as.integer(ft), as.integer(type),  as.character(pos), as.integer(posN), as.integer(minFreq), as.integer(N), as.integer(kigo), as.character(opt), PACKAGE="RMeCab")# as.integer(sym), as.character(kigo), 

    if(weight == "no" || weight == ""){
       invisible( t(dtm))
##       break
    }else{
      argW <- unlist(strsplit(weight, "*", fixed = T))
      
      for(i in 1:length(argW)){
        dtm <- t(dtm)
        
        if(argW[i] == "tf"){
          dtm <- localTF(dtm)
        }else if(argW[i] == "tf2"){
          dtm <- localLogTF(dtm)
        }else if(argW[i] == "tf3"){
          dtm <- localBin(dtm)
        }else if(argW[i] == "idf"){
          dtm <- dtm * globalIDF(dtm)
        }else if(argW[i] == "idf2"){
          dtm <- dtm * globalIDF2(dtm)
        }else if(argW[i] == "idf3"){
          dtm <- dtm * globalIDF2(dtm)
        }else if(argW[i] == "idf4"){
          dtm <- dtm * globalEntropy(dtm)
        } else if(argW[i] == "norm"){
          
          dtm <- t(t(dtm) * mynorm(dtm))
        }
      }
      if(any(is.na ( dtm))){
        cat("Warning! Term document matrix includes NA!", "\n")
      }
      invisible( dtm )
    }


  }
