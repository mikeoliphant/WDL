/*
  WDL - timing.h

  this is based on some public domain Pentium RDTSC timing code from usenet in 1996.

  To enable this, your app must #define TIMING, include timing.h, call timingInit(), then timingEnter(x)/timingLeave(x) a bunch
  of times (where x is 0-64), then timingPrint at the end. 

*/

#ifndef _TIMING_H_
#define _TIMING_H_


//#define TIMING


#include "wdltypes.h"

#if defined(TIMING) && !defined(__alpha)
#ifdef __cplusplus
extern "C" {
#endif
void _timingInit(void);
void _timingPrint(void);
void _timingEnter(int);
void _timingLeave(int);
int _timingQuery(int, WDL_INT64*);
#ifdef __cplusplus
}
#endif
#define timingPrint() _timingPrint()
#define timingInit() _timingInit()
#define timingLeave(x) _timingLeave(x)
#define timingEnter(x) _timingEnter(x)
#define timingQuery(x,y) _timingQuery(x,y)
#else
#define timingPrint()
#define timingInit()
#define timingLeave(x)
#define timingEnter(x)
#define timingQuery(x,y) (0)
#endif

#endif