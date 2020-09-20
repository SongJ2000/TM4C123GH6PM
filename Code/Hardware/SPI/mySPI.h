#ifndef __MYSPI_H
#define __MYSPI_H

#include "system.h"

void mySPIInit(void);
void mySPITransmit(unsigned char *data, unsigned int len);

#endif
