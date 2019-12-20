## Ubuntu 配置
### 中文显示异常
![""](./photo/font.png)
原因：
当系统使用的是英文环境时，Ubuntu默认采用的字体Noto Sans CJK优先显示日文汉字，这一问题可以通过修改配置文件/etc/fonts/conf.avail/64-language-selector-prefer.conf来修复
64-language-selector-prefer.conf原文件内容:
```
<?xml version="1.0"?>
<!DOCTYPE fontconfig SYSTEM "fonts.dtd">
<fontconfig>
	<alias>
		<family>sans-serif</family>
		<prefer>
			<family>Noto Sans CJK JP</family>
			<family>Noto Sans CJK KR</family>
			<family>Noto Sans CJK SC</family>
			<family>Noto Sans CJK TC</family>
		</prefer>
	</alias>
	<alias>
		<family>serif</family>
		<prefer>
			<family>Noto Serif CJK JP</family>
			<family>Noto Serif CJK KR</family>
			<family>Noto Serif CJK SC</family>
			<family>Noto Serif CJK TC</family>
		</prefer>
	</alias>
	<alias>
		<family>monospace</family>
		<prefer>
			<family>Noto Sans Mono CJK JP</family>
			<family>Noto Sans Mono CJK KR</family>
			<family>Noto Sans Mono CJK SC</family>
			<family>Noto Sans Mono CJK TC</family>
		</prefer>
	</alias>
</fontconfig>
```
JP <--->日文
KR <---> 韩文
SC <---> 简体中文
TC <---> 繁体中文

修改上面的文件，将顺序改成SC TC JP KR

### ssr

#### 检测服务器是否被墙
网站: ping.pe

#### 代理设置
##### 终端走代理
export http_proxy=http://proxyAddress:port
export http_proxy=http://127.0.0.1:12333
export https_proxy=http://127.0.0.1:12333



## vim
### 下载vim 
>> git clone https://github.com/vim/vim.git

### 安装vim
1. cd vim
2.  ./configure --with-features=huge --enable-python3interp --enable-pythoninterp --with-python-config-dir=/usr/lib/python2.7/config-x86_64-linux-gnu/ --enable-rubyinterp --enable-luainterp --enable-perlinterp  --enable-multibyte --enable-cscope      --prefix=/usr/local/vim/
3. sudo make install

### 配置vim
配置插件
#### YouCompleteMe



















## syscall
```c
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

long syscall(long number, ...);
	Reuturns:  The return value is defined by the system call being invoked.  In 
general, a 0 return value indicates success. A -1 return value indicates an error, and an
 error code is stored in errno.

Description:
  syscall 是库函数，可以用于调用一些汇编接口的系统调用，这些系统调用没有C语言包装函数。
```











## gettid
```c
#include <sys/types.h>
pid_t gettid(void);
	Returns: On success, returns the thread ID of the calling process. This call is 
always successful.

Description:  gettid()  returns  the  caller's thread ID (TID).  In a single-threaded 
process, the thread ID is equal to the process ID (PID, as returned by getpid(2)).  In a 
multithreaded process, all threads have the  same  PID,  but each one has a unique TID.

Note: There is no glibc wrapper for this system call
```