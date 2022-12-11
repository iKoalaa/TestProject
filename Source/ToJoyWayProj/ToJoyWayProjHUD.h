// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ToJoyWayProjHUD.generated.h"

UCLASS()
class AToJoyWayProjHUD : public AHUD
{
	GENERATED_BODY()

public:
	AToJoyWayProjHUD();

	virtual void DrawHUD() override;

private:
	class UTexture2D* CrosshairTex;

};

