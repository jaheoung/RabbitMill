// Fill out your copyright notice in the Description page of Project Settings.


#include "MapToolWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include <fstream>
#include <string>
#include <vector>

#include "../../../../../Source/RabbitMill/MyPlayerController.h"
#include "../../../../../Source/RabbitMill/InGame/Utils/MeshActor.h"
#include "RabbitMill/MyGameInstance.h"
#include "RabbitMill/InGame/Utils/ToolConnector.h"

void UMapToolWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bAlwaysReregisterWithWindowsMenu = true;
	
	mapSizeInputText->SetText(FText::AsNumber(mapWidth));
	mapGapInputText->SetText(FText::AsNumber(mapCheckGap));
	pathGridRangeText->SetText(FText::AsNumber(pathGridRange));

	mapSizeInputText->OnTextChanged.AddDynamic(this, &UMapToolWidget::ChangeMapSize);
	mapGapInputText->OnTextChanged.AddDynamic(this, &UMapToolWidget::ChangeMapCheckGap);
	
	imageSaveButton->OnClicked.AddDynamic(this, &UMapToolWidget::ImageSaveClick);
	heightMapSaveButton->OnClicked.AddDynamic(this, &UMapToolWidget::HeightMapSaveClick);
	pathGridSaveButton->OnClicked.AddDynamic(this, &UMapToolWidget::PathGridSaveClick);
	pathGridLoadButton->OnClicked.AddDynamic(this, &UMapToolWidget::PathGridLoadClick);
	pathGridRangeButton->OnClicked.AddDynamic(this, &UMapToolWidget::PathGridRangeClick);

	SetMapToolOnOff(true);
}

void UMapToolWidget::ImageSaveClick()
{
	FHitResult hitResult;
	FVector startPoint = FVector(0, 0, 100000);
	FVector downVec = FVector::DownVector * 150000;

	std::ofstream imgFile;
	FString path = FPaths::ProjectDir();
	path.Append(TEXT("map.pgm"));
	imgFile.open(*path);

	// 기록할 최대 높이값.
	float maxHeight = 5500;
	float rate = 255 / maxHeight;
	int count = mapWidth / mapCheckGap;
	
	std::string str;
	str.append("P2 ");
	str.append(std::to_string(count));
	str.append(" ");
	str.append(std::to_string(count));
	str.append(" 255\n");
	imgFile.write(str.data(), str.size());
	str.clear();
	
	for (int x = mapWidth - 1; x > -1; x -= mapCheckGap) // top 뷰에서 바라봤을때 x 가 위쪽 y 가 오른쪽으로 전개된다.
		{
		startPoint.X = x;
		for (int y = 0; y < mapWidth; y += mapCheckGap)
		{
			startPoint.Y = y;	
			if (GEditor->PlayWorld->LineTraceSingleByChannel(hitResult, startPoint, startPoint + downVec, ECollisionChannel::ECC_Visibility,
				FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam))
			{
				str.append(std::to_string((int)(hitResult.Location.Z * rate)));
				str.append(" ");
			}
			else
			{
				str.append("0 ");
			}
		}
		str.append("\n");
		imgFile.write(str.data(), str.size());
		str.clear();
		}

	imgFile.close();

	GEditor->OnModalMessageDialog(EAppMsgType::Ok, FText::FromString(TEXT("저장되었습니다.")), FText::FromString(TEXT("알림")));
}

void UMapToolWidget::HeightMapSaveClick()
{
	if ((mapWidth % mapCheckGap) > 0)
	{
		// 마지막 인자 알림 텍스트를 실패 텍스트로 바꾸면 빌드 실패한다 -_-;;
		GEditor->OnModalMessageDialog(EAppMsgType::Ok, FText::FromString(TEXT("맵 크기를 간격으로 나눴을때 나머지가 없어야 합니다.")), FText::FromString(TEXT("알림")));
		
		return;
	}
		
	FHitResult hitResult;
	FVector startPoint = FVector(0, 0, 100000);
	FVector downVec = FVector::DownVector * 150000;
	
	FString hpath = FPaths::ProjectContentDir();
	hpath.Append(TEXT("CustomFiles/heightMap.bin"));
	std::ofstream heightFile(*hpath, std::ios::out | std::ios::binary);

	heightFile.write((char*)&mapWidth, sizeof(int));
		
	TArray<float> mdata;
	startPoint.X = startPoint.Y = 0;
	for (int x = 0; x < mapWidth; x += mapCheckGap) // top뷰 기준 위쪽 방향.
	{
		startPoint.X = x;
		for (int y = 0; y < mapWidth; y += mapCheckGap) // top뷰 기준 오른쪽 방향.
		{
			startPoint.Y = y;	
			if (GEditor->PlayWorld->LineTraceSingleByChannel(hitResult, startPoint, startPoint + downVec, ECollisionChannel::ECC_Visibility,
				FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam))
			{
				mdata.Emplace(hitResult.Location.Z);
			}
			else
			{
				mdata.Emplace(0);
			}
		}
	}

	heightFile.write((char*)(mdata.GetData()), sizeof(float) *mdata.Num());

	UE_LOG(LogTemp, Warning, TEXT("data count : %d,   write count : %d"), mdata.Num(), (sizeof(float) * mdata.Num())); 
		
	heightFile.close();

	GEditor->OnModalMessageDialog(EAppMsgType::Ok, FText::FromString(TEXT("저장되었습니다.")), FText::FromString(TEXT("알림")));
}

void UMapToolWidget::PathGridSaveClick()
{
	UMyGameInstance* gameInstance = Cast<UMyGameInstance>(GEditor->PlayWorld->GetGameInstance());
	
	AMeshActor* toolMesh = gameInstance->toolConnector->meshActor;
	
	if (toolMesh == nullptr)
		return;;
	
	FString path = FPaths::ProjectContentDir();
	path.Append(TEXT("CustomFiles/pathGrid.bin"));
	std::ofstream pathGridFile(*path, std::ios::out | std::ios::binary);
	
	// 인게임에서 index 를 읽어 x,y 변환 후 x ~ x+spacing, y ~ y+spacing 을 한 블럭으로 보면 될듯.
	pathGridFile.write((char*)&(toolMesh->spacing), sizeof(int32));
	
	pathGridFile.write((char*)toolMesh->xysByIndex.GetData(), sizeof(bool) * toolMesh->xysByIndex.Num());
	
	pathGridFile.close();

	GEditor->OnModalMessageDialog(EAppMsgType::Ok, FText::FromString(TEXT("저장되었습니다.")), FText::FromString(TEXT("알림")));
}

void UMapToolWidget::PathGridLoadClick()
{
	UMyGameInstance* gameInstance = Cast<UMyGameInstance>(GEditor->PlayWorld->GetGameInstance());

	gameInstance->toolConnector->InitToolMeshActor(true);
	
    AMeshActor* toolMesh = gameInstance->toolConnector->meshActor;
    
    if (toolMesh == nullptr)
    	return;;
    
    FString path = FPaths::ProjectContentDir();
    path.Append(TEXT("CustomFiles/pathGrid.bin"));
    std::ifstream pathGridFile(*path, std::ios::in | std::ios::binary);

	if (pathGridFile.is_open())
	{
		int32 spacing = 0;
		pathGridFile.read((char*)&spacing, sizeof(int32));

		gameInstance->toolConnector->toolMapGap = spacing;
	
		pathGridFile.seekg(0, pathGridFile.end);
		size_t fileSize = pathGridFile.tellg();
		fileSize -= sizeof(int32);
		pathGridFile.seekg(sizeof(int32));
	
		size_t arrSize = fileSize / sizeof(bool);
	
		bool* buf = new bool[arrSize];
	
		pathGridFile.read((char *) buf, fileSize);
	
		toolMesh->xysByIndex.Empty();
		toolMesh->xysByIndex.Append(buf, arrSize);

		delete[] buf;

		pathGridFile.close();

		UE_LOG(LogTemp, Warning, TEXT("size ==== %d"), toolMesh->xysByIndex.Num());

		int w = FMath::Sqrt(arrSize);
		toolMesh->ConvertIndexToXys(w * spacing, spacing);
		toolMesh->CreateCusXYPlane(spacing);

		GEditor->OnModalMessageDialog(EAppMsgType::Ok, FText::FromString(TEXT("로드 완료.")), FText::FromString(TEXT("알림")));
	}
}



void UMapToolWidget::ChangeMapSize(const FText& txt)
{
	int&& getInt = std::stoi(*txt.ToString());
	mapWidth = getInt;
}

void UMapToolWidget::ChangeMapCheckGap(const FText& txt)
{
	int&& getInt = std::stoi(*txt.ToString());
	mapCheckGap = getInt;
}

void UMapToolWidget::PathGridRangeClick()
{
	UMyGameInstance* gameInstance = Cast<UMyGameInstance>(GEditor->PlayWorld->GetGameInstance());
	gameInstance->toolConnector->toolSelectRange = std::stoi(*(pathGridRangeText->GetText().ToString()));
}


void UMapToolWidget::SetMapToolOnOff(bool&& isOn)
{
	FString path = FPaths::ProjectDir();
	path.Append(TEXT("toolSetting.bin"));
	std::ofstream heightFile(*path, std::ios::out | std::ios::binary);
	
	heightFile.write((char*)&isOn, sizeof(bool)); // 첫번째는 맵툴용으로 사용.

	heightFile.close();


	// std::string changeProp = "bUseMouseForTouch";
	//
	// FString configPath = FPaths::ProjectConfigDir();
	// configPath.Append(TEXT("DefaultInput.ini"));
	// std::ifstream readInputSettingFile(*configPath, std::ios::in);
	//
	// std::vector<std::string> strs;  
	// if (readInputSettingFile.is_open())
	// {
	// 	std::string str;
	// 	while (!readInputSettingFile.eof())
	// 	{
	// 		std::getline(readInputSettingFile, str);
	//
	// 		std::string::size_type pos = str.find(changeProp); 
	// 		if (pos != std::string::npos)
	// 		    str.replace(pos, str.length(), changeProp.append(isOn ? "=False\n" : "=True\n"));
	// 		else
	// 			str.push_back('\n');
	//
	// 		strs.push_back(str);
	// 	}
	// 	readInputSettingFile.close();
	// }
	//
	// std::ofstream writeInputSettingFile(*configPath, std::ios::out);
	//
	// if (writeInputSettingFile.is_open())
	// {
	// 	for (std::vector<std::string>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	// 	{
	// 		writeInputSettingFile.write((char*)((*iter).c_str()), (*iter).size());
	// 	}
	// 	
	// 	writeInputSettingFile.close();
	// }
}

void UMapToolWidget::NativeDestruct()
{
	SetMapToolOnOff(false);
}