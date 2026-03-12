// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"

/**
 * Custom details panel layout for AMyCustomActor
 */
class FMyCustomActorDetails : public IDetailCustomization
{
public:
	// Creates a new instance of this customization
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;

private:
	// Handle checkbox state
	bool IsCheckBoxChecked() const;
	void OnCheckBoxChanged(bool bNewState);

	// Handle button click
	FReply OnButtonClick();

	// Handle value slider
	void OnValueChanged(float NewValue);
	float GetValue() const;

	// Handle category visibility
	EVisibility GetBasicSettingsVisibility() const;
	EVisibility GetVisualSettingsVisibility() const;

	// Get selected objects
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;

	// Cached properties
	bool bShowAdvancedSettings;
};
