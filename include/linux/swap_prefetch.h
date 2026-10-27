/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SWAP_PREFETCH_H
#define _LINUX_SWAP_PREFETCH_H

/*
 * Pluggable swap-in prefetcher. A module registers a predict() callback;
 * swapin_readahead() then asks it, on every swap-in fault, which pages near
 * the faulting one to read in along with it, and issues all of those reads
 * together with the faulting page's (asynchronously, like the built-in
 * read-ahead). With no prefetcher registered, or when predict() returns
 * false for a fault, the kernel's own read-ahead runs unchanged.
 */

#include <linux/types.h>

struct vm_fault;

struct swap_prefetch_req {
	u64 fwd;	/* bit k set: also read the page k + 1 pages after the fault  (+1 .. +64) */
	u64 back;	/* bit k set: also read the page k + 1 pages before the fault (-1 .. -64) */
};

struct swap_prefetch_ops {
	/*
	 * Called under rcu_read_lock() on each swap-in fault, before any read
	 * is issued; must not sleep. Fill @req and return true to handle this
	 * fault (an empty @req means "read only the faulting page"), or return
	 * false to fall back to the kernel's read-ahead for it. A requested
	 * page is only read if it lies in the faulting VMA and the same page
	 * table (2MB-aligned range) as the fault, and is currently swapped out.
	 */
	bool (*predict)(struct vm_fault *vmf, struct swap_prefetch_req *req);
};

int register_swap_prefetcher(const struct swap_prefetch_ops *ops);
void unregister_swap_prefetcher(const struct swap_prefetch_ops *ops);

#endif /* _LINUX_SWAP_PREFETCH_H */
