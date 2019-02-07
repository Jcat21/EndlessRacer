// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "DeathTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRACERTHATENDS_API ADeathTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ADeathTriggerBox();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void Respawn(AActor* MyOverlappedActor, AActor* OtherActor);

	void RespawnTimer();

	TArray<AActor*> respawningShips;
	AActor* ship;
};
