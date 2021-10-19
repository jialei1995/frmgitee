#ifndef __CELLTEMPRATURE_H
#define	__CELLTEMPRATURE_H
#include "main.h"
uint16_t Find_Adc_Temp( uint16_t val );
void Update_CellTemps(void);
void Up_CellTemps(void);

extern const uint16_t Pos5vNTC3380AdTempTABLE[];
extern int tbTempCnt ;

#endif

