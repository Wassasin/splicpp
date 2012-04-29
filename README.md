splicpp
=======

`splicpp` is a Simple Programming Language compiler in C++.

How to compile it
=================

`splicpp` can be compiled with `G++ 4.6` and `clang 3.0`. It uses `Boost 1.46` or higher.

Compilation has been tested with the following setups:
* Ubuntu 11.10, G++ 4.6.1, Boost 1.46.1
* Ubuntu 12.04 LTS, G++ 4.6.3, Boost 1.46.1
* Ubuntu 12.04 LTS, clang 3.0, Boost 1.49.0

On Ubuntu 11.10, to install software required to compile `splicpp`, use the following command:

```
$ sudo apt-get install build-essential cmake libboost-system-dev libboost-program-options-dev libboost-regex-dev libboost-filesystem-dev libboost-serialization-dev
```

To compile `splicpp`, use the following steps:

1. In the `splicpp` root directory, create a build directory.

	```
	$ mkdir build
	$ cd build
	```
	
2. Use `cmake` to create a MakeFile.

	```
	$ cmake ../src
	```

3. Compile using `make`

	```
	$ make
	```

How to use it
=============

`splicpp` is not yet finished, and can not yet generate code. For instructions, type the following:

```
$ ./splicpp --help
```
