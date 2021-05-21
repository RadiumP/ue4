// Fill out your copyright notice in the Description page of Project Settings.


#include "SBot.h"


#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/Gameplaystatics.h"
#include "GameFramework/Character.h"
#include "Components/SHealthComponent.h"

// Sets default values
ASBot::ASBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;


	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASBot::HandleTakeDamage);
	
	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistance = 100;
	
}

// Called when the game starts or when spawned
void ASBot::BeginPlay()
{
	Super::BeginPlay();

	 NextPathPoint = GetNextPathPoint();
}

FVector ASBot::GetNextPathPoint()
{
	const auto PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	
	if(NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}
	return GetActorLocation();
}

// Called every frame
void ASBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistnaceToTarget = (GetActorLocation() -  NextPathPoint).Size();
	
	if(DistnaceToTarget <= RequiredDistance)
//		!GetActorLocation().Equals(NextPathPoint))
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		

		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		ForceDirection *= MovementForce;
		
		
		
		MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	}

}


void ASBot::HandleTakeDamage(USHealthComponent* HealthComponent, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//Change color when hit
	if(MatIns == nullptr)
	{
		MatIns = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	if(MatIns)
	{
		MatIns->SetScalarParameterValue("LastTimeDmgTaken", GetWorld()->TimeSeconds);
	}
	//Explode when 0
	
}