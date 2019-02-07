// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "PlayerShipController.h"
#include "PlayerShip.h"


void APlayerShipController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &APlayerShipController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerShipController::MoveRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerShipController::Fire);
	InputComponent->BindAction("Return", IE_Pressed, this, &APlayerShipController::ReturnToMenu);
	InputComponent->BindAction("Escape", IE_Pressed, this, &APlayerShipController::Escape);

}
void APlayerShipController::MoveForward(float value) {
	APlayerShip* mShip = Cast<APlayerShip>(GetPawn());
	if (mShip != nullptr) {
		mShip->MoveForward(value);
	}
}
void APlayerShipController::MoveRight(float value) {
	APlayerShip* mShip = Cast<APlayerShip>(GetPawn());
	if (mShip != nullptr) {
		mShip->MoveRight(value);
	}
}

void APlayerShipController::Fire() {
	APlayerShip* mShip = Cast<APlayerShip>(GetPawn());
	if (mShip != nullptr) {
		mShip->FireWeapon();
	}
}

void APlayerShipController::ReturnToMenu() {
	APlayerShip* mShip = Cast<APlayerShip>(GetPawn());
	if (mShip != nullptr && mShip->IsFinished()) {
		UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
	}
}

void APlayerShipController::Escape() {
	APlayerShip* mShip = Cast<APlayerShip>(GetPawn());
	if (mShip != nullptr) {
		UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
	}
}