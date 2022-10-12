// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LocalMPController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEUNREAL_API ALocalMPController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	void MoveForward_P1(float Axis);
	void MoveForward_P2(float Axis);
};
