// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class ENDLESSRACERTHATENDS_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

	UPROPERTY(EditAnywhere, Category = Damage)
		TArray<TSubclassOf<AActor>> LevelSegments;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
