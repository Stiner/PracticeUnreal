// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMPViewportClient.h"

bool ULocalMPViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (EventArgs.IsGamepad())
	{
		FInputKeyEventArgs newEventArgs = EventArgs;
		newEventArgs.ControllerId += 1;
		return Super::InputKey(newEventArgs);
	}
	else
	{
		return Super::InputKey(EventArgs);
	}
}

bool ULocalMPViewportClient::InputAxis(FViewport* TargetViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (bGamepad)
	{
		return Super::InputAxis(TargetViewport, ControllerId + 1, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
	else
	{
		return Super::InputAxis(TargetViewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
}
