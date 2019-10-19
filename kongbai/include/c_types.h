#ifndef _C_TYPES_H_
#define _C_TYPES_H_


/**  0x表示你给的数据是十六进制
short、int、long 是C语言中常见的整数类型，其中 int 称为整型，short 称为短整型，long 称为长整型
 short 的长度是确定的，是两个字节,表示无符号16位数
 int 建议为一个机器字长。32 位环境下机器字长为 4 字节，表示无符号32位数
 short 占据的内存大小是2 个byte
 int占据的内存大小是4 个byte；
 long占据的内存大小是4 个byte
 float占据的内存大小是4 个byte
 double占据的内存大小是8 个byte；
 char占据的内存大小是1 个byte。   unsigned char范围为0-255

 unsigned   的作用就是将数字类型无符号化,而unsigned int的范围：0-255。

 signed在默认情况下声明的整型变量都是有符号的类型（char有点特别）,signed int 大小区间为-128-127
 无符号版本和有符号版本的区别就是无符号类型能保存2倍于有符号类型的正整数数据。
*/

typedef unsigned char       uint8_t;  //typedef 定义一种类型的别名，而不只是简单的宏替换。可以用作同时声明指针型的多个对象。
typedef signed char         sint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short        sint16_t;
typedef signed short        int16_t;
typedef unsigned int        uint32_t;
typedef signed long         sint32_t;
typedef signed int          int32_t;
typedef signed long long    sint64_t;
typedef unsigned long long  uint64_t;
typedef unsigned long long  u_int64_t;
typedef float               real32_t;
typedef double              real64_t;

typedef unsigned char       uint8;
typedef unsigned char       u8;
typedef signed char         sint8;
typedef signed char         int8;
typedef signed char         s8;
typedef unsigned short      uint16;
typedef unsigned short      u16;
typedef signed short        sint16;
typedef signed short        s16;
typedef unsigned int        uint32;
typedef unsigned int        u_int;
typedef unsigned int        u32;
typedef signed int          sint32;
typedef signed int          s32;
typedef int                 int32;
typedef signed long long    sint64;
typedef unsigned long long  uint64;
typedef unsigned long long  u64;
typedef float               real32;
typedef double              real64;

#define __le16      u16

typedef unsigned int        size_t;

#define __packed        __attribute__((packed))

/**  #ifndef #define #endif
 * #ifndef x //先测试x是否被宏定义过
 * #define x //程序段1 //如果x没有被宏定义过，定义x，并编译程序段1
 * #endif  程序段2 //如果x已经定义过了则编译程序段2的语句，“忽视”程序段1。
 *
 */
#define LOCAL       static
#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

/* probably should not put STATUS here 可能不应该把状态放在这里*/
typedef enum {
    OK = 0,
    FAIL,
    PENDING,
    BUSY,
    CANCEL,
} STATUS;

#define BIT(nr)                 (1UL << (nr))

#define REG_SET_BIT(_r, _b)  (*(volatile uint32_t*)(_r) |= (_b))
#define REG_CLR_BIT(_r, _b)  (*(volatile uint32_t*)(_r) &= ~(_b))

#define DMEM_ATTR __attribute__((section(".bss")))
#define SHMEM_ATTR

#ifdef ICACHE_FLASH
#define ICACHE_FLASH_ATTR __attribute__((section(".irom0.text")))
#define ICACHE_RODATA_ATTR __attribute__((section(".irom.text")))
#else
#define ICACHE_FLASH_ATTR
#define ICACHE_RODATA_ATTR
#endif /* ICACHE_FLASH */

#define STORE_ATTR __attribute__((aligned(4)))

#ifndef __cplusplus
typedef unsigned char   bool;
#define BOOL            bool
#define true            (1)
#define false           (0)
#define TRUE            true
#define FALSE           false


#endif /* !__cplusplus */

#endif /* _C_TYPES_H_ */
