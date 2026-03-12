// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorMode.h"
#include "MyEditorModeToolkit.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "HitProxies.h"
#include "SceneView.h"
#include "EditorViewportClient.h"
#include "EditorModeManager.h"

#include "GameFramework/Actor.h"
#include "Toolkits/ToolkitManager.h"

#define LOCTEXT_NAMESPACE "MyEditorMode"

const FEditorModeID FMyEditorMode::EM_EditorModeId = FEditorModeID(TEXT("EditorExtensionDemoMode"));

FMyEditorMode::FMyEditorMode()
	: bModeActive(false)
	, CurrentTool(NAME_None)
	, ClickCount(0)
	, LastClickLocation(FVector::ZeroVector)
{
}

void FMyEditorMode::Initialize()
{
	// Legacy FEdMode should initialize toolkit in Enter() when a valid toolkit host exists.
}

void FMyEditorMode::Enter()
{
	FEdMode::Enter();

	if (UsesToolkits() && !Toolkit.IsValid())
	{
		TSharedPtr<IToolkitHost> ToolkitHost = Owner ? Owner->GetToolkitHost() : nullptr;
		if (ToolkitHost.IsValid())
		{
			Toolkit = MakeShareable(new FMyEditorModeToolkit);
			Toolkit->Init(ToolkitHost);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MyEditorMode: Toolkit host is invalid; running without mode toolkit UI."));
		}
	}

	bModeActive = true;
	ClickCount = 0;

	// Log that we've entered the mode
	UE_LOG(LogTemp, Log, TEXT("MyEditorMode: Entered"));
}

void FMyEditorMode::Exit()
{
	bModeActive = false;

	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Log that we've exited the mode
	UE_LOG(LogTemp, Log, TEXT("MyEditorMode: Exited"));

	FEdMode::Exit();
}

void FMyEditorMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);

	// Custom tick logic can go here
}

void FMyEditorMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);

	// Custom rendering can go here
	// For example, draw a simple line at the last click location
	if (bModeActive && !LastClickLocation.IsZero())
	{
		PDI->DrawLine(
			LastClickLocation - FVector(50.0f, 0.0f, 0.0f),
			LastClickLocation + FVector(50.0f, 0.0f, 0.0f),
			FLinearColor::Red,
			SDPG_Foreground
		);

		PDI->DrawLine(
			LastClickLocation - FVector(0.0f, 50.0f, 0.0f),
			LastClickLocation + FVector(0.0f, 50.0f, 0.0f),
			FLinearColor::Red,
			SDPG_Foreground
		);

		PDI->DrawLine(
			LastClickLocation - FVector(0.0f, 0.0f, 50.0f),
			LastClickLocation + FVector(0.0f, 0.0f, 50.0f),
			FLinearColor::Blue,
			SDPG_Foreground
		);
	}
}

void FMyEditorMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);

	if (bModeActive)
	{
		// Draw HUD elements
		FString InfoText = FString::Printf(TEXT("MyEditorMode Active - Clicks: %d"), ClickCount);
		FCanvasTextItem TextItem(FVector2D(10.0f, 10.0f), FText::FromString(InfoText), GEngine->GetSmallFont(), FLinearColor::White);
		Canvas->DrawItem(TextItem);

		if (!LastClickLocation.IsZero())
		{
			FString LocationText = FString::Printf(TEXT("Last Click: %s"), *LastClickLocation.ToString());
			FCanvasTextItem LocationItem(FVector2D(10.0f, 30.0f), FText::FromString(LocationText), GEngine->GetSmallFont(), FLinearColor::Yellow);
			Canvas->DrawItem(LocationItem);
		}
	}
}

bool FMyEditorMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	if (bModeActive)
	{
		ClickCount++;
		LastClickLocation = Click.GetOrigin() + Click.GetDirection() * 1000.0f;

		UE_LOG(LogTemp, Log, TEXT("MyEditorMode: Click at %s (Count: %d)"), *LastClickLocation.ToString(), ClickCount);

		// Return true to indicate we handled the click
		return true;
	}

	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

bool FMyEditorMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (bModeActive)
	{
		// Handle custom key input
		if (Key == EKeys::Escape && Event == IE_Released)
		{
			// Reset mode state
			ClickCount = 0;
			LastClickLocation = FVector::ZeroVector;
		}
	}

	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FMyEditorMode::InputAxis(FEditorViewportClient* InViewportClient, FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime)
{
	return FEdMode::InputAxis(InViewportClient, Viewport, ControllerId, Key, Delta, DeltaTime);
}

bool FMyEditorMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	return FEdMode::InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
}

bool FMyEditorMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	// Allow selection in this mode
	return true;
}

void FMyEditorMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();
}

void FMyEditorMode::SetCurrentTool(const FName& ToolId)
{
	CurrentTool = ToolId;
}

#undef LOCTEXT_NAMESPACE
