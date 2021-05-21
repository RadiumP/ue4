// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PlayerController.h"
#include "Engine/World.h "
#include "DrawDebugHelpers.h"


#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber activated."));
	
	FindPhysicsHandle();
	SetupInput();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	

	
	if (!PhysicsHandle)
	{
		return;
	}
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}

}

void UGrabber::Grab()
{



	UE_LOG(LogTemp, Warning, TEXT("Grabbed"));
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* ActorHit = HitResult.GetActor();
	//if hit, attach physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)
		{
			return;
		}
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);
	}
}

void UGrabber::Release()
{

	
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	

	if (!PhysicsHandle)
	{
		return;
	}
	
	PhysicsHandle->ReleaseComponent();
	

	
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PH"));
	}
	
	
}

void UGrabber::SetupInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if (Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() 
{
	


	//DrawDebugLine(GetWorld(), PlayerViewLocation, PlayerReachLocation, FColor::Blue, false, 0.0f, 0, 5.0f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());


	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersLocation(), 
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);



	
	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{

	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
	);

	return PlayerViewLocation + PlayerViewRotation.Vector() * reach;
}

FVector UGrabber::GetPlayersLocation() const
{

	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
	);

	return PlayerViewLocation;
}