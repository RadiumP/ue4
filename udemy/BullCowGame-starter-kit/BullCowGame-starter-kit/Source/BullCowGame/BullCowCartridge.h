// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"


struct FBullCow
{
	int32 Bull = 0;
	int32 Cow = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:

		
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void InitGame();
	void GuessAgain();
	void EndGame();
	void ProcessGuess(const FString& Guess, int32& Counter);
	bool IsIsogram(const FString& Guess) const;
	FBullCow GetBullCows(const FString& Guess) const;

	// Your declarations go below!
	private:
		FString HiddenWord;
		int32 Lives;
		bool bGameOver;
		


};
