\name{docMatrix}
\alias{docMatrix}
\alias{docVector}
\title{docMatrix}
\description{
  creates a document-term matrix out of
all textfiles in a given directory.
}
\usage{
docMatrix( mydir, pos = "Default", minFreq = 1, weight ="no", kigo=0, co = 0)
docVector( filename, pos,  posN, minFreq, kigo , dic = "", mecabrc = "", etc = "")
}
\arguments{
  \item{filename}{filename (may include path).}
  \item{mydir}{the directory path to textfiles. }
  \item{pos}{specifies which parts of speech should be
	extracted. Default being  noun and adjective.}
   \item{posN}{specifies length of  parts of speech which should be extracted. }   
  \item{minFreq}{words of a document appearing less than minDocFreq
	within that document will be ignored.}
  \item{weight}{ Calculates a weighted document-term matrix with
	some options.}
  \item{kigo}{ if total must include number of symbols set kigo =
	1. Default being 0}
  \item{co}{retrieve co-ocurrence term matrix. default being = 0}
   \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab} 
%   \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
}
\details{
  All textfiles in the specified directory are read in and a matrix is composed.
  Every cell of the matrix shows the actual frequency of
each word. 
  
  \code{docVector()} is a supporting function that creates a
document-term frequency list.
  
%  For every generated matrix, an own environment is added as an
%  attribute which holds the triples that are stored by
%  \code{setTriple()} and can be  retrieved with \code{getTriple()}.
  
}
\value{
  \item{docMatrix}{the document-term matrix }
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
% % docMatrix(td)

% % # read them, drop german stopwords
% % data(stopwords_de)
% % docMatrix(td, stopwords=stopwords_de)

% % # read them based on a controlled vocabulary
% % voc = c("dog", "mouse")
% % docMatrix(td, vocabulary=voc, minWordLength=1)

% % # clean up
% % unlink(td, recursive=TRUE)

% }
\keyword{array}
