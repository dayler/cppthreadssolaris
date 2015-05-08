/* 
 * File:   TimeUtils.hpp
 * Author: dayler
 *
 * Created on May 7, 2015, 11:06 PM
 */

#ifndef TIMEUTILS_HPP
#define	TIMEUTILS_HPP

#include <time.h>

#define MILLION_I 1000000

void timevalToTimespec(struct timeval* from, struct timespec* to)
{
    to->tv_sec = from->tv_sec;
    to->tv_nsec = from->tv_usec * 1000;
}

/* resolve seconds carry */
static inline void fixTv(struct timeval *t1)
{
  if (t1->tv_usec >= MILLION_I)
  {
    t1->tv_sec++;
    t1->tv_usec -= MILLION_I;
  }
  
  if (t1->tv_usec < 0)
  {
    t1->tv_sec--;
    t1->tv_usec += MILLION_I;
  }
}

void millisToTimespec(struct timespec* ts, unsigned long millis)
{
    ts->tv_sec = millis / 1000;
    ts->tv_nsec = (millis % 1000) * 1000000;
}

// timeval add: t1 += t2
void tvAdd(struct timeval *t1, struct timeval *t2)
{
    t1->tv_sec += t2->tv_sec;
    t1->tv_usec += t2->tv_usec;
    fixTv(t1);
}

/* add an int (milliseconds) to a timeval */
void tvAddMillis(struct timeval *t1, long ms)
{
    t1->tv_sec += (ms / 1000);
    t1->tv_usec += 1000*(ms % 1000);
    fixTv(t1);
}


/* add an int (microseconds) to a timeval */
void tvAddMicroSec(struct timeval *t1, long us)
{
    t1->tv_sec += us / MILLION_I;
    t1->tv_usec += us % MILLION_I;
    fixTv(t1);
}

#endif	/* TIMEUTILS_HPP */

