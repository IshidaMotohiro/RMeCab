# 2008 03 28 # the third arg omitted
# pos = c("名詞","形容詞")
# 2014 09 24  file.info(filename)$isdir added

Ngram <-
  function(filename, type = 0, N = 2, pos = "Default" ,  dic = "" , mecabrc = "", etc = ""   ){
filename <- paste(dirname(filename), basename(filename), sep = "/")
#    gc()
    if(! file.exists(filename) || length (dir (filename)) > 0){
      stop("file not found")
    }
    if(file.info(filename)$isdir){
        stop("this is directory. Please input file name")
    }

    if(type != 0 && type != 1 && type != 2){
            stop("type must be 0 or 1 or 2")
    }
    if( type > 0 &&  (any( pos == "" | is.na(pos)) )){
      stop("specify pos argument!")
    }
    if( length(pos) == 1 && pos == "Default" ){
      posN <- 0
    }else{
      posN <- length(pos)
    }
##     if(type > 0){# 形態素 or 品詞情報
##       if( posN < 1 ){
##         stop("specify pos argument!")
##       }
##     }
    if(N < 2){
      stop("N argument  must be equal to or larger than 2")  
    }

    if( is.null(dic) || is.na(dic)){
      dic = ""
    } else if( (xl <- nchar(dic))  > 0 ) {
      if (substring(dic, xl-3) != ".dic" || !(file.exists(dic)) )
        {
          cat ("specified dictionary file not found; result by default dictionary.\n")
          dic = ""
        }
      else {
        dic <- paste(" -u", dic)
     }
    }

    if(  is.null(mecabrc) || is.na(mecabrc) || (nchar(mecabrc)) < 2  ){
     mecabrc = ""
   } else {
     if ( !(file.exists(mecabrc)) )
       {
         cat ("specified mecabrc not found; results by default mecabrc.\n")
         mecabrc = ""
       }
     else {
       mecabrc <- paste("-r", mecabrc)
     }
   }

    opt <- paste(dic, mecabrc, etc)
  
    
    .Call("Ngram", as.character(filename), as.integer(type), as.integer(N), as.character(pos), as.integer(posN),   as.character(opt),   PACKAGE="RMeCab")
}
