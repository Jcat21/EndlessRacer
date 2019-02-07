// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Bullet.h"
#include "GameFramework/Character.h"
#include "PlayerShip.generated.h"


UCLASS()
class ENDLESSRACERTHATENDS_API APlayerShip : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerShip();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);

	void MoveRight(float value);

	void FireWeapon();

	void FinishRace() { finishedRace = true; }
	bool IsFinished() { return finishedRace; }

	void SetIsRespawning(bool b) { isRespawning = b; }
	bool GetIsRespawning() { return isRespawning; }

	void SetHealth(float health) { mHealth = health; }
	float GetHealth() { return mHealth; }

	void ReduceHealth(float damage);

	void Respawn();

	UPROPERTY(EditAnywhere)
		FString ShipName;

	UPROPERTY(VisibleAnywhere)
		float SpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category=Damage)
		TSubclassOf<ABullet> Bullet;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundCue* FireLoopSound;

	UPROPERTY(Transient)
		class UAudioComponent* FireAC;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundCue* DeathSound;

	UPROPERTY(Transient)
		class UAudioComponent* DeathAC;

protected:
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);
	UAudioComponent* PlayDeathSound(USoundCue* Sound);

private:
	float lastPressedTime;
	bool forwardLastFrame = false;
	FTimer timer;

	// Timer for countdown
	float startTime;

	float mHealth;

	bool finishedRace = false;

	bool strafeLastFrame = false;	

	bool isRespawning = false;

};
