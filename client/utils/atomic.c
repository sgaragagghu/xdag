#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include <windows.h>
#include "atomic.h"

#define atomic_exchange_switch_max64(object, desired) \
switch(sizeof(*object)) { \
	case 1: \
		return _InterlockedExchange8((volatile char*)object, desired); \
	case 2: \
		return _InterlockedExchange16((volatile short*)object, desired); \
	case 4: \
		return _InterlockedExchange((volatile long*)object, desired); \
	case 8: \
		return InterlockedExchange64((volatile LONG64*)object, desired); \
	default: \
		xdag_err("This atomic data size %d is not supported on windows", sizeof(*object)); \
}

uint_least64_t atomic_exchange_uint_least64(atomic_uint_least64_t *object, uint_least64_t desired)
{
	atomic_exchange_switch_max64(object, desired);
	return 0;
}

#define atomic_compare_exchange_strong_switch_max64(object, expected, desired) \
switch(sizeof(*object)) { \
	case 1: { \
		char comparand = 0; \
		memcpy(&comparand, expected, sizeof(*expected)); \
		char dstValue = _InterlockedCompareExchange8((volatile char*)ptr, desired, comparand); \
		int success = dstValue == comparand; \
		if (!success) { \
			memcpy(expected, &comparand, sizeof(*expected)); \
		} \
		return success; } \
	case 2: { \
		short comparand = 0; \
		memcpy(&comparand, expected, sizeof(*expected)); \
		short dstValue = _InterlockedCompareExchange16((volatile short*)ptr, desired, comparand); \
		int success = dstValue == comparand; \
		if (!success) { \
			memcpy(expected, &comparand, sizeof(*expected)); \
		} \
		return success; } \
	case 4: { \
		long comparand = 0; \
		memcpy(&comparand, expected, sizeof(*expected)); \
		long dstValue = _InterlockedCompareExchange((volatile long*)ptr, desired, comparand); \
		int success = dstValue == comparand; \
		if (!success) { \
			memcpy(expected, &comparand, sizeof(*expected)); \
		} \
		return success; } \
	case 8: { \
		__int64 comparand = 0; \
		memcpy(&comparand, expected, sizeof(*expected)); \
		__int64 dstValue = _InterlockedCompareExchange64((volatile __int64*)ptr, desired, comparand); \
		int success = dstValue == comparand; \
		if (!success) { \
			memcpy(expected, &comparand, sizeof(*expected)); \
		} \
		return success; } \
	default: \
		xdag_err("This atomic data size %d is not supported on windows", sizeof(*object)); \
}

int atomic_compare_exchange_strong_uintptr(atomic_uintptr_t *ptr, uintptr_t *expected, uintptr_t desired)
{
	atomic_compare_exchange_strong_switch_max64(ptr, expected, desired);
	return 0;
}

int atomic_compare_exchange_strong_uint_least64(atomic_uint_least64_t *ptr, uint_least64_t *expected, uint_least64_t desired)
{
	atomic_compare_exchange_strong_switch_max64(ptr, expected, desired);
	return 0;
}