// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ABlackHoleActor::ABlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = MeshComp;


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleActor::OverlapInnerSphere);
	SphereComp->SetSphereRadius(150);
	SphereComp->SetupAttachment(MeshComp);

}




// Called when the game starts or when spawned
void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	
	

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), BoxActors);

	
}


void ABlackHoleActor::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("hi"));

	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	
		for (AActor* Box : BoxActors)
		{
			if (Box)
			{
				TArray<UPrimitiveComponent*> BoxComps;
				Box->GetComponents<UPrimitiveComponent>(BoxComps);

				auto Direction = GetActorLocation() - Box->GetActorLocation();


				//UE_LOG(LogTemp, Warning, TEXT("%d,%d,%d"), Direction.X, Direction.Y, Direction.Z);

				for (auto Prim : BoxComps)
				{
					if (Prim && Prim->IsSimulatingPhysics())
					{
						Prim->AddForce(Direction * Force);
						//UE_LOG(LogTemp, Warning, TEXT("%s"), *BoxMesh->GetName());
					}
				}

				
				
			}
		}

	
}

