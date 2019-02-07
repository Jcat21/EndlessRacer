// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "PlayerHUD.h"

void APlayerHUD::BeginPlay() {
	Super::BeginPlay();
}

void APlayerHUD::DrawHUD() {
	Super::DrawHUD();
	
	if (Canvas) {
		// Draw the countdown or the timers
		if (elapsedTime < 3.0f) DrawCountdown();
		else DrawTime();

		DrawResults();
		
		// Draw different elemnts depending on if race is still happening.
		if (playerFinishedTime == 0.0f) {
			DrawPlace();
			DrawHealth();
		}
		else DrawReturnToMenu();
	}
}

void APlayerHUD::DrawTime() {
	if (playerFinishedTime != 0.0f) return;
	float ScreenX = Canvas->SizeX / 2.0f;
	float ScreenY = 60.0f;

	// Center this text
	float xOffset, yOffset;
	FString currentString = FString::SanitizeFloat(elapsedTime - 3.0f);
	currentString = currentString.Mid(0, currentString.Find(".") + 4);
	GetTextSize(currentString, xOffset, yOffset, CountdownFont, 0.5f);
	xOffset /= 2; yOffset /= 2;

	DrawText(currentString, TimerColor, ScreenX - xOffset, ScreenY - yOffset, CountdownFont, 0.5f);
}

void APlayerHUD::DrawCountdown() {
	float ScreenX = Canvas->SizeX / 2.0f;
	float ScreenY = Canvas->SizeY * 1.0f / 4.0f;
	
	// Center this text
	int countdown = 4.0f - elapsedTime;
	float xOffset, yOffset;
	FString currentString = FString::FromInt(countdown);
	GetTextSize(currentString, xOffset, yOffset, CountdownFont);
	xOffset /= 2; yOffset /= 2;

	DrawText(currentString, CountdownColor, ScreenX-xOffset, ScreenY-yOffset, CountdownFont);
}

void APlayerHUD::DrawResults()
{
	float ScreenX = Canvas->SizeX / 20.0f;
	float ScreenY = Canvas->SizeY / 20.0f;
	
	// Draw the header for the results
	if (Results.Num() > 0)
		DrawText(TEXT("RESULTS:"), ResultsColor, ScreenX, ScreenY, HUDFont);
	ScreenY += 40;
	// Draw the order in which ships finished
	for (int i = 0; i < Results.Num(); ++i)
	{
		FString name = Results[i];
		
		DrawText(FString::FromInt(i + 1) + TEXT(": ") + name, ResultsColor, ScreenX, ScreenY + (i) * 30, HUDFont, 0.66578f);
	}
}

void APlayerHUD::DrawPlace() {
	// Find the player ship in the map and get its x position
	float playerX = 0.0f;
	for (auto it : ShipToX) {
		if (it.Key->GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
			playerX = it.Value;
		}
	}

	// Now increment the place for each value farther than the player
	int place = 1;
	for (auto it : ShipToX) {
		if (it.Value > playerX) ++place;
	}

	// Now draw it to the screen finally
	float ScreenX = Canvas->SizeX / 8.0f;
	float ScreenY = Canvas->SizeY * 4.0f / 5.0f;

	// Center this text
	float xOffset, yOffset;
	FString currentString = FString::FromInt(place);
	GetTextSize(currentString, xOffset, yOffset, CountdownFont);
	xOffset /= 2; yOffset /= 2;
	DrawText(currentString, PlaceColor, ScreenX - xOffset, ScreenY - yOffset, CountdownFont);
}

void APlayerHUD::DrawHealth() {
	float ScreenX = Canvas->SizeX * 6.5f / 8.0f;
	float ScreenY = Canvas->SizeY * 4.0f / 5.0f;

	float xOffset, yOffset;
	FString currentString = "Health: " + FString::FromInt(PlayerHealth) + "/100";
	GetTextSize(currentString, xOffset, yOffset, HUDFont);
	xOffset /= 2; yOffset /= 2;
	DrawText(currentString, (PlayerHealth > 50) ? HealthHighColor : HealthLowColor, ScreenX - xOffset, ScreenY - yOffset, HUDFont);
}

void APlayerHUD::UpdatePlace(APlayerShip* ship, float x) {
	ShipToX.FindOrAdd(ship);
	ShipToX[ship] = x;
}

void APlayerHUD::DrawReturnToMenu() {
	float ScreenX = Canvas->SizeX / 2.0f;
	float ScreenY = Canvas->SizeY / 2.0f - 50.0f;

	// Center this text
	float xOffset, yOffset;
	FString currentString = TEXT("Thank You For Playing!");
	GetTextSize(currentString, xOffset, yOffset, HUDFont);
	xOffset /= 2; yOffset /= 2;
	DrawText(currentString, ReturnToMenuColor, ScreenX - xOffset, ScreenY - yOffset, HUDFont);

	ScreenY += 50.0f;
	currentString = TEXT("Press ENTER to");
	GetTextSize(currentString, xOffset, yOffset, HUDFont);
	xOffset /= 2; yOffset /= 2;
	DrawText(currentString, ReturnToMenuColor, ScreenX - xOffset, ScreenY - yOffset, HUDFont);

	ScreenY += 50.0f;
	currentString = TEXT("Return To Menu");
	GetTextSize(currentString, xOffset, yOffset, HUDFont);
	xOffset /= 2; yOffset /= 2;
	DrawText(currentString, ReturnToMenuColor, ScreenX - xOffset, ScreenY - yOffset, HUDFont);
}