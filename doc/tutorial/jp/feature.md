Sharqitとは
==========

sharqitは量子回路最適化のためのライブラリです。
C++およびPythonプログラムから呼び出して使用することができるSDKとして提供しています。
処理本体はC++で実装されているため高速です。
現在実装されている最適化の機能は以下の2つです。

- ZX-calculusを用いたTゲートの削減
- Phase Polynomialを用いた量子ゲートの削減

また、このライブラリの機能をシェルから実行できるコマンドラインツールも提供しています。

## 参考文献

以下の文献を参考に実装しています。

### ZX-calculusを用いた手法

1. Ross Duncan, Aleks Kissinger, Simon Perdrix, John van de Wetering,
"Graph-theoretic Simplification of Quantum Circuits with the ZX-calculus",
[arXiv:1902.03178](https://arxiv.org/abs/1902.03178)

2. Aleks Kissinger, John van de Wetering,
"Reducing T-count with the ZX-calculus",
[arXiv:1903.10477](https://arxiv.org/abs/1903.10477)

3. Miriam Backens, Hector Miller-Bakewell, Giovanni de Felice, Leo Lobski, John van de Wetering,
"There and back again: A circuit extraction tale",
[arXiv:2003.01664](https://arxiv.org/abs/2003.01664)

4. Korbinian Staudacher,
"Optimization Approaches for Quantum Circuits using ZX-calculus"
[Ludwig maximilian university of munich thesis](https://www.mnm-team.org/pub/Diplomarbeiten/stau21/PDF-Version/stau21.pdf)

### Phase Polynomialを用いた手法

5. Yunseong Nam, Neil J. Ross, Yuan Su, Andrew M. Childs, Dmitri Maslov,
"Automated optimization of large quantum circuits with continuous parameters",
[arXiv:1710.07345](https://arxiv.org/abs/1710.07345)
