// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "PlayerShip.h"
#include "DeathTriggerBox.h"
#include "PlayerHUD.h"



// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mHealth = 100.0f;
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	if (GetCharacterMovement() != nullptr)
	{
		GetCharacterMovement()->GravityScale = 0;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}

	GetCharacterMovement()->MaxFlySpeed = 1500.0f;

	/*for (TObjectIterator<ATargetPoint> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		ATargetPoint *tp = *Itr;
		if (Itr->GetName() == TEXT("FirstSpawnPoint"))
			RespawnPoint = tp;
	}*/

	timer = FTimer();
	lastPressedTime = timer.GetCurrentTime();
	startTime = timer.GetCurrentTime();
}

// Called every frame
void APlayerShip::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	timer.Tick(DeltaTime);


	// Send this ship's position to the GUI so it can calculate place
	APlayerHUD* ph = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	ph->UpdatePlace(this, GetCharacterMovement()->GetActorLocation().X);

	// Update the health on the hud each frame
	if (GetController() == UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		// Notify the hud if we are the player
		ph->UpdateHealth(mHealth);
	}

	// Start from the muzzle's position
	FVector StartPos = GetCharacterMovement()->GetActorLocation();
	// Get forward vector of MyPawn
	FVector Down = -GetActorUpVector();
	// Calculate end position
	FVector EndPos = StartPos + Down*500.0f;

	// Perform trace to retrieve hit info
	const FName TraceTag("Normal Trace Tag");
	//GetWorld()->DebugDrawTraceTag = TraceTag; // Draws debug lines for the trace
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.TraceTag = TraceTag;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	// This fires the ray and checks against all objects w/ collision
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos,
		FCollisionObjectQueryParams::AllObjects, TraceParams);
	// Did this hit anything?
	if (Hit.bBlockingHit)
	{
		// Get angle between current up and new up
		FVector dir = Hit.ImpactNormal; dir.Normalize();
		FVector currUp = GetActorUpVector(); currUp.Normalize();
		float angle = acos(Dot3(dir, currUp));
		FVector cross = FVector::CrossProduct(dir, currUp);
		if (cross.X < 0) angle *= -1;

		// Set the actor's rotation's roll to the new angle
		FRotator currRotation = GetActorRotation();
		FRotator rotation = FRotator(currRotation.Yaw, currRotation.Pitch, angle);
		AddActorLocalRotation(rotation);
	}

	if (Hit.GetActor() && Hit.GetActor()->IsA<ADeathTriggerBox>()) {
		AddMovementInput(-GetActorUpVector(), 1.0f);
	}

	if (!Hit.bBlockingHit || Hit.Distance >= 100.0f) {
		AddMovementInput(-GetActorUpVector(), 1.0f);
	}
}

// Called to bind functionality to input
//void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//	InputComponent->BindAxis("MoveForward", this, &APlayerShip::MoveForward);
//	InputComponent->BindAxis("MoveRight", this, &APlayerShip::MoveRight);
//}

void APlayerShip::MoveForward(float value)
{
	// If the race has ended or hasn't started don't allow input
	if (timer.GetCurrentTime() - startTime <= 3.0f || finishedRace) return;

	if (isRespawning) return;

	if (Controller != nullptr)
	{
		if (!forwardLastFrame)
		{
			lastPressedTime = timer.GetCurrentTime();
		}
		SpeedMultiplier = FMath::Lerp(0.0f, 1.0f, FMath::Clamp(timer.GetCurrentTime() - lastPressedTime, 0.0f, 1.0f));
		if (!(value == -1.0f && GetCharacterMovement()->Velocity.X <= 0.0f))
		{
			//GEngine->AddOnScreenDebugMessage(-1, timer.GetCurrentDeltaTime(), FColor::Green, FString::SanitizeFloat(SpeedMultiplier));
			AddMovementInput(GetActorForwardVector(), value*SpeedMultiplier);
		}


		forwardLastFrame = (value == 1.0f || value == -1.0f);
	}
}

void APlayerShip::MoveRight(float value)
{
	// If the race has ended or hasn't started don't allow input
	if (timer.GetCurrentTime() - startTime <= 3.0f || finishedRace) return;

	if (isRespawning) return;

	if (Controller != nullptr)
	{
		if (value != 0.0f) {
			AddMovementInput(GetActorRightVector(), value);
			strafeLastFrame = true;
		}
		else
		{
			if (strafeLastFrame)
			{
				GetCharacterMovement()->Velocity.Y = 0.0f;
				GetCharacterMovement()->Velocity.Z = 0.0f;
			}
			strafeLastFrame = false;
		}
	}
}

void APlayerShip::FireWeapon()
{
	if (timer.GetCurrentTime() - startTime <= 3.0f || finishedRace) return;
	if (isRespawning) return;
	FVector loc = GetCharacterMovement()->GetActorLocation();
	loc.X += 100.0f;
	AActor* bullet = GetWorld()->SpawnActor<ABullet>(Bullet, loc, GetActorRotation());
	FireAC = PlayWeaponSound(FireLoopSound);
}

UAudioComponent* APlayerShip::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
	return AC;
}

UAudioComponent* APlayerShip::PlayDeathSound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
	return AC;
}

void APlayerShip::ReduceHealth(float damage)
{
	mHealth -= damage;
	if (mHealth <= 0.0001f) {
		mHealth = 0.0f;
		SetIsRespawning(true);
		Respawn();
	}
}

void APlayerShip::Respawn() {
	PlayDeathSound(DeathSound);

	// Get the original position of the ship.
	float x = GetCharacterMovement()->GetActorLocation().X;

	// Using the length of each pipe segment (2700) calculate how many segments we have gone
	x += 2700.0f / 2;
	int segments = x / 2700.0f;

	// Find a new X based on how many segments you have progressed
	float newX = (segments*2700.0f) - (2700.0f / 2) + 10.0f;
	FVector newLocation = FVector(newX, 0.0f, 0.0f);

	// Get a random rotator to randomize the players positioning on the pipe
	FRotator rotation = FRotator(0.0f, 0.0f, FMath::RandRange(0.0f, 360.0f));

	// Set the ship to that new position and velocity to zero.
	SetActorTransform(FTransform(rotation, newLocation));

	// Move the ship down to the edge of the pipe.
	newLocation = newLocation - GetActorUpVector()*1230.0f;
	SetActorTransform(FTransform(rotation, newLocation));

	GetCharacterMovement()->Velocity.Y = 0.0f;
	GetCharacterMovement()->Velocity.Z = 0.0f;
	GetCharacterMovement()->Velocity.X = 0.0f;
	SetIsRespawning(false);
	mHealth = 100.0f;
}