// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CWHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/CWInputComponent.h"
#include "CWGameplayTags.h"
#include "CWDebugHelper.h"



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

void ACWHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    checkf(InputConfigDataAsset, TEXT("InputConfigDataAsset is not set"));

    // Get the local player
    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer(); 
    
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    check(Subsystem);

    Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

    UCWInputComponent* CWInputComponent = CastChecked<UCWInputComponent>(PlayerInputComponent);

    CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
    CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
    CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Jump, ETriggerEvent::Triggered, this, &ThisClass::Input_Jump);
    //CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Sprint, ETriggerEvent::Triggered, this, &ThisClass::Sprint);
    //CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Crouch);
    //CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Interact, ETriggerEvent::Triggered, this, &ThisClass::Interact);
} 

void ACWHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
    Debug::Print(TEXT("Hello World This is Version 3"));
}

void ACWHeroCharacter::Input_Move(const FInputActionValue &InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

    const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

    if(MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if(MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ACWHeroCharacter::Input_Look(const FInputActionValue &InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

    if(LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if(LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ACWHeroCharacter::Input_Jump(const FInputActionValue &InputActionValue)
{
    // Use standard Unreal Engine movement state functions
    if(GetCharacterMovement()->IsFalling())
    {
        // Character is in air - could implement double jump here
        Jump();
    }
    else
    {
        // Character is on ground
        Jump();
    }
}
