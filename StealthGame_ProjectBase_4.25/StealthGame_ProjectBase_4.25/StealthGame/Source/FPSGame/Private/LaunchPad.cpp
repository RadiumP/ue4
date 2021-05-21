// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"


// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	

	

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Comp"));
	
	OverlapComp->SetBoxExtent(FVector(200.0f));
	
	
	RootComponent = OverlapComp;
	MeshComp->SetupAttachment(OverlapComp);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);

	

}

void ALaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator Direction = GetActorRotation();
	Direction.Pitch += Angle;

	FVector LaunchVelocity = Direction.Vector() * Speed;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{

		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);

		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
		UGameplayStatics::PlaySound2D(this, LaunchingSound);
	}
	else if (OtherComp)	
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
		if (OtherComp->IsSimulatingPhysics())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Phy"));
			OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
			UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
			UGameplayStatics::PlaySound2D(this, LaunchingSound);
		}
	}
}
