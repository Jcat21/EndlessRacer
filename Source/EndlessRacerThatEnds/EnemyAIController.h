// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRACERTHATENDS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	void Tick(float deltaTime) override;
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;
private:
	enum CurrentState
	{
		Start,
		Chase,
		Attack,
		Dead
	};
	CurrentState state;
	float AttackRange = 150.0f;

	float TimeSinceLastFire = 0.0f;
	
};
