// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * Asset type actions for UMyCustomAsset
 */
class FMyCustomAssetTypeActions : public FAssetTypeActions_Base
{
public:
	explicit FMyCustomAssetTypeActions(uint32 InAssetCategory);

	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual const TArray<FText>& GetSubMenus() const override;
	virtual bool CanFilter() override;
	virtual bool ShouldForceWorldCentric() override;

private:
	uint32 AssetCategory;

	// Submenu names
	static TArray<FText> CachedSubMenus;
};
