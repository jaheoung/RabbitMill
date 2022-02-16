// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraActor.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCameraActor::AMyCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyCameraActor::SetChaseTarget(AActor* unit, FVector posOffset, FVector rotOffset, float _zoomDistance)
{
	if (unit == nullptr)
		return;

	baseCamPosNormalOffset = posOffset.GetSafeNormal();
	camRotOffset = rotOffset;
	hasChaseTarget = true;
	chaseTarget = unit;
	zoomDistance = _zoomDistance;

	UpdateZoom();
}

void AMyCameraActor::SetFixTarget(FVector targetPos, FVector posOffset, FVector rotOffset, float _zoomDistance)
{
	baseCamPosNormalOffset = posOffset.GetSafeNormal();
	camRotOffset = rotOffset;
	hasChaseTarget = true;
	zoomDistance = _zoomDistance;

	UpdateZoom();
	// 직접 갱신.
	Update(targetPos);
}

void AMyCameraActor::SetProjectionMode(ECameraProjectionMode::Type mode)
{
	UCameraComponent* getComp = GetCameraComponent();
	check(getComp != nullptr);
	getComp->SetProjectionMode(mode);
}



void AMyCameraActor::SetPosOffset(const bool& isUp, const float& amount)
{
	if (isUp)
		baseCamPosNormalOffset += GetActorUpVector() * amount;
	else
		baseCamPosNormalOffset += GetActorRightVector() * amount;
	
	baseCamPosNormalOffset = baseCamPosNormalOffset.GetSafeNormal();
	UpdateZoom();
}


void AMyCameraActor::SetZoom(float plusZoom)
{
	zoomDistance += plusZoom;
}

void AMyCameraActor::UpdateZoom()
{
	camPosOffset = baseCamPosNormalOffset * zoomDistance;
}

// Called when the game starts or when spawned
void AMyCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hasChaseTarget == false)
		return;

	if (chaseTarget == nullptr)
		return;
	
	Update(chaseTarget->GetActorLocation());
}

void AMyCameraActor::Update(FVector targetPos)
{
	FVector resultPos = targetPos + camPosOffset;
	SetActorLocation(resultPos);
	
	targetPos += camRotOffset;
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(resultPos, targetPos));
}

