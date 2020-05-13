# RMeCab

## Interface to MeCab

日本語形態素解析MeCabインターフェイスのR言語用パッケージです。解析エンジン部分はC言語/C＋＋で作成されています。


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

次のように表示されれば、インストールは成功しています。
```
> unlist (res)
    名詞     助詞     名詞     助詞     名詞     助詞     名詞 
 "すもも"     "も"   "もも"     "も"   "もも"     "の"   "うち" 
 ```
 
 
MeCabのインストールから始めたいという方は、以下のサイトで、自身のOSに対応したセクションの解説を参照してください。


http://sites.google.com/site/rmecab/ でも説明しています(内容はこのページと同じです)。
