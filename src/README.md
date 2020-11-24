# An Efficient, Scalable, Extensible and Accurate Framework to Analyse Large-Scale Sequences of DNA Barcodes

## Dependencies
- [Boost](https://www.boost.org/doc/libs/1_72_0/more/getting_started/unix-variants.html)
- [libcurl](https://curl.haxx.se/docs/install.html)
- GCC 7.5.0
## Guide
```bash
$ git clone git@github.com:lmpn/ecsi.git
$ cd iSpecID
$ mkdir build
$ cmake -DCMAKE_BUILD_TYPE:STRING=Release -H. -Bbuild -G "Unix Makefiles"
$ cmake --build build --config Release --target all -- -j 6
```
If your main compiler isn't GCC 7.5.0 use the following command to configure:
```bash
$ cmake --no-warn-unused-cli 
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE 
	-DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo 
	-DCMAKE_C_COMPILER:FILEPATH=<C_compiler_path>
	-DCMAKE_CXX_COMPILER:FILEPATH=<CXX_compiler_path>
	-H<cmake_lists_dir>
	-B<build_dir>
	-G "Unix Makefiles"
$  cmake --build . --config RelWithDebInfo --target all -- -j 
```
