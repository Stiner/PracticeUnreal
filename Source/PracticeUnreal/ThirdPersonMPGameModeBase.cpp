// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonMPGameModeBase.h"

AThirdPersonMPGameModeBase::AThirdPersonMPGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonMPGameModeBase::OnDeadPlayerCharacter_Implementation()
{
}
