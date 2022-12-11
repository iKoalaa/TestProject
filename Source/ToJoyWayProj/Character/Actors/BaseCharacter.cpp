// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "GameFramework/GameModeBase.h"
#include "ToJoyWayProj/ProjectGameMode.h"
#include "ToJoyWayProj/Character/Components/DamageComponent.h"
#include "ToJoyWayProj/Character/Components/HealthComponent.h"
#include "ToJoyWayProj/Character/Components/WeaponHolderComponent.h"
#include "ToJoyWayProj/Weapon/Actors/BaseWeapon.h"

#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

ABaseCharacter::ABaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	SK_Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	SK_Hands->SetOnlyOwnerSee(true);
	SK_Hands->SetupAttachment(FirstPersonCameraComponent);
	SK_Hands->bCastDynamicShadow = false;
	SK_Hands->CastShadow = false;
	SK_Hands->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	SK_Hands->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	WeaponHolderComponent = CreateDefaultSubobject<UWeaponHolderComponent>(TEXT("WeaponHolderComponents"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

#if WITH_EDITOR
	BackWeaponPlaceholder = CreateDefaultSubobject<UBillboardComponent>(TEXT("BackWeaponPlaceholder"));
#endif
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapBegin);
	HealthComponent->OnDie.AddDynamic(this, &ABaseCharacter::Die);
}

void ABaseCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseCharacter::OnOverlapBegin);
	HealthComponent->OnDie.RemoveDynamic(this, &ABaseCharacter::Die);
}

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("SwitchWeaponUp", IE_Pressed, this, &ABaseCharacter::SwitchWeaponUp);
	PlayerInputComponent->BindAction("SwitchWeaponDown", IE_Pressed, this, &ABaseCharacter::SwitchWeaponDown);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::OnFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);
}

void ABaseCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseWeapon* const PickableWeapon = Cast<ABaseWeapon>(Other))
	{
		WeaponHolderComponent->AddWeapon(PickableWeapon);
	}
}

void ABaseCharacter::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHolderComponent->DropAllWeapon();
	Destroy();
	Cast<AProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->Respawn();
}

void ABaseCharacter::SwitchWeaponDown()
{
	if (WeaponHolderComponent->IsEmpty())
	{
		return;
	}

	WeaponHolderComponent->NextWeapon();
	WeaponHolderComponent->GetActiveWeapon();
}

UWeaponHolderComponent& ABaseCharacter::GetWeaponHolderComponent() const
{
	return *WeaponHolderComponent;
}

void ABaseCharacter::SwitchWeaponUp()
{
	if (WeaponHolderComponent->IsEmpty())
	{
		return;
	}
	WeaponHolderComponent->PreviewWeapon();
	WeaponHolderComponent->GetActiveWeapon();
}

const FTransform& ABaseCharacter::GetBackWeaponTransform() const
{
	return WeaponBackPosition;
}

#if WITH_EDITOR
void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (BackWeaponPlaceholder)
	{
		WeaponBackPosition = BackWeaponPlaceholder->GetComponentTransform();
	}
}
#endif

void ABaseCharacter::OnFire()
{
	if (WeaponHolderComponent && !WeaponHolderComponent->IsEmpty())
	{
		WeaponHolderComponent->GetActiveWeapon().Fire();
		if (UAnimMontage* const FireAnimation = WeaponHolderComponent->GetActiveWeapon().GetFireAnimation())
		{
			if (UAnimInstance* const AnimInstance = SK_Hands->GetAnimInstance())
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void ABaseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
