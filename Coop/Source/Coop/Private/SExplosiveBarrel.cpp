// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Coop/Coop.h"
#include "Components/SHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Framework/Docking/LayoutExtender.h"
#include "Kismet/Gameplaystatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));

	RootComponent = MeshComp;
	RadialForceComp->SetupAttachment(MeshComp);
	

	SetReplicates(true);
	SetReplicateMovement(true);
	 
	//GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);

	MeshComp->SetMaterial(0, NormalMat);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	
	
	RadialForceComp->Radius = 250.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->bAutoActivate = true;
	
}

void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* HealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(bExploded)
	{
		return;
	}
	if(Health <= 0.f && !bExploded)
	{
		
		bExploded = true;

		//Explode();
		OnRep_Explode();
		
		//MeshComp->AddForce(FVector(0, 0.0, 1.0) * 500000.0f);
		MeshComp->AddImpulse(FVector::UpVector * 400.f, NAME_None, true);

		RadialForceComp->FireImpulse();
	}
}

void ASExplosiveBarrel::Explode()
{

	
	
	
	
		UGameplayStatics::SpawnEmitterAttached(ExplodingEffect, MeshComp);

	
		MeshComp->SetMaterial(0, AfterMat);
	
	
}

void ASExplosiveBarrel::OnRep_Explode()
{

	//FX only because mvmnt is rep
	UGameplayStatics::SpawnEmitterAttached(ExplodingEffect, MeshComp);

	
	MeshComp->SetMaterial(0, AfterMat);
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//client
	DOREPLIFETIME(ASExplosiveBarrel, bExploded);

}