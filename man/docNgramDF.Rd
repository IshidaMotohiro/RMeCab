
\name{docNgramDF}
\alias{docNgramDF}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ docNgramDF }
\description{
 \code{ docNgramDF} returns returns data frame of N-gram from a file or all
 files in a given dataframe. Each word of N-gram makes one column. 
}
\usage{
  docNgramDF(mojiVec = "MeCab", type = 0, pos = "Default", baseform =0, minFreq = 1, N = 1, kigo = 0, weight = "no", co = 0 , dic = "", mecabrc = "", etc = "" )
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{mojiVec}{column of dataframe}
  \item{type}{ Default being 0.  }
  \item{pos}{argument3. Default being  noun and adjective.}
  \item{baseform}{}
  \item{minFreq}{words of a document appearing less than minDocFreq within that document will be ignored.}
  \item{N}{N-gram. Default being 1}
  \item{kigo}{ if total must include number of symbols, set sym = 1. Default being 0}
  \item{weight}{ see weight.R}
  \item{co}{to make co-occurence matrix.}
   \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ not implemented (to specify mecab resource file)  }
  \item{etc}{other options to mecab, not used now}
  %%   \item{posN}{argument4. Default being 0}
  %   \item{sym}{ if total must include number of symbols, set sym = 1. Default being 0}
  %  \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
}
%% \details{
%%   If necessary, more details than the description above 
%% }
\value{
  returns a data frame.
}
% \references{put references to the literature/web site here  }
\references{  \url{http://www.amazon.co.jp/dp/4627848420}  }
\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.com} }%\author{ishida motohiro }
% \note{ note 
%  \section{Warning }{....} 
% }
% \seealso{ objects to See Also as \code{\link{help}},  }
%\examples{
%   \dontrun{
%   }
%}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation dataframe.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
