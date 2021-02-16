// Copyright 2020 Baptiste Hutteau Licensed under the Apache License, Version 2.0

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

#include "SpaceKitPrecisionEditor/Public/VectorGenericEntryBox.h"

#define LOCTEXT_NAMESPACE "FVectorGenericStructCustomization"

// Property customization, for vector type. In order to don't recode this struct for each vector class, we made it generic
template<typename VectorType, typename ScalarType>
class FVectorGenericStructCustomization : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance(
		FText InLabelX = LOCTEXT("LabelX", "X"), FText InLabelY = LOCTEXT("LabelY", "Y"), FText InLabelZ = LOCTEXT("LabelZ", "Z"),
		FText InTooltipX = LOCTEXT("TooltipX", "Axis X"), FText InTooltipY = LOCTEXT("TooltipY", "Axis Y"), FText InTooltipZ = LOCTEXT("TooltipZ", "Axis Z")
	)
	{
		return MakeShareable(new FVectorGenericStructCustomization(InLabelX, InLabelY, InLabelZ, InTooltipX, InTooltipY, InTooltipZ));
	}

	static TSharedRef<IPropertyTypeCustomization> MakeInstanceDefaults()
	{
		return MakeInstance();
	}

	void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		using SVectorGenericEntryBoxSpecialized = SVectorGenericEntryBox<VectorType, ScalarType>;
		
		VectorFixedPropertyHandle = PropertyHandle;

		void* Data;
		PropertyHandle->GetValueData(Data);
		VectorType* Value = static_cast<VectorType*>(Data);

		HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.FillWidth(500.f)
			.Padding(2.f, 0.f)
			[
				SNew(SVectorGenericEntryBoxSpecialized)
				.Vector(this, &FVectorGenericStructCustomization::GetVectorFixed)
				.OnVectorCommitted(this, &FVectorGenericStructCustomization::SetVectorFixed)
				.Font(IDetailLayoutBuilder::GetDetailFont())
				.LabelX(LabelX)
				.LabelY(LabelY)
				.LabelZ(LabelZ)
				.TooltipX(TooltipX)
				.TooltipY(TooltipY)
				.TooltipZ(TooltipZ)
			]
		];
	}

	void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
	}

	VectorType GetVectorFixed() const
	{
		TSharedPtr<IPropertyHandle> VectorFixedSharedPtr = VectorFixedPropertyHandle.Pin();

		void* Data;
		VectorFixedSharedPtr->GetValueData(Data);
		VectorType* Value = static_cast<VectorType*>(Data);

		return Value ? *Value : VectorType::Identity;
	}

	void SetVectorFixed(ScalarType NewValue, EAxis::Type Axis, ETextCommit::Type CommitType)
	{
		TSharedPtr<IPropertyHandle> VectorFixedSharedPtr = VectorFixedPropertyHandle.Pin();

		void* Data;
		VectorFixedSharedPtr->GetValueData(Data);
		VectorType* Value = static_cast<VectorType*>(Data);

		VectorFixedSharedPtr->NotifyPreChange();
		Value->GetAxis(Axis) = NewValue;
		VectorFixedSharedPtr->NotifyPostChange();
	}

private:
	
	TWeakPtr<IPropertyHandle> VectorFixedPropertyHandle;

	FText LabelX, LabelY, LabelZ;
	FText TooltipX, TooltipY, TooltipZ;

protected:

	FVectorGenericStructCustomization(const FText& InLabelX, const FText& InLabelY, const FText& InLabelZ, const FText& InTooltipX, const FText& InTooltipY, const FText& InTooltipZ)
		: IPropertyTypeCustomization(), LabelX(InLabelX), LabelY(InLabelY), LabelZ(InLabelZ), TooltipX(InTooltipX), TooltipY(InTooltipY), TooltipZ(InTooltipZ)
	{
	}
};

#undef LOCTEXT_NAMESPACE
