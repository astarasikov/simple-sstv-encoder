#ifndef __COMMON_FUNCTIONS_HPP__
#define __COMMON_FUNCTIONS_HPP__

#include <errno.h>
#include <sched.h>

inline int sstv_sched_yield(void) {
	return sched_yield();
}

//TODO: gettext
#define _(x) x

#ifdef SSTV_DEBUG
#include <cstdio>
#define Dprintf(fmt, x...) printf(fmt, ##x)
#else
#define Dprintf(fmt, x...) do {} while (0)
#endif

#endif
