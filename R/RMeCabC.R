# 2008 03 28 # the third arg omitted
RMeCabC <-
function(str, mypref = 0, dic = "", mecabrc = "", etc = ""){
#   gc()
  if(!is.character (str)){
    str <- as.character (str)
  }
  
  if(nchar(str) < 1){
     stop("first argument must be specified")
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
       dic <- paste("-u", dic)
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
   
  ret <- .Call("RMeCabC", as.character(str), as.integer(mypref), as.character(opt), PACKAGE="RMeCab")
   # Encoding(ret) <- "UTF-8"
   return(ret)
}
