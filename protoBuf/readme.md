[toc]

# install

google protobuf github:`https://github.com/protocolbuffers/protobuf`

C++ Installation - Unix
To build protobuf from source, the following tools are needed:

autoconf
automake
libtool
make
g++
unzip
On Ubuntu/Debian, you can install them with:
`$ sudo apt-get install autoconf automake libtool curl make g++ unzip`
On other platforms, please use the corresponding package managing tool to install them before proceeding.

+ To get the source, download one of the release .tar.gz or .zip packages in the release page:

https://github.com/protocolbuffers/protobuf/releases/latest

+ You can also get the source by "git clone" our git repository. Make sure you have also cloned the submodules and generated the configure script (skip this if you are using a release .tar.gz or .zip package):

```
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf
git submodule update --init --recursive
./autogen.sh
```

+ To build and install the C++ Protocol Buffer runtime and the Protocol Buffer compiler (protoc) execute the following:

```
./configure --prefix=/usr/local/protobuf_ver
make
make check
sudo make install
sudo ldconfig # refresh shared library cache.
```


## tips

1. add `-lpthread` option when compile app which use protobuf

this option should loacl behind of `-lprotobuf`, like this `-lprotobuf -lpthread`
otherwise, protobufObj.PrintDebugString() will cause coredump;
