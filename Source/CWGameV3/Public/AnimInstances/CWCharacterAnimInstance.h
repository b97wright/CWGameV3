// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CWBaseAnimInstance.h"
#include "PoseSearch/PoseSearchTrajectoryTypes.h"
#include "CWCharacterAnimInstance.generated.h"

class ACWBaseCharacter; 
class UCharacterMovementComponent;
class UCWCharacterMovementComponent;
class UCWCharacterTrajectoryComponent;

// Using UE5's built-in Pose Search trajectory structures

/**
 * 
 */
UCLASS()
class CWGAMEV3_API UCWCharacterAnimInstance : public UCWBaseAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation();
	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime);

protected:

	UPROPERTY()
    ACWBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY()
	UCWCharacterTrajectoryComponent* OwningTrajectoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float Velocity;

	// Movement state detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsOnGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsFalling;

	// Legacy trajectory arrays (kept for backward compatibility if needed)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	TArray<FVector> TrajectoryPositions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	TArray<FQuat> TrajectoryRotations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	TArray<float> TrajectoryTimes;

	// Additional trajectory data properties (Pose Search compatible)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	float TrajectoryDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	FVector TrajectoryEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	FPoseSearchQueryTrajectory PoseSearchTrajectory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|TrajectoryData")
	bool bHasTrajectoryData;

};