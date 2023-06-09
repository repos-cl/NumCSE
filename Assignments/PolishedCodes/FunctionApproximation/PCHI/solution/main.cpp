#include <vector>

#include "pchi.hpp"

int main() {
  std::vector<double> N_nodes, h, err_reconstr, err_zero;

  err_zero = fppchipConvergence();
  err_reconstr = rspchipConververgence();

  int table_size = err_zero.size();
  int N = 4;

  for (int i = 0; i < table_size; i++) {
    N_nodes.push_back(N);
    h.push_back(10. / N);
    N *= 2;
  }

  std::cout << "N_nodes"
            << "\t"
            << "err_zero"
            << "\t"
            << "err_reconstr\n";
  for (unsigned int i = 0; i < h.size(); ++i) {
    std::cout << N_nodes[i] << "\t" << err_zero[i] << "\t" << err_reconstr[i]
              << std::endl;
  }

  // Error plot
  plt::figure();
  plt::title("Error VS no. of nodes");
  plt::xlabel("h");
  plt::ylabel("max |f(t) - s(t)|");
  plt::loglog(h, err_reconstr, "b", {{"label", "$s_{reconstr}$"}});
  plt::loglog(h, err_zero, "r", {{"label", "$s_{zero}$"}});

  std::vector<double> vec_size_lin(h.size());
  std::vector<double> vec_size_pow3(h.size());

  for (unsigned int i = 0; i < h.size(); i++) {
    vec_size_lin[i] = h[i] / h[0];
    vec_size_pow3[i] = pow(h[i], 3) / pow(h[0], 3);
  }

  plt::loglog(h, vec_size_lin, "k--", {{"label", "$O(h)$"}});
  plt::loglog(h, vec_size_pow3, "k--", {{"label", "$O(h^3)$"}});

  plt::legend();
  plt::savefig("./cx_out/pchi_conv.png");
}
