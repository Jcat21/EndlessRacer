// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRacerThatEnds.h"
#include "EnemyAIController.h"
#include "PlayerShip.h"
#include "DeathTriggerBox.h"
#include "EndTriggerBox.h"
#include "stdlib.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	state = Start;
}

void AEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	TimeSinceLastFire += deltaTime; // Increment our shot delay variable

	if (Cast<APlayerShip>(GetPawn()) != nullptr)
	{
		float clearanceOffset = 75.0f;

		// Ship Reference Vectors
		FVector DownVec = -GetActorUpVector();
		FVector ForwardVec = GetActorForwardVector();
		FVector RightVec = GetActorRightVector();

		// Ship Reference Points
		FVector ShipPos = Cast<APlayerShip>(GetPawn())->GetCharacterMovement()->GetActorLocation();
		FVector ShipLeftPos = ShipPos - clearanceOffset*RightVec;
		FVector ShipRightPos = ShipPos + clearanceOffset*RightVec;

		// Get various point in front of ship to check for obstacles
		FVector FirstForwardEndPos = ShipPos + 500.0f*ForwardVec;
		FVector SecondForwardEndPos = ShipPos + 1000.0f*ForwardVec;
		FVector ThirdForwardEndPos = ShipPos + 1500.0f*ForwardVec;
		FVector FirstDownEndPos = FirstForwardEndPos + DownVec*1500.0f;
		FVector SecondDownEndPos = SecondForwardEndPos + DownVec*1500.0f;
		FVector ThirdDownEndPos = ThirdForwardEndPos + DownVec*1500.0f;

		// Clearance Endpoints
		FVector LeftClearanceEndPos = ShipLeftPos + ForwardVec*1000.0f; // Swap when speed is increasedd to 1500.0f
		FVector RightClearanceEndPos = ShipRightPos + ForwardVec*1000.0f;
		FVector LeftDownEndPos = ShipLeftPos + DownVec*1500.0f;
		FVector RightDownEndPos = ShipRightPos + DownVec*1500.0f;

		// Perform trace to retrieve hit info
		const FName TraceTag("Normal Trace Tag");
		//GetWorld()->DebugDrawTraceTag = TraceTag; // Draws debug lines for the trace
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(Cast<APlayerShip>(GetPawn()));
		TraceParams.TraceTag = TraceTag;
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;

		APlayerShip* mShip = Cast<APlayerShip>(GetPawn());

		// This fires the ray and checks against all objects w/ collision
		//down hits for holes
		FHitResult FirstDownHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(FirstDownHit, FirstForwardEndPos, FirstDownEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		FHitResult SecondDownHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(SecondDownHit, SecondForwardEndPos, SecondDownEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		FHitResult ThirdDownHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(ThirdDownHit, ThirdForwardEndPos, ThirdDownEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		//forward hits for walls/objects
		FHitResult ForwardHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(ForwardHit, ShipPos, SecondForwardEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		// Do hits for clearance
		FHitResult LeftClearanceHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(LeftClearanceHit, ShipLeftPos, LeftClearanceEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		FHitResult RightClearanceHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(RightClearanceHit, ShipRightPos, RightClearanceEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		//prevent steering into holes on the rigth/left
		FHitResult RightDownHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(RightDownHit, ShipRightPos, RightDownEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		FHitResult LeftDownHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(LeftDownHit, ShipLeftPos, LeftDownEndPos,
			FCollisionObjectQueryParams::AllObjects, TraceParams);

		// Store the values to call for our controller
		float ForwardValue = 0.0f;
		float RightValue = 0.0f;


		// Did this hit anything?
		if (ForwardHit.bBlockingHit || LeftClearanceHit.bBlockingHit || RightClearanceHit.bBlockingHit)
		{
			// Usenormals for determining wall collisions
			FVector normal = ForwardHit.ImpactNormal; normal.Normalize();
			float nX = normal.X; nX = fabs(nX);
			bool middleColl = (ForwardHit.GetActor() && !ForwardHit.GetActor()->IsA<AEndTriggerBox>() && nX >= 0.90f);

			FVector leftN = LeftClearanceHit.ImpactNormal; leftN.Normalize();
			float lnX = leftN.X; lnX = fabs(lnX);
			bool leftColl = (LeftClearanceHit.GetActor() && !LeftClearanceHit.GetActor()->IsA<AEndTriggerBox>() && lnX >= 0.90f);

			FVector rightN = RightClearanceHit.ImpactNormal; rightN.Normalize();
			float rnX = rightN.X; rnX = fabs(rnX);
			float rightColl = (RightClearanceHit.GetActor() && !RightClearanceHit.GetActor()->IsA<AEndTriggerBox>() && rnX >= 0.90f);

			if (middleColl || leftColl || rightColl) {
				if ((rand() % 101) >= 50)
				{
					// Decide to accelerate if threshold away from wall.
					if (ForwardHit.Distance < 200) ForwardValue = 0.0f;
					else ForwardValue = 1.0f;
				}
				else
				{
					ForwardValue = 1.0f;
				}
				RightValue = (rightColl && !middleColl && !leftColl) ? -1.0f : 1.0f;
			}
			else {
				ForwardValue = 1.0f;
			}
		}
		else if (FirstDownHit.bBlockingHit || SecondDownHit.bBlockingHit || ThirdDownHit.bBlockingHit)
		{
			bool FirstHit = FirstDownHit.GetActor() && FirstDownHit.GetActor()->IsA<ADeathTriggerBox>();
			bool SecondHit = SecondDownHit.GetActor() && SecondDownHit.GetActor()->IsA<ADeathTriggerBox>();
			bool ThirdHit = ThirdDownHit.GetActor() && ThirdDownHit.GetActor()->IsA<ADeathTriggerBox>();
			if (FirstHit || SecondHit || ThirdHit) {
				if ((rand() % 101) >= 10)
				{
					// Behave normally large% of the time.
					if (ThirdHit) ForwardValue = 1.0f;
					if (SecondHit) ForwardValue = 1.0f;
					if (FirstHit) ForwardValue = -1.0f;
				}
				else
				{
					// 25% of time charge forward regarless of obstacle.
					ForwardValue = 1.0f;
				}
				RightValue = 1.0f;
			}
			else {
				ForwardValue = 1.0f;
			}
		}
		else
		{
			RightValue = 1.0f;
		}

		// Call the inputs for the ship
		if (RightDownHit.GetActor() && RightDownHit.GetActor()->IsA<ADeathTriggerBox>() && RightValue == 1.0f)
		{
			RightValue = 0.0f;
			ForwardValue = 1.0f;
		}
		if (LeftDownHit.GetActor() && LeftDownHit.GetActor()->IsA<ADeathTriggerBox>() && RightValue == -1.0f)
		{
			RightValue = 0.0f;
			ForwardValue = 1.0f;
		}

		mShip->MoveRight(RightValue);
		mShip->MoveForward(ForwardValue);

		//GetWorld()->DebugDrawTraceTag = TraceTag;
		
		FCollisionQueryParams WeaponTraceParams;
		WeaponTraceParams.AddIgnoredActor(mShip);
		WeaponTraceParams.TraceTag = TraceTag;
		WeaponTraceParams.bTraceAsyncScene = true;
		WeaponTraceParams.bReturnPhysicalMaterial = true;
		
		FHitResult FireHit(ForceInit);
		GetWorld()->LineTraceSingleByObjectType(FireHit, ShipPos, ThirdForwardEndPos,
			FCollisionObjectQueryParams::AllObjects, WeaponTraceParams);

		//fire weapon
		if(FireHit.bBlockingHit && FireHit.GetActor() && FireHit.GetActor()->IsA<APlayerShip>() && TimeSinceLastFire >= 0.3141592653f){
			mShip->FireWeapon();
			TimeSinceLastFire = 0.0f;
		}
	}
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		state = Attack;
		APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	}
}