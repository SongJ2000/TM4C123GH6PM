#ifndef __MYIIC_H
#define __MYIIC_H

#include "system.h"

// SCL: PB2     SDA: PB3
void myIICInit(void);

/** 
 * @brief 写iic
 * @param addr   器件地址
 * @param headByte   头字节
 * @param length   数据体长度，可以为0
 * @param data_t   数据体指针
 */
void myIICWriteLen(unsigned char addr, unsigned char headByte, unsigned char length, unsigned char *data_t);

/** 
 * @brief 读iic
 * @param addr   器件地址
 * @param headByte   头字节
 * @param length   数据体长度，最小为1
 * @param data_t   数据体指针
 */
void myIICReadLen(unsigned char addr, unsigned char headByte, unsigned char length, unsigned char *data_t);


#endif
