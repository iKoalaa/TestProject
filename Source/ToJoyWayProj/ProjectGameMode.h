// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectGameMode.generated.h"

UCLASS()
class TOJOYWAYPROJ_API AProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectGameMode();
	
protected:
	virtual void BeginPlay() override;
	
public:
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> SpawnPoint;

	void Respawn();
};



