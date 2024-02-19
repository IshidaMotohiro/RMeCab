.onAttach <- function(libname, pkgname) {
  if (.Platform$OS.type == "windows") {
    packageStartupMessage(
      paste(
        "You need 64-bit version of MeCab with UTF-8 dictionary on Windows.",
        "Download it from 'https://github.com/ikegami-yukino/mecab/releases',",
        "uninstall old 32-bit MeCab and install new 64-bit MeCab.",
        sep = "\n"
      )
    )
  }
}
