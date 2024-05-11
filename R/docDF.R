# 2009 03 09
docDF <-
  function(target, column = 0, type = 0, pos = NULL, minFreq = 1, N = 1, Genkei = 0, weight = "", nDF = 0, co = 0,  dic = "" , mecabrc = "", etc = ""  ){

    if(is.null(pos)){
      posN <- 0
    }else {
      posN <- length(pos)
      if(posN == 1){
        if( nchar(posN) < 1){
           stop("specify part of speech")
        }
      }
    }    


    if( is.null(dic) || is.na(dic)){
      dic = ""
  } else if( (xl <- nchar(dic))  > 0  ) {
      dic <- paste(dirname(dic), basename(dic), sep = "/")
      if ( !(file.exists(dic)) )
        {
          cat ("specified dictionary file not found; result by default dictionary.\n")
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
                                        # 2015 12 11
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
    
#    gc()
    if(is.data.frame(target)){
      ft <- 2# データフレームのある列
      fileN = length(target[, column, drop = TRUE]  )
      if( fileN < 1){
        stop("data frame must be equal to or larger than 1 ")      
      }else{
        file <- as.vector( target[, column, drop = T] )
      }
    }  else  if(any(suppressWarnings(dir(target) ) > 0)){
      ft <- 1 ##ディレクトリが指定された
      file <- dir(target)
      fileN = length(file)  
    } else if (file.exists(target)){
      ft <- 0 # 単独ファイル
      file <- basename(target)
      fileN = length(file)  
      target <- dirname(target)
    } else{
      stop("specify target!")
    }
    if(co == 1 || co == 2 || co == 3){
      type = 1
      N = 1
      nDF <- 0
    }else{
      co = 0
    }

    if(type != 0 && type != 1){
      stop("type must be 0 (character) or 1 (word)")
    }
    if(Genkei != 0 && Genkei != 1){
      stop("Genkei must be 0 (infinitive) or 1")
    }

    if(minFreq < 1){
      stop("minFreq argument must be equal to or larger than 1 ")      
    }
    if(N < 1){
      stop("N argument  must be equal to or larger than 1")  
    }

    
    dtm <- .Call("docDF", as.character(target), as.character(file), as.numeric(fileN), as.integer(ft), as.integer(type),  as.character(pos), as.integer(posN), as.integer(minFreq), as.integer(N), as.integer(Genkei),  as.integer(nDF), as.character(opt),  PACKAGE="RMeCab")

    if(co > 0 ){
      dtmTerm <- dtm[,1:3]
##       dtmCo <- coOccurrence( dtm[,(1+3):ncol(dtm)], co)
##       colnames(dtmCo) <- as.character(dtm[,1])
      dtm <- cbind(dtmTerm,   coOccurrence( dtm[,(1+3):ncol(dtm)], co)  )
      colnames(dtm) <- c("TERM", "POS1", "POS2", as.character(dtm[,1]))
    }else if(weight == "" || weight == "no"){
      
    }else {
          
      if(nDF == 1){
        colN <- N
      }else{
        colN <- 1
      }

      argW <- unlist(strsplit(weight, "*", fixed = T))
      
      for(i in 1:length(argW)){
        if(argW[i] == "no"){
          break
        }else if(argW[i] == "tf"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <- localTF(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <- localTF(dtm[, (colN+3):ncol(dtm)])
          }
        }else if(argW[i] == "tf2"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <- localLogTF(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <- localLogTF(dtm[, (colN+3):ncol(dtm)])
          }
                                        # dtm <- localLogTF(dtm)
        }else if(argW[i] == "tf3"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <-  localBin(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <-  localBin(dtm[, (colN+3):ncol(dtm)])
          }
                                        #dtm <- localBin(dtm)
        }else if(argW[i] == "idf"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <-  dtm[,(colN+1):ncol(dtm)]*globalIDF(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <- dtm[,(colN+3):ncol(dtm)]*globalIDF(dtm[, (colN+3):ncol(dtm)])
          }
                                        #dtm <- dtm * globalIDF(dtm)
        }else if(argW[i] == "idf2"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <- dtm[,(colN+1):ncol(dtm)]*globalIDF2(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <- dtm[,(colN+3):ncol(dtm)]*globalIDF2(dtm[, (colN+3):ncol(dtm)])
          }
                                        #dtm <- dtm * globalIDF2(dtm)
        }else if(argW[i] == "idf3"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <-   dtm[,(colN+1):ncol(dtm)]*globalIDF3(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <-   dtm[,(colN+3):ncol(dtm)]*globalIDF3(dtm[, (colN+3):ncol(dtm)])
          }
                                        #dtm <- dtm * globalIDF2(dtm)
        }else if(argW[i] == "idf4"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <- dtm[,(colN+1):ncol(dtm)] * globalEntropy(dtm[, (colN+1):ncol(dtm)])
          }else{
            dtm[,(colN+3):ncol(dtm)]  <- dtm[,(colN+3):ncol(dtm)] * globalEntropy(dtm[, (colN+3):ncol(dtm)])
          }
                                        #dtm <- dtm * globalEntropy(dtm)
        } else if(argW[i] == "norm"){
          if(type == 0){
            dtm[,(colN+1):ncol(dtm)]  <-  t(t(dtm[,(colN+1):ncol(dtm)]) *  mynorm(dtm[, (colN+1):ncol(dtm)]))
          }else{
            dtm[,(colN+3):ncol(dtm)]  <-  t(t(dtm[,(colN+3):ncol(dtm)]) * mynorm(dtm[, (colN+3):ncol(dtm)]))
          }
                                        #dtm <- t(t(dtm) * mynorm(dtm))
        }
      }
      if(type == 0){
        if(any(is.na ( dtm[, (colN+1):ncol(dtm)]))){
          cat("Warning! Term document matrix includes NA!", "\n")
        }
      }else{
        if(any(is.na ( dtm[, (colN+3):ncol(dtm)]))){
          cat("Warning! Term document matrix includes NA!", "\n")
        }
      }
    }
    
    invisible( dtm)

  }
