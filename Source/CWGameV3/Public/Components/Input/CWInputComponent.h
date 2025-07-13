// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "CWInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class CWGAMEV3_API UCWInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallBackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc CallbackFunc);
	
};

template<class UserObject, typename CallBackFunc>
inline void UCWInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc CallbackFunc)
{
	checkf(InInputConfig->NativeInputActions.Num() > 0, TEXT("No Native Input Actions found in Input Config: %s"), *InInputConfig->GetName());

	if(UInputAction* InputAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(InputAction, TriggerEvent, ContextObject, CallbackFunc);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Native Input Action found for tag: %s in Input Config: %s"), *InInputTag.ToString(), *InInputConfig->GetName());
	}
}
