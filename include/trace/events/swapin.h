/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM swapin

#if !defined(_TRACE_SWAPIN_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_SWAPIN_H

#include <linux/tracepoint.h>
#include <linux/sched.h>

/*
 * One event per fault on a swapped-out page (do_swap_page()), recorded
 * before the swap-cache / synchronous-device paths split, so every swap-in
 * fault is seen whatever the swap device. Used to collect prefetcher
 * training traces: enable with
 *   echo 1 > /sys/kernel/tracing/events/swapin/swapin_fault/enable
 * and restrict to one process with
 *   echo 'tgid == <pid>' > /sys/kernel/tracing/events/swapin/swapin_fault/filter
 */
TRACE_EVENT(swapin_fault,

	TP_PROTO(unsigned long address, unsigned long ip, bool in_swap_cache),

	TP_ARGS(address, ip, in_swap_cache),

	TP_STRUCT__entry(
		__field(unsigned long,	address)	/* exact faulting address */
		__field(unsigned long,	ip)		/* user-space PC */
		__field(int,		tgid)
		__field(bool,		in_swap_cache)	/* found in the swap cache: no device read */
	),

	TP_fast_assign(
		__entry->address	= address;
		__entry->ip		= ip;
		__entry->tgid		= current->tgid;
		__entry->in_swap_cache	= in_swap_cache;
	),

	TP_printk("tgid=%d addr=%lx ip=%lx cached=%d",
		  __entry->tgid, __entry->address, __entry->ip,
		  __entry->in_swap_cache)
);

#endif /* _TRACE_SWAPIN_H */

#include <trace/define_trace.h>
