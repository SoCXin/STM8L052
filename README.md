# [STM8L](https://github.com/mcuyun/STM8L) 

STM8单片机低功耗系列

[![sites](mcuyun/mcuyun.png)](http://www.mcuyun.com)

#### 更多MCU：[mcuyun](https://github.com/mcuyun/whyme)可访问 www.mcuyun.com

---

## 组成

#### docs

包括芯片手册等资料  STM8S中文参考手册.pdf

#### demo

包括IAR工程和相关库文件

#### examples

包括makefile工程

---

## Building

Dependencies:
 1. [STM8/sdcc]
 2. [STM8/stm8flash]

Building and flashing example project:

```
cd ./examples/<example>
make flash

```
Uncomment `--peep-file $(LIBDIR)/util/extra.def` option in the Makefile to enable additional optimizer rules.


---

###  www.mcuyun.com   |    qitas@qitas.cn


