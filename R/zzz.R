## .onLoad <- function(libname, pkgname)
##   {

## }

## .onUnLoad <- function(libname, pkgname)
## {

## }

#' @title .onAttach
#' @description prints out a friendly reminder message to the user when
#'              the RAQSAPI library is loaded.
#' @inheritParams base .onAttech
#' @return NULL
#' @noRd
.onAttach <- function(libname, pkgname)
{
    if(.Platform$OS.type == "windows")
        packageStartupMessage("You need 64 bit version of MeCab for Winndows, with UTF8 dictionary\nDowlnload it from 'https://github.com/ikegami-yukino/mecab/releases/tag/v0.996'")
  }
