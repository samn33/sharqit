Pythonパッケージ
===============

## 簡単な例

Sharqitパッケージを呼び出す簡単なPythonプログラムを作ってみることから始めてみましょう。
まず、以下のようなPythonプログラムをコピペしてどこかのディレクトリに置いてみてください。

    $ cat sample.py
    from sharqit import QCirc, Optimizer
    
    def main():
        qc_in = QCirc()
        qc_in.t(1)
        qc_in.h(0)
        qc_in.h(1)
        qc_in.cx(0,1)
        qc_in.h(0)
        qc_in.h(1)
        qc_in.tdg(1);
        qc_in.show()
	    
        opt = Optimizer()
        qc_out = opt.reduce_gates(qc_in, "zx")
        qc_out.show()
    
    if __name__ == '__main__':
        main()

これを実行すると、

    $ python sample.py
    q[0] --H-----*--H------
    q[1] --T--H--X--H--T+--
    q[0] --X--
    q[1] --*--

のようになります。最初の2行が最適化前の量子回路で後の2行が最適化後の量子回路を表しています。

一応、プログラムの中身を説明します。一行目の

    from sharqit import Phase, QCirc, Optimizer

で、sharqitパッケージから3つのクラス(Phase, QCirc, Optimizer)をインポートします。
main関数の中の

    qc_in = QCirc()

で、QCircクラスのインスタンスを生成します。この時点で量子回路は空です。次の

    qc_in.t(1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.cx(0,1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.tdg(1);

で空の量子回路に順番に量子ゲートを追加していきます。

ここで、t,h,cx,tdgというメソッドを適用していますが引数に記載されている数字は適用したい量子ビット番号を表しています。
2量子ビットゲートの場合は引数が2つ必要です。
最初の引数が制御側の量子ビット番号で2番目の引数が標的側の量子ビット番号を表します。
作成した量子回路はshowメソッドで表示することができます。

    qc_in.show()

次に量子回路を最適化するOptimizerクラスのオブジェクトを生成します。

    opt = Optimizer()

で良いです。最適化を実行するメソッドはreduce_gates()です。

    qc_out = opt.reduce_gates(qc_in, "zx")

第1引数に最適化したい元になる量子回路オブジェクトを指定します。
第2引数に最適化の手法("zx"または"pp")を指定します。"zx"はZXCalculus、"pp"はPhasePolynomialを表します。
返却値は最適化後の量子回路オブジェクトになるので、それをqc_outという変数で受けるようにします。最後に、

    qc_out.show()

で、結果の量子回路を表示します。以上です。


## 量子回路の作成

量子回路オブジェクトの作成にはいくつかの方法があります。

- 量子ゲートメソッドを追加していく方法
- 量子回路を記述したファイルを読み込む方法
- ランダムな量子回路を生成する方法

以下、順に説明します。

### 量子ゲートメソッドを追加していく方法

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

    from sharqit import Phase, QCirc
    qc = QCirc()
    # 角度PIを表すPhaseクラスのインスタンス
    PI = Phase("PI")
    # 回転角指定のない1量子ビットゲート
    qc.h(0)
    qc.t(0)
    # 回転角指定のない2-3量子ビットゲート
    qc.cx(0, 1)
    qc.csw(0, 1, 2)
    qc.crx(1, 2)
    # 回転角指定のある1,2量子ビットゲート
    qc.rz(2, 3*PI/4)
    qc.crx(1, 2, PI/2)

回転角はPhaseクラスのオブジェクトとして定義します。典型的な使い方は、上に示したように、PIラジアンを

    PI = Phase("PI");

のように定義して、3*PI/4, PI/2, -PI/4, ...のように指定するやり方です。それ以外にも、

    p = Phase(3, 4) # 3*PI/4を表す
    q = Phase(-1, 2) # -PI/2を表す

のように指定するやり方もあります。

### 量子回路を記述したファイルを読み込む方法

Sharqit形式の量子回路ファイルを読み込んで、量子回路オブジェクトとする方法もあります。
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

    qc_in = QCirc()
    qc_in.load("sample.sqc")

のようにすることで、ファイルに記述されたものと同じ量子回路オブジェクトが作成されます。

### ランダムな量子回路を生成する方法

量子回路の最適化の性能を評価したい場合、ランダムに発生させた量子ゲートからなる量子回路を作りたい場合があります。
Sharqitコマンドの--randオプションと同様のことをPythonプログラム内で実行することができます。

例えば、5量子ビットでトータルのゲート数が20の量子回路を、XゲートとTゲートとCNOTゲートの比率を4:5:3として作成したい場合、
add_randomメソッドを使って、

    qc = QCirc()
    qc.add_random(5, 20, "X:4,T:5 CX:3")

のようにします。指定する比率は小数であっても良いです。

## 量子回路の表示

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

折返しの位置は、デフォルトでは100文字になっていますが、showメソッドのwidthオプションで指定することもできます。

    qc_in.show(width=50)


## 量子回路の統計情報の取得

量子回路に含まれている各種ゲートの数や深さや量子ビット数は、以下のように取得・表示することができます。

    qc_in = QCirc()
    qc_in.t(1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.cx(0,1)
    qc_in.h(0)
    qc_in.h(1)
    qc_in.tdg(1)
    print("X_count =", qc_in.x_count())
    print("Z_count =", qc_in.z_count())
    print("H_count =", qc_in.h_count())
    print("S_count =", qc_in.s_count())
    print("T_count =", qc_in.t_count())
    print("RZ_count =", qc_in.rz_count())
    print("CX_count =", qc_in.cx_count())
    print("2Q_count =", qc_in.twoq_count())
    print("gate_count =", qc_in.gate_count())
    print("depth =", qc_in.depth())
    print("qubit_num =", qc_in.qubit_num())

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

また、print_statsメソッドでこれと同様な情報を一気に表示することもできます。

    qc_in.print_stats()

## 量子回路に含まれる量子ゲート情報取得

量子回路に含まれる量子ゲート情報は、例えば以下のように取得することができます。

    from sharqit import Phase, QGate, QGateKind, QCirc

    qc = QCirc()
    qc.rz(0, Phase(3,4));
    qc.h(0)
    qc.h(1)
    qc.cx(0,1)
    qc.h(0)
    qc.h(1)
    qc.rz(1, Phase(1,4));

    qgate_list = qc.get_qgates()
    for qgate in qgate_list:
        if qgate.kind == QGateKind.RZ:
            print("kind: RZ, qid: {}, phase: {}".format(qgate.qid, qgate.phase))
        elif qgate.kind == QGateKind.H:
            print("kind: H, qid: {}".format(qgate.qid))
        elif qgate.kind == QGateKind.CX:
            print("kind: CX, qid: {}".format(qgate.qid))

    print()
    for qgate in qgate_list:
        print(qgate.to_string()) 

変数qcに格納されている量子ゲート集合はget_qgatesメソッドにより、
QGateクラスのインスタンスからなるリストとして取得することができます。
QGateクラスにはkind,qid,phaseという3つのプロパティが定義されています。
kindは量子ゲートの種類、qidは適用する量子ビット番号、phaseはRZゲートの回転角を表しています。
kindは、列挙型QGateKIndのメンバーX,Z,S,Sdg,T,Tdg,H,RZ,CX,CZ,Id,Id2として表現されます。
qidは、1量子ビットゲートの場合、1つの整数値、2量子ビットゲートの場合、
2つの整数値からなるリストとして表現されます。
phaseはPhaseクラスのインスタンスとして表現されます。
また、QGateクラスのto_stringメソッドで量子ゲート演算を文字列で取得することもできます。

上のコードの実行結果は、以下のようになります。

    kind: RZ, qid: [0], phase: 3/4
    kind: H, qid: [0]
    kind: H, qid: [1]
    kind: CX, qid: [0, 1]
    kind: H, qid: [0]
    kind: H, qid: [1]
    kind: RZ, qid: [1], phase: 1/4
    
    RZ(3/4) 0
    H 0
    H 1
    CX 0 1
    H 0
    H 1
    RZ(1/4) 1

## 量子回路の最適化

「簡単な例」で、

    qc_out = opt.reduce_gates(qc_in, "zx")

のように実行しました。ここでreduce_gatesメソッドの2番目の引数に"zx"が指定されているので、
ZX-calculusを用いた手法で最適化計算がなされます。
Phase Polynomialを用いた手法で最適化計算をしたい場合、以下のように2番目の引数に手法を指定します。

    qc_out = opt.reduce_gates(qc_in, "pp")


### 量子回路の等価判定

2つの量子回路が等価かどうか、量子回路クラスのis_equalメソッドで以下のように確認することができます。

    print(qc_in.is_equal(qc_out))

SharqitコマンドやSharqitライブラリ同様、ひとつ注意点があります。
内部で量子回路を行列に展開して行列同士の等価判定を行っているため、
量子ビット数があまり大きな回路でこれを実行すると、
なかなか結果が返ってこなかったり、メモリが足りなくなることに起因したエラーが発生する可能性があります。


## 量子回路の保存

量子回路をSharqit独自のファイル形式で出力する場合、saveメソッドを使います。

    qc_out.save("bar.sqc")
