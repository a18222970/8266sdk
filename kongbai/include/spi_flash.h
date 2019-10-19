

//SPI ��������ӿ�,�����ڴ�
#ifndef SPI_FLASH_H
#define SPI_FLASH_H

//typedef�Ľ��������������µ����������������е�������,typedef int   CHANGE;ָ������CHANGE����int���ͣ�CHANGE����int
//enum ͨ��һ�ּ򵥵�;������һ��������һ������ֵ��ϵ��һ�𡣿��Դ���#define����
//typedef  enum������ö�����ͣ����ͱ���ȡֵ��enum{}��Χ��ȡ����ʹ���ж����޲��
typedef enum {
    SPI_FLASH_RESULT_OK, //FLASH����ɹ�
    SPI_FLASH_RESULT_ERR,//FLASH�������
    SPI_FLASH_RESULT_TIMEOUT//FLASH�����ʱ
} SpiFlashOpResult;

typedef struct{
	uint32	deviceId;
	uint32	chip_size;    // chip size in byte оƬ��С(�ֽ�)
	uint32	block_size;
	uint32  sector_size;
	uint32  page_size;
	uint32  status_mask;
} SpiFlashChip;

#define SPI_FLASH_SEC_SIZE      4096

uint32 spi_flash_get_id(void);
//typedef unsigned short      uint16;  short��int��long ��C�����г������������ͣ����� int ��Ϊ���ͣ�short ��Ϊ�����ͣ�long ��Ϊ������
//unsigned   �����þ��ǽ����������޷��Ż�,��unsigned int�ķ�Χ��0-255��
SpiFlashOpResult spi_flash_erase_sector(uint16 sec);
//typedef unsigned int        uint32;   int һ��ռ�� 4 ���ֽڣ�Byte�����ڴ棬���� 32 λ��Bit��
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
