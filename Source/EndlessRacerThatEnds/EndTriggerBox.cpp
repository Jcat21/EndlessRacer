// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "EndTriggerBox.h"
#include "PlayerShip.h"


AEndTriggerBox::AEndTriggerBox() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	OnActorBeginOverlap.AddDynamic(this, &AEndTriggerBox::PlayerFinished);
}

void AEndTriggerBox::BeginPlay() {
	Super::BeginPlay();
	timer = FTimer();
	startTime = timer.GetCurrentTime();
}

void AEndTriggerBox::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	timer.Tick(DeltaTime);

	elapsedTime = timer.GetCurrentTime() - startTime;

	// Update the hud time
	APlayerHUD* ph = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (ph) ph->SetElapsedTime(elapsedTime);
}

void AEndTriggerBox::PlayerFinished(AActor* MyOverlappedActor, AActor* OtherActor) {
	if (!ActorToTime.Contains(OtherActor)) {
		ActorToTime.Add(OtherActor, elapsedTime);

		// Check if the other is a player
		APlayerShip * OtherShip = Cast<APlayerShip>(OtherActor);
		if (OtherShip)
		{
			// Finish the race for that player and add them to the leaderboard
			OtherShip->FinishRace();
			APlayerHUD* ph = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			if (ph) {
				FString finishTime = FString::SanitizeFloat(elapsedTime-3.0f);
				finishTime = finishTime.Mid(0, finishTime.Find(".")+4);
				ph->AddResult(TEXT("(") + finishTime + TEXT(") ") + OtherShip->ShipName);
			}

			// Set the HUD to have the player finished time if it was the player.
			if (OtherShip->GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
				if (ph) ph->SetPlayerFinishedTime(elapsedTime-3.0f);
			}
		}
	}
}
