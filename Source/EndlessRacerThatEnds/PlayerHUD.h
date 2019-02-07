// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerShip.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRACERTHATENDS_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	void DrawHUD() override;
	

	void SetElapsedTime(float f) { elapsedTime = f; }
	void SetPlayerFinishedTime(float f) { playerFinishedTime = f; }

	void AddResult(FString name) { Results.Add(name); }
	void UpdateHealth(int h) { PlayerHealth = h; }

	void UpdatePlace(APlayerShip* ship, float x);

	UPROPERTY(EditAnywhere, Category = Fonts)
		UFont* HUDFont;

	UPROPERTY(EditAnywhere, Category = Fonts)
		UFont* CountdownFont;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor TimerColor;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor CountdownColor;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor PlaceColor;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor ResultsColor;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor ReturnToMenuColor;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor HealthHighColor;

	UPROPERTY(EditAnywhere, Category = Fonts)
		FColor HealthLowColor;


private:
	void DrawTime();
	void DrawCountdown();
	void DrawResults();
	void DrawPlace();
	void DrawHealth();
	void DrawReturnToMenu();

	float elapsedTime = 0.0f;
	float playerFinishedTime = 0.0f;

	TArray<FString> Results;

	// Map for tracking places
	TMap<APlayerShip*, float> ShipToX;
	int PlayerHealth = 100;
};
