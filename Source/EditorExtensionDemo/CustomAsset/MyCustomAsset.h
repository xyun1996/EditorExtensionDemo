// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyCustomAsset.generated.h"

class UTexture2D;

/**
 * Custom asset class for demonstrating custom asset editor.
 */
UCLASS(BlueprintType, Category = "EditorExtensionDemo")
class EDITOREXTENSIONDEMO_API UMyCustomAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UMyCustomAsset();

	// Asset name/identifier
	UPROPERTY(EditAnywhere, Category = "General", meta = (DisplayName = "Asset Name"))
	FString AssetName;

	// Description text
	UPROPERTY(EditAnywhere, Category = "General", meta = (MultiLine = true))
	FText Description;

	// Preview texture
	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<UTexture2D> PreviewTexture;

	// Numeric value
	UPROPERTY(EditAnywhere, Category = "Data", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Value;

	// Boolean flag
	UPROPERTY(EditAnywhere, Category = "Data")
	bool bEnabled;

	// Color value
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor AssetColor;

	// Integer array for demonstration
	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<int32> IntegerArray;

	// String map for demonstration
	UPROPERTY(EditAnywhere, Category = "Data")
	TMap<FString, FString> StringProperties;

	// Called after asset is created
	virtual void PostInitProperties() override;

	// Get display name for asset editor
	FText GetAssetDisplayName() const;
};
