## 安装pvtrace和Graphviz
1. 安装pvtrace

$ mkdir -p ~/project1 && cd ~/project1
$ wget http://www.mtjones.com/developerworks/pvtrace.zip
$ unzip pvtrace.zip -d pvtrace
$ cd pvtrace
$ make
$ sudo make install
# 查看pvtrace相关文件
$ ls -1 pvtrace
instrument.c
Makefile
stack.c
stack.h
symbols.c
symbols.h
trace.c
2. 安装graphviz

$ sudo yum install graphviz


## 测试

1. 编辑测试文件test.c

2. 编译测试程序

$ gcc -g -finstrument-functions test.c ./pvtrace/instrument.c -o test
注意： 必须有`-g -finstrument-functions`选项，否则后续就采集不到信息了。
3. 执行程序，生成信息文件trace.txt

$ ./test
4. 通过pvtrace、可执行文件及trace.txt，生成信息文件graph.dot

$ pvtrace test
5. 通过dot工具将graph.dot，转为图像文件graph.png

$ dot -Tpng graph.dot -o graph.png
6. 浏览生成的图片
