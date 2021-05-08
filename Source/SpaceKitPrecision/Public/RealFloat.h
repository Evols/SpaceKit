// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

// Include ttmath
#pragma warning(push)
#pragma warning(disable: 5051)
#define TTMATH_NOASM // That's super sad to not include ASM, but otherwise, it doesn't compile on MSVC/UE4
#include "SpaceKitPrecision/Private/ttmath/ttmath.h"
#pragma warning(pop)

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PrecisionSettings.h"
#include "Boost/Public/Boost.h"

#include "CoreMinimal.h"
#include "Core/Public/HAL/Platform.h"

#include "RealFloat.generated.h"

using float256 = boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<BOOST_REAL_FLOAT_SIZE,
    boost::multiprecision::backends::digit_base_2>, boost::multiprecision::et_off>;

struct FRealFixed;
/**
 * Type for a real number, that uses floating-point math.
 * It is meant to be used as a native type: you can do arithmetic directly using instances of this class.
 * This is declared as an USTRUCT() only to be integrated into Blueprints. USTRUCT does not add any performance overhead.
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "SpaceKitPrecision.RealFloatMath.MakeReal"))
struct SPACEKITPRECISION_API FRealFloat
{
    GENERATED_BODY()
	
    // Typedef for the actual storage
#if USE_BOOST_BIG
    using ttBigType = float256;
#else
    // Alternatively, if you prefer to use the ttmath numbers, you can use this
    using ttBigType = ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(TT_REAL_FLOAT_SIZE)>; 
#endif

/*
 * After a LOT of trial and error, we chose this solution to interface big numbers to UE4:
 * It permits to serialize data automatically, in a way that works with all UE4 systems, including FProperty (only using a custom serializer is not sufficient for editor related stuff).
 * The idea here is to store the data in a c-style (statically allocated) array, that will be used by UE4 internally.
 * It's exposed for *internal* C++ usage, so consider it private. The Value reference is just a reinterpret_cast reference to the bytes of InternalValue.
 */
protected:
    UPROPERTY()
    uint8 InternalValue[sizeof ttBigType];
public:
	ttBigType& Value = *reinterpret_cast<ttBigType*>(InternalValue);

public:

    FRealFloat();

    FRealFloat(const FRealFloat& InValue);

    explicit FRealFloat(const ttBigType& InValue);

    explicit FRealFloat(int32 InValue);

    explicit FRealFloat(uint32 InValue);

    explicit FRealFloat(int64 InValue);

    explicit FRealFloat(uint64 InValue);

    explicit FRealFloat(float InValue);

    explicit FRealFloat(double InValue);

    explicit FRealFloat(const char* InValue);

    explicit FRealFloat(const std::string& InValue);

    explicit FRealFloat(const FString& InValue);

    FRealFloat& operator=(const FRealFloat& Other)
    {
        Value = Other.Value;
        return *this;
    };

    // Converts this number to a double number. Note that this can lead to huge precision loss
    double ToDouble() const;

    // Converts this number to a float number. Note that this can lead to huge precision loss
    float ToFloat() const;

    FString ToString() const;

    explicit operator int32() const
    {
        return ToDouble();
    }

    bool ExportTextItem(FString& ValueStr, FRealFloat const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;
    bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);

    static FRealFloat GetMaxValue();

    static FRealFloat GetMinValue();

    static FRealFloat Pi;
    static FRealFloat HalfPi;
    static FRealFloat DegToRad;
};

SPACEKITPRECISION_API FRealFloat operator""_fl(const char* str);

// Type traits, so UE4 knows FRealFloat implements ExportTextItem and ImportTextItem
template<>
struct TStructOpsTypeTraits<FRealFloat> : public TStructOpsTypeTraitsBase2<FRealFloat>
{
    enum
    {
        WithExportTextItem = true,
        WithImportTextItem = true,
    };
};


inline FRealFloat operator+(const FRealFloat& x, const FRealFloat& y)
{
    return FRealFloat(x.Value + y.Value);
}

inline FRealFloat operator+=(FRealFloat& x, const FRealFloat& y)
{
    return x = x + y;
}

inline FRealFloat operator-(const FRealFloat& x, const FRealFloat& y)
{
    return FRealFloat(x.Value - y.Value);
}

inline FRealFloat operator-(double x, const FRealFloat& y)
{
    return FRealFloat(x) - y;
}

inline FRealFloat operator-=(FRealFloat& x, const FRealFloat& y)
{
    return x = FRealFloat(x - y);
}

inline FRealFloat operator-(const FRealFloat& x)
{
    return FRealFloat(-x.Value);
}

inline FRealFloat operator*(const FRealFloat& x, const FRealFloat& y)
{
    return FRealFloat(x.Value * y.Value);
}

inline FRealFloat operator*=(FRealFloat& x, const FRealFloat& y)
{
    return x = x * y;
}

inline FRealFloat operator/(const FRealFloat& x, const FRealFloat& y)
{
    return FRealFloat(x.Value / y.Value);
}

inline FRealFloat operator/=(FRealFloat& x, const FRealFloat& y)
{
    return x = x / y;
}

inline FRealFloat operator%(const FRealFloat& x, const FRealFloat& y)
{
    FRealFloat a;
    //FRealFloat::ttBigType Result = x.Value;
    // Result %= y.Value;
    // return FRealFloat(Result);
    return FRealFloat(0);
}

inline FRealFloat operator%=(FRealFloat&& x, const FRealFloat& y)
{
    return x = x % y;
}

inline bool operator<(const FRealFloat& x, const FRealFloat& y)
{
    return x.Value < y.Value;
}

inline bool operator<=(const FRealFloat& x, const FRealFloat& y)
{
    return x.Value <= y.Value;
}

inline bool operator>=(const FRealFloat& x, const FRealFloat& y)
{
    return x.Value >= y.Value;
}

inline bool operator>(const FRealFloat& x, const FRealFloat& y)
{
    return x.Value > y.Value;
}

inline bool operator==(const FRealFloat& x, const FRealFloat& y)
{
    return x.Value == y.Value;
}

inline bool operator!=(const FRealFloat& x, const FRealFloat& y)
{
    return x.Value != y.Value;
}

/**
 * Blueprints math library for RealFloat
 */
UCLASS(BlueprintType, Abstract)
class SPACEKITPRECISION_API URealFloatMath : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

// Make/break
public:
    // Makes a FRealFloat value. It's a custom type, because UE4 doesn't support it out of the box
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (BlueprintThreadSafe))
    static FRealFloat Make(FRealFloat From) { return From; }

// Basic conversions
public:

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat to String", CompactNodeTitle = "->", BlueprintAutocast))
    static FString ConvRealToString(FRealFloat InReal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat to Float", CompactNodeTitle = "->", BlueprintAutocast))
    static float ConvRealToFloat(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "String to RealFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static FRealFloat ConvStringToReal(const FString& InString);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Float to RealFloat", CompactNodeTitle = "->", BlueprintAutocast))
    static FRealFloat ConvFloatToReal(float InVal);

// Basic FRealFloat math
public:

    UFUNCTION(BlueprintPure, category = "RealFloat", meta=(DisplayName = "RealFloat + RealFloat", CompactNodeTitle="+"))
    static FRealFloat RealPlusReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta=(DisplayName = "RealFloat - RealFloat", CompactNodeTitle="-"))
    static FRealFloat RealMinusReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta=(DisplayName = "RealFloat * RealFloat", CompactNodeTitle="*"))
    static FRealFloat RealMultReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta=(DisplayName = "RealFloat / RealFloat", CompactNodeTitle="/"))
    static FRealFloat RealDivReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat == RealFloat", CompactNodeTitle = "=="))
    static bool RealEqualsReal(FRealFloat First, FRealFloat Second, FRealFloat Tolerance);

    UFUNCTION(BlueprintPure, category = "RealFixed", meta = (DisplayName = "RealFixed != RealFixed", CompactNodeTitle = "!="))
    static bool RealNotEqualsReal(const FRealFloat& First, const FRealFloat& Second, const FRealFloat& Tolerance);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat < RealFloat", CompactNodeTitle = "<"))
    static bool RealInfReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat <= RealFloat", CompactNodeTitle = "<="))
    static bool RealInfEqReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat > RealFloat", CompactNodeTitle = ">"))
    static bool RealSupReal(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat >= RealFloat", CompactNodeTitle = ">="))
    static bool RealSupEqReal(FRealFloat First, FRealFloat Second);

// Advanced FRealFloat math
public:
    
    UFUNCTION(BlueprintPure, category = "RealFloat")
    static FRealFloat NormalizeAngleRad(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat sin (Radians)", CompactNodeTitle = "SINr"))
    static FRealFloat SinRad(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat cos (Radians)", CompactNodeTitle = "COSr"))
    static FRealFloat CosRad(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat sin (Radians)", CompactNodeTitle = "TANr"))
    static FRealFloat TanRad(FRealFloat InVal);
    
    UFUNCTION(BlueprintPure, category = "RealFloat")
    static FRealFloat NormalizeAngleDeg(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat sin (Degrees)", CompactNodeTitle = "SINd"))
    static FRealFloat SinDeg(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat cos (Degrees)", CompactNodeTitle = "COSd"))
    static FRealFloat CosDeg(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat sin (Degrees)", CompactNodeTitle = "TANd"))
    static FRealFloat TanDeg(FRealFloat InVal);


    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat asin (Radians)", CompactNodeTitle = "ASINr"))
    static FRealFloat AsinRad(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat acos (Radians)", CompactNodeTitle = "ACOSr"))
    static FRealFloat AcosRad(FRealFloat InVal);
    
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat atan(Radians)", CompactNodeTitle = "ATANr"))
    static FRealFloat AtanRad(FRealFloat InVal);
    
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat atan(Radians)", CompactNodeTitle = "ATANr"))
    static FRealFloat Atan2Rad(FRealFloat Y, FRealFloat X);
    
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat asin (Degrees)", CompactNodeTitle = "ASINd"))
    static FRealFloat AsinDeg(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat acos (Degrees)", CompactNodeTitle = "ACOSd"))
    static FRealFloat AcosDeg(FRealFloat InVal);
    
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat atan (Degrees)", CompactNodeTitle = "ATANd"))
    static FRealFloat AtanDeg(FRealFloat InVal);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "RealFloat atan (Degrees)", CompactNodeTitle = "ATANd"))
    static FRealFloat Atan2Deg(FRealFloat Y, FRealFloat X);


    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Pow FRealFloat", CompactNodeTitle = "Pow"))
    static FRealFloat Pow(FRealFloat X, FRealFloat Y);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Sqrt FRealFloat", CompactNodeTitle = "Sqrt"))
    static FRealFloat Sqrt(FRealFloat Val);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Exp RealFloat", CompactNodeTitle = "Pow"))
    static FRealFloat Exp(FRealFloat Val);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "LogE RealFloat", CompactNodeTitle = "LogE"))
    static FRealFloat LogE(FRealFloat Val);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Log2 RealFloat", CompactNodeTitle = "Log2"))
    static FRealFloat Log2(FRealFloat Val);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Log10 RealFloat", CompactNodeTitle = "Log10"))
    static FRealFloat Log10(FRealFloat Val);


    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Min RealFloat", CompactNodeTitle = "Min"))
    static FRealFloat Min(FRealFloat First, FRealFloat Second);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Max RealFloat", CompactNodeTitle = "Max"))
    static FRealFloat Max(FRealFloat First, FRealFloat Second);
	
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Clamp RealFloat", CompactNodeTitle = "Clamp"))
    static FRealFloat Clamp(FRealFloat Val, FRealFloat MinVal, FRealFloat MaxVal);
    
    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Abs RealFloat", CompactNodeTitle = "Abs"))
    static FRealFloat Abs(FRealFloat Val);

    UFUNCTION(BlueprintPure, category = "RealFloat", meta = (DisplayName = "Sign RealFloat", CompactNodeTitle = "Sign"))
    static FRealFloat Sign(FRealFloat Val);

};
