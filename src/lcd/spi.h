#ifndef   _SPI_H_
#define   _SPI_H_

#include <stdconst.h>

void SPIInit(void);
unsigned int SPITxReady(void);
unsigned int SPIRxReady(void);
void SPIWrite(UBYTE* src_buf, UBYTE len);
void SPIRead(UBYTE* dst_buf, UBYTE len);
void SPIWriteDMA(UBYTE* src_buf, UBYTE len);
void SPIReadDMA(UBYTE* dst_buf, UBYTE len);
void SPIPIOSetData(void);
void SPIPIOClearData(void);

#endif
