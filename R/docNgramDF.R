# 2009 03 09
##   function(mojiVec = "めかぶ", type = 0, pos = c("名詞","形容詞"),  baseform = 0, minFreq = 1, N = 1, co = 0,  sym = 0, kigo = "記号", weight = "no"){
docNgramDF <-
  function(mojiVec = "MeCab", type = 0, pos = "Default" ,  baseform = 0, minFreq = 1, N = 1, kigo =0, weight = "no", co = 0,  dic = "", mecabrc = "", etc = ""   ){

    rowN <- length(mojiVec)
    if(rowN < 1){
      stop("character vector must be specified.")
    }

##     if(is.null(pos)){
##       posN <- 0
##     }else {
##       posN <- length(pos)
##       if(posN == 1){
##         if( nchar(posN) < 1){
##            stop("specify part of speech")
##         }
##       }
##     }    
 


    if(type != 0 && type != 1){
      stop("type must be 0 (character) or 1 (word)")
    }
##     if(baseform != 0 && baseform != 1){
##       stop("baseform must be 0 (infinitive) or 1")
##     }

    if( type > 0 &&  (any( pos == "" | is.na(pos))) ){
      stop("specify pos argument!")
    }
    if( length(pos) == 1 && pos == "Default" ){
      posN <- 0
    }else{
      posN <- length(pos)
    }
    
    if(minFreq < 1){
      stop("minFreq argument must be equal to or larger than 1 ")      
    }
    if(N < 1){
      stop("N argument  must be equal to or larger than 1")  
    }


    
    if( is.null(dic) || is.na(dic)){
      dic = ""
  } else if( (xl <- nchar(dic))  > 0 ) {
      dic <- paste(dirname(dic), basename(dic), sep = "/")
     if ( !(file.exists(dic)) )
       {
         cat ("specified dictionary file not found; result by default dictionary.\n") # 
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

    dtm <- .Call("docNgramDF", as.character(mojiVec), as.numeric(rowN), as.integer(type),  as.character(pos),  as.integer(posN), as.integer(minFreq), as.integer(N),  kigo = 0,  as.character(opt), PACKAGE = "RMeCab")
##    dtm <- .Call("docNgramDF", as.character(mojiVec), as.numeric(rowN), as.integer(type),  as.character(pos),  as.integer(posN), as.integer(minFreq), as.integer(N),   sym = 0, kigo = "記号",  PACKAGE="RMeCab")


        ## ##   class(dtm) <- "RMeCabMatrix"
    if(type == 1 && (co == 1 || co == 2 || co == 3) ){
      
      dtm <- coOccurrence( removeInfo (dtm), co)
##      invisible(dtm)
    }
    ##   ######### < 2008 05 04 uncommented>
    if(weight == "no" || weight == ""){
##       invisible( dtm)
##       break
    }else{
      # dtm <- t(dtm)# こちらは列が単語になっている
      argW <- unlist(strsplit(weight, "*", fixed = T))
      
      for(i in 1:length(argW)){
        if(argW[i] == "tf"){
          dtm <- localTF(dtm)
        }else if(argW[i] == "tf2"){
          dtm <- localLogTF(dtm)
        }else if(argW[i] == "tf3"){
          dtm <- localBin(dtm)
        }else if(argW[i] == "idf"){
          dtm <- dtm * globalIDF(dtm , tp =1 )
        }else if(argW[i] == "idf2"){
          dtm <- dtm * globalIDF2(dtm , tp =1)
        }else if(argW[i] == "idf3"){
          dtm <- dtm * globalIDF3(dtm , tp =1)
        }else if(argW[i] == "idf4"){
          dtm <- dtm * globalEntropy(dtm , tp =1)
        } else if(argW[i] == "norm"){
          
          dtm <- dtm * mynorm(dtm , tp =1)
        }
      }
      if(any(is.na ( dtm))){
        cat("Warning! Term document matrix includes NA!", "\n")
      }
    }
    
    invisible( dtm)
    
  }
