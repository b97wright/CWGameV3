// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CWHeroCharacter.h"
#include "CWDebugHelper.h"

void ACWHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
    Debug::Print(TEXT("Hello World This is Version 3"));
} 