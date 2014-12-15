\name{RMeCabC}
\alias{RMeCabC}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ RMeCabC }
\description{
 \code{ RMeCabC} makes MeCab analyze Japanese sentences, and
returns its results in the form of a list.  If 1 is designated as the
second argument, it returns each morpheme in its basic form.
% Japanese-sentence default value has been set up at 256 characters.  If
% longer sentences are included, the forth argument can be raised up to 102400.
}
\usage{
%  RMeCabC(str, mypref, mybuf)
  RMeCabC(str, mypref, dic = "", mecabrc = "", etc = "")
% 
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{str}{ japanese strings(sentences)}
  \item{mypref}{ Default being 0, the same morphemic forms that appear
	on the text are returned.  If 1 is designated, the basic forms of
	them are instead.  }
  \item{dic}{ to specify user dictionary, e.x. ishida.dic }
  \item{mecabrc}{ to specify mecab resource file }
  \item{etc}{other options to mecab}
%  \item{mybuf}{ enough buffer for japanese strings}
}
\details{
  If necessary, more details than the description above 
}
\value{
  returns a list.
%  Describe the value returned
%   If it is a LIST, use
%    \item{comp1 }{Description of 'comp1'}
%    \item{comp2 }{Description of 'comp2'}
%   ...
}

\references{石田基広『Rによるテキストマイニング入門』森北出版 2008}
\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.com} }%\author{ishida motohiro }
% \note{ urther not 
%  \section{Warning }{....} 
% }
\seealso{ objects to See Also as \code{\link{help}},  }
% \examples{
%   \dontrun{
% % z <- RMeCabC("すもももももももものうち",1,512)
%  z <- RMeCabC("すもももももももものうち",1) % the third arg omitted
%   }
% }
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ misc }
% \keyword{  }% __ONLY ONE__ keyword per line
