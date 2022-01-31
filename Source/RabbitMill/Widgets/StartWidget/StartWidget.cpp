// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RabbitMill/MyGameInstance.h"
#include "RabbitMill/RabbitMillGameModeBase.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	startButton->OnClicked.AddDynamic(this, &UStartWidget::StartButtonClick);
}

void UStartWidget::StartButtonClick()
{
	UMyGameInstance* gi = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (gi != nullptr)
		gi->GoToLevel("InGame");
}

