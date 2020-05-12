[toc]

## 传输文件

-l 选项表示监听,作为服务端

-p 表示端口

-v 动态显示

+ 服务端发送

`nc -lvp 9000<a.out`

+ 客户端接收

`nc -v ip port >a.out`

或者使用 -o 选项指定输出文件，输出流依然会打印到屏幕

`nc -v -o filename ip port `

## 目录传输(使用tar)

`tar -cvf - dir/ | nc -l -p 9000`

`nc -n 192.168.1.100 9000 | tar -xvf -`

## 压缩后传输

`tar -cvf - ddd/ | bzip2 -z | nc -l -p 9000`

`nc -n 192.168.1.100 9000 | bzip2 -d | tar -xvf -`

