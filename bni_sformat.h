/* bni_sformat.h - v0.1 public domain string formating for C - Bjorn Nilsson 2021

	This single-header-file, in Sean Berret, style provides a
	way to format text, integers, pointers and floats into a provided buffer.

	To use this library, include this file in *one* C file.
		#define BNI_SFORMAT_IMPLEMENTETAION
		#include "bni_base.h"
		#include "bni_sformat.h"

	TODO: include snprintf compatible wrapper
	#define BNI_SFORMAT_WRAPPERS


LICENSE

	MIT licensed. See end of file for license information.

DOCUMNETATION

	This header library depends on the types and asssert and utility
	functions declared in bni_base.h

	Functions:
		bni_formater_create:
		  BniStringFormater * bni_formater_create(BniArena *a , u32 c , s8 *s)
			Create a memory area with room for c arguments to be
			injected into format string s from arena a.

		bni_formater_reset:
		  void bni_formater_reset(BniStringFormater *)
			Reset formater argument count and enable reuse
			with new parameters

		bni_push_format_arg:
		  void bni_push_format_arg (BniStringFormater *f, BniStringFormatArg a)
			Push a new argument a onto then next free position in formater f.

		bni_format_str:
		  u32 bni_format_str (u8 *o , umm s, BniStringFormatArg *a)
			output a string to buffer o of size s according to spec in
			format argument a.
			Return the number of bytes used in o.

		bni_format_integer:
		  u32 bni_format_integer(u8 *o , umm s, BniStringFormatArg *a)
			output a signed or unsigned integer to buffer o of size s
			according to spec in fomat argument a.
			Return the number of bytes used in o.

		bni_format_float:
		  u32 bni_format_float(u8 *o , umm s, BniStringFormatArg *a)
			output a float to buffer o of size s
			according to spec in fomat argument a.
			Return the number of bytes used in o.

		bni_format:
		  u32 bni_format(u8 *o, umm c, BniStringFormater *f)
			output the content of the formater f to buffer o of size c.
			Return the number of bytes used in o.

		bni_format_arg_f:
		  BniStringFormatArg bni_format_arg_f(u64 f, f32 v, u16 w, u16 p)
			creats a format argument for a float.
			According to the format flags f, widht w and precision p

		bni_format_arg_s:
		  BniStringFormatArg bni_format_arg_s(u64 f, s64 v, u16 w, u16 p)
			creats a format argument for a signed integer.
			According to the format flags f, widht w and precision p

		bni_format_arg_u:
		  BniStringFormatArg bni_format_arg_u(u64 f, u64 v, u16 w, u16 p)
			creats a format argument for a unsigned integer.
			According to the format flags f, widht w and precision p

		bni_format_arg_p:
		  BniStringFormatArg bni_format_arg_u(u64 f, void *v, u16 w, u16 p)
			creats a format argument for a pointer
			According to the format flags f, widht w and precision p

		bni_format_arg_str:
		  BniStringFormatArg bni_format_arg_cstr(u64 f, s8 *v, u16 w, u16 p)
			creats a format argument for a string
			According to the format flags f, widht w and precision p

	Macros:
		These macros are a short hand to use the bni_format_arg_xxx functions
		with the usual flag combinations.

		The macros are on the form bni_fa_xxxxt where xxxx are format specifiers
		and t denotes the type of the data passed.

		x format specifiers are optional and have the following mnemonics:

			c = cushion  (add space infront of positive nbrs)
			d = decorate (add + to in front of positive nbrs)
			z = zeros pad
			x = hex
			o = octal
			[l|r] = justify [(l) left, (r) right]

		the format specifiers are always stacked int the order: [c|d][z][x|o][l|r]
		right justification is the norm its just used in the shorthand to expose
		the width field to the user.

		t type identifier is mandatroy and always one of:
			s = signed
			u = unsigned
			[f|e|g] = float [(f) float, (e) exponent, (g) sientific]
			p = pointer
			t = string [(t) text]

		Unsigned macros
		  bni_fa_clu(val,width)
		  bni_fa_dlu(val,width)
		  bni_fa_zlu(val,width)
		  bni_fa_czlu(val,width)
		  bni_fa_dzlu(val,width)
		  bni_fa_zxlu(val,width)
		  bni_fa_zolu(val,width)
		  bni_fa_xlu(val,width)
		  bni_fa_olu(val,width)
		  bni_fa_lu(val,width)
		  bni_fa_zu(val,width)
		  bni_fa_czu(val,width)
		  bni_fa_dzu(val,width)
		  bni_fa_zxu(val,width)
		  bni_fa_zou(val,width)
		  bni_fa_xru(val,width)
		  bni_fa_oru(val,width)
		  bni_fa_cru(val,width)
		  bni_fa_dru(val,width)
		  bni_fa_u(val)
		  bni_fa_cu(val)
		  bni_fa_du(val)
		  bni_fa_xu(val)
		  bni_fa_ou(val)

		Usigned macros
		  bni_fa_cls(val,width)
		  bni_fa_dls(val,width)
		  bni_fa_zls(val,width)
		  bni_fa_czls(val,width)
		  bni_fa_dzls(val,width)
		  bni_fa_zxls(val,width)
		  bni_fa_zols(val,width)
		  bni_fa_xls(val,width)
		  bni_fa_ols(val,width)
		  bni_fa_ls(val,width)
		  bni_fa_zs(val,width)
		  bni_fa_czs(val,width)
		  bni_fa_dzs(val,width)
		  bni_fa_zxs(val,width)
		  bni_fa_zos(val,width)
		  bni_fa_xrs(val,width)
		  bni_fa_ors(val,width)
		  bni_fa_crs(val,width)
		  bni_fa_drs(val,width)
		  bni_fa_s(val)
		  bni_fa_cs(val)
		  bni_fa_ds(val)
		  bni_fa_xs(val)
		  bni_fa_os(val)

		Pointer macros
		  bni_fa_p(val,width)
		  bni_fa_rp(val,width)
		  bni_fa_lp(val,width)

		String macros
		  bni_fa_t(val)
		  bni_fa_lt(val,width)
		  bni_fa_rt(val,width)
		  bni_fa_t_(val,width,precision)
		  bni_fa_lt_(val,width,precision)
		  bni_fa_rt_(val,width,precision)

		Float macros
		  bni_fa_f(val)
		  bni_fa_cf(val)
		  bni_fa_df(val)
		  bni_fa_crf(val,width)
		  bni_fa_drf(val,width)
		  bni_fa_zf(val,width)
		  bni_fa_czf(val,width)
		  bni_fa_dzf(val,width)
		  bni_fa_lf(val,width)
		  bni_fa_clf(val,width)
		  bni_fa_dlf(val,width)
		  bni_fa_zlf(val,width)
		  bni_fa_czlf(val,width)
		  bni_fa_dzlf(val,width)
		  bni_fa_xf(val)
		  bni_fa_e(val)
		  bni_fa_ce(val)
		  bni_fa_de(val)
		  bni_fa_re(val,width)
		  bni_fa_cre(val,width)
		  bni_fa_dre(val,width)
		  bni_fa_le(val,width)
		  bni_fa_cle(val,width)
		  bni_fa_dle(val,width)
		  bni_fa_rf_(val,width,precision)
		  bni_fa_crf_(val,width,precision)
		  bni_fa_drf_(val,width,precision)
		  bni_fa_zf_(val,width,precision)
		  bni_fa_czf_(val,width,precision)
		  bni_fa_dzf_(val,width,precision)
		  bni_fa_lf_(val,width,precision)
		  bni_fa_clf_(val,width,precision)
		  bni_fa_dlf_(val,width,precision)
		  bni_fa_zlf_(val,width,precision)
		  bni_fa_czlf_(val,width,precision)
		  bni_fa_dzlf_(val,width,precision)
		  bni_fa_re_(val,width,precision)
		  bni_fa_cre_(val,width,precision)
		  bni_fa_dre_(val,width,precision)
		  bni_fa_le_(val,width,precision)
		  bni_fa_cle_(val,width,precision)
		  bni_fa_dle_(val,width,precision)

EXAMPLE

	The library support sprintf like formating, replacing '%' place holder
	characters in a format string with a text rendition of of the supplied
	datatype. However, I do not use varargs and the data type and format
	flags are not supplied in the format string. Instead they are specified
	as the values are "pushed" onto the formater which is then passed to
	bni_format for rendering of the string. To reuse the formater call
	bni_formater_reset to push the first argument again.

	Example:

		char buf[4096];
		v3f triangles[99];

		...

		at = 0;
		BniStringFormater fmt = bni_formater_create(arena, 10
			, "%: {%,%,%}, {%,%,%}, {%,%,%}\n");
		for (int i = 0; i < 99; i += 3)
		{
			v3f p1 = triangles[i];
			v3f p2 = triangles[i+1];
			v3f p3 = triangles[i+2];
			bni_push_format_arg(fmt, bni_fa_zs(i % 3, 2));
			bni_push_format_arg(fmt, bni_fa_czf(p1.x, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p1.y, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p1.z, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p2.x, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p2.y, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p2.z, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p3.x, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p3.y, 7, 3));
			bni_push_format_arg(fmt, bni_fa_czf(p3.z, 7, 3));
			at += bni_format(buf + at, buf_size - at, fmt);
			bni_formater_reset(fmt);
		}



	To just format a number into an abitrary buffer without the need of
	a formater do:

	float my_float = 123456789.0f;
	BniStringFormatArg fa = bni_dze(my_float, 0, 3);
	at += bin_format_float(buf + at, 4096 - at, &fa);

	which would put: +1.235e+08 into buf.

 */


#ifndef INCLUDED_BNI_SFORMAT_H
#define INCLUDED_BNI_SFORMAT_H

#ifndef INCLUDED_BNI_BASE_H
#error bni_sformat.h requires types and utility functions from bni_base.h
#endif

#define BNI_SIGN_MASK (1<<31)
#define BNI_F32_EXP_MASK 0x7F800000
#define BNI_F32_SIGNIFICAND_MASK 0x007FFFFF
#define BNI_F32_TO_BITPATTERN(val) (*(unsigned int*)(&(val)))
#define BNI_F32_SIGNIFICAND_BITS(val) \
	 (BNI_F32_TO_BITPATTERN(val) & BNI_F32_SIGNIFICAND_MASK)
#define BNI_F32_EXP_STORED(val) \
	 (unsigned char)((BNI_F32_TO_BITPATTERN(val)&BNI_F32_EXP_MASK)>>23)
#define BNI_F32_EXP_REAL(val) (char)(BNI_F32_EXP_STORED(val) - 127)
#define BNI_F32_INF_POS 0x7F800000
#define BNI_F32_INF_NEG 0xFF800000
#define BNI_F32_IS_NAN(val) ((BNI_F32_TO_BITPATTERN(val) & BNI_F32_INF_POS) \
                && (BNI_F32_TO_BITPATTERN(val) & BNI_F32_SIGNIFICAND_MASK))
#define BNI_BASE_2_TO_10 0.30103f
#define BNI_DIGITS_PER_ITERATION 8
#define BNI_F32_IS_ZERO(val) (((*(u32*)(&(val))) & 0x7FFFFFFF) == 0)
#define BNI_F32_MAX_SIGNIFICAT_DIGITS 10

/* flag laout is:
 * 31 - 24 - data type
 * 23 - 16 - format flags
 * 15 -  0 - width
 */

#define BNI_FAM_TYPE        0xFFFF000000000000
#define BNI_FAM_FORMAT      0x0000FFFF00000000
#define BNI_FAM_PRECISION   0x00000000FFFF0000
#define BNI_FAM_WIDTH       0x000000000000FFFF

// format flags
#define BNI_FA_SIENTIFIC    0x0000000100000000
#define BNI_FA_HEX          0x0000000200000000
#define BNI_FA_EXP          0x0000000400000000
#define BNI_FA_GROUP        0x0000000800000000
#define BNI_FA_PAD_ZERO     0x0000001000000000
#define BNI_FA_SPACE        0x0000002000000000
#define BNI_FA_JUSTIFY_LEFT 0x0000004000000000
#define BNI_FA_PLUS         0x0000008000000000

#define BNI_FAT_UNSIGNED    0xF000000000000000
#define BNI_FAT_SIGNED      0x0F00000000000000
#define BNI_FAT_IEEE        0x00C0000000000000

// datatype
#define BNI_FAT_U64         0x8000000000000000
#define BNI_FAT_U32         0x4000000000000000
#define BNI_FAT_U16         0x2000000000000000
#define BNI_FAT_U8          0x1000000000000000
#define BNI_FAT_S64         0x0800000000000000
#define BNI_FAT_S32         0x0400000000000000
#define BNI_FAT_S16         0x0200000000000000
#define BNI_FAT_S8          0x0100000000000000
#define BNI_FAT_F64         0x0080000000000000
#define BNI_FAT_F32         0x0040000000000000
#define BNI_FAT_PTR         0x0020000000000000
#define BNI_FAT_STR         0x0010000000000000
#define BNI_FAT_CSTR        0x0008000000000000

#define BNI_FAC_C__L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_SPACE)
#define BNI_FAC_D__L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PLUS)
#define BNI_FAC__Z_L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PAD_ZERO)
#define BNI_FAC_CZ_L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_SPACE | BNI_FA_PAD_ZERO)
#define BNI_FAC_DZ_L  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PLUS  | BNI_FA_PAD_ZERO)
#define BNI_FAC__ZXL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PAD_ZERO | BNI_FA_HEX)
#define BNI_FAC__ZOL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_PAD_ZERO | BNI_FA_OCT)
#define BNI_FAC___XL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_HEX)
#define BNI_FAC___OL  (BNI_FA_JUSTIFY_LEFT | BNI_FA_OCT)
#define BNI_FAC____L  (BNI_FA_JUSTIFY_LEFT)

#define BNI_FAC_C___ (BNI_FA_SPACE)
#define BNI_FAC_D___ (BNI_FA_PLUS)
#define BNI_FAC__Z__ (BNI_FA_PAD_ZERO)
#define BNI_FAC_CZ__ (BNI_FA_SPACE | BNI_FA_PAD_ZERO)
#define BNI_FAC_DZ__ (BNI_FA_PLUS  | BNI_FA_PAD_ZERO)
#define BNI_FAC__ZX_ (BNI_FA_PAD_ZERO | BNI_FA_HEX)
#define BNI_FAC__ZO_ (BNI_FA_PAD_ZERO | BNI_FA_OCT)
#define BNI_FAC___X_ (BNI_FA_HEX)
#define BNI_FAC___O_ (BNI_FA_OCT)
#define BNI_FAC_____ 0

/* Unsigned formaters */
#define bni_fa_clu(val,width)  bni_format_arg_u(BNI_FAC_C__L,(val),(width),0)
#define bni_fa_dlu(val,width)  bni_format_arg_u(BNI_FAC_D__L,(val),(width),0)
#define bni_fa_zlu(val,width)  bni_format_arg_u(BNI_FAC__Z_L,(val),(width),0)
#define bni_fa_czlu(val,width) bni_format_arg_u(BNI_FAC_CZ_L,(val),(width),0)
#define bni_fa_dzlu(val,width) bni_format_arg_u(BNI_FAC_DZ_L,(val),(width),0)
#define bni_fa_zxlu(val,width) bni_format_arg_u(BNI_FAC__ZXL,(val),(width),0)
#define bni_fa_zolu(val,width) bni_format_arg_u(BNI_FAC__ZOL,(val),(width),0)
#define bni_fa_xlu(val,width)  bni_format_arg_u(BNI_FAC___XL,(val),(width),0)
#define bni_fa_olu(val,width)  bni_format_arg_u(BNI_FAC___OL,(val),(width),0)
#define bni_fa_lu(val,width)   bni_format_arg_u(BNI_FAC____L,(val),(width),0)

#define bni_fa_zu(val,width)   bni_format_arg_u(BNI_FAC__Z__,(val),(width),0)
#define bni_fa_czu(val,width)  bni_format_arg_u(BNI_FAC_CZ__,(val),(width),0)
#define bni_fa_dzu(val,width)  bni_format_arg_u(BNI_FAC_DZ__,(val),(width),0)
#define bni_fa_zxu(val,width)  bni_format_arg_u(BNI_FAC__ZX_,(val),(width),0)
#define bni_fa_zou(val,width)  bni_format_arg_u(BNI_FAC__ZO_,(val),(width),0)
#define bni_fa_xru(val,width)  bni_format_arg_u(BNI_FAC___X_,(val),(width),0)
#define bni_fa_oru(val,width)  bni_format_arg_u(BNI_FAC___O_,(val),(width),0)
#define bni_fa_cru(val,width)  bni_format_arg_u(BNI_FAC_C___,(val),(width),0)
#define bni_fa_dru(val,width)  bni_format_arg_u(BNI_FAC_D___,(val),(width),0)

#define bni_fa_u(val)          bni_format_arg_u(BNI_FAC_____,(val),0,0)
#define bni_fa_cu(val)         bni_format_arg_u(BNI_FAC_C___,(val),0,0)
#define bni_fa_du(val)         bni_format_arg_u(BNI_FAC_D___,(val),0,0)
#define bni_fa_xu(val)         bni_format_arg_u(BNI_FAC___X_,(val),0,0)
#define bni_fa_ou(val)         bni_format_arg_u(BNI_FAC___O_,(val),0,0)

/* Signed formaters */
#define bni_fa_cls(val,width)  bni_format_arg_s(BNI_FAC_C__L,(val),(width),0)
#define bni_fa_dls(val,width)  bni_format_arg_s(BNI_FAC_D__L,(val),(width),0)
#define bni_fa_zls(val,width)  bni_format_arg_s(BNI_FAC__Z_L,(val),(width),0)
#define bni_fa_czls(val,width) bni_format_arg_s(BNI_FAC_CZ_L,(val),(width),0)
#define bni_fa_dzls(val,width) bni_format_arg_s(BNI_FAC_DZ_L,(val),(width),0)
#define bni_fa_zxls(val,width) bni_format_arg_s(BNI_FAC__ZXL,(val),(width),0)
#define bni_fa_zols(val,width) bni_format_arg_s(BNI_FAC__ZOL,(val),(width),0)
#define bni_fa_xls(val,width)  bni_format_arg_s(BNI_FAC___XL,(val),(width),0)
#define bni_fa_ols(val,width)  bni_format_arg_s(BNI_FAC___OL,(val),(width),0)
#define bni_fa_ls(val,width)   bni_format_arg_s(BNI_FAC____L,(val),(width),0)

#define bni_fa_zs(val,width)   bni_format_arg_s(BNI_FAC__Z__,(val),(width),0)
#define bni_fa_czs(val,width)  bni_format_arg_s(BNI_FAC_CZ__,(val),(width),0)
#define bni_fa_dzs(val,width)  bni_format_arg_s(BNI_FAC_DZ__,(val),(width),0)
#define bni_fa_zxs(val,width)  bni_format_arg_s(BNI_FAC__ZX_,(val),(width),0)
#define bni_fa_zos(val,width)  bni_format_arg_s(BNI_FAC__ZO_,(val),(width),0)
#define bni_fa_xrs(val,width)  bni_format_arg_s(BNI_FAC___X_,(val),(width),0)
#define bni_fa_ors(val,width)  bni_format_arg_s(BNI_FAC___O_,(val),(width),0)
#define bni_fa_crs(val,width)  bni_format_arg_s(BNI_FAC_C___,(val),(width),0)
#define bni_fa_drs(val,width)  bni_format_arg_s(BNI_FAC_D___,(val),(width),0)

#define bni_fa_s(val)          bni_format_arg_s(BNI_FAC_____,(val),0,0)
#define bni_fa_cs(val)         bni_format_arg_s(BNI_FAC_C___,(val),0,0)
#define bni_fa_ds(val)         bni_format_arg_s(BNI_FAC_D___,(val),0,0)
#define bni_fa_xs(val)         bni_format_arg_s(BNI_FAC___X_,(val),0,0)
#define bni_fa_os(val)         bni_format_arg_s(BNI_FAC___O_,(val),0,0)

/* Pointer formaters */
#define bni_fa_p(val,width)    bni_format_arg_p(BNI_FAC__ZX_,(val),size of(size_t),0)
#define bni_fa_rp(val,width)   bni_format_arg_p(BNI_FAC__ZX_,(val),(width),0)
#define bni_fa_lp(val,width)   bni_format_arg_p(BNI_FAC__ZXL,(val),(width),0)

/* Text formaters */
#define bni_fa_t(val)          bni_format_arg_cstr(BNI_FAC_____,(val),0, 0)
#define bni_fa_lt(val,width)   bni_format_arg_cstr(BNI_FAC____L,(val),(width), 0)
#define bni_fa_rt(val,width)   bni_format_arg_cstr(BNI_FAC_____,(val),(width), 0)
#define bni_fa_t_(val,width,precision)   bni_format_arg_cstr(BNI_FAC_____,(val),(width),(precision))

/* Float formaters */
#define bni_fa_f(val)          bni_format_arg_f(BNI_FAC_____,(val),0,6)
#define bni_fa_cf(val)         bni_format_arg_f(BNI_FAC_C___,(val),0,6)
#define bni_fa_df(val)         bni_format_arg_f(BNI_FAC_D___,(val),0,6)

#define bni_fa_crf(val,width)  bni_format_arg_f(BNI_FAC_C___,(val),(width),6)
#define bni_fa_drf(val,width)  bni_format_arg_f(BNI_FAC_D___,(val),(width),6)
#define bni_fa_zf(val,width)   bni_format_arg_f(BNI_FAC__Z__,(val),(width),6)
#define bni_fa_czf(val,width)  bni_format_arg_f(BNI_FAC_CZ__,(val),(width),6)
#define bni_fa_dzf(val,width)  bni_format_arg_f(BNI_FAC_DZ__,(val),(width),6)

#define bni_fa_lf(val,width)   bni_format_arg_f(BNI_FAC____L,(val),(width),6)
#define bni_fa_clf(val,width)  bni_format_arg_f(BNI_FAC_C__L,(val),(width),6)
#define bni_fa_dlf(val,width)  bni_format_arg_f(BNI_FAC_D__L,(val),(width),6)
#define bni_fa_zlf(val,width)  bni_format_arg_f(BNI_FAC__Z_L,(val),(width),6)
#define bni_fa_czlf(val,width) bni_format_arg_f(BNI_FAC_CZ_L,(val),(width),6)
#define bni_fa_dzlf(val,width) bni_format_arg_f(BNI_FAC_DZ_L,(val),(width),6)

#define bni_fa_xf(val)         bni_format_arg_u(BNI_FAC__ZX_,(*(u32*)&(val)),8,0)

#define bni_fa_e(val)          bni_format_arg_f(BNI_FA_EXP | BNI_FAC_____,(val),0,6)
#define bni_fa_ce(val)         bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C___,(val),0,6)
#define bni_fa_de(val)         bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D___,(val),0,6)
#define bni_fa_re(val,width)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC_____,(val),(width),6)
#define bni_fa_cre(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C___,(val),(width),6)
#define bni_fa_dre(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D___,(val),(width),6)
#define bni_fa_le(val,width)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC____L,(val),(width),6)
#define bni_fa_cle(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C__L,(val),(width),6)
#define bni_fa_dle(val,width)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D__L,(val),(width),6)

/* _-suffix exposes precision field */
#define bni_fa_rf_(val,width,precision)   bni_format_arg_f(BNI_FAC_____,(val),(width),(precision))
#define bni_fa_crf_(val,width,precision)  bni_format_arg_f(BNI_FAC_C___,(val),(width),(precision))
#define bni_fa_drf_(val,width,precision)  bni_format_arg_f(BNI_FAC_D___,(val),(width),(precision))
#define bni_fa_zf_(val,width,precision)   bni_format_arg_f(BNI_FAC__Z__,(val),(width),(precision))
#define bni_fa_czf_(val,width,precision)  bni_format_arg_f(BNI_FAC_CZ__,(val),(width),(precision))
#define bni_fa_dzf_(val,width,precision)  bni_format_arg_f(BNI_FAC_DZ__,(val),(width),(precision))

#define bni_fa_lf_(val,width,precision)   bni_format_arg_f(BNI_FAC____L,(val),(width),(precision))
#define bni_fa_clf_(val,width,precision)  bni_format_arg_f(BNI_FAC_C__L,(val),(width),(precision))
#define bni_fa_dlf_(val,width,precision)  bni_format_arg_f(BNI_FAC_D__L,(val),(width),(precision))
#define bni_fa_zlf_(val,width,precision)  bni_format_arg_f(BNI_FAC__Z_L,(val),(width),(precision))
#define bni_fa_czlf_(val,width,precision) bni_format_arg_f(BNI_FAC_CZ_L,(val),(width),(precision))
#define bni_fa_dzlf_(val,width,precision) bni_format_arg_f(BNI_FAC_DZ_L,(val),(width),(precision))

#define bni_fa_re_(val,width,precision)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC_____,(val),(width),(precision))
#define bni_fa_cre_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C___,(val),(width),(precision))
#define bni_fa_dre_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D___,(val),(width),(precision))
#define bni_fa_le_(val,width,precision)   bni_format_arg_f(BNI_FA_EXP | BNI_FAC____L,(val),(width),(precision))
#define bni_fa_cle_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_C__L,(val),(width),(precision))
#define bni_fa_dle_(val,width,precision)  bni_format_arg_f(BNI_FA_EXP | BNI_FAC_D__L,(val),(width),(precision))

/* Argument flag tests */
#define BNI_FA_E_OR_G(fa) \
	((((fa)->format_info) & BNI_FA_SIENTIFIC) \
	 || (((fa)->format_info) & BNI_FA_EXP))
#define BNI_FAT_IS_STRING(fa) \
	((((fa)->format_info) & BNI_FAT_STR) == BNI_FAT_STR)
#define BNI_FAT_IS_CSTRING(fa) \
	((((fa)->format_info) & BNI_FAT_CSTR) == BNI_FAT_CSTR)
#define BNI_FAT_IS_POINTER(fa) \
	((((fa)->format_info) & BNI_FAT_PTR) == BNI_FAT_PTR)
#define BNI_FAT_IS_SIGNED(fa) \
	((((fa)->format_info) & BNI_FAT_SIGNED) >  0)
#define BNI_FAT_IS_INTEGER(fa) \
	((((fa)->format_info) & (BNI_FAT_UNSIGNED | BNI_FAT_SIGNED)) >  0)
#define BNI_FAT_IS_FLOAT(fa) \
	((((fa)->format_info) & (BNI_FAT_F32 | BNI_FAT_F64)) >  0)

#define BNI_FA_HAS_HEX(fa) \
	((((fa)->format_info) & BNI_FA_HEX) == BNI_FA_HEX)
#define BNI_FA_HAS_PLUS(fa) \
	((((fa)->format_info) & BNI_FA_PLUS) == BNI_FA_PLUS)
#define BNI_FA_HAS_PAD(fa) \
	((((fa)->format_info) & BNI_FA_SPACE | BNI_FA_PAD_ZERO) > 0)
#define BNI_FA_HAS_SPACE(fa) \
	((((fa)->format_info) & BNI_FA_SPACE) == BNI_FA_SPACE)
#define BNI_FA_HAS_PAD_ZERO(fa) \
	((((fa)->format_info) & BNI_FA_PAD_ZERO) == BNI_FA_PAD_ZERO)
#define BNI_FA_HAS_JUSITIFY_LEFT(fa) \
	((((fa)->format_info) & BNI_FA_JUSTIFY_LEFT) == BNI_FA_JUSTIFY_LEFT)



#define BNI_ALLOC_STACK_FORMATER(name, nargs, fmt_str) \
	BniStringFormater name[(1 + (nargs))]; \
	name[0].format_string = fmt_str; \
	name[0].arg_max = (nargs); \
	name[0].arg_count = 0

#if 0
#ifndef BNI_BASE_H_INCLUDED
// NOTE(bjorn): define a memcopy even if BNI_BASE_H
// was not used
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
#endif
#endif

// 16 bytes
typedef struct BniStringFormatArg {
	u64 format_info;
	union {
		u64 u;
		s64 s;
		f64 d;
		f32 f;
		void *p;
		char *c_str;
	} v;

} BniStringFormatArg;

// 16 bytes + array of 16 bytes
typedef struct BniStringFormater {
//	BniStringFormatArg *args;
	s8 *format_string;
	u32 arg_max;
	u32 arg_count;
	BniStringFormatArg args[0];
} BniStringFormater;


typedef struct BniInternalFormatBuffer
{
	u8  mem[32];
	u16 precision;
	u16 width;
	s8  exp;
	s16 left_part_zeros;
	s16 right_part_zeros;
	u8 sd_div;
	u8 sd_end;
	u8 sd_start;
	u8 significant_digits;
	s8 sign;
	s8 pad_char;
	s8 pad_count;
	u16 output_len;
} BniInternaFormatBuffer;

#endif

#ifdef BNI_SFORMAT_IMPLEMENTATION
static u32
bni_format_float
(
	u8 *out
	, umm size
	, BniStringFormatArg *arg
)
{
	u32 written = 0;
	u32 dot_space = 1;
	bool nan = false;
	BniInternaFormatBuffer buf;
	memset(&buf, 255, sizeof(BniInternaFormatBuffer));
	buf.precision = (u16)((arg->format_info & BNI_FAM_PRECISION)>>16);
	buf.width = arg->format_info & BNI_FAM_WIDTH;
	f32 val = arg->v.f;
	bool output_zero = false;

	bni_assert((umm)buf.width <= size);

	s16 exp_b2 = BNI_F32_EXP_REAL(val);
	u32 bits = BNI_F32_SIGNIFICAND_BITS(val);
	if (buf.precision == 0)
		dot_space = 0;

	if (exp_b2 == -127) // stored exp == 0
	{
		if (bits == 0)
		{
			//NOTE(bjorn): 0.0f -- output zeros according to format
			// no further processing needed.
			if (BNI_FA_E_OR_G(arg))
			{
				// 0.[precision]e+00
				buf.output_len = buf.precision + 5 + dot_space;
				buf.significant_digits = 0;

			}
			else
			{
				// 0.[precision]
				buf.output_len = buf.precision + 1 + dot_space;
				buf.significant_digits = 0;
			}
			output_zero = true;
		}
		else
		{
			// NOTE(bjorn): subnormal number. Adapt binary exponent
			// to what it would have been if it where a normal number
			// -150 < exp_b2 < -127 on completion
			s32 count = 23;
			for (;bits != 0
				; bits >>= 1, --count)
			{
				;
			}
			exp_b2 -= count + 1; // 23 == sigificand bit count
		}
	}
	else if (exp_b2 == -128) // stored exp == 255
	{
		if (bits == 0)
		{
			bni_copy_memory(buf.mem, "+INF", 4);
			buf.output_len = 4;
			if (BNI_F32_TO_BITPATTERN(val) & BNI_SIGN_MASK)
			{
				*buf.mem = '-';
				val = (float)(BNI_F32_TO_BITPATTERN(val) & (~BNI_SIGN_MASK));
			}
		}
		else
		{
			bni_copy_memory(buf.mem, "NAN", 3);
			buf.output_len = 3;
		}
		nan = true;
	}

	if (!nan && !output_zero)
	{

		buf.exp = (s16)((f32)exp_b2 * BNI_BASE_2_TO_10 + 0.5f);

		s32 digits_to_generate = 0;
		if (BNI_FA_E_OR_G(arg))
		{
			digits_to_generate = buf.precision + 6; // 1.[precision]e+xx
			buf.output_len = buf.precision + 5 + dot_space;
			buf.significant_digits = buf.precision + 1; // %g
			buf.sd_div = 1;
		}
		else // %f
		{
			if (buf.exp < 0 && buf.precision < -buf.exp)
			{
				// NOTE(Bjorn): Not enough precision output zero as 0.[precision]
				buf.output_len = buf.precision + 1 + dot_space;
				buf.significant_digits = 0;
				digits_to_generate = 0;
			}
			else
			{
				buf.output_len = buf.precision + buf.exp + 1 + dot_space;
				buf.significant_digits = buf.precision + buf.exp + 1;
				digits_to_generate = buf.precision + buf.exp
									+ BNI_F32_MAX_SIGNIFICAT_DIGITS;
			}
		}

		// x.yz...e+38 / 10e+36 = x.yz...e+02
		// x.yz...e-38 * 10e+40 = x.yz...e+02
		f32 scaled = val;
		if (scaled < 0)
			scaled = -scaled;

		if (buf.exp > 0)
		{
			// NOTE(bjorn): As we aim for 10 digits make sure
			// we get 2 in the first iteration an 8 in the second
			// leading zeros are free while trailing zeros costs
			// a division
			s8 scale_exp = buf.exp - 2;
			if (scale_exp < 0)
			{
				scale_exp = 0;
			}
			buf.exp = scale_exp;
			f32 scale_factor = 1.0f;
			f32 base = 10.0f;
			while (scale_exp)
			{
				if (scale_exp & 1)
					scale_factor *= base;
				base *= base;
				scale_exp >>= 1;
			}
			scaled /= scale_factor;
		}
		else if (buf.exp <= 0)
		{
			s8 scale_exp = (-buf.exp + 2);
			buf.exp = -scale_exp;
			if (scale_exp & 0x20)
				scaled *= 1e32L;
			if (scale_exp & 0x10)
				scaled *= 1e16L;
			if (scale_exp & 0x08)
				scaled *= 1e8L;
			if (scale_exp & 0x04)
				scaled *= 1e4L;
			if (scale_exp & 0x02)
				scaled *= 1e2L;
			if (scale_exp & 0x01)
				scaled *= 1e1L;
		}

		u32 at = 1;
		u32 digits_generated = 0;
		if (digits_to_generate > 0)
		{
			// NOTE(bjorn): Output at most 10 digits 1 more that the 6-9
			// digits of precision in a float the extra digit is for rounding
			if (digits_to_generate > BNI_F32_MAX_SIGNIFICAT_DIGITS)
				digits_to_generate = BNI_F32_MAX_SIGNIFICAT_DIGITS;

			*buf.mem = '0';

			while( 0 < digits_to_generate && 0.0f < scaled)
			{
				unsigned int integral_part = (unsigned int)scaled;

				digits_to_generate -= BNI_DIGITS_PER_ITERATION;
				if (0 < digits_to_generate)
					scaled = (scaled - (float)integral_part) * 1e8f;

				s32 n = BNI_DIGITS_PER_ITERATION;
				u8 *p = buf.mem + at + BNI_DIGITS_PER_ITERATION;
				for(;n > 0 && 0 < integral_part ; --n)
				{
					*--p = '0' + integral_part % 10;
					integral_part /= 10;
				}
				while(--n >= 0)
					*--p = '0';

				at += BNI_DIGITS_PER_ITERATION;
			}
			digits_generated = at - 1;
		}
		if (digits_generated > 0)
		{
			// NOTE(bjorn): Undo the exponent scaling, account for
			// any leading zeros introduced by the conversion
			buf.exp += BNI_DIGITS_PER_ITERATION-1;


			// NOTE(bjorn): Find first significant digit
			// and adjust exponent to reflect the change
			u8 *p = &buf.mem[1];
			while(*p == '0')
			{
				--digits_generated;
				--buf.exp;
				++p;
			}

			// Clamp significant digits to digits_generated
			// and round if we have generated more digits
			// than we need
			if (digits_generated < buf.significant_digits)
				buf.significant_digits = digits_generated;
			else if (p[buf.significant_digits] >= '5')
			{
				s8 n = buf.significant_digits -1;
				while(p[n] == '9')
				{
					--buf.significant_digits;
					--n;
				}
				++p[n];
			}

			buf.sd_start = at - digits_generated;
		}
	}

	if (BNI_FA_HAS_PLUS(arg) || val < 0)
	{
		++buf.output_len;
		buf.sign = val < 0 ? '-' : '+';
	}
	else if (BNI_FA_HAS_SPACE(arg))
	{
		++buf.output_len;
		buf.sign = ' ';
	}
	else
	{
		buf.sign = 'x';
	}

	u32 at = 0;
	buf.pad_count = 0;
	if (BNI_FA_HAS_PAD(arg) && buf.width > buf.output_len)
	{
		buf.pad_char = ' ';
		if (BNI_FA_HAS_PAD_ZERO(arg))
		{
			buf.pad_char = '0';
		}
		buf.pad_count = buf.width - buf.output_len;
		if (!BNI_FA_HAS_JUSITIFY_LEFT(arg))
		{
			if (BNI_FA_HAS_PAD_ZERO(arg) && buf.sign != 'x')
			{
				*out = buf.sign;
				++at;
			}
			memset(out, buf.pad_char, buf.pad_count);
			at += buf.pad_count;
		}
	}

	if (nan)
	{
		bni_copy_memory(out + at, buf.mem, buf.output_len);
	}
	else
	{
		if (buf.sign != 'x')
		{
				*out = buf.sign;
				++at;
		}

		if (buf.significant_digits == 0)
		{
			//NOTE (bjorn): output 0 correctly
			if (BNI_FA_E_OR_G(arg))
			{
				memset(out + at, '0', buf.output_len - 4);
				bni_copy_memory(out + at + buf.output_len - 4, "e+00", 4);
			}
			else
			{
				memset(out + at, '0', buf.output_len);
			}

			if (dot_space == 1)
				out[at + 1] = '.';

			at += buf.output_len;
		}
		else
		{
			u8 *d = out + at;
			u8 *s = buf.mem + buf.sd_start;
			if (BNI_FA_E_OR_G(arg))
			{
				s8 exp_sign = '+';
				if (buf.exp < 0) {
					buf.exp = -1*buf.exp;
					exp_sign = '-';
				}

				// x.
				*d++ = *s++;
				if (dot_space == 1)
					*d++ = '.';

				buf.right_part_zeros = buf.precision - (buf.significant_digits - 1);
				if (buf.right_part_zeros > 0)
				{
					buf.sd_end = buf.significant_digits;
				}
				else
				{
					buf.right_part_zeros = 0;
					buf.sd_end = buf.precision + 1;
				}

				s32 n;
				for (n = buf.sd_div; n < buf.sd_end; ++n)
				{
					*d++ = *s++;
				}
				memset(d, '0', buf.right_part_zeros);
				d += buf.right_part_zeros;
				*d++ = 'e';
				*d++ = exp_sign;

				d += 2;
				for(n = 2
					; n > 0 && 0 < buf.exp
					; --n)
				{
					*--d = '0' + buf.exp % 10;
					buf.exp /= 10;
				}
				while (--n >= 0)
					*--d = '0';
				//d += 2;
				at = d + 3 - out;
			}
			else
			{
				// %f
				if (buf.exp > 0)
				{
					buf.left_part_zeros = buf.exp + 1 - buf.significant_digits;
					if (buf.left_part_zeros > 0)
					{
						buf.sd_div = buf.significant_digits;
						buf.right_part_zeros = buf.precision;
					}
					else
					{
						buf.right_part_zeros = buf.precision + buf.left_part_zeros;
						buf.left_part_zeros = 0;
						if (buf.right_part_zeros < 0)
							buf.right_part_zeros = 0;
					}

					for (int n = 0; n < buf.sd_div; ++n)
					{
						*d++ = *s++;
					}
					memset(d, '0', buf.left_part_zeros);
					d += buf.left_part_zeros;

					if (dot_space == 1)
						*d++ = '.';

					for (int n = buf.sd_div; n < buf.significant_digits; ++n)
					{
						*d++ = *s++;
					}
					memset(d, '0', buf.right_part_zeros);
					d += buf.right_part_zeros;
				}
				else
				{
					// NOTE(Bjorn): We only need to handle the case
					// where precision >= -exp the case when only 0.0...0
					// fits within requested percision where handled erlier
					// i.e. buf.sd_div > 0

					buf.left_part_zeros = -buf.exp - 1;
					buf.sd_div = buf.precision - buf.left_part_zeros;
					buf.right_part_zeros = 0;
					if (buf.sd_div > buf.significant_digits)
					{
						buf.right_part_zeros = buf.precision
							-  buf.left_part_zeros - buf.significant_digits;
						buf.sd_div = buf.significant_digits;
					}

					memset(d, '0', buf.left_part_zeros + 2);
					d += buf.left_part_zeros + 2;
					for (int n = 0; n < buf.sd_div; ++n)
					{
						*d++ = *s++;
					}
					memset(d, '0', buf.right_part_zeros);
					d += buf.right_part_zeros;
					out[at + 1] = '.';
				}
				at = d - out;
			}
		}
	}

	if (BNI_FA_HAS_JUSITIFY_LEFT(arg) && buf.pad_count)
	{
			memset(out + at, buf.pad_char, buf.pad_count);
	}

	return buf.output_len + buf.pad_count;
}





#define BNI_FORMAT_BUF_SIZE 32
s8 bni_integer_lut[16] = "0123456789ABCDEF";

static u32
bni_format_integer
(
	u8 *out
	, umm size
	, BniStringFormatArg *arg
)
{
	u8 buf[BNI_FORMAT_BUF_SIZE];
	u8 buf_at = BNI_FORMAT_BUF_SIZE;

	u16 width = arg->format_info & BNI_FAM_WIDTH;
	u8 significant_digits = 0;
	u8 output_len = 1;
	u8 sign = 0;
	if (BNI_FA_HAS_HEX(arg))
	{
		u64 val = arg->v.u;

		while(val != 0)
		{
			buf[--buf_at] =  bni_integer_lut[val & 0x0F];
			val >>= 4;
			++significant_digits;
		}
	}
	else
	{
		u64 val = 0;

		if (BNI_FA_HAS_SPACE(arg))
				sign = ' ';
		if (BNI_FA_HAS_PLUS(arg))
				sign = '+';
		if (BNI_FAT_IS_SIGNED(arg))
		{
			if(arg->v.s < 0)
				sign = '-';
			val = (u64)(-1 * arg->v.s);
		}
		else
		{
			val = arg->v.u;
		}

		while(val != 0)
		{
			buf[--buf_at] =  bni_integer_lut[val % 10];
			val /= 10;
			++significant_digits;
		}
	}

	if (significant_digits)
	{
		output_len += significant_digits - 1;
	}

	if (sign)
	{
		++output_len;
	}

	u8  pad_char = 0;
	u16 pad_count = 0;
	u32 at = 0;
	if (width > output_len)
	{
		if (BNI_FA_HAS_PAD_ZERO(arg))
			pad_char = '0';
		else
			pad_char = ' ';

		pad_count = width - output_len;
	}

	if (pad_char && !BNI_FA_HAS_JUSITIFY_LEFT(arg))
	{
		if (pad_char == '0' && sign)
		{
			*out = sign;
			++at;
		}
		memset(out + at, pad_char, pad_count);
		at += pad_count;
		if (pad_char == ' ' && sign)
		{
			out[at] = sign;
			++at;
		}
	}
	else
	{
		if (sign)
		{
			out[at] = sign;
			++at;
		}
	}

	if (arg->v.u == 0)
	{
		out[at] = '0';
		++at;
	}
	else
	{
		bni_copy_memory(out + at, buf + buf_at, significant_digits);
	}

	if (pad_char == ' ' && BNI_FA_HAS_JUSITIFY_LEFT(arg))
	{
		memset(out + at, pad_char, pad_count);
	}

	return output_len + pad_count;
}

static u32
bni_format_str
(
	  u8 *out
	, umm count
	, BniStringFormatArg *arg
)
{
	u16 width = arg->format_info & BNI_FAM_WIDTH;
	u32 output_len = (u16)((arg->format_info & BNI_FAM_PRECISION)>>16);
	s8 *p = arg->v.c_str;
	if (output_len == 0)
	{
		while(*p != 0)
			++p;
		output_len = p - arg->v.c_str;
	}

	if (BNI_FA_HAS_HEX(arg))
	{
		output_len *= 2;
	}

	bni_assert(count > output_len);

	s32 pad_count = width - output_len;
	u32 at = 0;
	if (pad_count < 0)
	{
		pad_count = 0;
	}
	else
	{
		bni_assert(count > output_len + pad_count);
		if (!BNI_FA_HAS_JUSITIFY_LEFT(arg))
		{
			memset(out, ' ', pad_count);
			at += pad_count;
		}
	}

	if (BNI_FA_HAS_HEX(arg))
	{
		s8 *end = arg->v.c_str + output_len;
		for(s8 *p = arg->v.c_str
			; p < end
			; ++p)
		{
			s8 v = *p;
			out[at++] = bni_integer_lut[(v>>4) & 0x0F];
			out[at++] = bni_integer_lut[v & 0x0F];
		}
	}
	else
	{
		s8 *end = arg->v.c_str + output_len;
		for(s8 *p = arg->v.c_str
			; p < end
			; ++p)
		{
			out[at++] = *p;
		}
	}

	if (pad_count && BNI_FA_HAS_JUSITIFY_LEFT(arg))
	{
		memset(out + at, ' ', pad_count);
	}
	return output_len + pad_count;
}


u32
bni_format
(
  	u8 *dest
  , umm count
  , BniStringFormater *formater
)
{
	umm dest_at = 0;
	umm bytes_consumed = 0;
	s8 *template = formater->format_string;
	BniStringFormatArg *arg;
	u32 argc = 0;
	s8 buf[BNI_FORMAT_BUF_SIZE];

	// real printf code goes in here
	while(*template != 0)
	{
		if (*template == '%')
		{
			if (*(template + 1) == '%')
			{
				++template;
				*dest++ = '%';
				++dest_at;
			}
			else
			{
				bni_assert(argc < formater->arg_count);
				arg = &formater->args[argc];

				s32 buf_at = BNI_FORMAT_BUF_SIZE;
				u16 width = arg->format_info & BNI_FAM_WIDTH;
				u16 precision = ((arg->format_info & BNI_FAM_PRECISION)>>16);
				if (BNI_FAT_IS_POINTER(arg))
				{
					umm val = (umm)(((char*)arg->v.p) + 0);
					while(val != 0)
					{
						buf[--buf_at] =  bni_integer_lut[val & 0x0F];
						val >>= 4;
					}
				}
				else if (BNI_FAT_IS_INTEGER(arg))
				{
					bytes_consumed = bni_format_integer(dest, count - dest_at, arg);
					dest += bytes_consumed;
					dest_at += bytes_consumed;
				}
				else if (BNI_FAT_IS_FLOAT(arg))
				{
					bytes_consumed = bni_format_float(dest, count - dest_at, arg);
					dest += bytes_consumed;
					dest_at += bytes_consumed;
				}
				else if (BNI_FAT_IS_CSTRING(arg))
				{
					bytes_consumed = bni_format_str(dest, count - dest_at, arg);
					dest += bytes_consumed;
					dest_at += bytes_consumed;
				}
#if 0
				else if (BNI_FAT_IS_DOUBLE(arg))
#endif
				else
				{
					bni_copy_memory(dest, "!Unkown format type!", 20);
					dest_at += 20;
				}
				++argc;
			}
		}
		else
		{
				*dest++ = *template;
				++dest_at;
		}
		++template;
	}
	return dest_at;
}

static inline void
bni_formater_reset(BniStringFormater *formater)
{
	formater->arg_count = 0;
}

static inline umm
bni_formater_size(BniStringFormater *formater)
{
	return sizeof(BniStringFormater)
		+ formater->arg_max * sizeof(BniStringFormatArg);
}

#if 0
static inline BniStringFormater *
bni_formater_create_buf
(
	  u8 *buf
	, umm size
	, u32 max
	, s8 *format_string
)
{
	bni_assert(size > sizeof(BniStringFormater)
			   + max * sizeof(BniStringFormatArg));
	BniStringFormater * result = (BniStringFormater*)buf;
	result->args = (BniStringFormatArg*)(buf + sizeof(BniStringFormater));
	result->arg_max = max;
	result->arg_count = 0;
	result->format_string = format_string;
	return result;
}
#endif

static inline BniStringFormater *
bni_formater_create
(
	BniArena *arena
	, u32 max
	, s8 *format_string
)
{
	BniStringFormater *result = bni_arena_push_array(arena, BniStringFormater
			, sizeof(BniStringFormater) * (max + 1));
	//BniStringFormater *result = bni_arena_push_struct(arena, BniStringFormater);
	//result->args = bni_arena_push_array(arena, BniStringFormatArg, max);
	result->format_string = format_string;
	result->arg_max = max;
	result->arg_count = 0;

	return result;
}

static inline void
bni_push_format_arg
(
	  BniStringFormater *format
	, BniStringFormatArg arg
)
{
	bni_assert(format->arg_count < format->arg_max);
	format->args[format->arg_count++] = arg;
}

static inline BniStringFormatArg
bni_format_arg_f
(
	  u64 format_info
	, f32 val
	, u16 width
	, u16 precision
)
{
	BniStringFormatArg result;
	result.format_info = (BNI_FAT_F32 | format_info) | (precision) << 16 | (width);
	result.v.f = val;
	return result;
}

static inline BniStringFormatArg
bni_format_arg_s
(
	  u64 format_info
	, s64 val
	, u16 width
	, u16 precision
)
{
	BniStringFormatArg result;
	result.format_info = (BNI_FAT_S64 | format_info) | (precision) << 16 | (width);
	result.v.s = val;
	return result;
}


static inline BniStringFormatArg
bni_format_arg_u
(
	  u64 format_info
	, u64 val
	, u16 width
	, u16 precision
)
{
	BniStringFormatArg result;
	result.format_info = (BNI_FAT_U64 | format_info) | (precision) << 16 | (width);
	result.v.u = val;
	return result;
}

static inline BniStringFormatArg
bni_format_arg_cstr
(
	  u64 format_info
	, s8 *val
	, u16 width
	, u16 precision
)
{
	BniStringFormatArg result;
	result.format_info = (BNI_FAT_CSTR | format_info) | (precision) << 16 | (width);
	result.v.c_str = val;
	return result;
}

#endif

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
