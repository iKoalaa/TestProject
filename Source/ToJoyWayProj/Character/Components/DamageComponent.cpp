// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageComponent.h"

#include "HealthComponent.h"

void UDamageComponent::ApplyDamage(float Damage)
{
	if(UHealthComponent* const HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->ApplyDamage(Damage);
	}
}
