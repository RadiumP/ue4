// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SHealthComponent.h"
#include "GameFramework/Pawn.h"
#include "SBot.generated.h"


class USHealthComponent;

UCLASS()

class COOP_API ASBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USHealthComponent* HealthComp;
	
	FVector GetNextPathPoint();

	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* HealthComponent, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category="Bot");
	float MovementForce;
	UPROPERTY(EditDefaultsOnly, Category="Bot");
	bool bUseVelocityChange;
	UPROPERTY(EditDefaultsOnly, Category="Bot");
	float RequiredDistance;

	UMaterialInstanceDynamic* MatIns;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
