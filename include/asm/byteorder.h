/**
# Copyright 2017 Ralic Lo<ralic.lo.eng@ieee.org> . All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

#ifndef _ASM_X86_BYTEORDER_H
#define _ASM_X86_BYTEORDER_H

#include <asm/types.h>
#include <linux/compiler.h>

#ifdef __GNUC__

#ifdef __i386__

static inline __attribute_const__ __u32 ___arch__swab32(__u32 x)
{
#ifdef CONFIG_X86_BSWAP
	asm("bswap %0" : "=r" (x) : "0" (x));
#else
	asm("xchgb %b0,%h0\n\t"	/* swap lower bytes	*/
	    "rorl $16,%0\n\t"	/* swap words		*/
	    "xchgb %b0,%h0"	/* swap higher bytes	*/
	    : "=q" (x)
	    : "0" (x));
#endif
	return x;
}

static inline __attribute_const__ __u64 ___arch__swab64(__u64 val)
{
	union {
		struct {
			__u32 a;
			__u32 b;
		} s;
		__u64 u;
	} v;
	v.u = val;
#ifdef CONFIG_X86_BSWAP
	asm("bswapl %0 ; bswapl %1 ; xchgl %0,%1"
	    : "=r" (v.s.a), "=r" (v.s.b)
	    : "0" (v.s.a), "1" (v.s.b));
#else
	v.s.a = ___arch__swab32(v.s.a);
	v.s.b = ___arch__swab32(v.s.b);
	asm("xchgl %0,%1"
	    : "=r" (v.s.a), "=r" (v.s.b)
	    : "0" (v.s.a), "1" (v.s.b));
#endif
	return v.u;
}

#else /* __i386__ */

static inline __attribute_const__ __u64 ___arch__swab64(__u64 x)
{
	asm("bswapq %0"
	    : "=r" (x)
	    : "0" (x));
	return x;
}

static inline __attribute_const__ __u32 ___arch__swab32(__u32 x)
{
	asm("bswapl %0"
	    : "=r" (x)
	    : "0" (x));
	return x;
}

#endif

/* Do not define swab16.  Gcc is smart enough to recognize "C" version and
   convert it into rotation or exhange.  */

#define __arch__swab64(x) ___arch__swab64(x)
#define __arch__swab32(x) ___arch__swab32(x)

#define __BYTEORDER_HAS_U64__

#endif /* __GNUC__ */

#ifndef _UAPI_LINUX_BYTEORDER_LITTLE_ENDIAN_H
#define _UAPI_LINUX_BYTEORDER_LITTLE_ENDIAN_H

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1234
#endif
#ifndef __LITTLE_ENDIAN_BITFIELD
#define __LITTLE_ENDIAN_BITFIELD
#endif

#include <linux/types.h>
#include <linux/swab.h>

#define __constant_htonl(x) ((__force __be32)___constant_swab32((x)))
#define __constant_ntohl(x) ___constant_swab32((__force __be32)(x))
#define __constant_htons(x) ((__force __be16)___constant_swab16((x)))
#define __constant_ntohs(x) ___constant_swab16((__force __be16)(x))
#define __constant_cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define __constant_le64_to_cpu(x) ((__force __u64)(__le64)(x))
#define __constant_cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __constant_le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __constant_cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __constant_le16_to_cpu(x) ((__force __u16)(__le16)(x))
#define __constant_cpu_to_be64(x) ((__force __be64)___constant_swab64((x)))
#define __constant_be64_to_cpu(x) ___constant_swab64((__force __u64)(__be64)(x))
#define __constant_cpu_to_be32(x) ((__force __be32)___constant_swab32((x)))
#define __constant_be32_to_cpu(x) ___constant_swab32((__force __u32)(__be32)(x))
#define __constant_cpu_to_be16(x) ((__force __be16)___constant_swab16((x)))
#define __constant_be16_to_cpu(x) ___constant_swab16((__force __u16)(__be16)(x))
#define __cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define __le64_to_cpu(x) ((__force __u64)(__le64)(x))
#define __cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __le16_to_cpu(x) ((__force __u16)(__le16)(x))
#define __cpu_to_be64(x) ((__force __be64)__swab64((x)))
#define __be64_to_cpu(x) __swab64((__force __u64)(__be64)(x))
#define __cpu_to_be32(x) ((__force __be32)__swab32((x)))
#define __be32_to_cpu(x) __swab32((__force __u32)(__be32)(x))
#define __cpu_to_be16(x) ((__force __be16)__swab16((x)))
#define __be16_to_cpu(x) __swab16((__force __u16)(__be16)(x))

static inline __le64 __cpu_to_le64p(const __u64 *p)
{
	return (__force __le64)*p;
}
static inline __u64 __le64_to_cpup(const __le64 *p)
{
	return (__force __u64)*p;
}
static inline __le32 __cpu_to_le32p(const __u32 *p)
{
	return (__force __le32)*p;
}
static inline __u32 __le32_to_cpup(const __le32 *p)
{
	return (__force __u32)*p;
}
static inline __le16 __cpu_to_le16p(const __u16 *p)
{
	return (__force __le16)*p;
}
static inline __u16 __le16_to_cpup(const __le16 *p)
{
	return (__force __u16)*p;
}
static inline __be64 __cpu_to_be64p(const __u64 *p)
{
	return (__force __be64)__swab64p(p);
}
static inline __u64 __be64_to_cpup(const __be64 *p)
{
	return __swab64p((__u64 *)p);
}
static inline __be32 __cpu_to_be32p(const __u32 *p)
{
	return (__force __be32)__swab32p(p);
}
static inline __u32 __be32_to_cpup(const __be32 *p)
{
	return __swab32p((__u32 *)p);
}
static inline __be16 __cpu_to_be16p(const __u16 *p)
{
	return (__force __be16)__swab16p(p);
}
static inline __u16 __be16_to_cpup(const __be16 *p)
{
	return __swab16p((__u16 *)p);
}
#define __cpu_to_le64s(x) do { (void)(x); } while (0)
#define __le64_to_cpus(x) do { (void)(x); } while (0)
#define __cpu_to_le32s(x) do { (void)(x); } while (0)
#define __le32_to_cpus(x) do { (void)(x); } while (0)
#define __cpu_to_le16s(x) do { (void)(x); } while (0)
#define __le16_to_cpus(x) do { (void)(x); } while (0)
#define __cpu_to_be64s(x) __swab64s((x))
#define __be64_to_cpus(x) __swab64s((x))
#define __cpu_to_be32s(x) __swab32s((x))
#define __be32_to_cpus(x) __swab32s((x))
#define __cpu_to_be16s(x) __swab16s((x))
#define __be16_to_cpus(x) __swab16s((x))


#endif /* _UAPI_LINUX_BYTEORDER_LITTLE_ENDIAN_H */
#endif /* _ASM_X86_BYTEORDER_H */