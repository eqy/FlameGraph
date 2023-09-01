if [ "$#" -ne 2 ]; then
  echo "Usage:  graph.sh /path/to/appname userspace_stack_unwind_method"
  echo "        Options for user_space_stack_unwinding_method are fp, lbr, and dwarf."
  echo "        I only found it produced intelligible flame graphs with dwarf."
  echo "        For /path/to/appname, you probably want build/microbenchmark."
  exit 1
else
    appname=$1
    userspace_stack_unwind_method=$2
fi

# Sets up helper env vars in case these aren't available by default as root.
CUDA_HOME=/home/mcarilli/cudaversions/11
echo "Using Cuda toolkit at " $CUDA_HOME
export PATH=${CUDA_HOME}/bin:${PATH}
export LD_LIBRARY_PATH=${CUDA_HOME}/lib64:${LD_LIBRARY_PATH}
export LIBRARY_PATH=${CUDA_HOME}/lib64:${LIBRARY_PATH}
export TORCH_SHOW_CPP_STACKTRACES=1

# options for userspace_stack_unwind_method are fp, lbr, dwarf.
# for some reason fp gives me an unintelligible flame graph.
# maybe because pytorch was not compiled with -g, or because it was (implicitly)
# compiled with -fomit-frame-pointer.
echo "Appname: " $1
echo "User-space stack unwinding method: " $userspace_stack_unwind_method


FlameGraphUtils="`pwd`/.."

mkdir -p output
outdir="output"
pushd $outdir
# The -F (frequency) argument to perf may also be useful here.
/usr/lib/linux-tools-5.15.0-82/perf record --call-graph $userspace_stack_unwind_method ../$appname
/usr/lib/linux-tools-5.15.0-82/perf script | ${FlameGraphUtils}/stackcollapse-perf.pl > out.perf-folded
${FlameGraphUtils}/flamegraph.pl out.perf-folded > perf.svg
popd
