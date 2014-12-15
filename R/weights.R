#
# 2008 May 5 09:56
#
## ##  @Book{kitaB02,
##   author =	 {北 研二 and 津田 和彦 and 獅子堀 正幹},
##   yomi   ={Kenji Kita},
##   title = 	 {情報検索アルゴリズム},
##   publisher = 	 {共立出版},
##   year = 	 2002
## }
####################################
#
# 以下は行に単語という前提で計算する
# 
entropy <-
  function (m, tp=0) {
    if(tp == 1){
      # m <- removeInfo ( m ) 
      gf = colSums(m, na.rm = TRUE)  # 大域的頻度 F_i
      p = m / gf                     # 各出現頻度 / 大域頻度
      ndocs = nrow(m)                #文書数（行側に文章）
      entropy = - colSums( (p*log(p)) / log(ndocs), na.rm = TRUE )
    }else {
      m <- removeInfo ( m ) 
      gf = rowSums(m, na.rm = TRUE)  # 大域的頻度 F_i
      p = m / gf                     # 各出現頻度 / 大域頻度
      ndocs = ncol(m)                #文書数（列側に文章）
      entropy = - rowSums( (p*log(p)) / log(ndocs), na.rm = TRUE )
    }
    return ( entropy )
  }

####################################################
### local weights ###

# binary weights
localBin <-
  function(m) {
    m <- removeInfo ( m ) 
    return( (m>0)*1 )
  }

# termfrequency
localTF <-
  function(m) {
    m <- removeInfo ( m ) 
    return( m )
  }

# log'ed termfrequency
localLogTF <- function(m) {
  m <- removeInfo ( m ) 
  return( log(m+1) )
}

#
#######################################################
### glocal weights ###
#
# inverse document frequency

globalIDF <-
  function(m, tp=0) {

#    m <- removeInfo ( m ) 
#    df = rowSums(localBin(m), na.rm=TRUE)
#    return ( log2(ncol(m)/df)  )
    if(tp == 1){
      df0 <- colSums(localBin( m ), na.rm=TRUE)# 各単語を含む文書数 (文書頻度 n_i)
      return ( ( log2(nrow( m )/df0) + 1 ) )    # 全体の文書数を割る
    }else{
      df0 <- rowSums(localBin( m ), na.rm=TRUE)# 各単語を含む文書数 (文書頻度 n_i)
      return ( ( log2(ncol( removeInfo(m) )/df0 ) + 1 ) )
    }
  }


# global frequency * IDF
globalIDF2 <-
  function(m, tp=0) {
#     m <- removeInfo ( m )

    if(tp == 1){
      gf <- colSums( m  , na.rm = TRUE)# 大域的頻度 F_i
      df <- colSums(localBin( m ), na.rm=TRUE)      # 文書頻度  n_i
    }else{
      gf <- rowSums( removeInfo( m ) , na.rm = TRUE)# 大域的頻度 F_i
      df <- rowSums(localBin( m ), na.rm=TRUE)      # 文書頻度  n_i
    }
    return ( gf/df )
  }

# probabilistic IDF
globalIDF3 <-
  function(m, tp=0) {
#     m <- removeInfo ( m )
    if(tp == 1){
      df <- colSums(localBin( m ), na.rm=TRUE)# 各単語を含む文書数 (文書頻度)
      return ( log2 ( nrow( m - df) /df  ) )# 行側に文書
      
    }else{
      df <- rowSums(localBin( m ), na.rm=TRUE)# 各単語を含む文書数 (文書頻度)
      return ( log2 ( ncol( removeInfo(m) - df) /df  ) )
    }
    
  }

# global weighting = 1 + entropy
globalEntropy <-
  function(m, tp=0) {
#    m <- removeInfo ( m )
    
    return ( (1 - entropy( m )) )
  }


## # normalisation
globalNorm <-
  function(m, tp=0) {
    if(tp == 1){
      return ( 1 / sqrt( rowSums(( m*m ), na.rm = TRUE) ) )# 各行をノルムで割る
    }else{
      m <- removeInfo ( m ) 
      return ( 1 / sqrt( colSums(( m*m ), na.rm = TRUE) ) )# 各列をノルムで割る
    }
  }

mynorm <-
  function(m, tp=0){
    if(tp == 1){
      return ( 1 / sqrt( rowSums(( m*m ), na.rm = TRUE) ) )# 各行をノルムで割る
    }else{
      return ( 1 / sqrt( colSums(( m*m ), na.rm = TRUE) ) )# 各列をノルムで割る
    }
  }

removeInfo <-
  function(m){
    if(!is.matrix( m) &&  !is.data.frame(m) ){
      stop("first argument must be matrix! or data.frame")
    }
    grep1 <- grep("TOTAL-TOKENS",  rownames( m ))
    grep2 <- grep("LESS-THAN-",    rownames( m ))

    if(length(grep1) == 0 & length(grep2) == 0) {
      return (m)
    } else{ 
      return( m [-c(grep1, grep2), ]  )
    }
    return(m)

  }
#################
## /esearch/PublicPapers/TermDocWeighting
