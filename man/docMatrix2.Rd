\name{docMatrix2}
\alias{docMatrix2}
\title{docMatrix2}
\description{
  creates a document-term matrix out of a file or 
all textfiles in a given directory.
}
\usage{
docMatrix2(directory, pos= "Default",  minFreq = 1, weight = "no", kigo
= 0, co = 0 , dic = "", mecabrc = "", etc = "" )%% pos= c("名詞","形容詞"kigo = "記号", sym = 0,
}
\arguments{
  \item{directory}{directory path or a filename (may include path).}
  \item{pos}{specifies which parts of speech should be extracted. Default being  noun and adjective.}
%%   \item{sym}{ if total must include number of symbols, set sym = 1. Default being 0}
  \item{minFreq}{words of a document appearing less than minDocFreq
	within that document will be ignored.}
  \item{weight}{ Calculates a weighted document-term matrix with
	some options.}
  \item{kigo}{if total must include number of symbols, set sym = 1. Default being 0}
  \item{co}{retrieve co-ocurrence term matrix. default being = 0}
  \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab}
%  \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
}
\details{
  All textfiles in the specified directory are read in and a matrix is composed.
  Every cell of the matrix shows the actual frequency of
  each word.
  }
  
%   \code{
  
% %  For every generated matrix, an own environment is added as an
% %  attribute which holds the triples that are stored by
% %  \code{setTriple()} and can be  retrieved with \code{getTriple()}.
  
% }

\value{
  \item{docMatrix2}{the document-term matrix }
}

\references{石田基広『Rによるテキストマイニング入門』森北出版 2008}

\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.comt} }
% \seealso{ \code{\link[Rstem]{wordStem}}, \code{\link{stopwords_de}}, \code{\link{stopwords_en}}, \code{\link{setTriple}}, \code{\link{getTriple}}}
% \examples{

% % # create some files
% % td = tempfile()
% % dir.create(td)
% % write( c("dog", "cat", "mouse"), file=paste(td, "D1", sep="/") )
% % write( c("hamster", "mouse", "sushi"), file=paste(td, "D2", sep="/") )
% % write( c("dog", "monster", "monster"), file=paste(td, "D3", sep="/") )

% % # read them, create a document-term matrix
% % docMatrix2(td)

% % # read them, drop german stopwords
% % data(stopwords_de)
% % docMatrix2(td, stopwords=stopwords_de)

% % # read them based on a controlled vocabulary
% % voc = c("dog", "mouse")
% % docMatrix2(td, vocabulary=voc, minWordLength=1)

% % # clean up
% % unlink(td, recursive=TRUE)

% }
\keyword{array}
