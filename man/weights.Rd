\name{weights}
\alias{entropy}
\alias{localTF}
\alias{localBin}
\alias{localLogTF}
\alias{globalIDF}
\alias{globalIDF2}
\alias{globalIDF3}
\alias{globalNorm}
\alias{globalEntropy}
\alias{removeInfo}
\alias{mynorm}
% \alias{gw_gfidf}
% \alias{entropy}
% \alias{gw_entropy}
\title{Weighting Matrix (Matrices) options}
\description{
  Calculates a weighted document-term matrix according to the chosen local and/or global weighting options.
}
\usage{
    entropy(m, tp=0)
    localTF(m)
    localBin(m)
    localLogTF(m)
    globalIDF(m, tp=0)
    globalIDF2(m, tp=0)
    globalIDF3(m, tp=0)
    globalNorm(m, tp=0)
    globalEntropy(m, tp=0)
    removeInfo (m)
    mynorm(m, tp=0)
%     gw_gfidf(m)
%     entropy(m)
%     gw_entropy(m)
}
\arguments{
   \item{m}{a document-term matrix.}
}
\details{
%    When combining a local and a global weighting options to be applied on a 
%    given textmatrix \code{m} via \eqn{dtm = lw(m) \cdot gw(m)}, where
   
%    \itemize{
%     \item \eqn{m} is the given document-term matrix,
    
%     \item \eqn{lw(m)} is one of the local weight functions
% 	\code{localBin}, \code{localTF} and \code{localLogTF}
    
%     \item \eqn{gw(m)} is one of the global weight functions \code{globalIDF()}, \code{globalIDF2()}, \code{globalEntropy()}}.
%    }
   
%    This set of weighting schemes includes the local weightings (lw)
%    raw, log, binary and the global weightings% (gw) normalisation,
%    two versions of the 
%    inverse document frequency (idf), and entropy
%    %%in both the original Shannon as well as  in a slightly modified, more common version:
%       \code{localTF()} returns a completely unmodified \eqn{n \times m} matrix (placebo function).
   
%    \code{localBin()} returns binary values of the \eqn{n \times m} matrix. Every cell is assigned 1, iff the term frequency is not equal to 0.
      
%     \code{localLogTF()} returns the logarithmised \eqn{n \times m} matrix. \eqn{log(m_{i,j}+1)} is applied on every cell.

% %%    \code{gw\_normalisation()} returns a normalised \eqn{n \times m} matrix. Every cell equals 1 divided by the square root of the document vector length.
   
%    \code{globalIDF()} returns the inverse document frequency in a \eqn{n \times m} matrix. Every cell is 1 plus the logarithmus of the number of documents divided by the number of documents where the term appears.
   
%    \code{globalIDF2()} returns the global frequency multiplied with idf. Every cell equals the sum of the frequencies of one term divided by the number of documents where the term shows up.
   
%    \code{globalEntropy()} returns the entropy.% (as defined by Shannon).
   
% %%   \code{gw\_entropy()} returns one plus entropy.

}
\value{
    Returns the weighted textmatrix of the same size and format as the input matrix.
}
\references{
% Dumais, S. (1992) \emph{Enhancing Performance in Latent Semantic Indexing (LSI) Retrieval}. Technical Report, Bellcore.

% Nakov, P., Popova, A., and Mateev, P. (2001) \emph{Weight functions impact on LSA performance}. In: Proceedings of the Recent Advances in Natural language processing, Bulgaria, pp.187-193.

% Shannon, C. (1948) \emph{A Mathematical Theory of Communication}. In:
% The Bell System Technical Journal 27(July), pp.379--423.


}
\author{ Motohiro ISHIDA \email{ishida.motohiro@gmail.com} }
\examples{

% ## use the logarithmised term frequency as local weight and 
% ## the inverse document frequency as global weight.

% vec1 = c( 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
% vec2 = c( 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0 )
% vec3 = c( 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0 )
% m = cbind(vec1,vec2, vec3)
% weighted = localTF(m) * globalIDF(m)

}
\keyword{array}
