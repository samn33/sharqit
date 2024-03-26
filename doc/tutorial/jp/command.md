コマンドラインツール
===================

## 量子回路の用意

まず、最適化したい量子回路を以下のように用意します。

    $ cat sample.sqc
    T 1
    H 0
    H 1
    CX 0 1
    H 0
    H 1
    T+ 1

これは、sharqitで定義している独自形式のテキストファイルです。
ファイル形式の詳細については、[量子ファイル形式](file_format.md)を参照してください。

## 量子回路図の表示

さて、このような記号の羅列では量子回路のイメージがつかみにくいです。
そのため、sharqitコマンドでは--showオプションが用意されていて、
以下のように多少グラフィカルに量子回路を表示することができます。

    $ sharqit --show sample.sqc
	q[0] --H-----*--H------
	q[1] --T--H--X--H--T+--

## 量子回路の統計情報

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

## 量子回路の最適化

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
Phase Polynomialを用いた方法は以下のように--opt=ppオプションで計算できます(ZX-calculusの場合は--opt=zxです)。

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

## 量子回路の等価判定

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

## 最適化に関連して出力される情報

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

## ランダムな量子回路の生成

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
