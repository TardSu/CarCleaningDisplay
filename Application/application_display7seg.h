#ifndef __APPLICATION_DISPLAY7SEG_H__
#define __APPLICATION_DISPLAY7SEG_H__

//void Display7Segment(char *dat,unsigned char dot,unsigned char cmddis);

void Display7SegmentTime(S_RTC_TIME_DATA_T time);
void Display7SegmentMoney(double money);

#endif
