// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "MapToolWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAPTOOL_API UMapToolWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	// 맵 크기 (정사각형)
	int mapWidth = 18900;
	// 이 간격으로 맵 크기만큼 읽어서 처리.
	int mapCheckGap = 10;

	int pathGridRange = 1;
	
	UPROPERTY(meta=(BindWidget))
	class UEditableText* mapSizeInputText;
	UPROPERTY(meta=(BindWidget))
	class UEditableText* mapGapInputText;
	UPROPERTY(meta=(BindWidget))
	class UButton* imageSaveButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* heightMapSaveButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* pathGridSaveButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* pathGridLoadButton;
	UPROPERTY(meta=(BindWidget))
	class UEditableText* pathGridRangeText;
	UPROPERTY(meta=(BindWidget))
	class UButton* pathGridRangeButton;
	

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ImageSaveClick();
	UFUNCTION()
	void HeightMapSaveClick();
	UFUNCTION()
	void PathGridSaveClick();
	UFUNCTION()
	void PathGridLoadClick();
	UFUNCTION()
	void ChangeMapSize(const FText& txt);
	UFUNCTION()
	void ChangeMapCheckGap(const FText& txt);
	UFUNCTION()
	void PathGridRangeClick();

	void SetMapToolOnOff(bool&& isOn);

	virtual void NativeDestruct() override;
};
