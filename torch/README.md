Inspired by https://github.com/bhosmer/ptperf.

1. Edit `CUDA_HOME` in `graph.sh` to point to your Cuda toolkit install.
2. `bash ./build.sh`
3. `sudo bash graph.sh ./build/microbenchmark dwarf`.  It may take 30 seconds to a minute.
4. flamegraph (.svg) will appear in `output`. Compare to the structure of main.cpp.
