// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Coop/Coop.h"
#include "Kismet/Gameplaystatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "GeneratedCodeHelpers.h"
#include "Net/UnrealNetwork.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
 	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";
	BaseDamage = 20.f;

	RoundsPerMinute = 300;

	//client matching server
	SetReplicates(true);

	//handle delay
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimebetweenShots = 60 / RoundsPerMinute;
}


void ASWeapon::Fire()
{

	if(GetLocalRole() < ROLE_Authority)
	{
		//Server side
		ServerFire();		
	}
	
	
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//For FPS mostly 
		
		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		TraceEndPoint = TraceEnd;
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;//for more accurate shot etc
		QueryParams.bReturnPhysicalMaterial = true;
		
		auto SurfaceType = SurfaceType_Default;
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_GameTraceChannel1, QueryParams))
		{
			//Hit and apply dmg

			AActor* HitActor = Hit.GetActor();

			


			 SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.0f;
			}

			PlayImpactEffect_(SurfaceType, Hit.ImpactPoint);
			

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			TraceEndPoint = Hit.ImpactPoint;
		}
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		}


		PlayFireEffect_(TraceEndPoint);

		if(GetLocalRole() == ROLE_Authority)
		{
			HitscanTrace.TraceTo = TraceEndPoint;
			HitscanTrace.SurfaceType = SurfaceType;
		}

		LastFiredTime = GetWorld()->TimeSeconds;
	}

}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max( LastFiredTime + TimebetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimebetweenShots, this, &ASWeapon::Fire, TimebetweenShots, true, FirstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimebetweenShots);
}

void ASWeapon::OnRep_HitScanTrace()
{
	//FX
	PlayFireEffect_(HitscanTrace.TraceTo);
	PlayImpactEffect_(HitscanTrace.SurfaceType, HitscanTrace.TraceTo);
}

void ASWeapon::PlayFireEffect_(FVector EndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}



	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		auto TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, EndPoint);
		}
	}

	AActor* MyOwner = Cast<APawn>(GetOwner());

	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetInstigatorController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}

}

void ASWeapon::PlayImpactEffect_(EPhysicalSurface SurfaceType,FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = ImpactEffect;

	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
		SelectedEffect = FleshImpactEffect;
		break;
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshCritImpactEffect;
		break;
	default:
		break;
	}


	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDir = ImpactPoint - MuzzleLocation;
		ShotDir.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDir.Rotation());
	}

}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//client
	DOREPLIFETIME_CONDITION(ASWeapon, HitscanTrace, COND_SkipOwner);//rep to everyone except the one uses weapon
	
}
