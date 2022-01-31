// Copyright Epic Games, Inc. All Rights Reserved.


#include "RabbitMillGameModeBase.h"
#include <assert.h>

#include "MyGameInstance.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "RabbitMill/Widgets/MainWidget.h"
#include "Widgets/WidgetManager.h"

ARabbitMillGameModeBase::ARabbitMillGameModeBase()
{
}

void ARabbitMillGameModeBase::StartPlay()
{
	Super::StartPlay();

	UMyGameInstance* gi = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (gi == nullptr)
		return;

	// 시작 위젯 생성.
	gi->GetWidgetManager().OpenWidget(WIDGET_TYPE::START_WIDGET, GetWorld());
}

