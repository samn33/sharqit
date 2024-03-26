インストール
===========

## 必要なソフトウェア

Sharqitの動作に必要となる以下のソフトウェアをインストールします。

    $ sudo apt install nlohmann-json3-dev 
	$ sudo apt install libeigen3-dev
	$ sudo apt install graphviz

Pythonパッケージを使用する場合、以下のパッケージも必要です。

    $ pip install nanobind

## コマンドラインツールとC++ライブラリ

以下のようにコマンドラインツールとC++ライブラリおよび関連するヘッダーファイルをインストールします。

	$ git clone https://github.com/samn33/sharqit.git
    $ cd sharqit/sharqit/cpp
    $ mkdir -p ~/lib ~/include ~/bin
    $ make
    $ make install

コマンドラインツールとC++ライブラリを使用可能とするために、以下の環境変数を設定します。

    export LD_LIBRARY_PATH="${HOME}/lib:$LD_LIBRARY_PATH"
    export PATH="${HOME}/bin:$PATH"

ここで、bashの場合の例を示しましたが、別のシェルの場合は適宜読み替えてください。

アンインストールする場合は、以下のようにします。
これで、sharqitコマンドとsharqitライブラリ(およびヘッダーファイル)が削除されます。

    $ make uninstall

## Pythonパッケージ

以下のようにsharqitパッケージをビルドしてインストールします。

    $ git clone https://github.com/samn33/sharqit.git
    $ cd sharqit
    $ python setup.py install --user

アンインストールする場合は、以下のようにします。

    $ pip uninstall sharqit
