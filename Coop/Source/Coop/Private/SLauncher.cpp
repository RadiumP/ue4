// Fill out your copyright notice in the Description page of Project Settings.


#include "SLauncher.h"
#include "SProjectile.h"
#include "Kismet/Gameplaystatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"


void ASLauncher::Fire()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{		
		if (ProjectileClass)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//For FPS mostly 

			

			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);

			
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			//ActorSpawnParams.Instigator = MyOwner;

			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<ASProjectile>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);


			if (MuzzleEffect)
			{
				UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
			}

		}

		
	}

}