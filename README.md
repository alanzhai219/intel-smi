# intel-smi
intel-smi is short for intel dGPU system monitor interface.

# setup level-zero
```bash
## clone level-zero
git clone https://github.com/oneapi-src/level-zero.git

## build and install
cd level-zero
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/your/specific/path ..
make install

## setup env
echo "export $L0_HOME=/your/specific/path" >> .bashrc
echo "export $LIBRARY_PATH=${L0_HOME}/lib:${LIBRARY_PATH}" >> .bashrc
echo "export $LD_LIBRARY_PATH=${L0_HOME}/lib:${LD_LIBRARY_PATH}" >> .bashrc
echo "export $C_INCLUDE_PATH=${L0_HOME}/include:${C_INCLUDE_PATH}" >> .bashrc
echo "export $CPLUS_INCLUDE_PATH=${L0_HOME}/include:${CPLUS_INCLUDE_PATH}" >> .bashrc
```

# build intel-smi
```bash
g++ intel-smi.cpp -lze_loader
```

# debug
- `ZE_ENABLE_LOADER_DEBUG_TRACE=1`: enable debug tracing feature of level-zero

# reference
- level-zero github: https://github.com/oneapi-src/level-zero
- level-zero docs: https://oneapi-src.github.io/level-zero-spec/level-zero/latest/index.html
