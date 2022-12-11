// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGameMode.h"
#include "ToJoyWayProjHUD.h"
#include "Character/Actors/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

AProjectGameMode::AProjectGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_BaseCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = AToJoyWayProjHUD::StaticClass();
}

void AProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectGameMode::Respawn()
{
	UWorld* World = GetWorld();

	ABaseCharacter* NewCharacter = World->SpawnActor<ABaseCharacter>(DefaultPawnClass, SpawnPoint->GetTransform());
	UGameplayStatics::GetPlayerController(World, 0)->Possess(NewCharacter);
}
