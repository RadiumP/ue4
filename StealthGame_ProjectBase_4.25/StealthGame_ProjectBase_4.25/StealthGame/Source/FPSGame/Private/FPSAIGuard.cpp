// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "FPSGameMode.h"



// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("AI Comp"));

	GuardState = EAIState::Idle;
	

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen); //Put this in ctor not working
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard); //Put this in ctor not working
	OGRotation = GetActorRotation();
	
	if (bPatrol)
	{
		Patrol();
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Seen"));
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}

}

void AFPSAIGuard::OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume)
{

	if (GuardState == EAIState::Alerted)
	{
		return;
	}


	//DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Blue, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt =  FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(Timehandle_RestOrientation);
	
	GetWorldTimerManager().SetTimer(Timehandle_RestOrientation, this, &AFPSAIGuard::ResetOrientaion, 3.0f);

	
	SetGuardState(EAIState::Suspicious);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}

}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
	OnRep_GuardState();

	//OnStateChanged(GuardState);
}

void AFPSAIGuard::Patrol()
{
	//UE_LOG(LogTemp, Warning, TEXT("Patrol"));
	if (!CurrentPatrolPoint || CurrentPatrolPoint == PatrolEnd)
	{
		//UE_LOG(LogTemp, Warning, TEXT("St"));
		CurrentPatrolPoint = PatrolStart;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("End"));
		CurrentPatrolPoint = PatrolEnd;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentPatrolPoint)
	{
		FVector Direction = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();

		float DistanceToGoal = Direction.Size();
		//UE_LOG(LogTemp, Warning, TEXT("%f"), DistanceToGoal);
		if (DistanceToGoal < 80.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("5"));
			Patrol();
		}
	}

}


void AFPSAIGuard::ResetOrientaion()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}


	//UE_LOG(LogTemp, Warning, TEXT("Turn: %d"), &OGRotation.Yaw);
	SetActorRotation(OGRotation);

	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		Patrol();
	}

}


//for client
void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}