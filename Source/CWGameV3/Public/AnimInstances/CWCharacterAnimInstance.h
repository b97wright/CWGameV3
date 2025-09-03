// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CWBaseAnimInstance.h"
#include "CWCharacterAnimInstance.generated.h"

class ACWBaseCharacter; 
class UCharacterMovementComponent;
class UCWCharacterMovementComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float Velocity;

};
