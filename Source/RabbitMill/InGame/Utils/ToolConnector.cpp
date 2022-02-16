// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolConnector.h"

#include "MeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "RabbitMill/MyGameInstance.h"

void UToolConnector::Init(UWorld* _world, UMyGameInstance* _gameInstance)
{
	world = _world;
	gameInstance = _gameInstance;
}


void UToolConnector::RightClickAxis(float amount)
{
	if (amount <= 0.0)
		return;

	InitToolMeshActor(false);

	APlayerController* playerController = world->GetFirstPlayerController();
	
	FHitResult outHit;
	// 두번째 인자인 bTraceComplex 는 "복잡한 충돌에 대해 추적해야 하는지 여부" 이렇게 주석 내용이 있음.
	if (playerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, outHit))
	{
		// outHit 정보에 Location, Normal, Actor... 있음.
		if (outHit.bBlockingHit)
		{
			FVector pos = outHit.Location;
			SetToolMesh(pos.X, pos.Y, false, toolSelectRange);
		}
	}
}


void UToolConnector::LeftClickAxis(float amount)
{
	if (amount <= 0.0)
		return;

	InitToolMeshActor(false);

	APlayerController* playerController = world->GetFirstPlayerController();
	
	FHitResult outHit;
	// 두번째 인자인 bTraceComplex 는 "복잡한 충돌에 대해 추적해야 하는지 여부" 이렇게 주석 내용이 있음.
	if (playerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, outHit))
	{
		// outHit 정보에 Location, Normal, Actor... 있음.
		if (outHit.bBlockingHit)
		{
			FVector pos = outHit.Location;
			SetToolMesh(pos.X, pos.Y, true, toolSelectRange);
		}
	}
}

void UToolConnector::SetToolMesh(const float& _x, const float& _y, bool isSelect, const int& range)
{
	if (range > 0)
	{
		float tempX = _x;
		float tempY = _y;
		float rangeGap = range * toolMapGap;

		for (int i = tempX + rangeGap, k = tempX - rangeGap; i >= k; i -= toolMapGap)
		{
			for (int q = tempY - rangeGap, w = tempY + rangeGap; q <= w; q += toolMapGap)
			{
				SetToolMesh(i, q, isSelect, 0);
			}
		}
	}

	int x = _x / toolMapGap; // 기본은 한칸이 300.
	int y = _y / toolMapGap;

	if (x < 0 || x >= compressionMapWidth || y < 0 || y >= compressionMapWidth)
		return;
	
	int index = x * compressionMapWidth + y;

	// 변경된 경우에만.
	if ((isSelect && !meshActor->xysByIndex[index]) ||
		(!isSelect && meshActor->xysByIndex[index])) // xysByIndex 는 toolMapGap 만큼 압축됨. 
	{
		// 그리기 위한 좌표 변환.
		x = index / compressionMapWidth;
		y = index % compressionMapWidth;
		x = x * toolMapGap;
		y = y * toolMapGap;

		if (isSelect)
		{
			meshActor->xys.Emplace(index, pf::Vec2i(x, y));
			meshActor->xysByIndex[index] = true;
		}
		else
		{
			meshActor->xys.Remove(index);
			meshActor->xysByIndex[index] = false;
		}
		meshActor->CreateCusXYPlane(toolMapGap);
	}
}

void UToolConnector::InitToolMeshActor(bool needMeshEmpty)
{
	if (meshActor == nullptr) // 초기화.
	{
		compressionMapWidth = (int)(gameInstance->mapWidth / (float)toolMapGap); // 한칸이 300*300 사이즈를 갖는다.
			
		meshActor = world->SpawnActor<AMeshActor>();
		meshActor->SetActorLocation(FVector(0, 0, 2000));

		if (needMeshEmpty)
			meshActor->xysByIndex.Empty();
		else
			meshActor->xysByIndex.Init(false, compressionMapWidth * compressionMapWidth);
	}
}