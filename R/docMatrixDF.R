## charVec = c("めかぶ","基広"), pos= c("名詞","動詞","形容詞"),
docMatrixDF <-
  function(  charVec = c("MeCab","CaBoCha"), pos= "Default" , minFreq = 1, weight = "no", co = 0,  dic = "" , mecabrc = "", etc = ""  ) {
    charLeng <- length(charVec)
    if(charLeng < 1){
      stop("character vector must be specified.")
    }
    if( any( pos == "" | is.na(pos)) ){
      stop("specify pos argument!")
    }
    if( length(pos) == 1 && pos == "Default" ){
      posN <- 0
    }else{
      posN <- length(pos)
    }
## ##   posN <- length(pos)
##     if(posN < 1){
##       stop("pos must be specified.")
##     }


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
   
    
    if(minFreq < 1){
      stop("minFreq > 0 must be specified.")     
    }
    
    dtm <- .Call("docMatrixDF", as.character(charVec), as.numeric(charLeng), pos, as.numeric(posN),  as.numeric(minFreq), as.character(opt), PACKAGE = "RMeCab")
    
    if(is.null(dtm)){
      stop("give less number to minFreq!")
    }
    
    dtm <- t(dtm)
                                        #  environment(dtm) = new.env()
    ## ##   class(dtm) <- "RMeCabMatrix"
    if(co == 1 || co == 2 || co == 3){
      
      dtm <- coOccurrence( removeInfo (dtm), co)
##      invisible(dtm)
    }
    ##   ######### < 2008 05 04 uncommented>
    if(weight == ""){
##       invisible( dtm)
##       break
    }
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
        dtm <- dtm * globalIDF3(dtm)
      }else if(argW[i] == "idf4"){
        dtm <- dtm * globalEntropy(dtm)
      } else if(argW[i] == "norm"){
        
        dtm <- t(t(dtm) * mynorm(dtm))
      }
    }
    if(any(is.na ( dtm))){
      cat("Warning! Term document matrix includes NA!", "\n")
    }
    invisible( dtm)  
  }
