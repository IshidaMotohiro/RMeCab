# 形態素解析の結果から「記号」を取る
rmSign <- function (x){
  if(!is.list(x)){
    stop("x must be a list")
  }
  for(i in 1:length(x)){
    if(any(is.na(x[[i]]))){
      x[[i]] <- NA
    }
    else{
      tmp <- NULL
      for(j in 1:length(x[[i]])){
          if(names(x[[i]][j]) != enc2native("\u8a18\u53f7") ){#記号 iconv("\u8a18\u53f7", "", "")
          tmp <- c(tmp, j)
        }
      }
      x[[i]] <- x[[i]] [tmp]
      tmp <- NULL
    }
  }
  return (x)
}
