\name{RMeCabFreq}
\alias{RMeCabFreq}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ RMeCabFreq }
\description{
 \code{ RMeCabFreq} takes Japanese text files as arguments, restores
 every morpheme to its basic form, and returns its part of speech and frequency as data frames.
}
\usage{
  RMeCabFreq(filename, dic = "", mecabrc = "", etc = "")

}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{filename}{ Japanese text}
  \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab} 
%  \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
%%   \item{dfY}{returns data.frame}  
}
\details{
  If necessary, more details than the description above 
}
\value{
  returns a data frame.
%   Describe the value returned
%   If it is a LIST, use
%    \item{comp1 }{Description of 'comp1'}
%    \item{comp2 }{Description of 'comp2'}
% %
...
}
%% \references{put references to the literature/web site here  }
\references{石田基広『Rによるテキストマイニング入門』森北出版 2008}

\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.com} }%\author{ishida motohiro }
% \note{urther not 
%  \section{Warning }{....} 
% }
\seealso{ objects to See Also as \code{\link{help}},  }
% \examples{
% %   \dontrun{
% %  dat <- read.csv("photo.csv", head = T)
% %  res <- RMeCabFreq(dat)
% %   }
% }
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
