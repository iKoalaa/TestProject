// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UDamageComponent;
class UHealthComponent;
class UWeaponHolderComponent;
class UCameraComponent;

UCLASS(config=Game)
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	ABaseCharacter();
	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* SK_Hands;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

#if WITH_EDITOR
	UPROPERTY(EditAnywhere, Category = "Actor Setup")
	UBillboardComponent* BackWeaponPlaceholder;
#endif
	
protected:
	UPROPERTY(EditAnywhere, Category = "Playble Components")
	UWeaponHolderComponent* WeaponHolderComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playble Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, Category = "Playble Components")
	UDamageComponent* DamageComponent;

	FTransform WeaponBackPosition;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	const FTransform& GetBackWeaponTransform() const;
	UWeaponHolderComponent& GetWeaponHolderComponent() const;

protected:

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	UFUNCTION()
	void Die();
	
	void OnFire();
	void SwitchWeaponUp();
	void SwitchWeaponDown();
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	USkeletalMeshComponent* GetHandsMeshComponent() const { return SK_Hands; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

