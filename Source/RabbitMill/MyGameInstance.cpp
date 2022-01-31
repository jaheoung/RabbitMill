// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/WidgetManager.h"

void UMyGameInstance::Init()
{
	Super::Init();

	widgetManager = NewObject<UWidgetManager>();
}

void UMyGameInstance::GoToLevel(FName levelName)
{
	if (widgetManager != nullptr)
		widgetManager->DestroyAllWidget();

	UGameplayStatics::OpenLevel(this, levelName);
}


UWidgetManager& UMyGameInstance::GetWidgetManager()
{
	check(widgetManager != nullptr);
	
	return *widgetManager;
}


