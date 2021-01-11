# RMeCab

## Interface to MeCab

日本語形態素解析器MeCabインターフェイスのR言語用パッケージです。解析エンジン部分はC言語/C＋＋で作成されています。


## インストール


R と MeCab の両方が既にインストールされているのならば R を起動し、コンソールで以下を実行すればインストールは完了です。

```
 # Windowsの場合
 install.packages("RMeCab", repos = "https://rmecab.jp/R") 
 # Mac あるいは Linux の場合 
 install.packages("RMeCab", repos = "https://rmecab.jp/R", type = "source") 
 ```
 
実行できるか、以下を試してみましょう。

```
library(RMeCab)
res <- RMeCabC("すもももももももものうち")
unlist (res)
```

次のように表示されれば、インストールに成功しています。

```
> unlist (res)
    名詞     助詞     名詞     助詞     名詞     助詞     名詞 
 "すもも"     "も"   "もも"     "も"   "もも"     "の"   "うち" 
 ```
 
 
MeCabのインストールから始めたいという方は、以下のサイトで、自身のOSに対応したセクションの解説を参照してください。


http://sites.google.com/site/rmecab/ でも説明しています。

### Neologd 辞書

以下のようにインストールします。

```
git clone --depth 1 https://github.com/neologd/mecab-ipadic-neologd.git
cd mecab-ipadic-neologd
./bin/install-mecab-ipadic-neologd -n
```

インストール先の確認

```
mecab -d /usr/lib/mecab/dic/mecab-ipadic-neologd
```

動作確認

```
echo "8月3日に放送された「中居正広の金曜日のスマイルたちへ」(TBS系)で、1日たった5分でぽっこりおなかを解消するというダイエット方法を紹介。キンタ ロー。にも密着。" | mecab

```

### RMeCabでNeologdを使う準備


/usr/local/etc/mecabrc を開いて以下のように変更するか、オリジナルのこのmecabrcファイルをホームフォルダに .mecabrc という隠しファイルとして保存し、これを以下のように変更してください。

```
;
; Configuration file of MeCab
;
; $Id: mecabrc.in,v 1.3 2006/05/29 15:36:08 taku-ku Exp $;
;
; dicdir =  /usr/local/lib/mecab/dic/ipadic
dicdir =  /usr/local/lib/mecab/dic/mecab-ipadic-neologd
 
```
