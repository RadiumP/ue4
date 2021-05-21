// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Component/MyHealthComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);


	HealthComponent = CreateDefaultSubobject<UMyHealthComponent>(TEXT("Health Component"));
	

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyPawn::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean =FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();

	TurretMesh->SetWorldRotation(TurretRotation);
}

void AMyPawn::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire"));
	if (ProjectileClass)
	{
		
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());

		TempProjectile->SetOwner(this);
		
	}
}

void AMyPawn::HandleDestruction()
{

	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());


	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);

}



// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}

