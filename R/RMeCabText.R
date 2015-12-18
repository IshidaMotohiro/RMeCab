# 2014 09 24 file.info(filename)$isdir added
RMeCabText <-
function(filename,  dic = "" , mecabrc = "", etc = ""  ){
#   gc()
# 2015 12 11     filename <- paste(dirname(filename), basename(filename), sep = "/")
  if(! file.exists(filename)){
    stop("file not found")
  }
    if(file.info(filename)$isdir){
        stop("this is directory. Please input file name")
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
  
  .Call("RMeCabText", as.character(filename) ,  as.character(opt),  PACKAGE="RMeCab")
}

