# wireshark
+ 抓包环境：
    + 本机环境内部抓包  监控网卡
    + 交换机  环境
    + 网络环境 

++ 过滤 写法
```c
ip.src==192.168.3.239   只接收src为后面ip的帧

ip.src==192.168.3.239 and ip.dst==14.215.177.38
```

中间窗口中：
Frame  物理层
Ethernet  数据链路层
Internet  网络层
Transmission  传输层
Transport Layer 应用层

tcp的3次握手4次挥手分析：抓包http包

tcpdump 和 Wireshark 就是最常用的网络抓包和分析工具
tcpdump 仅支持命令行格式使用，常用在 Linux 服务器中抓取和分析网络包。
Wireshark 除了可以抓包外，还提供了可视化分析网络包的图形页面。
所以，这两者实际上是搭配使用的，先用 tcpdump 命令在 Linux 服务器上抓包，接着把抓包的文件拖出到 Windows 电脑后，用 Wireshark 可视化分析。

tcpdump 抓的数据格式不好分析，通常转成.pcap 格式的文件，拖到wireshark去分析
`tcpdump -i th0 icmp and host 192.168.1.1 -w ping.pcap`

点击某一帧数据：
在Ethernet 数据链路层：可以看到 源mac  目的mac  地址信息
在Internet Protocol网络（IP）层：
    + 可以看到 Protocal version
    + 源ip  目的ip
    + Header Length：ip包头数据长度
    + Flags  不分片
    + ttl： Time To Live ---ttl 值
    + Protocol：ICMP  下层协议类型是ICMP还是别的
在 Internet Control Message Protocol中：---即ICMP协议
    + type：可以看到是ping命令执行 发起的本次网络通讯

### 总结
通讯的过程中每一层都有自己的责任范围，上次协议完成自己的处理工作，就交给下层处理
ICMP层：ICMP头+Data
IP层：IP头+ICMP层
数据链路层：以太帧头+IP层
数据层层包裹被转发出去 层层解包---最终对方就可以将数据解出来

seq的初始是个随机的值，后面ack的回复都是在这个值上一直+n，n是变化的

为什么抓到的 TCP 挥手是三次，而不是书上说的四次？

因为服务器端收到客户端的 FIN 后，服务器端同时也要关闭连接，这样就可以把 ACK 和 FIN 合并到一起发送，节省了一个包，变成了“三次挥手”。

而通常情况下，服务器端收到客户端的 FIN 后，很可能还没发送完数据，所以就会先回复客户端一个 ACK 包，稍等一会儿，完成所有数据包的发送后，才会发送 FIN 包，然后客户端再Ack 这也就是四次挥手了。

1. 异常 无法连接服务器
curl的自动超时重传：--服务器无法回复时
客户端发起了 SYN 包后，一直没有收到服务端的 ACK ，所以一直超时重传了 5 次，并且每次 RTO 超时时间是不同的：
第一次是在 1 秒超时重传
第二次是在 3 秒超时重传
第三次是在 7 秒超时重传
第四次是在 15 秒超时重传
第五次是在 31 秒超时重传
可以发现，每次超时时间 RTO 是指数（翻倍）上涨的，当超过最大重传次数后，客户端不再发送 SYN 包。

第一次握手的 SYN 超时重传次数，是如下内核参数指定的：
$ cat /proc/sys/net/ipv4/tcp_syn_retries
    5
重传5次后都收不到回复，客户端就会显示 连接超时

2. 异常 TCP 第二次握手 SYN、ACK 丢包
为了模拟客户端收不到服务端第二次握手 SYN、ACK 包，我的做法是在客户端加上防火墙限制，直接粗暴的把来自服务端的数据都丢弃，防火墙的配置如下：
iptables -I INPUT -s serverip -j DROP  扔掉服务器数据

| 咦？客户端设置了防火墙，屏蔽了服务端的网络包，为什么 tcpdump 还能抓到服务端的网络包？
+ 添加 iptables 限制后， tcpdump 是否能抓到包 ，这要看添加的 iptables 限制条件：
    + 如果添加的是 INPUT 规则，则可以抓得到包
    + 如果添加的是 OUTPUT 规则，则抓不到包
网络包进入主机后的顺序如下：
进来的顺序 Wire -> NIC -> tcpdump -> netfilter/iptables
出去的顺序 iptables -> tcpdump -> NIC -> Wire

3. TCP 第三次握手 ACK 丢包

为了模拟 TCP 第三次握手 ACK 包丢，我的实验方法是在服务端配置防火墙，屏蔽客户端 TCP 报文中标志位是 ACK 的包，也就是当服务端收到客户端的 TCP ACK 的报文时就会丢弃
服务端 iptables 配置命令如下：
`iptables -I INPUT -s serip -p tcp --tcp-flag ACK ACK -j DROP`
服务端收不到第三次握手的 ACK 包，所以一直处于 SYN_RECV 状态：
而客户端是已完成 TCP 连接建立，处于 ESTABLISHED 状态：
netstat -anp |grep serip、cliip