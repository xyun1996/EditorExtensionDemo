// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomActorDetails.h"
#include "CustomAsset/MyCustomActor.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"

#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "MyCustomActorDetails"

TSharedRef<IDetailCustomization> FMyCustomActorDetails::MakeInstance()
{
	return MakeShareable(new FMyCustomActorDetails);
}

void FMyCustomActorDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Cache selected objects
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	// Hide default categories we'll be customizing
	DetailBuilder.EditCategory("Basic Settings");
	DetailBuilder.EditCategory("Transform Settings");
	DetailBuilder.EditCategory("Visual Settings");
	DetailBuilder.EditCategory("Data Settings");
	DetailBuilder.EditCategory("Custom Data");

	// Get or create category
	IDetailCategoryBuilder& BasicCategory = DetailBuilder.EditCategory("Basic Settings", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& TransformCategory = DetailBuilder.EditCategory("Transform Settings", FText::GetEmpty(), ECategoryPriority::Default);
	IDetailCategoryBuilder& VisualCategory = DetailBuilder.EditCategory("Visual Settings", FText::GetEmpty(), ECategoryPriority::Default);
	IDetailCategoryBuilder& CustomCategory = DetailBuilder.EditCategory("Custom Data", FText::GetEmpty(), ECategoryPriority::Uncommon);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Basic Settings Customization
	////////////////////////////////////////////////////////////////////////////////////////////////

	// Add custom header row
	BasicCategory.AddCustomRow(LOCTEXT("BasicSettingsHeader", "Basic Settings Header"))
		.WholeRowContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("BasicSettingsHeaderLabel", "=== Basic Actor Configuration ==="))
				.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
			]
		];

	// Add the custom display label property
	BasicCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AMyCustomActor, DisplayLabel));

	// Add custom checkbox row
	BasicCategory.AddCustomRow(LOCTEXT("ActiveState", "Active State"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ActiveStateLabel", "Actor Active"))
			.ToolTipText(LOCTEXT("ActiveStateTooltip", "Toggle whether this actor is active"))
		]
		.ValueContent()
		[
			SNew(SCheckBox)
			.IsChecked_Lambda([this]() -> ECheckBoxState
			{
				if (SelectedObjects.Num() > 0)
				{
					if (AMyCustomActor* Actor = Cast<AMyCustomActor>(SelectedObjects[0].Get()))
					{
						return Actor->bIsActive ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
					}
				}
				return ECheckBoxState::Undetermined;
			})
			.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
			{
				for (TWeakObjectPtr<UObject>& WeakObj : SelectedObjects)
				{
					if (AMyCustomActor* Actor = Cast<AMyCustomActor>(WeakObj.Get()))
					{
						Actor->bIsActive = (NewState == ECheckBoxState::Checked);
					}
				}
			})
		];

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Transform Settings Customization
	////////////////////////////////////////////////////////////////////////////////////////////////

	TransformCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AMyCustomActor, bUseCustomTransform));

	// Add custom button row
	TransformCategory.AddCustomRow(LOCTEXT("ApplyTransform", "Apply Transform"))
		.WholeRowContent()
		[
			SNew(SButton)
			.Text(LOCTEXT("ApplyTransformButton", "Reset to Default"))
			.ToolTipText(LOCTEXT("ApplyTransformTooltip", "Reset the actor to default transform values"))
			.OnClicked_Lambda([this]() -> FReply
			{
				for (TWeakObjectPtr<UObject>& WeakObj : SelectedObjects)
				{
					if (AMyCustomActor* Actor = Cast<AMyCustomActor>(WeakObj.Get()))
					{
						Actor->CustomScale = FVector::OneVector;
						Actor->CustomRotation = FRotator::ZeroRotator;
						Actor->SetActorScale3D(FVector::OneVector);
						Actor->SetActorRotation(FRotator::ZeroRotator);
					}
				}
				return FReply::Handled();
			})
		];

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Visual Settings Customization
	////////////////////////////////////////////////////////////////////////////////////////////////

	// Add color picker property
	VisualCategory.AddProperty(GET_MEMBER_NAME_CHECKED(AMyCustomActor, ActorColor));

	// Add custom slider for opacity
	VisualCategory.AddCustomRow(LOCTEXT("OpacitySlider", "Opacity"))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("OpacityLabel", "Opacity"))
		]
		.ValueContent()
		.MinDesiredWidth(150.0f)
		[
			SNew(SSpinBox<float>)
			.MinValue(0.0f)
			.MaxValue(1.0f)
			.Value_Lambda([this]() -> float
			{
				if (SelectedObjects.Num() > 0)
				{
					if (AMyCustomActor* Actor = Cast<AMyCustomActor>(SelectedObjects[0].Get()))
					{
						return Actor->Opacity;
					}
				}
				return 1.0f;
			})
			.OnValueChanged_Lambda([this](float NewValue)
			{
				for (TWeakObjectPtr<UObject>& WeakObj : SelectedObjects)
				{
					if (AMyCustomActor* Actor = Cast<AMyCustomActor>(WeakObj.Get()))
					{
						Actor->Opacity = NewValue;
						Actor->Modify();
					}
				}
			})
		];

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Custom Data Category with Group
	////////////////////////////////////////////////////////////////////////////////////////////////

	// Create a group within the category
	IDetailGroup& VectorGroup = CustomCategory.AddGroup("VectorData", LOCTEXT("VectorDataLabel", "Vector Data"));

	// Add properties to the group
	VectorGroup.AddPropertyRow(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AMyCustomActor, CustomDataVector)));

	IDetailGroup& RotationGroup = CustomCategory.AddGroup("RotationData", LOCTEXT("RotationDataLabel", "Rotation Data"));
	RotationGroup.AddPropertyRow(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AMyCustomActor, CustomDataRotation)));
}

void FMyCustomActorDetails::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	CustomizeDetails(*DetailBuilder);
}

bool FMyCustomActorDetails::IsCheckBoxChecked() const
{
	if (SelectedObjects.Num() > 0)
	{
		if (const AMyCustomActor* Actor = Cast<AMyCustomActor>(SelectedObjects[0].Get()))
		{
			return Actor->bIsActive;
		}
	}
	return false;
}

void FMyCustomActorDetails::OnCheckBoxChanged(bool bNewState)
{
	for (TWeakObjectPtr<UObject>& WeakObj : SelectedObjects)
	{
		if (AMyCustomActor* Actor = Cast<AMyCustomActor>(WeakObj.Get()))
		{
			Actor->bIsActive = bNewState;
		}
	}
}

FReply FMyCustomActorDetails::OnButtonClick()
{
	return FReply::Handled();
}

void FMyCustomActorDetails::OnValueChanged(float NewValue)
{
	for (TWeakObjectPtr<UObject>& WeakObj : SelectedObjects)
	{
		if (AMyCustomActor* Actor = Cast<AMyCustomActor>(WeakObj.Get()))
		{
			Actor->Opacity = NewValue;
		}
	}
}

float FMyCustomActorDetails::GetValue() const
{
	if (SelectedObjects.Num() > 0)
	{
		if (const AMyCustomActor* Actor = Cast<AMyCustomActor>(SelectedObjects[0].Get()))
		{
			return Actor->Opacity;
		}
	}
	return 1.0f;
}

EVisibility FMyCustomActorDetails::GetBasicSettingsVisibility() const
{
	return EVisibility::Visible;
}

EVisibility FMyCustomActorDetails::GetVisualSettingsVisibility() const
{
	return EVisibility::Visible;
}

#undef LOCTEXT_NAMESPACE
