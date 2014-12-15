RMeCabMx <-
function(filename, pos, posN, minFreq = 1, kigo = 0,  dic = "", mecabrc = "", etc = ""  ){#  sym = 0, kigo = "記号"

   if(! file.exists(filename)){
     stop("file not found")
   }
   if(length(pos) < 1){
     stop("second argument must be specified.")
   }
##    if(posN != length(pos)){
##      posN = length(pos)
##    }
## #   gc()


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
         cat ("specified mecabrc not found; result by default mecabrc.\n")
         mecabrc = ""
       }
     else {
       mecabrc <- paste("-r", mecabrc)
     }
   }

   opt <- paste(dic, mecabrc, etc)
   

   # cat("* ")
  .Call("RMeCabMx", as.character(filename) , pos, as.integer(posN), as.integer(minFreq), as.integer(kigo), as.character(opt),  PACKAGE="RMeCab")
}


