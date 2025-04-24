// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CWHeroCharacter.h"
#include "CWDebugHelper.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACWHeroCharacter::ACWHeroCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = false;

    // Set capsule size
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    // Set controller rotation settings
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create and setup camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Create and setup follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ACWHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
    Debug::Print(TEXT("Hello World This is Version 3"));
} 