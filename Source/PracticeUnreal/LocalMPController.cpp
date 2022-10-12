// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMPController.h"

void ALocalMPController::SetupInputComponent()
{
	Super::SetupInputComponent();

	int32 id = GetLocalPlayer()->GetControllerId();
	if (id == 0)
	{
		InputComponent->BindAxis("MoveForward_P1", this, &ALocalMPController::MoveForward_P1);
	}
	else if (id == 1)
	{
		InputComponent->BindAxis("MoveForward_P2", this, &ALocalMPController::MoveForward_P2);
	}

	UE_LOG(LogTemp, Log, TEXT("%s ControllerID : %d"), *GetName(), id);
}

void ALocalMPController::MoveForward_P1(float Axis)
{
	FString msg = GetName().Append(FString::Printf(TEXT(" Axis P1 : % f"), Axis));
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Magenta, msg);
}

void ALocalMPController::MoveForward_P2(float Axis)
{
	FString msg = GetName().Append(FString::Printf(TEXT(" Axis P2 : % f"), Axis));
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Magenta, msg);
}
