// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorConfig.h"
#include "MySmartObjectBehaviorDefinition.generated.h"

UCLASS()
class PRACTICEUNREAL_API UMySmartObjectBehaviorDefinition : public UGameplayBehaviorSmartObjectBehaviorDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = SmartObject)
    int32 TestValue;
};
