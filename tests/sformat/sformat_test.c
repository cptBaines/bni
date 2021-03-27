#define BNI_SFORMAT_IMPLEMENTETAION
#define BNI_BASE_NOCRT_IMPLEMENTATION
#include "..\..\bni_base.h"
#include "..\..\bni_sformat.h"

#include "float_test_data.h"

#define array_size(a) (sizeof((a))/sizeof((a)[0]))

/*
 At the moment we fail 80 of the 700+ tests all those
 fail on the last decimal, I deem it sufficient for
 my needs at the moment.

 The main reason for these failures is that the string version of
 the values in float_test_data, which we use to verify our conversions,
 are generated from a double representation of the corresponding floating
 point bit-pattern. This gives more significant digits and therefor
 a better rounding.

	Test summary:
	  Passed: 668
	  Failed: 80
*/

static inline f32
float_from_hex(u32 hex)
{
	f32 result = *(float*)&hex;
	return result;
}

static inline bool
string_equal(s8 *a, s8 *b, u32 b_size)
{
	while(a != 0 && b_size > 0 && *a == *b)
	{
		++a; ++b; --b_size;
	}
	bool equal = (*a == 0 && b_size == 0);
	return equal;
}

s32
main(s32 argc, s8 **argv)
{
	s8 ascii[32] = {0};
	s8 out[256];

	STBniStringFormater *fmt = bni_formater_create(&bni__mem_arena
			, 3, "Failed %: expected: % != %\n");

	HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
	bni_assert(stdout);

	u32 bytes_to_write = 0;
	u32 failed = 0;
	u32 passed = 0;
	for (s32 i = 0
		; i < array_size(test_data)
		; ++i)
	{
		if (i == 72)
		{
			u32 test = 4;
		}
		f32 v = float_from_hex(test_data[i].hex);
		s8 *expected = test_data[i].expected_result;
		STBniStringFormatArg fa = bni_fa_e(v);
		u32 size = bni_format_float((u8*)ascii, 32, &fa);
		bool is_equal = string_equal(expected, ascii, size);
		if (!is_equal)
		{
			ascii[size] = 0;
			bni_push_format_arg(fmt, bni_fa_zu(i,3));
			bni_push_format_arg(fmt, bni_fa_t(expected));
			bni_push_format_arg(fmt, bni_fa_t(ascii));
			bytes_to_write = bni_format((u8*)out, 256, fmt);
			WriteFile(stdout, out, bytes_to_write, 0, 0);
			bni_formater_reset(fmt);
			++failed;
		}
		else
			++passed;
	}

	fmt = bni_formater_create(&bni__mem_arena
			, 2, "Test summary:\n  Passed: %\n  Failed: %\n");
	bni_push_format_arg(fmt, bni_fa_u(passed));
	bni_push_format_arg(fmt, bni_fa_u(failed));
	bytes_to_write = bni_format((u8*)out, 256, fmt);
	WriteFile(stdout, out, bytes_to_write, 0, 0);
}

// vim: ts=4 sw=4
