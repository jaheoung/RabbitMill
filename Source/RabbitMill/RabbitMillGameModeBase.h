// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RabbitMillGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API ARabbitMillGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARabbitMillGameModeBase();
	
	virtual void StartPlay() override;
};
