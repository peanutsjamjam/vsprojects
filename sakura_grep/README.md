# sakura_grep

## Windows のコマンドプロンプトで使う用の、grep コマンド


```
sgrep [-f "pattern;..."] [-hinRtv] word [dir]

  -f	pattern;... 検索するファイルのパターンを指定します。デフォルトは "*.cpp;*.h" です。

  -h

  -i

  -n

  -R

  -t	aaa

  word	aaa

  dir	検索対象のディレクトリを指定します。デフォルトは "." です。
```


* 要件：
	* サクラエディタがインストールされていて、sakura.exe の path が、環境変数 PATH に登録されていること。

* links 
	* サクラエディタ https://sakura-editor.github.io/
	* takamin さんの win-c https://github.com/takamin/win-c
