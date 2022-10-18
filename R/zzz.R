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
        packageStartupMessage("You need 64 bit version of MeCab with UTF8 dictionary on Windows.\nDowlnload it from 'https://github.com/ikegami-yukino/mecab/releases'\uninstall old 32 bit MeCab and install new 64 bit MeCab")
  }
