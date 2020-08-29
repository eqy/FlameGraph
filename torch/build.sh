TORCHDIR=`python -c "import torch; print(torch.__file__[:-12])"`
echo "Found Torch at " $TORCHDIR

mkdir -p build
pushd build
# Based on https://pytorch.org/tutorials/advanced/cpp_frontend.html#writing-a-basic-application 
cmake -DCMAKE_PREFIX_PATH=${TORCHDIR} ..
cmake --build . --config Release
# this also works
# cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/home/mcarilli/.local/lib/python3.8/site-packages/torch ..
# cmake --build . --config Debug
popd
