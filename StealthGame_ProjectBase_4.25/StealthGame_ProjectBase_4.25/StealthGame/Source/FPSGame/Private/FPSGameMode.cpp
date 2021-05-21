// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn)
	{
		//Only works in SP
		//InstigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass)
		{
			TArray<AActor*> ReturnActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnActors);

			if (ReturnActors.Num() > 0)
			{
				auto NewViewTarget = ReturnActors[0];

				//For server only
			/*	APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}*/

				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PC = It->Get();
					if (PC)
					{
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}

			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectingViewpointclass is NULL"));
		}


	}


	AFPSGameState* GS = GetGameState<AFPSGameState>();
	UE_LOG(LogTemp, Warning, TEXT("GS "));
	if (GS)
	{
		UE_LOG(LogTemp, Warning, TEXT("GS herer "));
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}


	OnMissionCompleted(InstigatorPawn, bMissionSuccess);



}

