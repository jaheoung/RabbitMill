// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API AInGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	class AMyCameraActor* mainCamera;
	
	virtual void StartPlay() override;
};
