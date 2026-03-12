// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomAsset.h"

UMyCustomAsset::UMyCustomAsset()
	: Value(50.0f)
	, bEnabled(true)
	, AssetColor(FLinearColor::Blue)
{
	AssetName = TEXT("NewCustomAsset");
	Description = FText::FromString(TEXT("A custom asset for demonstration purposes."));
}

void UMyCustomAsset::PostInitProperties()
{
	Super::PostInitProperties();
}

FText UMyCustomAsset::GetAssetDisplayName() const
{
	if (!AssetName.IsEmpty())
	{
		return FText::FromString(AssetName);
	}
	return FText::FromString(GetName());
}
