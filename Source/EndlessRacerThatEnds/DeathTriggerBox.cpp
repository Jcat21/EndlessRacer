// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "DeathTriggerBox.h"
#include "PlayerShip.h"

ADeathTriggerBox::ADeathTriggerBox() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADeathTriggerBox::BeginPlay() {
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ADeathTriggerBox::Respawn);
}

void ADeathTriggerBox::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ADeathTriggerBox::Respawn(AActor* MyOverlappedActor, AActor* OtherActor) {
	APlayerShip* mShip = Cast<APlayerShip>(OtherActor);
	if (mShip && !mShip->GetIsRespawning()) {
		mShip->SetIsRespawning(true);
		//ship = OtherActor;
		respawningShips.Add(OtherActor);
		FTimerHandle RespawnTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ADeathTriggerBox::RespawnTimer, 0.5f);
	}
}

void ADeathTriggerBox::RespawnTimer() {
	if (respawningShips.Num() != 0) {
		APlayerShip* ship = Cast<APlayerShip>(respawningShips.Pop());
		ship->Respawn();
	}
}