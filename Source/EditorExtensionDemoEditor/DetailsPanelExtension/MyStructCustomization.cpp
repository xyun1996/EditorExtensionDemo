// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyStructCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "MyStructCustomization"

TSharedRef<IPropertyTypeCustomization> FMyStructCustomization::MakeInstance()
{
	return MakeShareable(new FMyStructCustomization);
}

void FMyStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;

	// Get child handles
	XHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FVector, X));
	YHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FVector, Y));
	ZHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FVector, Z));

	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 2.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("X: "))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 4.0f, 0.0f)
			[
				XHandle->CreatePropertyValueWidget(false)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 2.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Y: "))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 4.0f, 0.0f)
			[
				YHandle->CreatePropertyValueWidget(false)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 2.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Z: "))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				ZHandle->CreatePropertyValueWidget(false)
			]
		];
}

void FMyStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// Add child properties with custom display
	ChildBuilder.AddProperty(XHandle.ToSharedRef());
	ChildBuilder.AddProperty(YHandle.ToSharedRef());
	ChildBuilder.AddProperty(ZHandle.ToSharedRef());
}

float FMyStructCustomization::GetFloatValue(const FName& PropertyName) const
{
	if (const TSharedPtr<IPropertyHandle>* Handle =
		(PropertyName == GET_MEMBER_NAME_CHECKED(FVector, X)) ? &XHandle :
		(PropertyName == GET_MEMBER_NAME_CHECKED(FVector, Y)) ? &YHandle :
		(PropertyName == GET_MEMBER_NAME_CHECKED(FVector, Z)) ? &ZHandle : nullptr)
	{
		if (Handle->IsValid())
		{
			float Value = 0.0f;
			if ((*Handle)->GetValue(Value) == FPropertyAccess::Success)
			{
				return Value;
			}
		}
	}
	return 0.0f;
}

void FMyStructCustomization::SetFloatValue(const FName& PropertyName, float NewValue)
{
	if (const TSharedPtr<IPropertyHandle>* Handle =
		(PropertyName == GET_MEMBER_NAME_CHECKED(FVector, X)) ? &XHandle :
		(PropertyName == GET_MEMBER_NAME_CHECKED(FVector, Y)) ? &YHandle :
		(PropertyName == GET_MEMBER_NAME_CHECKED(FVector, Z)) ? &ZHandle : nullptr)
	{
		if (Handle->IsValid())
		{
			(*Handle)->SetValue(NewValue);
		}
	}
}

#undef LOCTEXT_NAMESPACE
