// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CWDebugHelper.h"
#include "CWBaseCharacter.h"
#include "CWHeroCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDataAsset_InputConfig;
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

private:

#pragma region Components

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
USpringArmComponent* CameraBoom;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
UCameraComponent* FollowCamera;

#pragma endregion


#pragma region Input

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
UDataAsset_InputConfig* InputConfigDataAsset;

void Input_Move(const FInputActionValue& InputActionValue);
void Input_Look(const FInputActionValue& InputActionValue);
void Input_Jump(const FInputActionValue& InputActionValue);


#pragma endregion






};