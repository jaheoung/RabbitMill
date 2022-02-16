// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include <fstream>

#include "InGame/Utils/ToolConnector.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WidgetManager.h"

void UMyGameInstance::Init()
{
	Super::Init();

	// height map 로드.
	ReadHeightMap();
	
	// 툴이 실행중인지 확인.
	ReadToolSetting();

	if (isOnMapTool) // 맵 툴이 실행중이면.
	{
		toolConnector = NewObject<UToolConnector>();
	}
	
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

void UMyGameInstance::ReadToolSetting()
{
	FString path = FPaths::ProjectDir();
	path.Append(TEXT("toolSetting.bin"));
	std::ifstream readFile(*path, std::ios::in | std::ios::binary);
	
	if (readFile.is_open())
	{
		readFile.read((char*)&isOnMapTool, sizeof(bool)); // 첫번째는 맵툴 on off 정보.
		readFile.close();
	}
}

void UMyGameInstance::ReadHeightMap()
{
	heightMapdatas.Empty();

	FString hpath = FPaths::ProjectContentDir();
	hpath.Append(TEXT("CustomFiles/heightMap.bin"));
	std::ifstream readFile(*hpath, std::ios::in | std::ios::binary);

	if (readFile.is_open())
	{
		readFile.read((char*)&mapWidth, sizeof(int));

		readFile.seekg(0, readFile.end);
		size_t fileSize = readFile.tellg();
		fileSize -= sizeof(int);
		readFile.seekg(sizeof(int));
	
		size_t arrSize = fileSize / sizeof(float);
		float* buf = new float[arrSize];

		// float 단위 총 읽을 갯수로 array 를 생성하고
		// file 에서는 실제 사이즈 만큼 read 해야한다.
	
		readFile.read((char *) buf, fileSize);
	
		heightMapdatas.Append(buf, arrSize);

		delete[] buf;

		readFile.close();
	}
}

float UMyGameInstance::GetHeight(float& x, float& y)
{
	if (heightMapdatas.Num() == 0)
		return 0;
	
	if (heightMapWidth == 0)
		heightMapWidth = FMath::Sqrt(heightMapdatas.Num());

	if (x < 0 || x >= mapWidth || y < 0 || y >= mapWidth)
		return 0;

	if (heightMapRate <= 0)
		heightMapRate = heightMapWidth / (float)mapWidth;

	int&&  index = heightMapWidth * (int)(heightMapRate * x) + (int)(heightMapRate * y); // x 가 top view 기준 위쪽이라.. 헷깔림.
	// UE_LOG(LogTemp, Warning, TEXT("w : %d, x : %d, y : %d, rate : %f         getHeight : %f"), heightMapWidth, (int)(heightMapRate * x), (int)(heightMapRate * y), heightMapRate, heightMapdatas[index]);
	
	return heightMapdatas[index]; 
}


