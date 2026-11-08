/*
 * Host thread stacks: the size a thread that runs lifted code is created with.
 *
 * Elsewhere the reservations are generous (256 MB for a guest PPU thread and
 * an SPU job, 64 MB for a shader-compile worker): a reservation costs address
 * space only, and a 64-bit desktop has plenty. iOS does not: a process gets a
 * few GB of address space even with the extended-virtual-addressing
 * entitlement, the guest arena takes 3.5 GB of it, and a title runs about 70
 * threads. PS3_HOST_STACK(bytes) is the size to ask for: unchanged off iOS;
 * on iOS no more than PS3_IOS_STACK_CAP_MB (default 16), or
 * PS3RECOMP_IOS_STACK_MB=<n> from the environment.
 */
#ifndef PS3EMU_HOST_STACK_H
#define PS3EMU_HOST_STACK_H

#include <stddef.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#ifndef PS3_IOS_STACK_CAP_MB
#define PS3_IOS_STACK_CAP_MB 16
#endif

#if defined(__APPLE__) && TARGET_OS_IPHONE
static inline size_t ps3_host_stack_bytes(size_t want)
{
    size_t cap_mb = PS3_IOS_STACK_CAP_MB;
    const char* e = getenv("PS3RECOMP_IOS_STACK_MB");
    if (e && atoi(e) > 0) cap_mb = (size_t)atoi(e);
    const size_t cap = cap_mb << 20;
    return want > cap ? cap : want;
}
#define PS3_HOST_STACK(bytes) ps3_host_stack_bytes((size_t)(bytes))
#else
#define PS3_HOST_STACK(bytes) ((size_t)(bytes))
#endif

#endif
