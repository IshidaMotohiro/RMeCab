
## @Book{barnbrook96:_languag_comput,
##   author =		 {Geoff Barnbrook},
##   title = 		 {Language and Computers},
##   publisher = 	 {Edinburgh},
##   year = 		 1996
## }

collScores <-
  function(kekka, node, span = 0){
##   # kekka が collocate 関数の出力かどうかを判断する必要あり
##   # クラス設計完成させるまでは以下でごまかし
  if(  !is.data.frame(kekka) || (num2 <-  which(kekka$Term == "[[MORPHEMS]]")) < 1){
    stop("first argument must be result of collocate()")
  }
  if(nchar(node) < 1){
        stop("second argument (node)  must be a node morpheme")
  }
  if(span == 0){
    stop("third argument (span)  must be specified")
  }

  num1 <- which(kekka$Term == node)                 ## node の行数
  num3 <-  which(kekka$Term == "[[TOKENS]]")        ## 総語数の行数
  spanTokens <- span * 2 *  kekka[num1, "Total"]    ## node の出現回数 * span (前) * span (後)

  ##### T-scoore
   tscore <- (kekka[, "Span"] - ( kekka[, "Total"] / kekka[num3, "Total"] * spanTokens)) / sqrt(kekka[, "Span"]) # (実測値 - 共起語が span に出現する期待値) / 実測値の平方根 ## Barbnrook p.93, p.97
  tscore[c(num1, num2, num3)] <- NA
  kekka$T <- tscore

  ##### MI-score
  mutual <- log2( kekka[, "Span"] /   ( kekka[, "Total"] / kekka[num3, "Total"] * spanTokens) ) # (実測値/ 期待値)の対数
  mutual[c(num1, num2, num3)] <- NA
  kekka$MI <- mutual

  return(kekka)
  
## ##   ########################################
  

## ##   # nodeTotal <-  kekka[num1, "Total"]
## ##   InSpanTotal <-  kekka[num3, "Span"] - kekka[num1, "Span"]# node の頻度を引く
## ##   AllTotal <- kekka[num3, "Total"] - kekka[num1, "Total"]# node の頻度を引く
  
## ## ## #  tscore <- (kekka[-num, "Span"] - (kekka[-num, "Total"] * kekka[num, "Total"] / kekka[num3,"Total"] )) / sqrt(kekka[-num, "Span"])

## ## ## #  tscore <- (kekka[, "Span"] - (kekka[, "Total"] * kekka[num1, "Total"] / kekka[num3,"Total"] )) / sqrt(kekka[, "Span"])
## ##    tscore <- (kekka[, "Span"] - ( kekka[, "Total"] * kekka[num1, "Total"] / AllTotal )) / sqrt(kekka[, "Span"])
 
## ##   tscore[c(num1, num2, num3)] <- NA
## ##   kekka$T <- tscore

## ## ##   # mutual <- log2(  (kekka[, "Span"]* kekka[num3,"Total"] ) /  kekka[num1, "Total"] * kekka[, "Total"])


## ##   mutual <- log2(  (kekka[, "Span"]* AllTotal) /  kekka[num1, "Total"] * kekka[, "Total"])  
## ##   mutual[c(num1, num2, num3)] <- NA 
         
## ##   kekka$MI <- mutual
  
## ## ## ####
## ########## < g2>
## ##   # 以下は，lappy を使った効率的な処理に実装し直す予定
## ##   g2 <- as.numeric(length(kekka$Term))
  
## ##   for(i in 1: (length(kekka$Term)-2) ){# lappy で実装し直す予定
## ##     if (i == num1){## (080509)
## ##       next
## ##     }
## ##     left1 <- kekka[i, "Span"]#  i 番目の Term と node との共起頻度
## ##     # ? right1 <- kekka[num3,"Total"] - left1 # nodeの総共起回数 - left 
## ##     right1 <- kekka[num1, "Total"] - left1 # node の総頻度 - left1 (080509)
## ## ##     # right1 <- kekka[num1, "Total"] - left1 # node の総頻度 - left1
## ## ##     # right1 <- kekka[i,"Total"] -   kekka[i, "Span"]#  i 番目の Term のテキスト全体での総頻度から node との共起頻度を引く
## ##     # ? left2 <- 
## ##     left2 <-  kekka[i, "Total"] - left1 # 共起語の総語数 - node との共起頻度(080509)
## ## ##     #left2 <- InSpanTotal -  kekka[i, "Span"]#
## ## ##     #left2 <- kekka[num3, "Span"] -  kekka[i, "Span"]# node総頻度から対象との共起頻度を引く
## ##     right2 <-  AllTotal - left1 - right1 - left2 # 総token数 - 他三つのセル
## ## ##     # right2 <-  kekka[num3,"Total"] - left1 - right1 - left2 # テキスト全体での頻度から，他のセルの度数を引く
## ## ##     ## print(i); 
## ##     (testMt <- matrix(c(left1, left2, right1, right2), ncol = 2))
## ##     suppressWarnings (g2[i] <- loglin(testMt, c(1,2))$lrt )##
## ##   }

## ##   g2[c(num1, num2, num3)] <- NA   
## ##   kekka$G2 <- g2

## ##   ############## </ g2>
  
}  
