RMeCabDF <-
  function(dataf, coln, mypref = 0,  dic = "" , mecabrc = "", etc = ""  ){
    if(!is.data.frame(dataf) ){
      stop("the first argument must be a data frame!")
    }


    ## if( is.null(dic) || is.na(dic)){
    ##   dic = ""
    ## } else if( (xl <- nchar(dic))  > 0  ) {
    ##   if (substring(dic, xl-3) != ".dic" || !(file.exists(dic)) )
    ##     {
    ##       cat ("dictionary file not found; no dic file specified.\n")
    ##       dic = ""
    ##    }
    ##   else {
    ##     dic <- paste(" -u", dic)
    ##   }
    ## }

##     if(as.numeric(coln) & as.numeric(coln) < ncol(dataf) ){
##       stop("specified column not found!")      
##     }
##     if(as.character(coln) & !is.factor(dat[,coln]) | ! is.character(coln)){
##      stop("specified column must be factor or character!")      
##     }
##     if(!is.numeric(coln) || coln > ncol(dataf)){
##       stop("the second argument must be integer")
##     }
## #    gc()
    kekka <- list(length(dataf[,coln]))
    for(i in 1:length(dataf[,coln])){
      if( !is.factor(dataf[i,coln]) || is.na(dataf[i,coln])  ||  dataf[i, coln] == ""){
      # stop("line number %d include non-characters!", i)
        kekka[[i]] <- NA
      }else{
        kekka[[i]] <- unlist(RMeCabC(dataf[i,coln], mypref, dic, mecabrc, etc))# 
      }
    }
    return(kekka)
  }
# 
