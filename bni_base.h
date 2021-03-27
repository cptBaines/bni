/* bni_base.h - v0.1 basic types and utilities for bni libraries C - Bjorn Nilsson 2021

	This single-header-file, in Sean Berret, style provides:
		* my prefered type names. These are the type names used in other header file libs
		* my implementations of some basic utility C-lib functions
		* implementations of __stdcall mainCRTStartup() or __stdcall WinMainCRTStartup()
		  to enable building without CRT.
		* a non growable memory arena impl used for setting up argv
		* basic assert implementetation for console and gui apps

	To use this library, include this file in *one* C file.

		#define BNI_BASE_NOCRT_IMPLEMENTATION
		#include "bni_base.h"

	This will give you everything in the file and set up asserts and
	main for a console application. To setup for a gui application use:

		#define BNI_BASE_GUI_IMPLEMENTATION
		#define BNI_BASE_NOCRT_IMPLEMENTATION
		#include "bni_base.h"

COMPILE-TIME OPTIONS

	#define BNI_BASE_NOCRT_IMPLEMENTATION

	Enable copliation without windows CRT. Exposes an implementation of
	__stdcall mainCRTStartup() of __stdcall WinMainCRTStartup() depending
	on the definition of BNI_BASE_CONSOLE_IMPLEMENTATION
	or BNI_BASE_GUI_IMPLEMENTATION.

	Exposes a global arena g_mem_arena which is uesd for storing argv et. al
	The size of g_mem_arena is controlled by BNI_ARENA_SIZE

	Definition of BNI_BASE_NOCRT_IMPLEMENTATION implies
	#define BNI_BASE_ARENA_IMPLEMENTATION and #define BNI_BASE_ASSERT_IMIPLEMENTATION.

	#define BNI_BASE_ARENA_IMPLEMENTATION

	Exposes a basic implementation of a memory arena. The alignment of
	allocations from this arena is controlled by BNI_ARENA_MEMORY_ALIGNMENT

	Definition of BNI_BASE_ARENA_IMPLEMENTATION implies
	#define BNI_BASE_ASSERT_IMIPLEMENTATION.

	#define BNI_BASE_ASSERT_IMIPLEMENTATION.

	Exposes a basic implementation of assert. Needed by
	#define BNI_BASE_ARENA_IMPLEMENTATION and #define BNI_BASE_NOCRT_IMPLEMENTATION

	#define BNI_ARENA_SIZE

	Size in bytes of global arena g_mem_arean if
	#define BNI_BASE_NOCRT_IMPLEMENTATION is specified
	Default is 1M.

	#define BNI_ARENA_MEMORY_ALIGNMENT

	Alignemnt it bytes of the arena implementation exposed
	by #define BNI_BASE_ARENA_IMPLEMENTATION

	#define BNI_BASE_CONSOLE_IMPLEMENTATION

	Mutual exclusive with #define BNI_BASE_GUI_IMPLEMENTATION.
	Exposes an implementation of
	__stdcall mainCRTStartup() and sends assert messages to stderr.

	#define BNI_BASE_GUI_IMPLEMENTATION

	Mutual exclusive with #define BNI_BASE_CONSOLE_IMPLEMENTATION.
	Exposes an implementation of
	__stdcall WinMainCRTStartup() and sends assert messages to a message box.

LICENSE

	MIT licensed. See end fo file for license information.

DOCUMENTATION

	Utility Functions
		memset:
		  void *memset(void*, int, void*)
			my implementation of memset. clang needs this for zero
			initialization of structs

		bni_memset:
		  void *bni_memset(void*, int, void*)
			inline implementation of memset

		bni_copy_memory:
		  void * bni_copy_memory(void *, void *, umm)
			implementation of memcpy

		bni_count_non_zero_bytes:
		  u64 bni_count_non_zero_bytes(s8 *)
			implementation of strlen

	Arena Functions
		bni_arena_reserve:
		  void bni_arena_reserve(BniArena *a, umm n)
			Reserve n bytes of space in arean a

		void * bni_arena_alloc (BniArena *a , umm n)
			Return a pointer to a chunk of n bytes
		    from areana a

	Arena macros
		push_arena_struct:
		  T* push_arena_struct(BniArena *a, T)
			Return a pointer to type T, allocated
			from arena a

		push_arena_array:
		  T* push_arena_array(BniArena *a, T, umm c)
				Return a pointer to an array c elements of type T
				, allocated from arena a

	Assert macro
		bni_assert:
	      void bni_assert_expr_wide(expr)
			evaluate expr and terminates with exit
			code 3 on failure.

	NoCRT functions
		mainCRTStartup:
		  void __stdcall mainCRTStartup()
			Used by console applications to setup argv and
			calls main
		WinMainCRTStartup:
		  void __stdcall WinMainCRTStartup()
			Used by gui applications to setup argv and
			calls main

*/

#ifndef INCLUDED_BNI_BASE_H
#define INCLUDED_BNI_BASE_H

#define BNI_VERSION 1

#define BNI_BASE_CONSOLE_IMPLEMENTATION

#if defined(BNI_BASE_GUI_IMPLEMENTATION)
#undef BNI_BASE_CONSOLE_IMPLEMENTATION
#endif

#if defined(BNI_BASE_NOCRT_IMPLEMENTATION)
#define BNI_BASE_ARENA_IMPLEMENTATION
#endif

#if defined(BNI_BASE_ARENA_IMPLEMENTATION)
#define BNI_BASE_ASSERT_IMIPLEMENTATION
#endif

#include <stdint.h>

typedef char       s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u64      umm;
typedef s64      smm;

typedef float    f32;
typedef double   f64;

typedef s32      bool;
typedef s32      b32;
typedef s8       b8;
typedef s64      b64;

#define true  (1 == 1)
#define false (1 == 0)

typedef unsigned short wchar_t;


/* NOTE(bjorn): a non inlined version of memset
 * is needed by clang for zero initialization of structs
 * i.e.  struct my_struct = {0};
 */
void *
memset(void *dst, s32 value, u64 size)
{
	u8 *p = (u8*)dst;
	while (size > 0)
	{
		*p++ = (u8)value;
		--size;
	}
	return dst;
}

static inline void *
bni_memset(void *dst, s32 value, u64 size)
{
	u8 *p = (u8*)dst;
	while (size > 0)
	{
		*p++ = (u8)value;
		--size;
	}
	return dst;
}

static inline void *
bni_copy_memory(void *dst, void *src, umm size)
{
	u8 *d = (u8*)dst;
	u8 *s = (u8*)src;

	while (size > 0)
	{
		*d++ = *s++;
		--size;
	}
	return dst;
}

static inline u64
bni_count_non_zero_bytes(s8 *str)
{
	s8 *p = str;
	while (*p != '\0')
		++p;
	return ((umm)p - (umm)str);
}
#endif

#if defined(BNI_BASE_ASSERT_IMIPLEMENTATION)
#include <windows.h>

#define bni_assert_expr_wide(expr) L##expr
#define bni_assert(expr) if(!(expr)) bni_assert_(bni_assert_expr_wide(#expr) ":" __FILE_NAME__, __LINE__)
#define BNI_ASSERT_MSG_SIZE 128

void bni_assert_(wchar_t *expr_str, u32 lineno)
{
	u32 pos = 0;
	wchar_t msg[BNI_ASSERT_MSG_SIZE];

	wchar_t *p = msg;
	while(pos < BNI_ASSERT_MSG_SIZE - 1 && *expr_str != 0)
	{
		*p++ = *expr_str++;
		++pos;
	}

	if (pos + 1 + 11 < BNI_ASSERT_MSG_SIZE)
	{
		p += 11;
		*p = 0;
		--p;
		for(int i=0; i < 10; i++)
		{
			u32 digit = lineno % 10;
			lineno /= 10;
			*p-- = '0' + digit;
		}
		*p = ':';
	}
	else
	{
		*p = 0;
	}

#if defined(WIN_GUI)
	s32 ret = MessageBoxW(NULL, msg, L"Assert"
		, MB_ABORTRETRYIGNORE
		| MB_SETFOREGROUND
		| MB_TASKMODAL
		| MB_ICONERROR);
	if (ret == IDABORT)
		ExitProcess(3);

#else
	DWORD written = 0;
	HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
	WriteConsoleW(hStderr, msg, p - msg, &written, (void*)0);
	ExitProcess(3);
#endif
}
#endif /* BNI_BASE_ASSERT_IMIPLEMENTATION */

#if defined(BNI_BASE_ARENA_IMPLEMENTATION)


#ifndef BNI_MEM_ALIGNMENT
#define BNI_MEM_ALIGNMENT 8U
#endif

typedef struct BniArena
{
	umm  size;
	umm  used;
	umm  hwm;
	u8   *base;
} BniArena;

static inline void
bni_arena_reserve
(
	BniArena *arena
	, umm size
)
{
	umm fraction = (size & (BNI_MEM_ALIGNMENT-1));
	fraction = (fraction == 0 ? BNI_MEM_ALIGNMENT : fraction);
	umm alloc_size = size + BNI_MEM_ALIGNMENT - fraction;
	if (arena->used + alloc_size < arena->size)
	{
		arena->used += alloc_size;
	}
	else
	{
		bni_assert(!"Arena OOM");
	}
}

static inline void *
bni_arena_alloc
(
	  BniArena *arena
	, umm size
)
{
	void *result = 0;
	umm old_used = arena->used;
	bni_arena_reserve(arena, size);
	result = (void*)((u8*)arena->base + old_used);
	return result;
}

#define push_arena_struct(arena, type) \
	(type*)bni_arena_alloc((arena), sizeof(type))

#define push_arena_array(arena, type, count) \
	(type*)bni_arena_alloc((arena), sizeof(type) * (count))

#define push_struct(type) \
	(type*)bni_arena_alloc(&bni__mem_arena, sizeof(type))

#define push_mem(type, size) \
	(type*)bni_arena_alloc(&bni__mem_arena, size)

#define push_array(type, count) \
	(type**)bni_arena_alloc(&bni__mem_arena, sizeof(type) * (count))

#endif /* BNI_BASE_ARENA_IMPLEMENTATION */

#if defined(BNI_BASE_NOCRT_IMPLEMENTATION)

#ifndef BNI_ARENA_SIZE
#define BNI_ARENA_SIZE 1024*1024
#endif

static BniArena bni__mem_arena;

s32 main(s32 argc, char **argv);

int _fltused;

#if defined(BNI_BASE_CONSOLE_IMPLEMENTATION)
void __stdcall mainCRTStartup()
#else
void __stdcall WinMainCRTStartup()
#endif
{

	bni__mem_arena.size = BNI_ARENA_SIZE;
	bni__mem_arena.used = 0;
	bni__mem_arena.hwm  = 0;
	bni__mem_arena.base = VirtualAlloc(NULL, bni__mem_arena.size
			, MEM_COMMIT, PAGE_READWRITE);

	bni_assert(bni__mem_arena.base);
	s8 *cmdline = GetCommandLineA();

	u64 len = bni_count_non_zero_bytes(cmdline);
	s8 *args = push_mem(s8, len + 1);
	s8 **argv = (s8**)push_mem(s8, 0);
	s8 **argv_ind = 0;
	u32 argc = 0;

	s8 *src = cmdline;
	s8 *dst = args;
	s8 *beg_arg = args;

	bool in_string = false;
	while (*src != '\0')
	{
		if (*src == '"')
		{
			++src;
			in_string = !in_string;
		}
		else if (in_string || *src != ' ')
		{
			*dst++ = *src++;
		}
		else
		{
			++argc;
			*dst++ = '\0';
			while (*src == ' ')
				++src;

			argv_ind = (s8**)push_mem(s8, sizeof(s8*));
			*argv_ind = beg_arg;
			beg_arg = dst;
		}
	}
	++argc;	       // account for last arg
	*dst++ = '\0'; // zero terminate last string

	argv_ind = (s8**)push_mem(s8, sizeof(s8*));
	*argv_ind = beg_arg;

	argv_ind = (s8**)push_mem(s8, sizeof(s8*));
	*argv_ind = 0;

	int Result = main(argc,argv);
    ExitProcess(Result);
}
#endif /* BNI_BASE_NOCRT_IMPLEMENTATION */


/*
  MIT License
  Copyright (c) 2020 Bjorn Nilsson
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
  of the Software, and to permit persons to whom the Software is furnished to do
  so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

// vim: ts=4 sw=4
