

//SPI 串行外设接口,外设内存
#ifndef SPI_FLASH_H
#define SPI_FLASH_H

//typedef的解释是用来声明新的类型名来代替已有的类姓名,typedef int   CHANGE;指定了用CHANGE代表int类型，CHANGE代表int
//enum 通过一种简单的途径，把一串名字与一串整形值联系在一起。可以代替#define声明
//typedef  enum定义了枚举类型，类型变量取值在enum{}范围内取，在使用中二者无差别
typedef enum {
    SPI_FLASH_RESULT_OK, //FLASH结果成功
    SPI_FLASH_RESULT_ERR,//FLASH结果出错
    SPI_FLASH_RESULT_TIMEOUT//FLASH结果超时
} SpiFlashOpResult;

typedef struct{
	uint32	deviceId;
	uint32	chip_size;    // chip size in byte 芯片大小(字节)
	uint32	block_size;
	uint32  sector_size;
	uint32  page_size;
	uint32  status_mask;
} SpiFlashChip;

#define SPI_FLASH_SEC_SIZE      4096

uint32 spi_flash_get_id(void);
//typedef unsigned short      uint16;  short、int、long 是C语言中常见的整数类型，其中 int 称为整型，short 称为短整型，long 称为长整型
//unsigned   的作用就是将数字类型无符号化,而unsigned int的范围：0-255。
SpiFlashOpResult spi_flash_erase_sector(uint16 sec);
//typedef unsigned int        uint32;   int 一般占用 4 个字节（Byte）的内存，共计 32 位（Bit）
SpiFlashOpResult spi_flash_write(uint32 des_addr, uint32 *src_addr, uint32 size);
SpiFlashOpResult spi_flash_read(uint32 src_addr, uint32 *des_addr, uint32 size);

typedef SpiFlashOpResult (* user_spi_flash_read)(
		SpiFlashChip *spi,
		uint32 src_addr,
		uint32 *des_addr,
        uint32 size);

void spi_flash_set_read_func(user_spi_flash_read read);

bool spi_flash_erase_protect_enable(void);
bool spi_flash_erase_protect_disable(void);

#endif
