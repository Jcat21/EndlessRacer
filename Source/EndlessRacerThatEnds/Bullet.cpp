// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "Bullet.h"
#include "PlayerShip.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ABullet::KillBullet);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(3000.0f, 0.0f, 0.0f)*DeltaTime);

	
}

void ABullet::KillBullet(AActor* MyOverlappedActor, AActor* OtherActor)
{
	APlayerShip* ship = Cast<APlayerShip>(OtherActor);
	if (ship)
	{
		ship->ReduceHealth(25.0f);				//4 bullets to kill
	}
	this->Destroy();
	//GetMesh()->Deactivate();
}