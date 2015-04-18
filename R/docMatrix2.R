##  sym = 0, pos = c("名詞", "形容詞"),kigo = "記号", 
docMatrix2 <-
  function( directory,  pos = "Default"  , minFreq = 1, weight = "no", kigo = 0, co = 0,  dic = "" , mecabrc = "", etc = ""   ) {
    # posN <- length(pos)
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
    if(any( pos == "" | is.na(pos)) ){
      stop("specify pos argument!")
    }
    if( length(pos) == 1 && pos == "Default" ){
      posN <- 0
    }else{
      posN <- length(pos)
    }
##     if( posN  < 1){
##       stop("specify pos argument")
##     } else if("記号" %in% pos){
##       sym = 1 # 記号を頻度に含めて出力する
##     }

    if( is.null(dic) || is.na(dic)){
      dic = ""
    } else if( (xl <- nchar(dic))  > 0 ) {
     if (substring(dic, xl-3) != ".dic" || !(file.exists(dic)) )
       {
          cat ("specified dictionary file not found; result by default dictionary.\n")# 
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
       mecabrc <- paste(dirname(mecabrc), basename(mecabrc), sep = "/")
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
    
    if(minFreq < 1){
      stop("minFreq argument must be equal to or larger than 1!")      
    }
    
    
    dtm <- .Call("docMatrix2", as.character(directory), as.character(file), as.numeric(fileN), as.numeric(ft), as.character(pos), as.numeric(posN),  as.numeric(minFreq), as.numeric(kigo), as.character(opt), PACKAGE="RMeCab")## as.numeric(sym),  as.character(kigo), 
    
    if(is.null(dtm)){
      stop("chage the value of minFreq argument!")
    }
    
    dtm <- t(dtm)
                                        #  environment(dtm) = new.env()
    ## ##   class(dtm) <- "RMeCabMatrix"
    
    if(co == 1 || co == 2 || co == 3){
      
      dtm <- coOccurrence( removeInfo (dtm), co)
 ##     invisidoble(dtm)
    }
    
    
    ##   ######### < 2008 05 04 uncommented>
    if(weight == ""){
 ##      invisible( dtm)
##       break
    }else{
      argW <- unlist(strsplit(weight, "*", fixed = T))
      
      for(i in 1:length(argW)){
        if(argW[i] == "no"){
          invisible( dtm)  
          ##        cat("Term Document Matrix includes 2 information rows!", "\n")
          ##         cat("whose names are [[LESS-THAN-", minFreq,"]] and [[TOTAL-TOKENS]]", "\n", sep = "")
          ##         cat("if you remove these rows, execute", "\n", "result[ row.names(result) !=  \"[[LESS-THAN-", minFreq, "]]\" , ]", "\n", "result[ row.names(result) !=  \"[[TOTAL-TOKENS]]\" , ]","\n" , sep = "")
          break
        }else if(argW[i] == "tf"){
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

    }
    invisible( dtm)  
  }
