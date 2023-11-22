#include "sharq.h"

void test_load(const std::string& s) {

  try {
    Sharq::Optimizer opt;

    Sharq::QCirc qc_in;
    qc_in.load("sandbox/" + s + ".sqc");
    std::cout << "file : " << "sandbox/" + s + ".sqc" << std::endl;

    //std::cout << "== result (before) ==" << std::endl;
    //opt.show();

    Sharq::QCirc qc_out = opt.execute(qc_in);

    std::cout << "T-count: " << qc_in.t_count() << ","<< qc_out.t_count() << std::endl;
    std::cout << "equal? " << qc_in.is_equal(qc_out) << std::endl;

    std::cout << "== result ==" << std::endl;
    opt.show();
    //std::cout << opt << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}
    
void test(int n) {

  try {
    Sharq::Optimizer opt;
    Sharq::Phase PI("PI");
    Sharq::QCirc qc_in;

    if (n == 1) {
      qc_in.h(0).s(1).s(2).h(1).s(1).h(1).cx(2,1).z(0).h(1).cx(0,2).x(0).t(1).x(2);
      qc_in.h(2).cx(0,1).h(0).t(1).h(2).h(1).t(2).s(1).s(2).t(1).h(2).t(2).h(2).cx(0,1).cx(1,0);
      qc_in.t(0).s(1).t(2).s(0).h(0).t(0).cx(1,0).cx(2,1).h(0).h(1).s(0).cx(0,2).s(0).s(1);
    }
    else if (n == 2) {
      qc_in.x(1).cx(3,4).s(0).t(2).t(4);
    }
    else if (n == 3) {
      qc_in.x(0).h(1).h(0).s(1).cx(1,0).h(0).s(1).x(0).s(1).s(0).s(1).cx(1,0).s(0).s(1).s(0).h(0).cx(1,0).t(0);
    }
    else if (n == 3) {
      qc_in.cx(0,1).cx(0,1).cx(0,1).h(0).t(0).cx(1,0).cx(1,0);
    }
    else if (n == 4) {
      qc_in.cx(0,1).t(1).s(0).x(1).t(1).cx(1,0).h(1).s(1);
    }
    else if (n == 5) {
      qc_in.cx(1,0).h(0).h(1).cx(0,1).cx(1,0).cx(0,1).s(1).cx(0,1);
    }
    else if (n == 6) {
      qc_in.cx(0,1).cx(0,1).cx(1,0).cx(1,0).h(0).h(1).cx(0,1).s(1);
    }
    else if (n == 7) {
      qc_in.s(0).s(1).x(0).t(1).h(0).t(1).x(0).s(1).s(0).s(1);
    }
    else if (n == 8) {
      qc_in.cx(1,0).cx(0,1).cx(0,1).t(0).s(1).s(1).cx(0,1).cx(1,0).h(0).s(1);
    }
    else if (n == 9) {
      qc_in.h(0).x(1).h(0).cx(1,0).h(0).x(1).x(0).s(1).cx(1,0).h(0);
    }
    else if (n == 10) {
      qc_in.h(0).h(1).t(0).t(1).x(0).cx(0,1).s(0).cx(1,0).t(1).cx(1,0);
    }
    else if (n == 11) {
      qc_in.cx(1,0).cx(0,1).cx(0,1).t(0).s(1).s(1).cx(0,1).cx(1,0).h(0).s(1);
    }
    
    Sharq::QCirc qc_out = opt.execute(qc_in);
    std::cout << "equal? " << qc_in.is_equal(qc_out) << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

bool test_random(int qubit_num, int gate_num) {

  try {
    Sharq::Optimizer opt;
    Sharq::QCirc qc_in;
    qc_in.add_random((uint32_t)qubit_num, (uint32_t)gate_num,
		     {{"X", 1},
		      {"Z", 1},
		      {"H", 1},
		      {"S", 1},
		      {"T", 1},
		      {"CX", 1}});

    qc_in.save("sandbox/hoge.sqc");

    Sharq::QCirc qc_out = opt.execute(qc_in);
    std::cout << "qgate_num = " << qc_out.qgate_num() << std::endl;
    std::cout << "equal? " << qc_in.is_equal(qc_out) << std::endl;
    if (!qc_in.is_equal(qc_out)) return false;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
    return false;
  }

  return true;
}

void test_random_2(int qubit_num, int gate_num) {

  try {
    Sharq::QCirc qc_in;
    qc_in.add_random((uint32_t)qubit_num, (uint32_t)gate_num,
		     {{"X", 1},
		      {"Z", 1},
		      {"H", 1},
		      {"S", 1},
		      {"T", 1},
		      {"CX", 1}});

    Sharq::ZXDiagram zx = qc_in.to_zxdiagram();

    /* clock start */
    auto start = std::chrono::system_clock::now();

    zx.simplify();

    /* clock end */
    auto end = std::chrono::system_clock::now();
    auto dur = end - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    double proc_time = msec / 1000.;
    std::cout << "proc_time = " << std::to_string(proc_time) << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

void test_qcirc_stats()
{
  try {
    Sharq::Phase PI("PI");
    Sharq::Optimizer opt;
    Sharq::QCirc qc;
    qc.x(0).z(1).s(0).sdg(1).t(0).tdg(1).h(2).rz(0,PI/2).rz(1,PI/4);
    qc.show();

    std::map<std::string, uint32_t> sts = qc.stats();
    for (auto it = sts.begin(); it != sts.end(); ++it) {
      std::cout << it->first << ", " << it->second << std::endl;
    }
    std::cout << "qgate_num = " << qc.qgate_num() << std::endl;
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

void test_zxdiagram_stats()
{
  try {
    Sharq::Phase PI("PI");
    Sharq::Optimizer opt;
    Sharq::QCirc qc;
    qc.x(0).z(1).s(0).cz(1,2).sdg(1).h(0).t(0).tdg(1).h(2).rz(0,PI/2).rz(2,PI/4).cx(0,1);
    qc.show();

    Sharq::ZXDiagram zx = qc.to_zxdiagram();
    zx.show();

    std::map<std::string, uint32_t> sts = zx.stats();

    for (auto it = sts.begin(); it != sts.end(); ++it) {
      std::cout << it->first << ", " << it->second << std::endl;
    }
  }
  catch (std::runtime_error& e) {
    std::cerr << "runtime_error: " << e.what() << std::endl;
  }
}

void test_gfusion(int pg_num, int leaf_num)
{
  uint32_t qubit_num = 1;
  Sharq::ZXDiagram zx(qubit_num);
  std::vector<uint32_t> inputs = zx.inputs();
  std::vector<uint32_t> outputs = zx.outputs();
  zx.remove_edge(inputs[0], outputs[0]);

  std::vector<uint32_t> leafs(leaf_num);
  for (int i = 0; i < leaf_num; ++i) {
    leafs[i] = zx.append_node(Sharq::ZXNode(Sharq::ZXNodeKind::ZSpider, Sharq::Phase(1,4)));
  }

  std::vector<uint32_t> roots(pg_num);
  std::vector<uint32_t> phases(pg_num);
  for (int i = 0; i < pg_num; ++i) {
    roots[i] = zx.append_node(Sharq::ZXNode(Sharq::ZXNodeKind::ZSpider, Sharq::Phase(0)));
    phases[i] = zx.append_node(Sharq::ZXNode(Sharq::ZXNodeKind::ZSpider, Sharq::Phase(1,2)));
  }

  zx.connect_nodes(inputs[0], leafs[0], Sharq::ZXEdgeKind::Plain);
  for (int i = 1; i < leaf_num; ++i) {
    zx.connect_nodes(leafs[i-1], leafs[i], Sharq::ZXEdgeKind::Hadamard);
  }
  zx.connect_nodes(leafs[leaf_num-1], outputs[0], Sharq::ZXEdgeKind::Plain);

  for (int i = 0; i < pg_num; ++i) {
    zx.connect_nodes(roots[i], phases[i], Sharq::ZXEdgeKind::Hadamard);
  }

  for (int i = 0; i < pg_num; ++i) {
    for (int j = 0; j < leaf_num; ++j) {
      zx.connect_nodes(roots[i], leafs[j], Sharq::ZXEdgeKind::Hadamard);
    }
  }

  zx.kind(Sharq::ZXDiagramKind::GraphLike);
  
  zx.show();
  zx.to_svg_file("sandbox/hoge_in.svg");

  zx.gfusion();
  zx.to_svg_file("sandbox/hoge_out.svg");
}

int main(int argc, char** argv)
{
  std::string mode;
  if (argc > 1) {
    mode = argv[1];
  }
  
  if (mode == "load") {
    std::string s = argv[2];
    test_load(s);
  }
  else if (mode == "random") {
    int trial = atoi(argv[2]);
    int qubit_num = atoi(argv[3]);
    int gate_num = atoi(argv[4]);
    for (int n = 0; n < trial; ++n) {
      std::cout << "** n = " << n << " **" << std::endl;
      if (test_random(qubit_num, gate_num) == false) break;
    }
  }
  else if (mode == "random2") {
    int trial = atoi(argv[2]);
    int qubit_num = atoi(argv[3]);
    int gate_num = atoi(argv[4]);
    for (int n = 0; n < trial; ++n) {
      std::cout << "** n = " << n << " **" << std::endl;
      test_random_2(qubit_num, gate_num);
    }
  }
  else if (mode == "gfusion") {
    test_gfusion(3,3);
  }
  else if (argc == 2) {
    int n = stoi(mode);
    test(n);
  }

  return 0;
}
