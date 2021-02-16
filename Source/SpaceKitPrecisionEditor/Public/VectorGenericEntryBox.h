// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "SpaceKitPrecisionEditor/Public/RealGenericEditor.h"

#include "SlateCore/Public/Widgets/DeclarativeSyntaxSupport.h"
#include "SlateCore/Public/Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

class FArguments;
class SHorizontalBox;

#define LOCTEXT_NAMESPACE "SVectorGenericEntryBox"

// Widget for editing vector types, with three axes. Can be reused for any vector-ish types, including rotators
// This is inspired by SVectorInputBox
template<typename VectorType, typename ScalarType>
class SVectorGenericEntryBox : public SCompoundWidget
{
public:

	// Delegate for numeric value committed
	DECLARE_DELEGATE_ThreeParams(FOnValueCommitted, ScalarType /*NewValue*/, EAxis::Type /*ChangedAxis*/, ETextCommit::Type /*CommitType*/);

public:

	using SVectorGenericEntryBoxSpecialized = SVectorGenericEntryBox<VectorType, ScalarType>;
	SLATE_BEGIN_ARGS( SVectorGenericEntryBoxSpecialized )
		: _Font( FCoreStyle::Get().GetFontStyle("NormalFont") )
		, _bColorAxisLabels( false )
		{}

		// Value
		SLATE_ATTRIBUTE( VectorType, Vector )

		// Font
		SLATE_ATTRIBUTE( FSlateFontInfo, Font )

		// Label X
		SLATE_ARGUMENT(FText, LabelX)

		// Label Y
		SLATE_ARGUMENT(FText, LabelY)

		// Label Z
		SLATE_ARGUMENT(FText, LabelZ)

		// Tooltip X
		SLATE_ARGUMENT(FText, TooltipX)

		// Tooltip Y
		SLATE_ARGUMENT(FText, TooltipY)

		// Tooltip Z
		SLATE_ARGUMENT(FText, TooltipZ)

		// Color the axes
		SLATE_ARGUMENT( bool, bColorAxisLabels )

		// Called when a component of the vector is committed
		SLATE_EVENT( FOnValueCommitted, OnVectorCommitted )

	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs )
	{
		TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

		ChildSlot
		[
			HorizontalBox
		];

		VectorAttribute = InArgs._Vector;
		OnVectorValueCommitted = InArgs._OnVectorCommitted;

		ConstructAxis(EAxis::X, InArgs, HorizontalBox);
		ConstructAxis(EAxis::Y, InArgs, HorizontalBox);
		ConstructAxis(EAxis::Z, InArgs, HorizontalBox);
	}

private:

	TAttribute<VectorType> VectorAttribute;

	FOnValueCommitted OnVectorValueCommitted;

	void ConstructAxis(EAxis::Type Axis, const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox)
	{
#define GET_AXIS(axis, x, y, z) (((axis) == EAxis::X) ? (x) : (((axis) == EAxis::Y) ? (y) : (z)))

		const FLinearColor LabelColor = InArgs._bColorAxisLabels
			? GET_AXIS(Axis, SNumericEntryBox<float>::RedLabelBackgroundColor, SNumericEntryBox<float>::GreenLabelBackgroundColor, SNumericEntryBox<float>::BlueLabelBackgroundColor)
			: FLinearColor(0.0f, 0.0f, 0.0f, .5f);

		HorizontalBox->AddSlot()
		.VAlign(VAlign_Center)
		.FillWidth(1.f)
		.Padding(0.0f, 1.f, GET_AXIS(Axis, 2.f, 2.f, 0.f), 1.f)
		[
			SNew(SNumericEntryBox<TRealNumericBoxWrapper<ScalarType>>)
			.Font(InArgs._Font)
			.Value(this, &SVectorGenericEntryBox::GetValue, Axis)
			.OnValueCommitted(this, &SVectorGenericEntryBox::OnValueCommitted, Axis)
			.ToolTipText(GET_AXIS(Axis, InArgs._TooltipX, InArgs._TooltipY, InArgs._TooltipZ))
			.UndeterminedString(LOCTEXT("Invalid", "Invalid"))
			.TypeInterface(MakeShareable(new TRealNumericTypeInterface<ScalarType>()))
			.LabelPadding(0)
			.Label()
			[
				SNumericEntryBox<float>::BuildLabel(GET_AXIS(Axis, InArgs._LabelX, InArgs._LabelY, InArgs._LabelZ), FLinearColor::White, LabelColor)
			]
		];		
	}

	TOptional<TRealNumericBoxWrapper<ScalarType>> GetValue(EAxis::Type Axis) const
	{
		if (!VectorAttribute.IsBound()) return TOptional<TRealNumericBoxWrapper<ScalarType>>();
		return TRealNumericBoxWrapper<ScalarType>(VectorAttribute.Get().GetAxis(Axis));
	}

	void OnValueCommitted(TRealNumericBoxWrapper<ScalarType> NewValue, ETextCommit::Type CommitType, EAxis::Type Axis)
	{
		if (!OnVectorValueCommitted.IsBound()) return;
		OnVectorValueCommitted.ExecuteIfBound(NewValue.Val, Axis, CommitType);
	}
};

#undef LOCTEXT_NAMESPACE
