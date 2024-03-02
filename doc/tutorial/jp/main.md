チュートリアル
==============

## Sharqitとは

sharqitは量子回路最適化のためのC++ライブラリです。現在実装されている機能は以下の2つです。

- ZX-calculusを用いたTゲートの削減
- Phase Polynomialを用いた量子ゲートの削減

また、このライブラリの機能をシェルから実行できるコマンドラインツールも提供しています。


## インストール

まず、sharqitの動作に必要となる以下のソフトウェアをインストールします。

    $ sudo apt install nlohmann-json3-dev 
	$ sudo apt install libeigen3-dev
	$ sudo apt install graphviz

次に、sharqitをビルドして、インストールします。

	$ git clone https://github.com/samn33/sharqit.git
    $ cd sharqit/src
    $ mkdir -p ~/lib ~/include ~/bin
    $ make
    $ make install

CMakeでインストールすることもできます。

    $ git clone https://github.com/samn33/sharqit.git
    $ mkdir -p ~/lib ~/bin ~/include/sharqit
    $ cd sharqit; mkdir build; cd build
    $ cmake ..
    $ make
    $ make install

sharqtiライブラリとsharqitコマンドを使用可能とするために、以下の環境変数を設定します。

    export LD_LIBRARY_PATH="${HOME}/lib:$LD_LIBRARY_PATH"
    export PATH="${HOME}/bin:$PATH"

ここで、bashの場合の例を示しましたが、別のシェルの場合は適宜読み替えてください。

アンインストールする場合は、以下のようにします。
これで、SharqitコマンドとSharqitライブラリ(およびインクルードファイル)が削除されます。

    $ make uninstall


## Sharqitコマンド

### 量子回路のファイル形式

最適化したい量子回路を以下のように用意します。

    $ cat sample.sqc
    T 1
    H 0
    H 1
    CX 0 1
    H 0
    H 1
    T+ 1

これは、sharqitで定義している独自形式のテキストファイルです。
最初の行はTゲートを1番目の量子ビットに演算することを表しています。
2番目の行はHゲート(アダマールゲート)を0番目の量子ゲートに演算することを表しています。
以下同様に、Hゲートを1番目、CNOTゲートを0番目(制御側)と1番目(標的側)、
Hゲートを0番目、Hゲートを1番目、T+ゲートを1番目の量子ビットに
次々に演算することを表しています。

対応している量子ゲートは、X,Z,H,S,S+,T,T+,RZ,CX,CZの10種類です。
ここで、S+とT+は各々S,Tのエルミート共役を表します。
RZはZ軸周りの回転を表しています。回転角は、

    RZ(1/2) 0
    RZ(-3/4) 1
    RZ(7/4) 2
    ...

のように括弧内の分数で表します。
角度の単位はPIラジアンです。したがって1/2はPI/2、-3/4は-3PI/4を表します。

### 量子回路図の表示

さて、このような記号の羅列では量子回路のイメージがつかみにくいです。
そのため、sharqitコマンドでは--showオプションが用意されていて、
以下のように多少グラフィカルに量子回路を表示することができます。

    $ sharqit --show sample.sqc
	q[0] --H-----*--H------
	q[1] --T--H--X--H--T+--

### 量子回路の統計情報

各量子ゲートの数を知りたい場合、--statsオプションが使えます。

    $ sharqit --stats sample.sqc
    X_count  = 0
    Z_count  = 0
    H_count  = 4
    S_count  = 0
    T_count  = 2
    RZ_count = 2
    CX_count = 1
    2Q_count = 1
    gate_count = 7
    depth      = 5
    qubit_num  = 2

ここで、X-count, Z-count, H-count, S-count, T-count, RZ-count, CX-count, 2Q-count, gate-count, depth, qubit_numは各々、

- X-count ... Xゲートの数
- Z-count ... Zゲートの数
- H-count ... Hゲートの数
- S-count ... SおよびS+ゲートの数
- T-count ... TおよびT+ゲートの数
- RZ-count ... RZゲートの数
- CX-count ... CNOTゲートの数
- 2Q-count ... 2量子ビットゲートの数(=CNOTおよびCZゲートの数)
- gate-count ... トータルのゲート数
- depth ... 回路の深さ
- qubit_num ... 量子ビット数

を表しています。

### 量子回路の最適化

量子回路を最適化するのは、sharqitコマンドの--optオプションです。
結果の量子回路は標準出力に出力されるので、
以下のように(例えばfoo.sqcという)ファイルにリダイレクトするのが通常の使い方です。

    $ sharqit --opt sample.sqc > foo.sqc
    [zx diagram]
    xspider       = 1 -> 0
    zspider       = 1 -> 4
    clifford      = 6 -> 8
    non-clifford  = 0 -> 0
    hadamard      = 0 -> 3
    [quantum circuit]
    X-count  = 0 -> 0
    Z-count  = 0 -> 0
    H-count  = 4 -> 0
    S-count  = 0 -> 0
    T-count  = 2 -> 0
    RZ-count = 2 -> 0
    CX-count = 1 -> 1
    2Q-count = 1 -> 1
    gate-count = 7 -> 1
    depth      = 5 -> 1
    qubit_num  = 2 -> 2
    [info]
    proc time (sec)   = 0.000000
    kind of optimizer = ZX-calculus

標準エラー出力にいろいろ出力されますが一旦置いておいて(詳細後述)、foo.sqcがどうなっているか--showオプションで見てみます。

    $ sharqit --show foo.sqc
    q[0] --X--
    q[1] --*--

というわけで、正しく等価な量子回路が取得できました。

ところで、sharqitには、

- ZX-calculusを用いたTゲートの削減
- Phase Polynomialを用いた量子ゲートの削減

という2つの最適化機能が搭載されていると「Sharqitとは」で述べました。
実は、上の--optオプションで計算したのはZX-calculusを用いた方法になります。
Phase Polynomialを用いた方法は以下のように--opt=ppオプションで計算できます(ZX-calculusの場合は--opt=zxでも可)。

    $ sharqit --opt=pp sample.sqc > foo.sqc
    [quantum circuit]
	X-count  = 0 -> 0
    Z-count  = 0 -> 0
    H-count  = 4 -> 0
    S-count  = 0 -> 0
    T-count  = 2 -> 0
    RZ-count = 2 -> 0
    CX-count = 1 -> 1
    2Q-count = 1 -> 1
    gate-count = 7 -> 1
    depth      = 5 -> 1
    qubit_num  = 2 -> 2
    [info]
    proc time (sec)   = 0.000000
    kind of optimizer = Phase Polynomial

結果を--showオプションで見ると、

    $ sharqit --show foo.sqc
    q[0] --X--
    q[1] --*--

となり、この回路の場合はZX-calculusと同じ結果になります。
が、両者最適化の方針が異なっているため、一般には異なった結果を出力します。

おおまかには、ZX-calculusを用いた方法は、
Tゲートを削減することに重きがおかれたアルゴリズムになっているため、
Tゲートの削減率は良い半面、CNOTなどの2量子ビットゲートを多数吐き出す傾向があります。
トータルのゲート数が元のゲート数よりも多くなってしまう場合もあります。
一方のPhase Polynomialを用いた方法は、
ZX-calculusほどTゲートの削減率は良くない傾向ですが削減はされます(元のTゲート数を上回ることはないです、多分)。
また、CNOTゲートも削減されます(元のCNOTゲート数を上回ることはないです、多分)。

両手法とも、まだ実装上の改善の余地はあります。
[README.md](../../../README.md)に示した参考文献に従って実装しているのですが、
ベンチマーク回路での結果が少し違っていたりするので、他手法の採用も含めて、
今後のバージョンアップで性能向上する可能性はあります。

### 量子回路の等価判定

量子回路の最適化ができたら、それが本当に元の量子回路と等価なものなのか確認したくなります。
--eqオプションでそれが実行できます。等価な場合、

    $ sharqit --eq sample.sqc foo.sqc
	true

となり、等価でない場合、

    $ sharqit --eq sample.sqc bar.sqc
	false

となります。

このオプションに関してひとつ注意点があります。
内部で量子回路を行列に展開して行列同士の等価判定を行っているため、
量子ビット数があまり大きな回路でこれを実行すると、
なかなか結果が返ってこなかったり、メモリが足りなくなることに起因したエラーが発生する可能性があります。

### 最適化に関連して出力される情報

先ほど、--optオプションで最適化する説明をした際に、
標準エラー出力にズラズラ表示される情報については後述するとしていました。
ここで詳細を説明します。

--opt=ppオプションの場合、以下のような情報が表示されます。

    $ sharqit --opt=pp sample.sqc > foo.sqc
    [quantum circuit]
	X-count  = 0 -> 0
    Z-count  = 0 -> 0
    H-count  = 4 -> 0
    S-count  = 0 -> 0
    T-count  = 2 -> 0
    RZ-count = 2 -> 0
    CX-count = 1 -> 1
    2Q-count = 1 -> 1
    gate-count = 7 -> 1
    depth      = 5 -> 1
    qubit_num  = 2 -> 2
    [info]
    proc time (sec)   = 0.000000
    kind of optimizer = Phase Polynomial

ここで、X-count, Z-count, H-count, S-count, T-count, RZ-count, CX-count, 2Q-count, gate-count, depth, qubit_numは、
--statsオプションで表示される項目と同じもので、
「=」の右側の「整数値->整数値」は最適化前後の各項目に対応した数字の変化を表しています。また、

    proc time (sec)   = 0.000000

は、最適化計算にかかった時間を表しています（上の例ではあまりにも一瞬で計算が終わったので0になっています）。

    kind of optimizer = Phase Polynomial

は、最適化手法の種類を表しています。

--opt=zxオプションの場合、Phase Polynomialの場合と違って、以下のような情報も追加されます。

    [zx diagram]
    xspider       = 1 -> 0
    zspider       = 1 -> 4
    clifford      = 6 -> 8
    non-clifford  = 0 -> 0
    hadamard      = 0 -> 3

ZX-calculusを用いた方法の場合、
量子回路を一旦ZX-diagramという中間表現に変換しておいてこれを最適化（簡易化）するロジックになっており、
その中間表現を構成するX-Spider、Z-Spider、回転角がPI/2の倍数になっているClifford-spider、
それ以外のnon-Clifford-spider,およびHadamard-edgeの個数がどう変化したかを表しています。
各々の用語の意味については、[ZX-calculus](https://en.wikipedia.org/wiki/ZX-calculus)等の解説記事や
[README.md](../../../README.md)に示した参考文献を参照してください。

### ランダムな量子回路の生成

量子回路の最適化の性能を評価したい場合、ランダムに発生させた量子ゲートからなる量子回路を作りたい場合があります。
--randオプションを使うことでランダム回路を作成できます。例えば、3量子ビットでトータルのゲート数が100になる量子回路を、
CNOTゲートとHゲートとTゲートとRZ(3/2)ゲートの個数の比を1:2:3:5として作成したい場合、

    $ sharqit --rand 3,100,"CX":1,"H":2,"T":3,"RZ(1/2)":5 > bar.sqc

とします。比率の数字は小数であっても大丈夫です。

Sharqitコマンドの説明の最後に、--helpで表示されるヘルプメッセージを掲載します。
各オプションの仕様を忘れてしまった場合、sharqit --helpしてみてください。

    $ sharqit --help
    sharqit - quantum circuit optimizer
    [usage]
      sharqit [option] ([file]..)([params]) (> [file])
    [option]
      --opt(=kind) FILE  : optimize the circuit file, output to stdout.
                           --opt=zx: T-count reduction using ZX-calculus
                           --opt=pp: gate-count reduction using Phase Polynomials
      --rand PARAMS      : generate a random circuit file, output to stdout.
      --eq FILE1 FILE2   : verify two circuits are equal. (can't execute that have too many qubits)
      --stats FILE       : print stats of the circut file.
      --show FILE        : print the circuit diagram as ascii text.
      --help             : print help message.
      --version          : print version.
    [examples]
      $ sharqit --opt foo.sqc > bar.sqc
      $ sharqit --opt=zx foo.sqc > bar.sqc
      $ sharqit --opt=pp foo.sqc > bar.sqc
      $ sharqit --eq foo.sqc bar.sqc
      $ sharqit --rand 3,100,"X":1,"H":2,"T":3.5,"RZ(1/2)":1.5 > bar.sqc # 3 qubits,100 gates
      $ sharqit --stats foo.sqc
      $ sharqit --show foo.sqc
      ...
    Supported quantum gates are X,Z,H,S,S+,T,T+,RZ,CX,CZ.
    S+ and T+ are Hermitian conjugate of S and T respectively.
    RZ gate have one phase factor denoted by fraction brackled in parentheses.
    The unit of phase factor is PI radian, so 3/4 means 3PI/4, 1 means PI, and so on.


## Sharqitライブラリ

### 簡単な例

何はともあれ、Sharqitライブラリを呼び出す簡単なプログラムを作ってみることから始めてみましょう。
まず、以下のようなC++プログラムをコピペしてどこかのディレクトリに置いてみてください。

    $ cat sample.cpp
    #include "sharqit/sharqit.h"
    
    int main()
    {
      Sharqit::QCirc qc_in;
	  qc_in.t(1).h(0).h(1).cx(0,1).h(0).h(1).tdg(1);
	  qc_in.show();

      Sharqit::Optimizer opt;
      Sharqit::QCirc qc_out = opt.execute(qc_in);
      qc_out.show();
    
      return 0;
    }

これを、

    $ g++ -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample.cpp -lshrqt

のようにコンパイルすると、a.outという実行形式が作成されます。実行すると、

    $ ./a.out
    q[0] --H-----*--H------
    q[1] --T--H--X--H--T+--
    q[0] --X--
    q[1] --*--

のようになります。最初の2行が最適化前の量子回路で後の2行が最適化後の量子回路を表しています。
何をやっているかというと(言わずもがなかもしれませんが)、
Sharqitコマンドを説明したときに例として上げた量子回路の最適化プログラムを作成してみました、というわけです。

一応、プログラムの中身を説明します。一行目の

    #include "sharqit/sharqit.h"

で、Sharqitライブラリに対応したインクルードファイルをインクルードします。main関数の中の

    Sharqit::QCirc qc_in;
	
で、SharqitのQCircクラスのオブジェクトを生成します。この時点で量子回路は空です。次の
	
    qc_in.t(1).h(0).h(1).cx(0,1).h(0).h(1).tdg(1);

で空の量子回路に順番に量子ゲートを追加していきます。
t,h,cx,tdgというメソッドを適用するのですが引数に記載されている数字は適用したい量子ビット番号を表しています。
2量子ビットゲートの場合は引数が2つ必要です。
最初の引数が制御側の量子ビット番号で2番目の引数が標的側の量子ビット番号を表します。
作成した量子回路はshowメソッドで表示することができます。

    qc_in.show();

次に量子回路を最適化するOptimizerクラスのオブジェクトを生成します。

    Sharqit::Optimizer opt;

で良いです。最適化を実行するメソッドはexecute()です。引数に最適化したい元になる量子回路オブジェクトを指定します。

    Sharqit::QCirc qc_out = opt.execute(qc_in);

返却値は最適化後の量子回路オブジェクトになるので、それをqc_outという変数で受けるようにします。最後に、

    qc_out.show();

で、結果の量子回路を表示します。以上です。

### 量子回路の作成

量子回路オブジェクトの作成にはいくつかの方法があります。

- 量子ゲートメソッドを追加していく方法
- 量子回路を記述したファイルを読み込む方法
- ランダムな量子回路を生成する方法

以下、順に説明します。

#### 量子ゲートメソッドを追加していく方法

「簡単な例」で示した方法です。
例で示した量子ゲート以外に多数の量子ゲートに対応したメソッドが用意されています。

ベーシックな量子ゲートとして、以下が定義されています。

- x ... Xゲート
- z ... Zゲート
- s ... Sゲート
- sdg ... Sゲートのエルミート共役
- t ... Tゲート
- tdg ... Tゲートのエルミート共役
- h ... Hゲート
- rz ... RZゲート(Z軸まわりの回転ゲート)
- cx ... CNOTゲート
- cz ... CZゲート
- id ... 1量子ビットの恒等演算ゲート(何もしないゲート)
- id2 ... 2量子ビットの恒等演算ゲート(何もしないゲート)

Sharqitでは、あらゆる量子回路をこの量子ゲートによって内部的に表現するようにしています。
加えて、以下の量子ゲートに対応したメソッドを使うこともできます。
ベーシックな量子ゲートを組み合わせることで、これらメソッドを実現しています。

- y ... Yゲート
- rx ... RXゲート(X軸まわりの回転ゲート)
- ry ... RYゲート(Y軸まわりの回転ゲート)
- sx ... ルートXゲート(2乗するとXゲートになる)
- sxdg ... ルートXゲートのエルミート共役
- p ... 位相シフトゲート(グローバル位相を除きRZゲートと同等)
- cy ... 制御Yゲート
- csx ... 制御ルートXゲート
- csxdg ... 制御ルートXゲートのエルミート共役
- ch ... 制御Hゲート
- cs ... 制御Sゲート
- csdg ... 制御ルートXゲートのエルミート共役
- ct ... 制御Tゲート
- ctdg ... 制御Tゲートのエルミート共役
- crx ... 制御RXゲート
- cry ... 制御RYゲート
- crz ... 制御RZゲート
- cp ... 制御位相シフトゲート
- rxx ... イジングカップリングゲート(XX結合)
- ryy ... イジングカップリングゲート(YY結合)
- rzz ... イジングカップリングゲート(ZZ結合)
- sw ... swapゲート
- csw ... 制御swapゲート(フレドキンゲート)
- ccx ... 制御CNOTゲート(トフォリゲート)

これらメソッドの引数について説明します。
回転角指定のない1量子ビットゲートの場合の引数は1つで、適用する量子ビット番号を指定します。
回転角指定のない2量子ビットゲートの場合の引数は2つで、制御ビット番号、標的ビット番号の順に指定します。
回転角指定のない3量子ビットゲートの場合の引数は3つで、制御ビット番号、制御ビット番号、標的ビット番号の順で指定します。
回転角の指定がある量子ゲートの場合、上記引数に加えて、最後に角度を指定する必要があります。

例を示します。

    Sharqit::QCirc qc;
    Sharqit::Phase PI("PI"); // 角度PIを表すPhaseクラスのオブジェクト
    qc.h(0).t(0); // 回転角指定のない1量子ビットゲート
	qc.cx(0,1).csw(0,1,2).crx(1,2); // 回転角指定のない2,3量子ビットゲート
	qc.rz(2, 3*PI/4).crx(1,2,PI/2); // 回転角指定のある1,2量子ビットゲート

回転角はPhaseクラスのオブジェクトとして定義します。典型的な使い方は、上に示したように、PIラジアンを

    Sharqit::Phase PI("PI");

のように定義して、3*PI/4, PI/2, -PI/4, ...のように指定するやり方です。それ以外にも、

    Sharqit::Phase phase(3,4); // 3*PI/4を表す
    Sharqit::Phase phase(-1,2); // -PI/2を表す

のように指定するやり方もあります。

#### 量子回路を記述したファイルを読み込む方法

Sharqitコマンドで説明した量子回路ファイルを読み込んで、量子回路オブジェクトとする方法もあります。
以下のようなファイルがあるとします。

    $ cat sample.sqc
    T 1
    H 0
    H 1
    CX 0 1
    H 0
    H 1
    T+ 1

loadメソッドを使って、

    Sharqit::QCirc qc_in;
    qc_in.load("sample.sqc");

のようにすることで、ファイルに記述されたものと同じ量子回路オブジェクトが作成されます。

#### ランダムな量子回路を生成する方法

量子回路の最適化の性能を評価したい場合、ランダムに発生させた量子ゲートからなる量子回路を作りたい場合があります。
Sharqitコマンドの--randオプションと同様のことをC++プログラム内で実行することができます。

例えば、5量子ビットでトータルのゲート数が20の量子回路を、XゲートとTゲートとCNOTゲートの比率を4:5:3として作成したい場合、
add_randomメソッドを使って、

    Sharqit::QCirc qc_in;
    qc_in.add_random(5, 20, {{"X", 4},{"T", 5},{"CX", 3}});

のようにします。指定する比率は小数であっても良いです。

### 量子回路図の表示

「簡単な例」で、

    qc_in.show()
	
のように実行しました。この例の場合

    q[0] --H-----*--H------
    q[1] --T--H--X--H--T+--

のようになりますが、非常に深い回路の場合、以下のように折り返して表示されます。

    q[0] --X--T-----X--X-----*-----X--*--T--*--X--*--T--T--------------X--*--*-----*--X--T-----X--T--T-- ...
    q[1] -----------*--*-----|--X--*--X--T--|-----X--T--T--------*--*-----X--|--T--X--|--T--T--*--T----- ...
    q[2] --T--T--T--------T--X--T--------X--X--------T--T--X--T--X--X--T-----X--T-----*--T--T-----T----- ...
    
    T--X-----*-----X--------*--T--------*--*--------X-----X--------X--T--*--*--------*--T--T-----*--X--- ...
    ------*--X-----|--X--T--X--X--T--T--|--|--T--X--------|--X-----|--T--X--X--*--T--X--T--T-----X--*--T ...
    ------X-----T--*--T--T-----T--------X--X-----*--T--T--*--*--T--*--T--------X--T-----T--T--T--------- ...
    
    --------*--X--*--------
    --*--T--|--*--X--X--X--
    --X-----X--------------

折返しの位置は、デフォルトでは100文字になっていますが、showメソッドの引数で指定することもできます。

    qc_in.show(50)

### 量子回路の統計情報の取得

statsメソッドによって、std::map<std::string, uint32_t>のオブジェクトが返ってきます。
文字列をキーとしたした整数値によって各種情報が表現されているので、各々を以下のようにして取得することがきます。

    Sharqit::QCirc qc_in;
    qc_in.t(1).h(0).h(1).cx(0,1).h(0).h(1).tdg(1);
    std::map<std::string, uint32_t> stats = qc_in.stats();
    std::cout << "X-count  = " << stats["x_count"] << std::endl;
    std::cout << "Z-count  = " << stats["z_count"] << std::endl;
    std::cout << "H-count  = " << stats["h_count"] << std::endl;
    std::cout << "S-count  = " << stats["s_count"] << std::endl;
    std::cout << "T-count  = " << stats["t_count"] << std::endl;
    std::cout << "RZ-count = " << stats["rz_count"] << std::endl;
    std::cout << "CX-count = " << stats["cx_count"] << std::endl;
    std::cout << "2Q-count = " << stats["2q_count"] << std::endl;
    std::cout << "gate-count = " << stats["gate_count"] << std::endl;
    std::cout << "depth      = " << stats["depth"] << std::endl;
    std::cout << "qubit_num  = " << stats["qubit_num"] << std::endl;

この場合、以下のように表示されます。

    X-count  = 0
    Z-count  = 0
    H-count  = 4
    S-count  = 0
    T-count  = 2
    RZ-count = 2
    CX-count = 1
    2Q-count = 1
    gate-count = 7
    depth      = 5
    qubit_num  = 2

### 量子回路の最適化

「簡単な例」で、

    Sharqit::QCirc qc_out = opt.execute(qc_in);

のように実行しました。このときexecuteメソッドに2番目の引数を指定しない場合、ZX-calculusを用いた手法で最適化計算がなされます。
Phase Polynomialを用いた手法で最適化計算をしたい場合、以下のように2番目の引数に手法を指定します。

    Sharqit::QCirc qc_out = opt.execute(qc_in, Sharqit::OptimizerKind::PhasePolynomial);

ZX-calculusを用いた手法での計算を

    Sharqit::QCirc qc_out = opt.execute(qc_in, Sharqit::OptimizerKind::ZXCalculus);

のように明示的に指定することもできます。

### 量子回路の等価判定

2つの量子回路が等価かどうか、量子回路クラスのis_equalメソッドで以下のように確認することができます。

    bool eq = qc_in.is_equal(qc_out);
    std::cout << std::boolalpha << eq << std::endl;

Sharqitコマンド同様、ひとつ注意点があります。
内部で量子回路を行列に展開して行列同士の等価判定を行っているため、
量子ビット数があまり大きな回路でこれを実行すると、
なかなか結果が返ってこなかったり、メモリが足りなくなることに起因したエラーが発生する可能性があります。

### 量子回路の保存

量子回路をSharqit独自のファイル形式で出力する場合、saveメソッドを使います。

    qc_out.save("bar.sqc");
	
また、量子回路イメージをSVG形式で出力することもできます。

    qc_out.to_svg_file("bar.svg");


## 他の量子回路ファイル形式からSharqit形式への変換

OpenQASM2.0で記述されたClifford-T回路からSharqit形式に変換するPythonコード例を以下に示します。
[PyZX](https://github.com/Quantomatic/pyzx)の機能を使っています。

    $ cat qasm_to_sqc.py
    import pyzx as zx
    
    def gate_str(gate):
        gstr = ""
        if gate.name == "NOT":
            gstr = "X " + str(gate.target)
        elif gate.name == "Z":
            gstr = "Z " + str(gate.target)
        elif gate.name == "S" and gate.adjoint is False:
            gstr = "S " + str(gate.target)
        elif gate.name == "S" and gate.adjoint is True:
            gstr = "S+ " + str(gate.target)
        elif gate.name == "T" and gate.adjoint is False:
            gstr = "T " + str(gate.target)
        elif gate.name == "T" and gate.adjoint is True:
            gstr = "T+ " + str(gate.target)
        elif gate.name == "HAD":
            gstr = "H " + str(gate.target)
        elif gate.name == "CNOT":
            gstr = "CX " + str(gate.control) + " " + str(gate.target)
        elif gate.name == "CZ":
            gstr = "CZ " + str(gate.control) + " " + str(gate.target)
        return gstr
    
    if __name__ == '__main__':
        qc_src = zx.Circuit.load("sample.qasm")
        qc_bas = qc_src.to_basic_gates()
        with open("sample.sqc", mode='w') as f:
            for gate in qc_bas.gates:
                f.write(gate_str(gate) + "\n")


以上
