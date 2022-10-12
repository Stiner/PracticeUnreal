// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMPCharacter.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ALocalMPCharacter::ALocalMPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALocalMPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALocalMPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALocalMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

