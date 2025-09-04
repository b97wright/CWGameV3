// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/CWCharacterAnimInstance.h"
#include "Characters/CWBaseCharacter.h"
#include "Characters/CWHeroCharacter.h"
#include "Components/Movement/CWCharacterMovementComponent.h"
#include "Components/Trajectory/CWCharacterTrajectoryComponent.h"

void UCWCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ACWBaseCharacter>(TryGetPawnOwner());
	// Initialize animation-specific variables and references here
	// This function is called when the animation instance is created

    if (OwningCharacter)
    {
        OwningMovementComponent = OwningCharacter->GetCharacterMovement();
        
        // Cast to CWHeroCharacter to get trajectory component
        if (ACWHeroCharacter* HeroCharacter = Cast<ACWHeroCharacter>(OwningCharacter))
        {
            OwningTrajectoryComponent = HeroCharacter->GetTrajectoryComponent();
        }
    }
}

void UCWCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaTime)
{

    if(!OwningCharacter || !OwningMovementComponent) return;

    // Cast to our custom movement component to access the motion matching functions
    UCWCharacterMovementComponent* CustomMovementComponent = Cast<UCWCharacterMovementComponent>(OwningMovementComponent);
    if (!CustomMovementComponent) return;

    // Update movement history and get trajectory data through the movement component
    CustomMovementComponent->UpdateMovementHistory();
    CustomMovementComponent->CleanupOldHistory();
    CustomMovementComponent->PredictTrajectory(1.0f, 0.1f);
    
    // Get trajectory information
    FVector PredictedEndPoint = CustomMovementComponent->PredictLocation(1.0f);
    TArray<FVector> TrajectoryPoints = CustomMovementComponent->PredictTrajectory(1.0f, 0.1f);
    
    // Calculate trajectory distance and time
    float CalculatedTrajectoryDistance = 0.0f;
    if (TrajectoryPoints.Num() > 1)
    {
        for (int32 i = 1; i < TrajectoryPoints.Num(); i++)
        {
            CalculatedTrajectoryDistance += FVector::Dist(TrajectoryPoints[i-1], TrajectoryPoints[i]);
        }
    }
    
    // Get movement data
    Direction = CustomMovementComponent->GetMovementDirection().X;
    Velocity = CustomMovementComponent->GetCurrentVelocity().Size();
    GroundSpeed = CustomMovementComponent->GetCurrentSpeed();
    
    // Check for acceleration (you'll need to implement this in your movement component)
    // For now, we'll use a simple velocity change check
    static FVector PreviousVelocity = FVector::ZeroVector;
    FVector CurrentVelocity = CustomMovementComponent->GetCurrentVelocity();
    bHasAcceleration = !(CurrentVelocity - PreviousVelocity).IsNearlyZero(1.0f);
    PreviousVelocity = CurrentVelocity;

    // Update trajectory data if trajectory component is available
    if (OwningTrajectoryComponent)
    {
        TrajectoryDistance = OwningTrajectoryComponent->GetTrajectoryDistance();
        TrajectoryEndPoint = OwningTrajectoryComponent->GetTrajectoryEndPoint();
        bHasTrajectoryData = TrajectoryDistance > 0.0f;
    }
    else
    {
        TrajectoryDistance = 0.0f;
        TrajectoryEndPoint = FVector::ZeroVector;
        bHasTrajectoryData = false;
    }

	// Update animation variables and logic here
	// This function is called every frame and is thread-safe
	// Use this for updating animation state based on character movement, etc.
}

