// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Movement/CWCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CWDebugHelper.h"

UCWCharacterMovementComponent::UCWCharacterMovementComponent()
{
	// Initialize default values
	bIsSprinting = false;
	bIsCrouching = false;
	MaxHistorySize = 50;
	SprintSpeedMultiplier = 1.5f;
	CrouchSpeedMultiplier = 0.5f;

	// Set default movement properties
	MaxWalkSpeed = 600.0f;
	MaxAcceleration = 2048.0f;
	BrakingDecelerationWalking = 2048.0f;
	GroundFriction = 8.0f;
	JumpZVelocity = 600.0f;
	AirControl = 0.2f;
	MinAnalogWalkSpeed = 20.0f;
	BrakingDecelerationFlying = 0.0f;
	AirControlBoostMultiplier = 2.0f;
	AirControlBoostVelocityThreshold = 0.0f;
}

void UCWCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize movement history
	MovementHistory.Empty();
	TimeHistory.Empty();
	
	Debug::Print(TEXT("CWCharacterMovementComponent initialized"));
}

void UCWCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update movement history for trajectory prediction
	UpdateMovementHistory();
}

void UCWCharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	Super::SetMovementMode(NewMovementMode, NewCustomMode);
	
	// Update movement state based on new mode
	switch (NewMovementMode)
	{
	case MOVE_Walking:
		// Reset sprint and crouch when walking
		if (bIsSprinting)
		{
			StopSprint();
		}
		break;
	case MOVE_Falling:
		// Stop sprinting when falling
		if (bIsSprinting)
		{
			StopSprint();
		}
		break;
	}
}

bool UCWCharacterMovementComponent::IsMoving() const
{
	return !Velocity.IsNearlyZero(1.0f);
}

bool UCWCharacterMovementComponent::IsSprinting() const
{
	return bIsSprinting;
}

bool UCWCharacterMovementComponent::IsCrouching() const
{
	return bIsCrouching;
}

void UCWCharacterMovementComponent::StartSprint()
{
	if (!bIsSprinting && IsMoving() && MovementMode == MOVE_Walking)
	{
		bIsSprinting = true;
		MaxWalkSpeed *= SprintSpeedMultiplier;
		Debug::Print(TEXT("Sprint started"));
	}
}

void UCWCharacterMovementComponent::StopSprint()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		MaxWalkSpeed /= SprintSpeedMultiplier;
		Debug::Print(TEXT("Sprint stopped"));
	}
}

void UCWCharacterMovementComponent::ToggleCrouch()
{
	if (MovementMode == MOVE_Walking)
	{
		if (bIsCrouching)
		{
			// Stand up
			bIsCrouching = false;
			MaxWalkSpeed /= CrouchSpeedMultiplier;
			Debug::Print(TEXT("Standing up"));
		}
		else
		{
			// Crouch down
			bIsCrouching = true;
			MaxWalkSpeed *= CrouchSpeedMultiplier;
			Debug::Print(TEXT("Crouching"));
		}
	}
}

FVector UCWCharacterMovementComponent::PredictLocation(float TimeAhead) const
{
	if (MovementHistory.Num() < 2)
	{
		// Not enough history, use current velocity
		return GetOwner()->GetActorLocation() + (Velocity * TimeAhead);
	}

	// Use the last known location and velocity for prediction
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	return CalculatePredictedLocation(TimeAhead, CurrentLocation, Velocity);
}

TArray<FVector> UCWCharacterMovementComponent::PredictTrajectory(float Duration, float TimeStep) const
{
	TArray<FVector> TrajectoryPoints;
	
	if (MovementHistory.Num() < 2)
	{
		// Simple linear prediction if no history
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		for (float Time = 0.0f; Time <= Duration; Time += TimeStep)
		{
			FVector PredictedPoint = CurrentLocation + (Velocity * Time);
			TrajectoryPoints.Add(PredictedPoint);
		}
		return TrajectoryPoints;
	}

	// Use movement history for more accurate prediction
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	for (float Time = 0.0f; Time <= Duration; Time += TimeStep)
	{
		FVector PredictedPoint = CalculatePredictedLocation(Time, CurrentLocation, Velocity);
		TrajectoryPoints.Add(PredictedPoint);
	}

	return TrajectoryPoints;
}

FVector UCWCharacterMovementComponent::GetCurrentVelocity() const
{
	return Velocity;
}

float UCWCharacterMovementComponent::GetCurrentSpeed() const
{
	return Velocity.Size();
}

FVector UCWCharacterMovementComponent::GetMovementDirection() const
{
	if (Velocity.IsNearlyZero())
	{
		return FVector::ZeroVector;
	}
	return Velocity.GetSafeNormal();
}

void UCWCharacterMovementComponent::UpdateMovementHistory()
{
	if (GetOwner())
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		float CurrentTime = GetWorld()->GetTimeSeconds();
		
		MovementHistory.Add(CurrentLocation);
		TimeHistory.Add(CurrentTime);
		
		CleanupOldHistory();
	}
}

void UCWCharacterMovementComponent::CleanupOldHistory()
{
	// Remove old entries if we exceed the maximum history size
	while (MovementHistory.Num() > MaxHistorySize)
	{
		MovementHistory.RemoveAt(0);
		TimeHistory.RemoveAt(0);
	}
}

FVector UCWCharacterMovementComponent::CalculatePredictedLocation(float TimeAhead, const FVector& CurrentLocation, const FVector& CurrentVelocity) const
{
	// Simple physics-based prediction
	// In a real implementation, you might want to consider:
	// - Gravity effects
	// - Air resistance
	// - Collision detection
	// - More complex movement patterns
	
	FVector PredictedLocation = CurrentLocation + (CurrentVelocity * TimeAhead);
	
	// Apply gravity if falling
	if (MovementMode == MOVE_Falling)
	{
		float GravityZ = GetGravityZ();
		PredictedLocation.Z += 0.5f * GravityZ * TimeAhead * TimeAhead;
	}
	
	return PredictedLocation;
} 