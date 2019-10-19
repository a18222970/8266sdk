# 项目说明
把我需要的功能在学习的路上一个一个整合完善


### 修改如下代码:

```makefile
#Makefile of project line25-line29
BOOT?=none
APP?=0
SPI_SPEED?=40
SPI_MODE?=DIO
SPI_SIZE_MAP?=6
```
###烧录地址，不支持云端升级
* eagle.flash.bin-------->0x00000
* eagle.irom0text.bin---->0x10000
* blank.bin --> 0x3FE000
* esp_init_data_default_v08.bin --> 0x3FC000



