\name{docNgram}
\alias{docNgram}
\alias{makeNgram}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ docNgram }
\description{
 \code{ docNgram} returns data frame of N-gram based on file's directory.

}
\usage{
  docNgram(mydir, type = 1, N = 2, pos = "Default"  , dic = "", mecabrc = "", etc = "")
  makeNgram(filename, type = 1, N = 2, pos , dic = "", mecabrc = "", etc = "")
% 
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{mydir}{ files directory }
  \item{filename}{ a file }
  \item{type}{ Default being 0.  }
  \item{N}{N-gram. Default being 2}
  \item{pos}{part of speech. Default being  noun and adjective.}
  \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ not implemented (to specify mecab resource file) }
  \item{etc}{other options to mecab, not used now}  
  %   \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
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
%% \note{ note 
%% %  \section{Warning }{....} 
%% }
%% \seealso{ objects to See Also as \code{\link{help}},  }
%% \examples{
%% %   \dontrun{
%% %   }
%% }
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
