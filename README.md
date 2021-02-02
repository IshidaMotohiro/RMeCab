# RMeCab

## Interface to MeCab

This is a package of Japanese morphological analyzer MeCab interface for R language. The analysis engine part is written in C/C++.


## install mecab

For Mac or Linux

```
$ cd ~/Downloads
$ curl -fsSL ‘https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7cENtOXlicTFaRUE’ -o mecab-0.996.tar.gz
$ tar xf mecab-0.996.tar.gz
$ cd mecab-0.996
$ ./configure –with-charset=utf8
$ make
$ sudo make install
$ sudo ldconfig
$ cd ~/Downloads
$ curl -fsSL ‘https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7MWVlSDBCSXZMTXM’ -o mecab-ipadic-2.7.0-20070801.tar.gz
$ tar zvxf mecab-ipadic-2.7.0-20070801.tar.gz
$ tar xf mecab-ipadic-2.7.0-20070801.tar.gz
$ cd mecab-ipadic-2.7.0-20070801
$ ./configure –with-charset=utf-8
$ make
$ sudo make install
$ sudo ldconfig
```


https://rmecab.jp/new/ 


### install neologd(option)



```
git clone --depth 1 https://github.com/neologd/mecab-ipadic-neologd.git
cd mecab-ipadic-neologd
./bin/install-mecab-ipadic-neologd -n
```

```
mecab -d /usr/lib/mecab/dic/mecab-ipadic-neologd
```


### how to use neologd with RMeCab


open /usr/local/etc/mecabrc or copy the file to your home folder as .mecabrc 
and edit as follows.

```
;
; Configuration file of MeCab
;
; $Id: mecabrc.in,v 1.3 2006/05/29 15:36:08 taku-ku Exp $;
;
; dicdir =  /usr/local/lib/mecab/dic/ipadic
dicdir =  /usr/local/lib/mecab/dic/mecab-ipadic-neologd
 
```
