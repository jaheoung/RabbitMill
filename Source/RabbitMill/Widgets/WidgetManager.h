// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetManager.generated.h"

UENUM()
enum class WIDGET_TYPE
{
	MAIN_WIDGET,
	START_WIDGET,
};

/**
 * 
 */
UCLASS()
class RABBITMILL_API UWidgetManager : public UObject
{
	GENERATED_BODY()

public:
	class UMainWidget* mainWidget;
	
	void OpenWidget(WIDGET_TYPE type, UWorld* world);
	
	void CloseWidget(WIDGET_TYPE type);

	void CloseAllWidget();

	void DestroyAllWidget();

	TSubclassOf<class UWidgetBase> GetWidgetClass(WIDGET_TYPE type);
	
	template<typename T>
	T* GetWidget(WIDGET_TYPE type)
	{
		UUserWidget** getWidget = widgetMap.Find(type);

		if (getWidget != nullptr)
		{
			return Cast<T>(*getWidget);
		}
		return nullptr;
	}

private:
	UPROPERTY()
	TMap<WIDGET_TYPE, UWidgetBase*> widgetMap;
};
