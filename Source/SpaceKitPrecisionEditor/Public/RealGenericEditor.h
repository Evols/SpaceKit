// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "SGraphPin.h"
#include "Slate/Public/Widgets/Input/SNumericEntryBox.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Runtime/Launch/Resources/Version.h"
#include "RealFloat.h"

/**
 * This is a wrapper, that will be provided to SNumericEntryBox<>, in place of a real type.
 * Making real types acceptable by SNumericEntryBox<> would make them bogus (yes, we tried), as it requires an implicit cast from and to double. Hence the need for this struct.
 */
template<typename RealType>
struct TRealNumericBoxWrapper
{
	RealType Val;
	double Repr;

	TRealNumericBoxWrapper()
	{
	}

	TRealNumericBoxWrapper(const RealType& InControlledValue)
	{
		Val = InControlledValue;
		Repr = Val.ToDouble();
	}

	TRealNumericBoxWrapper(int32 InValue)
	{
		Val = RealType(InValue);
		Repr = Val.ToDouble();
	}

	TRealNumericBoxWrapper(double InValue)
	{
		Val = RealType(InValue);
		Repr = Val.ToDouble();
	}

	operator double() const
	{
		return Val.ToDouble();
	}

#if 1
	// This is very weird, but it seems to work, so, here we go !
	operator double&()
	{
		return Repr;
	}
#endif

	TRealNumericBoxWrapper& operator+=(const TRealNumericBoxWrapper& Other)
	{
		Val += Other.Val;
		Repr = Val.ToDouble();
		return *this;
	}

	bool operator==(const TRealNumericBoxWrapper& Other)
	{
		return Val == Other.Val;
	}
};

// Needed to use SNumericEntryBox
template<typename RealType>
struct TNumericLimits<TRealNumericBoxWrapper<RealType>>
{
	static TRealNumericBoxWrapper<RealType> Max()
	{
		return TRealNumericBoxWrapper<RealType>(RealType::GetMaxValue());
	}

	static TRealNumericBoxWrapper<RealType> Min()
	{
		return TRealNumericBoxWrapper<RealType>(-RealType::GetMaxValue());
	}

	static TRealNumericBoxWrapper<RealType> Lowest()
	{
		return TRealNumericBoxWrapper<RealType>(RealType::GetMinValue());
	}
};

// Needed to use SNumericEntryBox
template<typename RealType>
static TRealNumericBoxWrapper<RealType> LexTryParseString(TRealNumericBoxWrapper<RealType>& OutValue, const TCHAR* Buffer)
{
	OutValue = TRealNumericBoxWrapper<RealType>(RealType(FString(Buffer)));
	return OutValue;
}


// Default numeric type interface
template<typename RealType>
struct TRealNumericTypeInterface : public INumericTypeInterface<TRealNumericBoxWrapper<RealType>>
{
	// The default minimum fractional digits
	static const int32 DefaultMinFractionalDigits = 0;

	// The default maximum fractional digits
	static const int32 DefaultMaxFractionalDigits = 20;

	// The current minimum fractional digits
	int32 MinFractionalDigits = DefaultMinFractionalDigits;

	// The current maximum fractional digits
	int32 MaxFractionalDigits = DefaultMaxFractionalDigits;

	// Gets the minimum and maximum fractional digits.
	int32 GetMinFractionalDigits() const override
	{
		return MinFractionalDigits;
	}

	int32 GetMaxFractionalDigits() const override
	{
		return MaxFractionalDigits;
	}

	// Sets the minimum and maximum fractional digits - A minimum greater than 0 will always have that many trailing zeros
	void SetMinFractionalDigits(const TAttribute<TOptional<int32>>& NewValue) override
	{
		MinFractionalDigits = NewValue.Get().IsSet() ? FMath::Max(0, NewValue.Get().GetValue()) : DefaultMinFractionalDigits;
	}

	virtual void SetMaxFractionalDigits(const TAttribute<TOptional<int32>>& NewValue) override
	{
		MaxFractionalDigits = NewValue.Get().IsSet() ? FMath::Max(0, NewValue.Get().GetValue()) : DefaultMaxFractionalDigits;
	}

	// Convert the type to/from a string
	virtual FString ToString(const TRealNumericBoxWrapper<RealType>& Value) const override
	{
		FString ExportedString = Value.Val.ToString();
		if (!ExportedString.Contains(TEXT(".")) && !ExportedString.Contains(TEXT("e")))
		{
			ExportedString += ".0";
		}
		return ExportedString;
	}

	virtual TOptional<TRealNumericBoxWrapper<RealType>> FromString(const FString& InString, const TRealNumericBoxWrapper<RealType>& ExistingValue) override
	{
		return TOptional<TRealNumericBoxWrapper<RealType>>();
	}

	// Check whether the typed character is valid
	bool IsCharacterValid(TCHAR InChar) const override
	{
		static const FString ValidChars = TEXT("1234567890-.e");
		return InChar != 0 && ValidChars.GetCharArray().Contains(InChar);
	}
};

// Adds a field to the input pin, so the user can set a value to the pin
template<typename RealType>
class SRealGraphPin : public SGraphPin
{
public:

	SLATE_BEGIN_ARGS(SRealGraphPin<RealType>) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
	}

public:

	TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(SBox)
		.MinDesiredWidth(18)
		.MaxDesiredWidth(400)
		[
			SNew(SNumericEntryBox<TRealNumericBoxWrapper<RealType>>)
			.EditableTextBoxStyle(FAppStyle::Get(), "Graph.EditableTextBox")
			.BorderForegroundColor(FSlateColor::UseForeground())
			.Visibility(this, &SRealGraphPin<RealType>::GetDefaultValueVisibility)
			.IsEnabled(this, &SRealGraphPin<RealType>::GetDefaultValueIsEnabled)
			.Value(this, &SRealGraphPin<RealType>::GetNumericValue)
			.OnValueCommitted(this, &SRealGraphPin<RealType>::SetNumericValue)
			.TypeInterface(MakeShareable(new TRealNumericTypeInterface<RealType>()))
		];
	}

	FSlateColor GetPinColor() const
	{
		return SGraphPin::GetPinColor();
	}

	bool GetDefaultValueIsEnabled() const
	{
		return !GraphPinObj->bDefaultValueIsReadOnly;
	}

	TOptional<TRealNumericBoxWrapper<RealType>> GetNumericValue() const
	{
		FString MutableDefaultString = GraphPinObj->GetDefaultAsString();
		MutableDefaultString.RemoveFromStart("(");
		MutableDefaultString.RemoveFromEnd(")");

		return TRealNumericBoxWrapper<RealType>(RealType(MutableDefaultString));
	}

	void SetNumericValue(TRealNumericBoxWrapper<RealType> InValue, ETextCommit::Type CommitType)
	{
		const FString TypeValueString = FString::Printf(TEXT("(%s)"), *InValue.Val.ToString());
		if (GraphPinObj->GetDefaultAsString() != TypeValueString)
		{
			GraphPinObj->Modify();
			GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, *TypeValueString);
		}
	}
};


#define LOCTEXT_NAMESPACE "FRealStructCustomization"

template<typename RealType>
class FRealStructCustomization : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FRealStructCustomization<RealType>());
	}

	TOptional<TRealNumericBoxWrapper<RealType>> GetNumericValue(TSharedRef<IPropertyHandle> PropertyHandle) const
	{
		TArray<FString> Values;
		PropertyHandle->GetPerObjectValues(Values);

		if (Values.Num() >= 1)
		{
			FString Value = Values[0];
			Value.RemoveFromStart("(");
			Value.RemoveFromEnd(")");
			return TRealNumericBoxWrapper<RealType>(RealType(Value));
		}

		return TOptional<TRealNumericBoxWrapper<RealType>>();
	}

	void SetNumericValue(TRealNumericBoxWrapper<RealType> InValue, ETextCommit::Type CommitType, TSharedRef<IPropertyHandle> PropertyHandle)
	{
		FScopedTransaction Transaction(LOCTEXT("Oui", "Non"));

		PropertyHandle->NotifyPreChange();
		PropertyHandle->SetPerObjectValues({ "(" + InValue.Val.ToString() + ")" });
		PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		PropertyHandle->NotifyFinishedChangingProperties();
	}

	void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SNumericEntryBox<TRealNumericBoxWrapper<RealType>>)
			.Value(this, &FRealStructCustomization<RealType>::GetNumericValue, PropertyHandle)
			.OnValueCommitted(this, &FRealStructCustomization<RealType>::SetNumericValue, PropertyHandle)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.TypeInterface(MakeShareable(new TRealNumericTypeInterface<RealType>()))
		];
	}


	void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
	}
};

#undef LOCTEXT_NAMESPACE
