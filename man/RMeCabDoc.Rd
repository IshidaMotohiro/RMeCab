\name{RMeCabDoc}
\alias{RMeCabDoc}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ RMeCabDoc }
\description{
 \code{ RMeCabDoc} makes MeCab analyze the specified Japanese text
 file, and returns its result in the form of a list. 
}
\usage{
  RMeCabDoc(filename, mypref= 1, kigo = 0, dic = "", mecabrc = "", etc = "")
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{filename}{Japanese text}
    \item{mypref}{ Default being 1, the basic forms of
	 the same morphem are returned.  If 0 is designated,  the same
	 morphemic forms that appear on the text.  }
   \item{kigo}{include symbols if 1 set.}
   \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab}  
%    \item{mydic}{ to specify user dictionary, e.x. ishida.dic }
}
\details{
  If necessary, more details than the description above 
}
\value{
  returns a list.
%   Describe the value returned
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
% %  z <- RMeCabDoc("dat1.txt")
% %   }
% }
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
