// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CWDebugHelper.h"
#include "CWBaseCharacter.h"
#include "CWHeroCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class CWGAMEV3_API ACWHeroCharacter : public ACWBaseCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

#pragma region Components

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
USpringArmComponent* CameraBoom;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
UCameraComponent* FollowCamera;

#pragma endregion

};