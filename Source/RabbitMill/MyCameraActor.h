// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class RABBITMILL_API AMyCameraActor : public ACameraActor
{
	GENERATED_BODY()
private:
	bool hasChaseTarget = false;
	FVector baseCamPosNormalOffset;
	FVector camPosOffset;
	FVector camRotOffset;
	AActor* chaseTarget;
	float zoomDistance;
	
public:	
	AMyCameraActor();

	/**
	 * @brief  카메라 추적 설정.
	 * @param  unit 추적 대상 엑터.
	 * @param  posOffset 카메라는 추적 대상으로부터 이 방향으로 zoomDistance 만큼 떨어져 위치한다.
	 * @param  rotOffset 추적 대상의 위치에 이 값을 더해줘 보는 위치를 변경할 수 있다.
	 * @param  _zoomDistance 추적 대상과 카메라의 거리.
	 */
	void SetChaseTarget(AActor* unit, FVector posOffset, FVector rotOffset, float _zoomDistance);
	/**
	 * @brief  카메라 고정 보기.
	 * @param  targetPos 바라볼 위치.
	 * @param  posOffset 카메라는 바라볼 위치로부터 이 방향으로 zoomDistance 만큼 떨어져 위치한다.
	 * @param  rotOffset 추적 대상의 위치에 이 값을 더해줘 보는 위치를 변경할 수 있다.
	 * @param  _zoomDistance 추적 대상과 카메라의 거리.
	 */
	void SetFixTarget(FVector targetPos, FVector posOffset, FVector rotOffset, float _zoomDistance);
	void SetZoom(float plusZoom);
	void UpdateZoom();
	void SetPosOffset(const bool& isUp, const float& amount);
	void Update(FVector targetPos);

	void SetProjectionMode(ECameraProjectionMode::Type mode);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
