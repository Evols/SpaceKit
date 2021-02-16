
#include "CoreMinimal.h"
#include "RealFixedGeneric.h"

// Parameters for real_fixed. Exponent size is 64 bits, which is the minimum
// We want a 128 bits wide mantissa, as a 64 mantissa is too small, and 192 is overkill.
// 102 bits mantissa and 26 exponent seems like a fair tradeoff between precision and upper bound:
// In unreal units (1 unit = 1cm), the precision quantum is 0.14nm, and the upper bound is 40000 light-years (so that the world can be 80000ly wide)
#define REAL_FIXED_MANTISSA_SIZE 102
#define REAL_FIXED_EXPONENT 26

using real_fixed_type = real_fixed<REAL_FIXED_MANTISSA_SIZE, REAL_FIXED_EXPONENT>;

// Whether to use boost for big numbers. Default is 1
#define USE_BOOST_BIG 1

// Parameters for boost cpp_bin_float. Default is 192
#define BOOST_REAL_FLOAT_SIZE 192

// Parameters for ttmath Big float. Exponent size is 64 bits, which is the minimum
#define TT_REAL_FLOAT_SIZE 128
