// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RabbitMill/Widgets/WidgetBase.h"
#include "StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API UStartWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UButton* startButton;

	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartButtonClick();
};
