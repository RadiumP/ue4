// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SLauncher.generated.h"

class ASProjectile;

UCLASS()
class COOP_API ASLauncher : public ASWeapon
{
	GENERATED_BODY()
	
protected:

	//UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ASProjectile> ProjectileClass;
};