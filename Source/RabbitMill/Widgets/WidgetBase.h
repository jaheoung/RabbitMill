// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class WIDGET_TYPE widgetType;

	virtual bool Opened();
	virtual void Open();
	virtual void Close();

protected:
	bool isOpened;
};
