RMeCabDoc <-
function(filename, mypref = 1, kigo = 0,  dic = "", mecabrc = "", etc = ""   ){
#  gc()
# 2015 12 11     filename <- paste(dirname(filename), basename(filename), sep = "/")
  if(! file.exists(filename)){
    stop("file not found")
  }


  if( is.null(dic) || is.na(dic)){
    dic = ""
  } else 
      if( (xl <- nchar(dic))  > 0 ) {
          dic <- paste(dirname(dic), basename(dic), sep = "/")
          if ( !(file.exists(dic)) )
      {
        cat ("dictionary file not found; no dic file specified.\n")
         dic = ""
      }
    else {
      dic <- paste(" -u", dic)
    }
  }
  
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

   opt <- paste(dic, mecabrc, etc)
   
  .Call("RMeCabDoc", as.character(filename) , as.numeric(mypref),  as.numeric(kigo),  as.character(opt),  PACKAGE="RMeCab" )
}

