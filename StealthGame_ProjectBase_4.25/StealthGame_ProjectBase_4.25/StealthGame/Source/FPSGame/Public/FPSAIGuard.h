// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;


UENUM(BlueprintType)
enum class EAIState : uint8 //to expose in Blueprint need uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;


	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	AActor* PatrolStart;
	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	AActor* PatrolEnd;
	
	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	bool bPatrol;

	AActor* CurrentPatrolPoint;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume);

	
	UFUNCTION()
	void ResetOrientaion();

	FRotator OGRotation;
	FTimerHandle Timehandle_RestOrientation;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	void Patrol();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
