#include <torch/torch.h>
#include <iostream>
// #include <chrono>

#define ITER 100000

void micro_requires_grad_false() {
  for (int i = 0; i < ITER; i++) {
    // auto a = torch::empty({2, 3}, torch::dtype(torch::kFloat32).device(torch::kCUDA, 1).requires_grad(true));
    auto a = torch::empty({2, 3}, torch::dtype(torch::kFloat32).device(torch::kCPU, 0));
  }
}

void micro_requires_grad_true() {
  for (int i = 0; i < ITER; i++) {
    // auto a = torch::empty({2, 3}, torch::dtype(torch::kFloat32).device(torch::kCUDA, 1).requires_grad(true));
    auto a = torch::empty({2, 3}, torch::dtype(torch::kFloat32).device(torch::kCPU, 0).requires_grad(true));
  }
}

void subfunc() {
  micro_requires_grad_true();
  micro_requires_grad_false();
}

int main() {
  micro_requires_grad_true();
  micro_requires_grad_false();

  // The two calls below help illustrate flamegraph's aggregation policy.
  // All samples with the same call chain get lumped together on the flamegraph.
  // The flamegraph shows each section with width proportional to the number of samples matching that particular call chain.
  // Therefore, these two separate invocations of subfunc will appear as one big "subfunc" section of the flame graph.
  // Their subcalls (micro_requires_grad_true() and micro_requires_grad_false()) are similarly aggregated.
  // The "micro_requires_grad_true/false" sections within the "subfunc" section will appear
  // roughly twice as wide as main's individual direct micro_requires_grad_true/false calls above, because roughly twice
  // as many samples should hit "main->subfunc->micro_requires_grad_*" call chains as hit the
  // "main->micro_requires_grad_*" call chains.
  subfunc();
  subfunc();
}

