// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponHolderComponent.generated.h"

class ABaseCharacter;
class ABaseWeapon;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOJOYWAYPROJ_API UWeaponHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponHolderComponent();
	
	void AddWeapon(ABaseWeapon* NewWeapon);
	void HideActiveWeapon();
	void PickWeapon();
	void NextWeapon();
	void PreviewWeapon();
	ABaseWeapon& GetActiveWeapon();
	void DropAllWeapon();
	bool IsEmpty();

protected:
	virtual void BeginPlay() override;

	void SwitchWeapon(int32 WeaponIndex);
	
private:
	
	//TODO: Do better, but later
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;
	TArray<ABaseWeapon*> Weapons;
	int32 ActiveWeaponIndex = -1;
};
