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

    // Use the default movement component (which is now properly configured)
    // Calculate movement data from the default movement component
    FVector CurrentVelocity = OwningMovementComponent->Velocity;
    Velocity = CurrentVelocity.Size();
    GroundSpeed = CurrentVelocity.Size();
    
    // Calculate direction properly for turning animations
    if (!CurrentVelocity.IsNearlyZero())
    {
        FVector ForwardVector = OwningCharacter->GetActorForwardVector();
        FVector RightVector = OwningCharacter->GetActorRightVector();
        
        // Calculate direction relative to character's facing direction
        float ForwardDot = FVector::DotProduct(CurrentVelocity.GetSafeNormal(), ForwardVector);
        float RightDot = FVector::DotProduct(CurrentVelocity.GetSafeNormal(), RightVector);
        
        // Use atan2 for proper direction calculation (-1 to 1)
        Direction = FMath::Atan2(RightDot, ForwardDot) / PI;
    }
    else
    {
        Direction = 0.0f;
    }
    
    bIsMoving = !CurrentVelocity.IsNearlyZero(10.0f);
    bIsOnGround = OwningMovementComponent->IsMovingOnGround();
    bIsSprinting = false; // You can implement sprint detection if needed
    bIsCrouching = OwningMovementComponent->IsCrouching();
    bIsFalling = OwningMovementComponent->IsFalling();
    
    // Check for acceleration
    static FVector PreviousVelocity = FVector::ZeroVector;
    bHasAcceleration = !(CurrentVelocity - PreviousVelocity).IsNearlyZero(5.0f);
    PreviousVelocity = CurrentVelocity;


    // Get trajectory data from the character (which handles all the complex calculations)
    if (ACWHeroCharacter* HeroCharacter = Cast<ACWHeroCharacter>(OwningCharacter))
    {
        TrajectoryDistance = HeroCharacter->GetTrajectoryDistance();
        TrajectoryEndPoint = HeroCharacter->GetTrajectoryEndPoint();
        bHasTrajectoryData = HeroCharacter->HasTrajectoryData();
        
        // Get the Pose Search compatible trajectory directly
        PoseSearchTrajectory = HeroCharacter->GetPoseSearchTrajectory();
        
        // Also populate legacy arrays for backward compatibility
        TrajectoryPositions.Empty();
        TrajectoryRotations.Empty();
        TrajectoryTimes.Empty();
        
        for (const FPoseSearchQueryTrajectorySample& Sample : PoseSearchTrajectory.Samples)
        {
            TrajectoryPositions.Add(Sample.Position);
            TrajectoryRotations.Add(Sample.Facing);
            TrajectoryTimes.Add(Sample.AccumulatedSeconds);
        }
    }
    else
    {
        // Fallback if not a hero character
        TrajectoryDistance = 0.0f;
        TrajectoryEndPoint = FVector::ZeroVector;
        PoseSearchTrajectory.Samples.Empty();
        TrajectoryPositions.Empty();
        TrajectoryRotations.Empty();
        TrajectoryTimes.Empty();
        bHasTrajectoryData = false;
    }

	// Update animation variables and logic here
	// This function is called every frame and is thread-safe
	// Use this for updating animation state based on character movement, etc.
}