// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "MainWidget.h"
#include "WidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"


TSubclassOf<UWidgetBase> UWidgetManager::GetWidgetClass(WIDGET_TYPE type)
{
	FSoftObjectPath path;

	switch (type)
	{
	case WIDGET_TYPE::MAIN_WIDGET:
		path = FString(
			TEXT("WidgetBlueprint'/Game/Developers/jaheoung/Widgets/BP_MainWidget.BP_MainWidget_C'"));
		break;
	case WIDGET_TYPE::START_WIDGET:
		path = FString(
			TEXT("WidgetBlueprint'/Game/Developers/jaheoung/Widgets/StartWidget/BP_StartWidget.BP_StartWidget_C'"));
		break;
	default:
		checkNoEntry();
		break;
	}

	TSharedPtr<FStreamableHandle> handle = UAssetManager::GetStreamableManager().RequestSyncLoad(path);

	if (handle.IsValid() == false)
		checkNoEntry();

	return Cast<UClass>(handle->GetLoadedAsset());
}


void UWidgetManager::OpenWidget(WIDGET_TYPE type, UWorld* world)
{
	if (world == nullptr)
		return;
	
	if (mainWidget == nullptr)
	{
		UUserWidget* newWidget = CreateWidget(world, GetWidgetClass(WIDGET_TYPE::MAIN_WIDGET));

		ensureMsgf(newWidget != nullptr, TEXT("위젯 주소가 잘못됨."));

		mainWidget = Cast<UMainWidget>(newWidget);

		check(mainWidget != nullptr);

		mainWidget->AddToViewport();
		mainWidget->Opened();
	}

	UWidgetBase** getWidget = widgetMap.Find(type);

	if (getWidget == nullptr)
	{
		UUserWidget* newWidget = CreateWidget(world, GetWidgetClass(type));

		ensureMsgf(newWidget != nullptr, TEXT("위젯 주소가 잘못됨."));

		UWidgetBase* getBaseWidget = Cast<UWidgetBase>(newWidget);

		if (getBaseWidget != nullptr)
		{
			getBaseWidget->widgetType = type;
			getBaseWidget->Open();
		}

		widgetMap.Emplace(type, getBaseWidget);
		mainWidget->AddChild(newWidget);
	}
	else
	{
		(*getWidget)->SetVisibility(ESlateVisibility::Visible);

		UWidgetBase* getBaseWidget = Cast<UWidgetBase>(*getWidget);

		if (getBaseWidget != nullptr)
		{
			getBaseWidget->Open();
			mainWidget->TopWidget(getBaseWidget);
		}
	}
}

void UWidgetManager::CloseWidget(WIDGET_TYPE type)
{
	UWidgetBase** getWidget = widgetMap.Find(type);

	if (getWidget != nullptr && (*getWidget) != nullptr)
	{
		(*getWidget)->SetVisibility(ESlateVisibility::Hidden);

		if ((*getWidget)->Opened())
			(*getWidget)->Close();
	}
}

void UWidgetManager::CloseAllWidget()
{
	for (auto elem : widgetMap)
	{
		UWidgetBase* getWidget = elem.Value;

		if (getWidget == nullptr || getWidget->Opened() == false)
			continue;

		getWidget->SetVisibility(ESlateVisibility::Hidden);
		getWidget->Close();
	}
}

void UWidgetManager::DestroyAllWidget()
{
	for (auto elem : widgetMap)
	{
		UWidgetBase* getWidget = elem.Value;

		if (getWidget == nullptr)
			continue;

		getWidget->ConditionalBeginDestroy();
	}

	widgetMap.Empty();
	mainWidget->ClearChildren();
	mainWidget->ConditionalBeginDestroy();
	mainWidget = nullptr;
}
