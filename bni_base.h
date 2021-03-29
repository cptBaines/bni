/* bni_base.h - v0.2 basic types and utilities for bni libraries C - Bjorn Nilsson 2021

	TODO(Bjorn): Reworked 2021-03-29 included utf8, dynamic arena and basic file io
				 Things that I usually need for a basic platform layer. Need to update
				 the documentation to reflect these changes. Specifically the
				 compile-time options which have changed. See comment bellow.



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

#ifndef INCLUDED_BNI_PLATFORM
#define INCLUDED_BNI_PLATFORM

#if !defined(BNI_PLATFORM_WINDOWS_IMPLEMENTATION) && !defined(BNI_PLATFORM_UNIX_IMPLEMENTATION)
#error BNI_PLATFORM_WINDOWS_IMPLEMENTATION or BNI_PLATFORM_UNIX_IMPLEMENTATION must be defined
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

#define array_len(a) (sizeof((a))/sizeof((a)[0]))
#define offsetof(struct_name, member_name) (((struct_name*)0)->(member_name));


/*
TODO(Bjorn): Document these new compile-time options. PLATFORM should probably become BASE
#define BNI_GUI controls GUI/CONSOLE versions default CONSOLE
#if !defined(BNI_PLATFORM_WINDOWS_IMPLEMENTATION) && !defined(BNI_PLATFORM_UNIX_IMPLEMENTATION)
Platform gives you
 bni_types
 bni_assert
 optionally bni_utf   #define BNI_BASE_UTF_IMPLEMENTATION
 optionally bni_arena #define BNI_BASE_ARENA_IMPLEMENTATION
 optionally bni_file - requires bni_utf && bni_arena #defined BNI_BASE_FILE_IMPLEMENTATION
 optionally bni_no_crt - requires bni_utf, bni_arean???  #define BNI_BASE_NOCRT_IMPLEMENTATION
 */


#if !defined(BNI_GUI)
#define BNI_CONSOLE
#endif

#if defined(BNI_BASE_FILE_IMPLEMENTATION)
#define BNI_BASE_UTF_IMPLEMENTATION
#define BNI_BASE_ARENA_IMPLEMENTATION
#endif

#if defined(BNI_PLATFORM_WINDOWS_IMPLEMENTATION)

#include <windows.h>

typedef struct BniFile
{
	HANDLE file;
	u32 platform_error;
} BniFile;

#else /* BNI_PLATFORM_UNIX_IMPLEMENTATION */

typedef struct BniFile
{
	s32 file;
	u32 platform_error;
} BniFile;

#endif

typedef struct BniArena
{
	u8 *base;
	umm size;
	umm used;
	umm min_block_size;
} BniArena;

typedef struct BniArenaHeader
{
	u8 *base;
	umm size;
	umm used;
} BniArenaHeader;

typedef struct TempMemory
{
	BniArena *arena;
	u8 *base;
	umm used;
} TempMemory;

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

#endif /* INCLUDED_BNI_PLATFORM */

#if defined(BNI_PLATFORM_WINDOWS_IMPLEMENTATION)

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

#if defined(BNI_GUI)
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

#if defined(BNI_BASE_UTF_IMPLEMENTATION)

static inline umm
bni_utf8_bytelen(s8 *buf)
{
	umm n = 0;
	while (*buf != 0)
	{
		n++;
		buf++;
	}
	return n;
}

static inline umm
bni_utf16le_bytelen(wchar_t *buf)
{
	umm n = 0;
	while (*buf != 0)
	{
		n++;
		buf++;
	}
	return (n * sizeof(wchar_t));
}

static inline umm
bni_utf8_strlen(s8 *buf)
{
	umm n = 0;
	while (*buf != 0)
	{
		if ((*buf & 0xC0) != 0x80)
			n++;
		buf++;
	}
	return n;
}

static inline umm
bni_utf16le_strlen(wchar_t *buf)
{
	umm n = 0;
	u8 c;
	while (*buf != 0)
	{
		c = (u8)((*buf) & 0x00FF);
		if (c <= 0xD7)
		{
			buf++;
			n++;
			continue;
		}
		if (c <= 0xDF)
		{
			if (*(buf+1) == 0)
				break;
			buf += 2;
			n++;
			continue;
		}
		buf++;
		n++;
	}
	return n;
}

static inline s32
bni_utf8_to_cp_u32i
(
	 u32 *cp
	, s8 *buf
	, umm size
)
{
	s8 enc_cp = *buf;
	int follow_bytes = 0;
	u32 tmp;
	if ((enc_cp & 0x80) == 0)
	{
		*cp = enc_cp;
		return 1;
	}

	if ((enc_cp & 0xF8) == 0xF0)
	{
		tmp = ((enc_cp &  0x07) << 18);
		follow_bytes =  3;
	}
	else if ((enc_cp & 0xF0) == 0xE0)
	{
		tmp = ((enc_cp &  0x0F) << 12);
		follow_bytes = 2;
	}
	else if ((enc_cp & 0xE0) == 0xC0)
	{
		tmp = ((enc_cp &  0x1F) << 6);
		follow_bytes = 1;
	}
	else
	{
		// TODO(bjor): Handle encoding error
		// Follow byte without a header byte
		// Skip for now
		bni_assert(!"UTF-8 encoding error");
		return 1;
	}

	buf++;
	int j = follow_bytes;
	while (j > 0 && ((*buf & 0xC0) == 0x80))
	{
		tmp |= (*buf++ & 0x03F) << (--j * 6);
	}
	if (j > 0)
	{
		// TODO(bjor): handle encoding error
		// to few follow bytes
		bni_assert(!"UTF-8 encoding error");
	} else {
		*cp = tmp;
	}
	return follow_bytes + 1 - j;
}

static inline s32
bni_cp_u32i_to_utf8(
	  s8 *buf
	, umm size
	, u32 cp
)
{

	bni_assert(buf != NULL && size >= 4);

	int bytes_consumed = 0;
	if (cp < 0x80)
	{
		*buf = (uint8_t)cp;
		bytes_consumed = 1;
	}
	else if (cp >= 0x80 && cp <= 0x07FF)
	{
		*(buf + 0) = (0xC0 | (uint8_t)(cp >> 6));
		*(buf + 1) = (0x80 | (uint8_t)(cp & 0x0000003F));
		bytes_consumed = 2;
	}
	else if (cp >= 0x0800 && cp <= 0xFFFF)
	{
		*(buf + 0) = (0xE0 | (uint8_t)(cp >> 12));
		*(buf + 1) = (0x80 | (uint8_t)((cp & 0x00000FC0) >> 6));
		*(buf + 2) = (0x80 | (uint8_t)(cp & 0x0000003F));
		bytes_consumed = 3;
	}
	else if (cp >= 0x10000 && cp <= 0x10FFFF)
	{
		*(buf + 0) = (0xF0 | (uint8_t)(cp >> 18));
		*(buf + 1) = (0x80 | (uint8_t)((cp & 0x0003F000) >> 12));
		*(buf + 2) = (0x80 | (uint8_t)((cp & 0x00000FC0) >> 6));
		*(buf + 3) = (0x80 | (uint8_t)(cp & 0x0000003F));
		bytes_consumed = 4;
	}
	else
	{
		bni_assert(!"Code point out of range > 0x10FFFF");
	}
	return bytes_consumed;
}

static inline s32
bni_cp_u32i_to_utf16le
(
	  u8  *buf
	, umm size
	, u32 cp
)
{
	bni_assert(buf != NULL && size >= 4);
	int bytes_consumed = 0;

	if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF))
	{
		*((uint16_t*)buf) = (uint16_t)cp;
		bytes_consumed = 2;
	}
	else if (cp > 0xFFFF)
	{
		cp -= 0x10000;
		*((uint16_t*)buf) = (uint16_t)((cp >> 10)   | 0xD800);
		*(((uint16_t*)buf) + 1) = (uint16_t)((cp & 0x3FF) | 0xDC00);
		bytes_consumed = 4;
	}
	else
	{
		bni_assert(!"Invalid code point, 0xD800 - 0xDFFF");
	}
	return bytes_consumed;
}

static inline s32
bni_utf16le_to_cp_u32i
(
	  u32 *cp
	, u8  *buf
	, umm  size
)
{
	u16 enc_cp = *((u16*)buf);
	s32 bytes_consumed = 0;

	if (enc_cp <= 0xD7FF || (enc_cp >= 0xE000 && enc_cp <= 0xFFFF))
	{
		*cp = enc_cp;
		bytes_consumed =  2;
	}
	else if (enc_cp >= 0xD800 && enc_cp <= 0xDBFF)
	{
		if (2 <= size)
		{
			u16 hi_surrogate = enc_cp;
			u16 lo_surrogate = *(((u16*)buf) + 1);
			if (lo_surrogate >= 0xDC00 && lo_surrogate <= 0xDFFF)
			{
				uint16_t hi = (hi_surrogate - 0xD800) * 0x400;
				uint16_t lo = (lo_surrogate - 0xDC00);
				*cp = (u32)hi + (u32)lo + 0x10000;
				bytes_consumed = 4;
			}
			else
			{
				// TODO(bjorn): Handle missmatch surrogate
				// pairs high but no low
				bni_assert(!"Missmatch surrogate high, no low");
			}
		}
		else
		{
			// TODO(bjorn): Handle missmatch surrogate
			// pairs high but no low
			bni_assert(!"high surrogate at end of buffer");
		}
	}
	else
	{
		// TODO(bjorn): Handle missmatch surrogate
		// pairs low but no high
		bni_assert(!"Missmatch surrogate low, no high");
	}

	return bytes_consumed;
}

static s32
bni_utf8_to_utf16le
(
	  u8 *utf16
	, umm len16
	, s8 *utf8
	, umm len8
)
{
	// NOTE(bjorn): len[8|16] is lenght in bytes
	bni_assert(len8 * 2 < len16);

	u32 cp = 0;
	umm nwrite = 0;

	// TODO(bjorn): Handle errors
	while(len8 > 0)
	{
		umm n;
		n = bni_utf8_to_cp_u32i(&cp, utf8, len8);
		utf8 += n;
		len8 -= n;

		n = bni_cp_u32i_to_utf16le(utf16, len16, cp);
		utf16 += n;
		len16 -= n;
		nwrite += n;
	}
	return nwrite;
}

static s32
bni_utf16le_to_utf8
(
	  s8 *utf8
	, umm len8
	, u8 *utf16
	, umm len16
)
{
	// NOTE(bjorn): len[8|16] is lenght in bytes
	bni_assert(len8 * 2 > len16);

	u32 cp = 0;
	umm nwrite = 0;

	// TODO(bjorn): Handle errors
	while(len16 > 0)
	{
		umm n;
		n = bni_utf16le_to_cp_u32i(&cp, utf16, len16);
		utf16 += n;
		len16 -= n;

		n = bni_cp_u32i_to_utf8(utf8, len8, cp);
		utf8 += n;
		len8 -= n;
		nwrite += n;
	}
	return nwrite;
}

#endif  /* BNI_BASE_UTF_IMPLEMENTATION */


#if defined(BNI_BASE_ARENA_IMPLEMENTATION)

#ifndef BNI_MEM_ALIGNMENT
#define BNI_MEM_ALIGNMENT 8U
#endif

#if defined(BNI_PLATFORM_WINDOWS_IMPLEMENTATION)

#define bni_arena_memory_alloc(size) \
	bni_win32_arena_memory_alloc((size))

#define bni_arena_memory_free(mem_block) \
	bni_win32_arena_memory_free((mem_block))

static inline void *
bni_win32_arena_memory_alloc(umm size)
{
	return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
}

static inline void
bni_win32_arena_memory_free(void *arena_block)
{
	VirtualFree(arena_block, 0, MEM_RELEASE);
}

#endif /* BNI_PLATFORM_WINDOWS_IMPLEMENTATION */

#define bni_arena_bootstrap(min_block_size) \
     (BniArena*)bni_arena_create(sizeof(BniArena), 0, (min_block_size))

#define bni_arena_bootstrap_struct(type, arena_member, min_block_size) \
     (type*)bni_arena_create(sizeof((type)), offsetoff((type), (arena_member)), (min_block_size))

#define bni_arena_push_array(arena, type, count) \
	(type*)bni_arena_reserve((arena), sizeof(type) * (count))

#define bni_arena_push_ptr(arena, type) \
	(type*)bni_arena_reserve((arena), sizeof(type*))

#define bni_arena_push_struct(type) \
	(type*)bni_arena_reserve((arena), sizeof(type))

static inline umm
bni_arena__aligned_size(umm size)
{
	umm fraction = (size & (BNI_MEM_ALIGNMENT-1));
	fraction = (fraction == 0 ? BNI_MEM_ALIGNMENT : fraction);
	umm alloc_size = size + BNI_MEM_ALIGNMENT - fraction;
	return alloc_size;
}

static inline BniArenaHeader
bni_arena_get_header(BniArena *arena)
{
	BniArenaHeader result = *(BniArenaHeader*)(arena->base
			- bni_arena__aligned_size(sizeof(BniArenaHeader)));
	return result;
}

static void *
bni_arena_create(umm size, umm offset, umm min_block_size)
{
	if (min_block_size < 64 * 1024)
		min_block_size = 64 * 1024;

	umm aligned_header_size = bni_arena__aligned_size(sizeof(BniArenaHeader));
	umm aligned_size = bni_arena__aligned_size(size);
	umm alloc_size = aligned_size + aligned_header_size;
	if (alloc_size < min_block_size)
		alloc_size = min_block_size;

	BniArenaHeader *hdr = (BniArenaHeader*)bni_arena_memory_alloc(alloc_size);
	bni_assert(hdr);

	hdr->base = ((u8*)hdr) + aligned_header_size;
	hdr->size = alloc_size - aligned_header_size;
	hdr->used = aligned_size;

	BniArena *arena = (BniArena*)(hdr->base + offset);
	arena->base = hdr->base;
	arena->size = hdr->size;
	arena->used = hdr->used;
	arena->min_block_size = min_block_size;

	return hdr->base;
}

void *
bni_arena_reserve(BniArena *arena, umm size)
{
	void *mem;
	umm aligned_size = bni_arena__aligned_size(size);

	if (arena->used + aligned_size < arena->size)
	{
		mem = arena->base + arena->used;
		arena->used += aligned_size;
	}
	else
	{
		umm aligned_header_size = bni_arena__aligned_size(sizeof(BniArenaHeader));
	    umm alloc_size = aligned_size + aligned_header_size;
		if (alloc_size < arena->min_block_size)
			alloc_size = arena->min_block_size;

		BniArenaHeader *hdr = (BniArenaHeader*)bni_arena_memory_alloc(size);
		bni_assert(hdr);
		hdr->base = arena->base;
		hdr->size = arena->size;
		hdr->used = arena->used;

		hdr->base = ((u8*)hdr) + aligned_header_size;
		hdr->size = alloc_size - aligned_header_size;
		hdr->used = aligned_size;
		mem = arena->base;
	}
	return mem;
}

TempMemory
begin_temp_memory(BniArena *arena)
{
	TempMemory result;
	result.arena = arena;
	result.base = arena->base;
	result.used = arena->used;
	return result;
}

// NOTE(bjorn): Disard everything on the arena
// up to and including pointer pointing to by object
void
end_temp_memory(TempMemory *tmp)
{
	BniArena *arena = tmp->arena;
	while(arena->base != tmp->base)
	{
		BniArenaHeader hdr = bni_arena_get_header(arena);
		bni_arena_memory_free(arena->base - sizeof(BniArenaHeader));
		arena->base = hdr.base;
		arena->size = hdr.size;
		arena->used = hdr.used;
	}
	bni_assert(arena->used >= tmp->used);
	arena->used = tmp->used;
}


#endif /* BNI_BASE_ARENA_IMPLEMENTATION */

#if defined(BNI_BASE_FILE_IMPLEMENTATION)

#if defined(BNI_PLATFORM_WINDOWS_IMPLEMENTATION)

#define bni_file_read(file, data, size) \
	bni_win32_file_read((file), (data), (size))

#define bni_file_write(file, data, size) \
	bni_win32_file_write((file), (data), (size))

#define bni_file_get_stdout() bni_win32_get_stdout()

#define bni_file_get_size(file) bni_win32_file_get_size((file))
#define bni_file_open(file_name, file_mode, arena) \
		bni_win32_file_open((file_name), (file_mode), (arena))

static inline BniFile
bni_win32_get_stdout()
{
	BniFile result = {0};
	result.file = GetStdHandle(STD_OUTPUT_HANDLE);
	return result;
}

static inline u32
bni_win32_file_write(BniFile file, u8 *data, u32 size)
{
	DWORD bytes_written;
	if (!WriteFile(file.file, data, size, &bytes_written, 0))
	{
		file.platform_error = GetLastError();
	}
	return (u32)bytes_written;
}

static inline u32
bni_win32_file_read(BniFile file, u8 *buf, u32 size)
{
	DWORD bytes_read;
	if (!ReadFile(file.file, buf, size, &bytes_read, 0))
	{
		file.platform_error = GetLastError();
	}
	return (u32)bytes_read;
}

static u64
bni_win32_file_get_size(BniFile file)
{
	LARGE_INTEGER size = {0};
	if(!GetFileSizeEx(file.file, &size))
	{
		file.platform_error = GetLastError();
	}
	return (u64)size.QuadPart;
}

// NOTE(bjorn): this is a purely internal enum.
// Figure out a good naming convetion Bni__FileMode???
typedef enum BniFileMode
{
	BniFileMode_read = 0x01,
	BniFileMode_write = 0x02,
	BniFileMode_append = 0x04,
} BniFileMode;


static inline u32
bni_win32__file_mode(s8 *mode_str)
{
	u32 mode = 0;
	while(*mode_str != 0)
	{
		if (*mode_str == 'r')
			mode |= BniFileMode_read;
		if (*mode_str == 'w')
			mode |= BniFileMode_write;
		if (*mode_str == 'a')
			mode |= BniFileMode_append;
		++mode_str;
	}
	return mode;
}

static inline s16 *
bni_win32_api_str(s8 *str, umm len, TempMemory *tmp)
{
	umm api_str_len = 2 * len;
	u8 *api_str = bni_arena_push_array(tmp->arena, u8, api_str_len + 8);
	u32 used = bni_utf8_to_utf16le((u8*)api_str, api_str_len + 2, str, len);
	*((u16*)api_str + used) = 0;
	return (s16*)api_str;
}

static BniFile
bni_win32_file_open(s8 *file_name, s8 *mode, BniArena *arena)
{
	BniFile result = {0};
	u32 file_mode = bni_win32__file_mode(mode);

	u32 access = 0;
	u32 create = 0;

	if (file_mode == BniFileMode_read)
	{
		access |= GENERIC_READ;
		create |= OPEN_EXISTING;
	}
	else if (file_mode == BniFileMode_write)
	{
		access |= GENERIC_WRITE;
		create |= CREATE_ALWAYS;
	}
	else if ((file_mode & (BniFileMode_write | BniFileMode_append))
			== (BniFileMode_write | BniFileMode_append))
	{
		access |= (GENERIC_READ | GENERIC_WRITE);
		create |= OPEN_EXISTING;
	}

	u32 len = bni_utf8_bytelen(file_name);

	TempMemory tmp = begin_temp_memory(arena);

	s16 *api_file_name = bni_win32_api_str(file_name, len, &tmp);
	result.file = CreateFileW((u16*)api_file_name, access, FILE_SHARE_READ | FILE_SHARE_WRITE
			, 0, create, FILE_ATTRIBUTE_NORMAL, 0);

	if (result.file == INVALID_HANDLE_VALUE)
	{
		result.platform_error = GetLastError();
	}

	end_temp_memory(&tmp);
	return result;
}

#endif /* BNI_PLATFORM_WINDOWS_IMPLEMENTATION */

#if defined(BNI_BASE_NOCRT_IMPLEMENTATION)

#ifndef BNI_ARENA_SIZE
#define BNI_ARENA_SIZE 1024*1024
#endif

static BniArena *bni__base_arena;

s32 main(s32 argc, char **argv);

int _fltused;

#if defined(BNI_CONSOLE)
void __stdcall mainCRTStartup()
#else
void __stdcall WinMainCRTStartup()
#endif
{

	// TODO(bjorn): Make this use GetCommandLineW and convert
	// args to utf8 before putting them in argv.

	bni__base_arena = bni_arena_bootstrap(1);
	bni_assert(bni__base_arena);
	BniArena *arena = bni__base_arena;

	s8 *cmdline = GetCommandLineA();

	u64 len = bni_count_non_zero_bytes(cmdline);
	s8 *args = bni_arena_push_array(arena, s8, len + 1);
	s8 **argv = (s8**)bni_arena_reserve(arena, 0);
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

			argv_ind = (s8**)bni_arena_push_ptr(arena, s8);
			*argv_ind = beg_arg;
			beg_arg = dst;
		}
	}
	++argc;	       // account for last arg
	*dst++ = '\0'; // zero terminate last string


	argv_ind = (s8**)bni_arena_push_ptr(arena, s8);
	*argv_ind = beg_arg;

	argv_ind = (s8**)bni_arena_push_ptr(arena, s8);
	*argv_ind = 0;

	int Result = main(argc,argv);
    ExitProcess(Result);
}
#endif /* BNI_BASE_NOCRT_IMPLEMENTATION */


#endif /* BNI_PLATFORM_IMPLEMENTATION */

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
