#ifndef __MYIIC_H
#define __MYIIC_H

#include "system.h"

// SCL: PB2     SDA: PB3
void myIICInit(void);

/** 
 * @brief дiic
 * @param addr   ������ַ
 * @param headByte   ͷ�ֽ�
 * @param length   �����峤�ȣ�����Ϊ0
 * @param data_t   ������ָ��
 */
void myIICWriteLen(unsigned char addr, unsigned char headByte, unsigned char length, unsigned char *data_t);

/** 
 * @brief ��iic
 * @param addr   ������ַ
 * @param headByte   ͷ�ֽ�
 * @param length   �����峤�ȣ���СΪ1
 * @param data_t   ������ָ��
 */
void myIICReadLen(unsigned char addr, unsigned char headByte, unsigned char length, unsigned char *data_t);


#endif
