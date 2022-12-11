// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOJOYWAYPROJ_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ApplyDamage(float Damage);
};
