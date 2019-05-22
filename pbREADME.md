[toc]
## protobuf install
github protobuf web have good tutor of install 

##Question

1,can not find lib of protobuf11
res:sudo vi /etc/ld.so.conf
add this line:/usr/local/protobuf/lib
it mean add path of proto to ldconfig
and then run command
`sudo ldconfig`
