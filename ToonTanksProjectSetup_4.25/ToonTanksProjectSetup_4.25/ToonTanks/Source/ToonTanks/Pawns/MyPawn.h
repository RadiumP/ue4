// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class UMyHealthComponent;


UCLASS()
class TOONTANKS_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();
	virtual void HandleDestruction();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void RotateTurret(FVector LookAtTarget);

	void Fire();

	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UMyHealthComponent* HealthComponent;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	    TSubclassOf<AProjectileBase> ProjectileClass;


	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere,Category = "Effects")
		TSubclassOf<UCameraShake> DeathShake;

	


};


