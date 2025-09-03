// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CWCharacterMovementComponent.generated.h"

/**
 * Custom character movement component that extends the base UCharacterMovementComponent
 * to provide enhanced movement functionality including trajectory prediction,
 * movement state management, and custom movement modes.
 */
UCLASS()
class CWGAMEV3_API UCWCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UCWCharacterMovementComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Movement state management
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	bool IsMoving() const;

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	bool IsSprinting() const;

	// Override the IsCrouching function to return true if the character is crouching
	virtual bool IsCrouching() const override;

	// Enhanced movement functions
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void ToggleCrouch();

	// Trajectory prediction
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	FVector PredictLocation(float TimeAhead) const;

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	TArray<FVector> PredictTrajectory(float Duration, float TimeStep = 0.1f) const;

	// Movement data
	UFUNCTION(BlueprintPure, Category = "Character Movement")
	FVector GetCurrentVelocity() const;

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	float GetCurrentSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	FVector GetMovementDirection() const;

protected:
	// Movement state tracking
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	bool bIsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	bool bIsCrouching;

	// Movement history for trajectory prediction
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	TArray<FVector> MovementHistory;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	TArray<float> TimeHistory;

	// Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	int32 MaxHistorySize = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float CrouchSpeedMultiplier = 0.5f;

public:
	// Internal helper functions - made public for animation instance access
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void UpdateMovementHistory();
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void CleanupOldHistory();
	
protected:
	FVector CalculatePredictedLocation(float TimeAhead, const FVector& CurrentLocation, const FVector& CurrentVelocity) const;
}; 