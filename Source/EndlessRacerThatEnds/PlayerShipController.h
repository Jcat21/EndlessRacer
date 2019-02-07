// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerShipController.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRACERTHATENDS_API APlayerShipController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	void MoveForward(float value);
	void MoveRight(float value);
	void Fire();
	void ReturnToMenu();
	void Escape();
};
