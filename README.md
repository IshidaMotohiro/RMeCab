# RMeCab



日本語形態素解析器MeCabインターフェイスのR言語用パッケージです。解析エンジン部分はC言語/C＋＋で作成されています。

## MeCabインストール

MeCab をインストールする必要があります。ここ https://rmecab.jp/new/ を参照してください。

## RMeCab インストール



R と MeCab の両方が既にインストールされているのならば R を起動し、コンソールで以下を実行すればインストールは完了です。

## Windowsの場合

> install.packages("RMeCab", repos = "https://rmecab.jp/R") 

## Mac あるいは Linux の場合 

> install.packages("RMeCab", repos = "https://rmecab.jp/R", type = "source") 

あるいは GitHub からインストールもできます。

### Windows で Rtools4 と MeCab をすでにインストールしている場合、あるいはMac あるいは Linux の場合

> install.packages("remotes")
> remotes::install_github("IshidaMotohiro/RMeCab")


実行できるか、以下を試してみましょう（辞書として neologd を利用している場合、一語に分類されます）。

> library(RMeCab)
> res <- RMeCabC("すもももももももものうち")
> unlist (res)

次のように表示されれば、インストールに成功しています。

> unlist (res)
    名詞     助詞     名詞     助詞     名詞     助詞     名詞 
 "すもも"     "も"   "もも"     "も"   "もも"     "の"   "うち" 

MeCabのインストールから始めたいという方は、以下のサイトで、自身のOSに対応したセクションの解説を参照してください。

http://sites.google.com/site/rmecab/ でも説明しています。


## Neologd 辞書

以下のようにインストールします。

> git clone --depth 1 https://github.com/neologd/mecab-ipadic-neologd.git
> cd mecab-ipadic-neologd
> ./bin/install-mecab-ipadic-neologd -n

インストール先の確認

> mecab -d /usr/lib/mecab/dic/mecab-ipadic-neologd

動作確認

> echo "8月3日に放送された「中居正広の金曜日のスマイルたちへ」(TBS系)で、1日たった5分でぽっこりおなかを解消するというダイエット方法を紹介。キンタ ロー。にも密着。" | mecab

###how to use neologd with RMeCab

open /usr/local/etc/mecabrc or copy the file to your home folder as .mecabrc and edit as follows.

;
; Configuration file of MeCab
;
; $Id: mecabrc.in,v 1.3 2006/05/29 15:36:08 taku-ku Exp $;
;
; dicdir =  /usr/local/lib/mecab/dic/ipadic
dicdir =  /usr/local/lib/mecab/dic/mecab-ipadic-neologd
 
たとえば、コマンドラインで以下のように操作することで、ファイルを編集できます。

> cp /usr/local/etc/mecabrc ~/.mecabrc
> sed -i -e ‘s/dicdir = \/usr\/local\/lib\/mecab\/dic\/ipadic/dicdir = \/usr\/local\/lib\/mecab\/dic\/mecab-ipadic-neologd/’ ~/.mecabrc
