// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMode.h"

#include "MyCameraActor.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void AInGameMode::StartPlay()
{
	Super::StartPlay();

	UWorld* world = GetWorld();

	if (world == nullptr)
		return;
	
	TSharedPtr<FStreamableHandle> handle = UAssetManager::GetStreamableManager().RequestSyncLoad(FString(
			TEXT("Blueprint'/Game/Developers/jaheoung/BP_CameraActor.BP_CameraActor_C'")));

	if (handle.IsValid() == false)
		checkNoEntry();

	mainCamera = Cast<AMyCameraActor>(world->SpawnActor(Cast<UClass>(handle->GetLoadedAsset())));

	if (mainCamera != nullptr)
	{
		mainCamera->SetFixTarget(FVector(0,0,0),  FVector(-1, 0, 1), FVector(0, 0, 100), 700);
		//mainCamera->SetProjectionMode(ECameraProjectionMode::Type::Orthographic);

		APlayerController* playerController = world->GetFirstPlayerController();

		if (playerController != nullptr)
		{
			playerController->SetViewTarget(mainCamera);
			mainCamera->EnableInput(playerController);			
		}
	}
}
