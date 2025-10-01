// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Trajectory/CWCharacterTrajectoryComponent.h"
#include "Components/Movement/CWCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CWDebugHelper.h"

UCWCharacterTrajectoryComponent::UCWCharacterTrajectoryComponent()
{
	// Set default values
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bShowTrajectory = false;
	TrajectoryDuration = 2.0f;
	TrajectoryTimeStep = 0.1f;
	TrajectoryColor = FLinearColor::Red;
	TrajectoryThickness = 2.0f;
	bIsRecording = false;
	bShowDebugInfo = false;
	bShowTrajectoryEndPoint = true;
	bShowTrajectoryDistance = true;
}

void UCWCharacterTrajectoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the movement component from the owner
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		MovementComponent = Cast<UCWCharacterMovementComponent>(Character->GetCharacterMovement());
		if (!MovementComponent)
		{
			// If our custom movement component isn't found, try to get the default one
			MovementComponent = Cast<UCWCharacterMovementComponent>(Character->GetCharacterMovement());
			Debug::Print(TEXT("Warning: Using default CharacterMovementComponent instead of CWCharacterMovementComponent"));
		}
	}

	// Initialize trajectory arrays
	CurrentTrajectoryPoints.Empty();
	RecordedTrajectoryPoints.Empty();

	Debug::Print(TEXT("CWCharacterTrajectoryComponent initialized"));
}

void UCWCharacterTrajectoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Always update trajectory data (needed for animation system)
	if (MovementComponent)
	{
		UpdateTrajectory();
	}

	// Draw trajectory if showing
	if (bShowTrajectory)
	{
		DrawTrajectory();
	}

	// Draw debug info if enabled
	if (bShowDebugInfo)
	{
		DrawDebugInfo();
	}

	// Record trajectory if recording
	if (bIsRecording && GetOwner())
	{
		RecordedTrajectoryPoints.Add(GetOwner()->GetActorLocation());
	}
}

void UCWCharacterTrajectoryComponent::ShowTrajectory(bool bShow)
{
	bShowTrajectory = bShow;
	
	if (!bShow)
	{
		// Clear any existing debug lines
		CurrentTrajectoryPoints.Empty();
	}
}

void UCWCharacterTrajectoryComponent::SetTrajectoryDuration(float Duration)
{
	TrajectoryDuration = FMath::Max(0.1f, Duration);
}

void UCWCharacterTrajectoryComponent::SetTrajectoryTimeStep(float TimeStep)
{
	TrajectoryTimeStep = FMath::Max(0.01f, TimeStep);
}

void UCWCharacterTrajectoryComponent::SetTrajectoryColor(FLinearColor Color)
{
	TrajectoryColor = Color;
}

float UCWCharacterTrajectoryComponent::GetTrajectoryDistance() const
{
	return CalculateTrajectoryDistance(CurrentTrajectoryPoints);
}

float UCWCharacterTrajectoryComponent::GetTrajectoryTime() const
{
	return TrajectoryDuration;
}

FVector UCWCharacterTrajectoryComponent::GetTrajectoryEndPoint() const
{
	if (CurrentTrajectoryPoints.Num() > 0)
	{
		return CurrentTrajectoryPoints.Last();
	}
	return FVector::ZeroVector;
}

TArray<FVector> UCWCharacterTrajectoryComponent::GetTrajectoryPoints() const
{
	return CurrentTrajectoryPoints;
}

bool UCWCharacterTrajectoryComponent::WillHitTarget(const FVector& TargetLocation, float Tolerance) const
{
	if (CurrentTrajectoryPoints.Num() < 2)
	{
		return false;
	}

	// Check if any point in the trajectory is close to the target
	for (const FVector& Point : CurrentTrajectoryPoints)
	{
		if (FVector::Dist(Point, TargetLocation) <= Tolerance)
		{
			return true;
		}
	}

	return false;
}

float UCWCharacterTrajectoryComponent::GetTimeToTarget(const FVector& TargetLocation) const
{
	if (CurrentTrajectoryPoints.Num() < 2 || !MovementComponent)
	{
		return -1.0f; // Invalid
	}

	float CurrentSpeed = MovementComponent->GetCurrentSpeed();
	if (CurrentSpeed <= 0.0f)
	{
		return -1.0f; // Not moving
	}

	// Find the closest point on trajectory to target
	FVector ClosestPoint = GetClosestPointOnTrajectory(TargetLocation);
	float DistanceToTarget = FVector::Dist(ClosestPoint, TargetLocation);
	
	// Simple time calculation based on current speed
	return DistanceToTarget / CurrentSpeed;
}

FVector UCWCharacterTrajectoryComponent::GetClosestPointOnTrajectory(const FVector& Point) const
{
	if (CurrentTrajectoryPoints.Num() < 2)
	{
		return Point;
	}

	FVector ClosestPoint = CurrentTrajectoryPoints[0];
	float ClosestDistance = FVector::Dist(Point, ClosestPoint);

	// Check each line segment in the trajectory
	for (int32 i = 0; i < CurrentTrajectoryPoints.Num() - 1; ++i)
	{
		FVector LineStart = CurrentTrajectoryPoints[i];
		FVector LineEnd = CurrentTrajectoryPoints[i + 1];
		
		FVector ClosestPointOnLine = FindClosestPointOnLine(Point, LineStart, LineEnd);
		float Distance = FVector::Dist(Point, ClosestPointOnLine);
		
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestPoint = ClosestPointOnLine;
		}
	}

	return ClosestPoint;
}

void UCWCharacterTrajectoryComponent::StartRecording()
{
	bIsRecording = true;
	RecordedTrajectoryPoints.Empty();
	Debug::Print(TEXT("Trajectory recording started"));
}

void UCWCharacterTrajectoryComponent::StopRecording()
{
	bIsRecording = false;
	Debug::Print(TEXT("Trajectory recording stopped"));
}

void UCWCharacterTrajectoryComponent::ClearRecordedTrajectory()
{
	RecordedTrajectoryPoints.Empty();
	Debug::Print(TEXT("Recorded trajectory cleared"));
}

TArray<FVector> UCWCharacterTrajectoryComponent::GetRecordedTrajectory() const
{
	return RecordedTrajectoryPoints;
}

void UCWCharacterTrajectoryComponent::UpdateTrajectory()
{
	if (!MovementComponent)
	{
		return;
	}

	// Get predicted trajectory from movement component
	CurrentTrajectoryPoints = MovementComponent->PredictTrajectory(TrajectoryDuration, TrajectoryTimeStep);
}

void UCWCharacterTrajectoryComponent::DrawTrajectory()
{
	if (CurrentTrajectoryPoints.Num() < 2)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Draw trajectory lines
	for (int32 i = 0; i < CurrentTrajectoryPoints.Num() - 1; ++i)
	{
		FVector Start = CurrentTrajectoryPoints[i];
		FVector End = CurrentTrajectoryPoints[i + 1];
		
		DrawDebugLine(World, Start, End, TrajectoryColor.ToFColor(true), false, -1.0f, 0, TrajectoryThickness);
	}

	// Draw trajectory end point if enabled
	if (bShowTrajectoryEndPoint)
	{
		FVector EndPoint = CurrentTrajectoryPoints.Last();
		DrawDebugSphere(World, EndPoint, 20.0f, 8, TrajectoryColor.ToFColor(true), false, -1.0f, 0, 2.0f);
	}
}

void UCWCharacterTrajectoryComponent::DrawDebugInfo()
{
	if (!GetOwner() || !MovementComponent)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector2D ScreenLocation;
	
	// Draw trajectory distance if enabled
	if (bShowTrajectoryDistance)
	{
		float Distance = GetTrajectoryDistance();
		FString DistanceText = FString::Printf(TEXT("Trajectory Distance: %.2f"), Distance);
		
		if (UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), OwnerLocation + FVector(0, 0, 100), ScreenLocation))
		{
			DrawDebugString(World, OwnerLocation + FVector(0, 0, 100), DistanceText, nullptr, FColor::White, 0.0f);
		}
	}

	// Draw current speed
	float Speed = MovementComponent->GetCurrentSpeed();
	FString SpeedText = FString::Printf(TEXT("Speed: %.2f"), Speed);
	DrawDebugString(World, OwnerLocation + FVector(0, 0, 80), SpeedText, nullptr, FColor::Yellow, 0.0f);
}

float UCWCharacterTrajectoryComponent::CalculateTrajectoryDistance(const TArray<FVector>& Points) const
{
	if (Points.Num() < 2)
	{
		return 0.0f;
	}

	float TotalDistance = 0.0f;
	for (int32 i = 0; i < Points.Num() - 1; ++i)
	{
		TotalDistance += FVector::Dist(Points[i], Points[i + 1]);
	}

	return TotalDistance;
}

FVector UCWCharacterTrajectoryComponent::FindClosestPointOnLine(const FVector& Point, const FVector& LineStart, const FVector& LineEnd) const
{
	FVector LineDirection = (LineEnd - LineStart).GetSafeNormal();
	FVector PointToStart = Point - LineStart;
	
	float ProjectionLength = FVector::DotProduct(PointToStart, LineDirection);
	
	// Clamp to line segment
	ProjectionLength = FMath::Clamp(ProjectionLength, 0.0f, FVector::Dist(LineStart, LineEnd));
	
	return LineStart + (LineDirection * ProjectionLength);
} 