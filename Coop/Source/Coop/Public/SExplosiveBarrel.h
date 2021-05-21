// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SHealthComponent.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USHealthComponent;
class URadialForceComponent;
UCLASS()
class COOP_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
    void OnHealthChanged(USHealthComponent* HealthComponent, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void Explode();

	UFUNCTION()
    void OnRep_Explode();

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* ExplodingEffect;


	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UMaterialInterface* NormalMat;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UMaterialInterface* AfterMat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	USHealthComponent* HealthComp;

	URadialForceComponent* RadialForceComp;
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing=OnRep_Explode)
	bool bExploded;
};
