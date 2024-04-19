// Fill out your copyright notice in the Description page of Project Settings.


#include "PUGameModeBase.h"
#include "Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "PUPlayerController.h"

APUGameModeBase::APUGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APUGameModeBase::OnDeadPlayerCharacter(AController* deadPlayer)
{
    APUPlayerController* playerController = Cast<APUPlayerController>(deadPlayer);
    if (IsValid(playerController))
    {
        UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Type::Quit, false);
    }
}

void APUGameModeBase::OnPostLogin(AController* NewPlayer)
{
    const FString msg = FString::Printf(TEXT("Logged in : %s"), *(NewPlayer->GetName()));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, msg);
}
