\name{docNgram2}
\alias{docNgram2}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ docNgram2 }
\description{
 \code{ docNgram2} returns returns data frame of N-gram from a file or all
 files in a given directory. Each word of N-gram makes one column. 
}
\usage{
  docNgram2(directory, type = 0, pos = "Default", minFreq = 1,N = 2, kigo = 0, weight = "no" , dic = "", mecabrc = "", etc = "")
%% pos = c("名詞","形容詞"), minFreq = 1,N = 2, sym = 0, kigo = "記号", 
%  function(directoy, ft = 1, type = 0, pos = c("名詞","形容詞"),  posN = 0, minFreq = 2, N = 2, sym = 0)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{directory}{directory path or a filename (may include path).}
  \item{type}{ Default being 0.  }
  \item{pos}{argument3. Default being  noun and adjective.}
%%   \item{posN}{argument4. Default being 0}
  \item{minFreq}{words of a document appearing less than minDocFreq
	within that document will be ignored.}
  \item{N}{N-gram. Default being 2}
%   \item{sym}{ if total must include number of symbols, set sym = 1. Default being 0}
  \item{kigo}{if total must include number of symbols, set sym = 1. Default being 0}
  \item{weight}{weight}
  \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab}  
%   \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
}
\details{
  If necessary, more details than the description above 
}
\value{
  returns a data frame.
%  Describe the value returned
%   If it is a LIST, use
%    \item{comp1 }{Description of 'comp1'}
%    \item{comp2 }{Description of 'comp2'}
%   ...
}
% \references{put references to the literature/web site here  }
\references{石田基広『Rによるテキストマイニング入門』森北出版 2008}

\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.com} }%\author{ishida motohiro }
% \note{ note 
%  \section{Warning }{....} 
% }
\seealso{ objects to See Also as \code{\link{help}},  }
% \examples{
% %   \dontrun{

% %   }
% }
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
