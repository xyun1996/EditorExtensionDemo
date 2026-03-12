// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

/**
 * Custom editor mode implementation
 */
class FMyEditorMode : public FEdMode
{
public:
	// Mode identifier
	static const FEditorModeID EM_EditorModeId;

	// Constructor
	FMyEditorMode();

	// FEdMode interface
	virtual void Initialize() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;

	// Input handling
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool InputAxis(FEditorViewportClient* InViewportClient, FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime) override;
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;

	// Selection
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;
	virtual void ActorSelectionChangeNotify() override;
	// Utility
	virtual bool UsesToolkits() const override { return true; }

	// Get current state
	bool IsModeActive() const { return bModeActive; }

	// Mode state
	void SetCurrentTool(const FName& ToolId);
	FName GetCurrentTool() const { return CurrentTool; }

private:
	// Mode state
	bool bModeActive;

	// Current active tool
	FName CurrentTool;

	// Click count for demo
	int32 ClickCount;

	// Last clicked location
	FVector LastClickLocation;
};
