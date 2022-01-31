// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API UMainWidget : public UWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UMainCanvasPanel* mainCanvas;

	void AddChild(UWidget* widget);

	void TopWidget(UWidget* widget);

	void ClearChildren();
};
