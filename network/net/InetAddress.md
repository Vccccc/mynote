# InetAddress
**Description**
InetAddress封装了sockaddr_in
构造函数有四个：
1. (ip,port)，如("1.2.3.4", 5678)
2. (ipPort)，如("1.2.3.4:5678")
3. (port, bool loopbackOnly false)， 如(5678, false)，用于listening
4. (sockaddr_in)