// Fill out your copyright notice in the Description page of Project Settings.


#include "CWGameplayTags.h"

namespace CWGameplayTags
{
    /** Input Tags */
    UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Move, TEXT("Input.Move"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Look, TEXT("Input.Look"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Jump, TEXT("Input.Jump"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Sprint, TEXT("Input.Sprint"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Crouch, TEXT("Input.Crouch"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Interact, TEXT("Input.Interact"));

    /** Combat Tags */
    UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_Attack, TEXT("Combat.Attack"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_Block, TEXT("Combat.Block"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_Parry, TEXT("Combat.Parry"));
    UE_DEFINE_GAMEPLAY_TAG(TAG_Combat_Death, TEXT("Combat.Death"));

    
}
