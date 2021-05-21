// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "WordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    InitGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
   
    
    if (bGameOver)
    {     
        ClearScreen();
        InitGame();
    }
    else
    {
        ProcessGuess(Input, Lives);
    }
}

void UBullCowCartridge::InitGame()
{
    int32 random = FMath::RandRange(0, List.Num() - 1);
    HiddenWord = List[random];//Cross platform purpose
    Lives = HiddenWord.Len();
    bGameOver = false;

    

    PrintLine(TEXT("Welcome to BullCowCartridge!"));
    PrintLine(TEXT("Press Tab to access Terminal."));
    PrintLine(FString::Printf(TEXT("Guess the %i letter word."), Lives));
    PrintLine(TEXT("Press Enter to contunue..."));
    
}

void UBullCowCartridge::GuessAgain()
{
    PrintLine(TEXT("Try Again. %i lives left."), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
  
    PrintLine(TEXT("Enter to replay..."));
   
}

void UBullCowCartridge::ProcessGuess(const FString& Guess, int32& Counter)
{
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Have to be isogram."));
        EndGame();
        return;
    }

    if (Guess.Equals(HiddenWord))
    {
        PrintLine(TEXT("W"));
        EndGame();
        return;
    }
    else
    {
        if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("Wrong length. Length: %i .  Try again"), HiddenWord.Len());

        }
        else
        {
            Counter--;

           
            auto Count = GetBullCows(Guess);
            PrintLine(TEXT("Bull: %i, Cow: %i"), Count.Bull, Count.Cow);
            if (Counter > 0)
            {
                GuessAgain();
            }
            else
            {

                PrintLine(TEXT("L"));
                EndGame();
                return;
            }
        }

    }
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const
{
    for (int32 Id = 0; Id < Guess.Len() - 1; Id++)
    {
        for (int32 NextId = Id + 1; NextId < Guess.Len(); NextId++)
        {
            if (Guess[Id] == Guess[NextId])
            {
                return false;
            }
        }
    }
    return true;
}

FBullCow UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    int32 BullCount = 0;
    int32 CowCount = 0;
    
    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            BullCount++;
            continue;
        }
        
        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
               CowCount++;
            }
        }
        
    }
    return FBullCow{ BullCount, CowCount };
}
