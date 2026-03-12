// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "IPropertyUtilities.h"

/**
 * Custom property type customization for FVector showing X, Y, Z as slider
 * This is meant as an example of struct customization
 */
class FMyStructCustomization : public IPropertyTypeCustomization
{
public:
	// Creates a new instance
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	// Helper to get property value
	float GetFloatValue(const FName& PropertyName) const;
	void SetFloatValue(const FName& PropertyName, float NewValue);

	// Property handle reference
	TSharedPtr<IPropertyHandle> StructPropertyHandle;

	// Child property handles
	TSharedPtr<IPropertyHandle> XHandle;
	TSharedPtr<IPropertyHandle> YHandle;
	TSharedPtr<IPropertyHandle> ZHandle;
};
