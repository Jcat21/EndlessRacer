// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "LevelGenerator.h"


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	// Keep track of location to spawn segments in
	FVector loc = FVector(0.0f, 0.0f, 0.0f);

	// First segment is plain.
	int lastIndex = -1;
	// Loop through next 19 segments and randomly select them.
	for (int i = 1; i < 20; i++) {
		// Select a chunk
		int segmentIndex = -1;
		while (segmentIndex == -1) {
			segmentIndex = rand() % LevelSegments.Num();
			if (segmentIndex == lastIndex) segmentIndex = -1;
			else lastIndex = segmentIndex;
		}
		// Increment the x of the location
		loc.X += 2700.0f;

		int numRotations = FMath::RandRange(0, 40);

		FRotator rotation = FRotator(0.0f, 0.0f, 360.0f/40.0f*numRotations);
		AddActorLocalRotation(rotation);
		GetWorld()->SpawnActor<AActor>(LevelSegments[segmentIndex], loc, rotation);
	}
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

