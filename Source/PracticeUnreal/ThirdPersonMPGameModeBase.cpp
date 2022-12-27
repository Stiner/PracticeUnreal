// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonMPGameModeBase.h"
#include "Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ThirdPersonMPPlayerController.h"

AThirdPersonMPGameModeBase::AThirdPersonMPGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonMPGameModeBase::OnDeadPlayerCharacter(AController* deadPlayer)
{
    AThirdPersonMPPlayerController* playerController = Cast<AThirdPersonMPPlayerController>(deadPlayer);
    if (IsValid(playerController))
    {
        UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Type::Quit, false);
    }
}

void AThirdPersonMPGameModeBase::OnPostLogin(AController* NewPlayer)
{
    const FString msg =  FString::Printf(TEXT("Logged in : %s"), *(NewPlayer->GetName()));
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, msg);
}
