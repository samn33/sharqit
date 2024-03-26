量子回路ファイル形式
====================

## ファイル形式の説明

Sharqitで独自定義しているファイル形式の例を示します。

    $ cat sample.sqc
    T 1
    H 0
    H 1
    CX 0 1
    H 0
    H 1
    T+ 1

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

## 参考: 他の量子回路ファイル形式からSharqit形式への変換

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


