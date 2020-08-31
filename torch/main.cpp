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
  /* FLAMEGRAPH IS NOT A TIMELINE.  It does not show order of execution.

  Flamegraph takes stack samples recorded by perf and postprocesses them for easy visualization.
  It aggregates them from the top of the stack (outermost call) downwards.
  lumping together calls to the same function at each level.
  The postprocessed .svg image displays each function with width proportional to the number of samples
  with the same call stack up to that function.

  The calls below help illustrate flamegraph's aggregation policy. */

  /* These calls are not aggregated with calls to micro_requires_grad* in subfunc(),
  because they occur at a different level of the call stack. */
  micro_requires_grad_true();
  micro_requires_grad_false();

  /* The two separate invocations of subfunc will appear as one big "subfunc" section of the flame graph.
  Their subcalls (micro_requires_grad_true() and micro_requires_grad_false()) are similarly aggregated.
  The "micro_requires_grad_true/false" sections within the "subfunc" section will appear
  roughly twice as wide as main's individual direct micro_requires_grad_true/false calls above, because subfunc() is
  called twice, so roughly twice as many samples should see "main->subfunc->micro_requires_grad_*" call stacks as see the
  "main->micro_requires_grad_*" call stacks. */
  subfunc();
  subfunc();
}

