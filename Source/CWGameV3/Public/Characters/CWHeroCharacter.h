// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CWDebugHelper.h"
#include "CWBaseCharacter.h"
#include "PoseSearch/PoseSearchTrajectoryTypes.h"
#include "CWHeroCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDataAsset_InputConfig;
class UCWCharacterMovementComponent;
class UCWCharacterTrajectoryComponent;
struct FInputActionValue;

/* ***************************************************** */
/* Our hero character class will need to use hard reference to the DataAsset_InputConfig */
/* because it does not reference expensive assets like blueprints, textures, etc. */
/* We use hard reference because it only references the DataAsset_InputConfig, which is a UDataAsset */
/* and not a UObject. */
/* UDataAsset is a class that is used to store data that is not related to a specific object. */
/* It is a good practice to use hard reference to DataAsset because it is more efficient than soft reference. */
/* Soft reference is a reference to a UObject that is not loaded in memory. */
/* Hard reference is a reference to a UObject that is loaded in memory. */
/* Hard reference is more efficient than soft reference because it does not require the object to be loaded in memory. */
/* ***************************************************** */

UCLASS()
class CWGAMEV3_API ACWHeroCharacter : public ACWBaseCharacter
{
	GENERATED_BODY()

public:

	// Constructor for the hero character class
	ACWHeroCharacter(); // Make a default constructor for the hero character class



protected:
	// Called when the game starts or when spawned
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// Component access functions
	UFUNCTION(BlueprintCallable, Category = "Character Components")
	UCWCharacterMovementComponent* GetCWMovementComponent() const { return Cast<UCWCharacterMovementComponent>(GetCharacterMovement()); }

	UFUNCTION(BlueprintCallable, Category = "Character Components")
	UCWCharacterTrajectoryComponent* GetTrajectoryComponent() const { return TrajectoryComponent; }

	// Movement helper functions
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void ToggleCrouch();

	// Trajectory helper functions
	UFUNCTION(BlueprintCallable, Category = "Character Trajectory")
	void ToggleTrajectoryVisualization();

	UFUNCTION(BlueprintCallable, Category = "Character Trajectory")
	void StartTrajectoryRecording();

	UFUNCTION(BlueprintCallable, Category = "Character Trajectory")
	void StopTrajectoryRecording();

	// Trajectory data access for animation
	UFUNCTION(BlueprintPure, Category = "Character Trajectory")
	float GetTrajectoryDistance() const;

	UFUNCTION(BlueprintPure, Category = "Character Trajectory")
	FVector GetTrajectoryEndPoint() const;

	UFUNCTION(BlueprintPure, Category = "Character Trajectory")
	bool HasTrajectoryData() const;

	UFUNCTION(BlueprintPure, Category = "Character Trajectory")
	TArray<FVector> GetTrajectoryPoints() const;

	// Pose Search compatible trajectory access
	UFUNCTION(BlueprintPure, Category = "Character Trajectory")
	const FPoseSearchQueryTrajectory& GetPoseSearchTrajectory() const;

private:

#pragma region Components

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
USpringArmComponent* CameraBoom;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
UCameraComponent* FollowCamera;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
UCWCharacterMovementComponent* CWMovementComponent;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trajectory", meta = (AllowPrivateAccess = "true"))
UCWCharacterTrajectoryComponent* TrajectoryComponent;

#pragma endregion

#pragma region Trajectory Data

// Cached trajectory data for animation system (Pose Search compatible)
UPROPERTY(BlueprintReadOnly, Category = "Trajectory Data", meta = (AllowPrivateAccess = "true"))
FPoseSearchQueryTrajectory PoseSearchTrajectory;

UPROPERTY(BlueprintReadOnly, Category = "Trajectory Data", meta = (AllowPrivateAccess = "true"))
float CachedTrajectoryDistance;

UPROPERTY(BlueprintReadOnly, Category = "Trajectory Data", meta = (AllowPrivateAccess = "true"))
FVector CachedTrajectoryEndPoint;

UPROPERTY(BlueprintReadOnly, Category = "Trajectory Data", meta = (AllowPrivateAccess = "true"))
bool bHasValidTrajectoryData;

#pragma endregion


#pragma region Input

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
UDataAsset_InputConfig* InputConfigDataAsset;

void Input_Move(const FInputActionValue& InputActionValue);
void Input_Look(const FInputActionValue& InputActionValue);
void Input_Jump(const FInputActionValue& InputActionValue);
void Input_Sprint(const FInputActionValue& InputActionValue);
void Input_Crouch(const FInputActionValue& InputActionValue);
void Input_ToggleTrajectory(const FInputActionValue& InputActionValue);

#pragma endregion

#pragma region Trajectory Management

// Private function to update cached trajectory data
void UpdateTrajectoryData();

#pragma endregion






};