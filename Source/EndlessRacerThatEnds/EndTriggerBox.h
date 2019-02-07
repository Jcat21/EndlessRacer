// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PlayerHUD.h"
#include "Engine/TriggerBox.h"
#include "EndTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRACERTHATENDS_API AEndTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AEndTriggerBox();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void PlayerFinished(AActor* MyOverlappedActor, AActor* OtherActor);

	float startTime, elapsedTime;
	FTimer timer;
	TMap<AActor*, float> ActorToTime;
};
