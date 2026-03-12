// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MyCustomAssetFactory.generated.h"

/**
 * Factory used by Content Browser to create UMyCustomAsset.
 */
UCLASS()
class EDITOREXTENSIONDEMOEDITOR_API UMyCustomAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UMyCustomAssetFactory();

	// UFactory interface
	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn,
		FName CallingContext) override;

	virtual uint32 GetMenuCategories() const override;
	virtual FText GetDisplayName() const override;
	virtual const TArray<FText>& GetMenuCategorySubMenus() const override;

private:
	uint32 MenuCategory;
	TArray<FText> SubMenus;
};
