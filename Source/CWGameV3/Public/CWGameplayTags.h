// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace CWGameplayTags
{
	/** Input Tags **/
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Move);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Look);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Jump);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Sprint);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Crouch);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Interact);

	/** Combat Tags **/
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Attack);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Block);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Parry);
	CWGAMEV3_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combat_Death);

	

	
}
