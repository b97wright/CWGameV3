// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

namespace Debug
{
    static void Print(const FString& Message, 
                     const FColor& Color = FColor::MakeRandomColor(), 
                     int32 Key = -1)
    {
        // Print to screen
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                Key,                    // Unique key (-1 = always create new message)
                7.0f,                   // Duration (seconds)
                Color,                  // Color
                Message                 // Message
            );
        }

        // Print to output log
        UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    }
} 