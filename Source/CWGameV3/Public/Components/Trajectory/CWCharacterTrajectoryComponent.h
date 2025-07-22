// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "CWCharacterTrajectoryComponent.generated.h"

class UCWCharacterMovementComponent;
class UStaticMeshComponent;

/**
 * Component responsible for visualizing and managing character trajectory
 * Provides trajectory prediction, visualization, and analysis capabilities
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CWGAMEV3_API UCWCharacterTrajectoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCWCharacterTrajectoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Trajectory visualization
	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void ShowTrajectory(bool bShow);

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void SetTrajectoryDuration(float Duration);

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void SetTrajectoryTimeStep(float TimeStep);

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void SetTrajectoryColor(FLinearColor Color);

	// Trajectory analysis
	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	float GetTrajectoryDistance() const;

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	float GetTrajectoryTime() const;

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	FVector GetTrajectoryEndPoint() const;

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	TArray<FVector> GetTrajectoryPoints() const;

	// Trajectory prediction
	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	bool WillHitTarget(const FVector& TargetLocation, float Tolerance = 50.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	float GetTimeToTarget(const FVector& TargetLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	FVector GetClosestPointOnTrajectory(const FVector& Point) const;

	// Trajectory recording
	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void StopRecording();

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	void ClearRecordedTrajectory();

	UFUNCTION(BlueprintCallable, Category = "Trajectory")
	TArray<FVector> GetRecordedTrajectory() const;

protected:
	// Component references
	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	UCWCharacterMovementComponent* MovementComponent;

	// Trajectory visualization settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	bool bShowTrajectory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	float TrajectoryDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	float TrajectoryTimeStep = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	FLinearColor TrajectoryColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	float TrajectoryThickness = 2.0f;

	// Trajectory data
	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	TArray<FVector> CurrentTrajectoryPoints;

	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	TArray<FVector> RecordedTrajectoryPoints;

	UPROPERTY(BlueprintReadOnly, Category = "Trajectory")
	bool bIsRecording = false;

	// Debug visualization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Debug")
	bool bShowDebugInfo = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Debug")
	bool bShowTrajectoryEndPoint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Debug")
	bool bShowTrajectoryDistance = true;

private:
	// Internal helper functions
	void UpdateTrajectory();
	void DrawTrajectory();
	void DrawDebugInfo();
	float CalculateTrajectoryDistance(const TArray<FVector>& Points) const;
	FVector FindClosestPointOnLine(const FVector& Point, const FVector& LineStart, const FVector& LineEnd) const;
}; 