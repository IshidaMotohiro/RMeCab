\name{RMeCabDF}
\alias{RMeCabDF}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ RMeCabDF}
\description{
 \code{ RMeCabDF} takes data frames as the first argument, and analyzes
 the columns specified by the second argument. Blank data should be
 replaced with NA.  If 1 is designated as the third argument, it returns
 each morpheme in its basic form. 
}
\usage{
%  RMeCabDF(dataf, coln, mypref, mybuf)
  RMeCabDF(dataf, coln, mypref, dic = "", mecabrc = "", etc = "")
% 
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{dataf}{ data.frame}
  \item{coln}{Column number or name which include Japanese sentences }
  \item{mypref}{ Default being 0, the same morphemic forms that appear
	on the text are returned.  If 1 is designated, the basic forms of
	them are instead.}
  \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab}
%  \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
% %  \item{mybuf}{ enough buffer for japanese strings}
}
\details{
  If necessary, more details than the description above 
}
\value{
 generates and returns a new data frame.
%   If it is a LIST, use
%    \item{comp1 }{Description of 'comp1'}
%    \item{comp2 }{Description of 'comp2'}
%   ...
}
% \references{put references to the literature/web site here  }
\references{石田基広『Rによるテキストマイニング入門』森北出版 2008}
\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.com} }%\author{ishida motohiro }
% \note{ not
%  \section{Warning }{....} 
% }
\seealso{ objects to See Also as \code{\link{help}},  }
% \examples{
% %   \dontrun{
% %  z <- RMeCabDF(dat, 2)
% %   }
% }
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
