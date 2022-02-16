// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<float> heightMapdatas;
	UPROPERTY()
	int mapWidth = 18900; // 내 맵 기본 사이즈.
	UPROPERTY()
	int heightMapWidth = 0;
	UPROPERTY()
	float heightMapRate = 0.0;
	
	float GetHeight(float& x, float&y);
	void ReadHeightMap();

	bool isOnMapTool = false;
	void ReadToolSetting();

	class UToolConnector* toolConnector;
	
	virtual void Init() override;
	class UWidgetManager& GetWidgetManager();
	void GoToLevel(FName levelName);

	
private:
	UWidgetManager* widgetManager;
};
