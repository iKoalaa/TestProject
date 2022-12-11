// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UArrowComponent;
UCLASS()
class TOJOYWAYPROJ_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Actor Settings")
	USkeletalMeshComponent* SK_Weapon;
	UPROPERTY(EditAnywhere, Category = "Actor Settings")
	TSubclassOf<ABaseBullet> BaseBulletSubclass;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

private:
	FTransform MuzzleTransform;
	
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	void Fire();
	UAnimMontage* GetFireAnimation();
	virtual void Tick(float DeltaTime) override;
};
