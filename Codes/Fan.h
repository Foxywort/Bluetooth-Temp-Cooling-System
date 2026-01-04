#ifndef __FAN_H__
#define __FAN_H__

void Fan_Init(void);
void Fan_SetLevel(unsigned char level);
unsigned char Fan_GetDutyPercent(void);

#endif