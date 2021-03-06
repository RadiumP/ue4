// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100.f;

	SetIsReplicated(true);
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//server handle dmg
	if(GetOwnerRole() == ROLE_Authority)//comps dont have role owners do
	{
		AActor* MyOwner = GetOwner();

	
		if(MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleAnyDamage);
		}
	}
	Health = DefaultHealth;
}

void USHealthComponent::HandleAnyDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health-Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("HP: %f"), Health);

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
	
}



//Correspond with Replicated vars
void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//client
	DOREPLIFETIME(USHealthComponent, Health);//rep to everyone except the one uses weapon
	
}