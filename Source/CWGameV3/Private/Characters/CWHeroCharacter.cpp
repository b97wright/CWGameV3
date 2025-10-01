// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CWHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Movement/CWCharacterMovementComponent.h"
#include "Components/Trajectory/CWCharacterTrajectoryComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/CWInputComponent.h"
#include "CWGameplayTags.h"
#include "CWDebugHelper.h"



ACWHeroCharacter::ACWHeroCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set capsule size
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    // Set controller rotation settings
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // DON'T create a custom movement component - just use the default one
    // CWMovementComponent = CreateDefaultSubobject<UCWCharacterMovementComponent>(TEXT("CharacterMovement"));
    
    // Configure the DEFAULT character movement
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
    
    // Create and setup trajectory component
    TrajectoryComponent = CreateDefaultSubobject<UCWCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));
    
    // Initialize trajectory data
    CachedTrajectoryDistance = 0.0f;
    CachedTrajectoryEndPoint = FVector::ZeroVector;
    bHasValidTrajectoryData = false;
    PoseSearchTrajectory.Samples.Empty();
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
    CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Sprint, ETriggerEvent::Triggered, this, &ThisClass::Input_Sprint);
    CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);
    CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_ToggleTrajectory, ETriggerEvent::Triggered, this, &ThisClass::Input_ToggleTrajectory);
    //CWInputComponent->BindNativeInputAction(InputConfigDataAsset, CWGameplayTags::TAG_Input_Interact, ETriggerEvent::Triggered, this, &ThisClass::Interact);
} 

void ACWHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
    Debug::Print(TEXT("Hello World This is Version 3"));
}

void ACWHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update trajectory data every frame
    UpdateTrajectoryData();
}

void ACWHeroCharacter::Input_Move(const FInputActionValue &InputActionValue)
{
    if (!GetCWMovementComponent())
    {
        Debug::Print(TEXT("ERROR: Movement component is null!"));
        return;
    }

    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
    Debug::Print(FString::Printf(TEXT("Input received: X=%.2f, Y=%.2f"), MovementVector.X, MovementVector.Y));
    
    // Debug movement component
    if (GetCharacterMovement())
    {
        Debug::Print(FString::Printf(TEXT("Movement Component: %s, MaxSpeed: %.2f"), 
            *GetCharacterMovement()->GetClass()->GetName(), 
            GetCharacterMovement()->MaxWalkSpeed));
    }
    else
    {
        Debug::Print(TEXT("ERROR: No movement component found!"));
    }

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

void ACWHeroCharacter::Input_Sprint(const FInputActionValue &InputActionValue)
{
    if (GetCWMovementComponent())
    {
        if (InputActionValue.Get<bool>())
        {
            GetCWMovementComponent()->StartSprint();
        }
        else
        {
            GetCWMovementComponent()->StopSprint();
        }
    }
}

void ACWHeroCharacter::Input_Crouch(const FInputActionValue &InputActionValue)
{
    if (GetCWMovementComponent() && InputActionValue.Get<bool>())
    {
        GetCWMovementComponent()->ToggleCrouch();
    }
}

void ACWHeroCharacter::Input_ToggleTrajectory(const FInputActionValue &InputActionValue)
{
    if (TrajectoryComponent && InputActionValue.Get<bool>())
    {
        static bool bTrajectoryVisible = false;
        bTrajectoryVisible = !bTrajectoryVisible;
        TrajectoryComponent->ShowTrajectory(bTrajectoryVisible);
        
        if (bTrajectoryVisible)
        {
            Debug::Print(TEXT("Trajectory visualization enabled"));
        }
        else
        {
            Debug::Print(TEXT("Trajectory visualization disabled"));
        }
    }
}

void ACWHeroCharacter::StartSprint()
{
    if (GetCWMovementComponent())
    {
        GetCWMovementComponent()->StartSprint();
    }
}

void ACWHeroCharacter::StopSprint()
{
    if (GetCWMovementComponent())
    {
        GetCWMovementComponent()->StopSprint();
    }
}

void ACWHeroCharacter::ToggleCrouch()
{
    if (GetCWMovementComponent())
    {
        GetCWMovementComponent()->ToggleCrouch();
    }
}

void ACWHeroCharacter::ToggleTrajectoryVisualization()
{
    if (TrajectoryComponent)
    {
        static bool bTrajectoryVisible = false;
        bTrajectoryVisible = !bTrajectoryVisible;
        TrajectoryComponent->ShowTrajectory(bTrajectoryVisible);
        
        if (bTrajectoryVisible)
        {
            Debug::Print(TEXT("Trajectory visualization enabled"));
        }
        else
        {
            Debug::Print(TEXT("Trajectory visualization disabled"));
        }
    }
}

void ACWHeroCharacter::StartTrajectoryRecording()
{
    if (TrajectoryComponent)
    {
        TrajectoryComponent->StartRecording();
        Debug::Print(TEXT("Trajectory recording started"));
    }
}

void ACWHeroCharacter::StopTrajectoryRecording()
{
    if (TrajectoryComponent)
    {
        TrajectoryComponent->StopRecording();
        Debug::Print(TEXT("Trajectory recording stopped"));
    }
}

void ACWHeroCharacter::UpdateTrajectoryData()
{
    if (TrajectoryComponent)
    {
        // Update cached trajectory data from the trajectory component
        CachedTrajectoryDistance = TrajectoryComponent->GetTrajectoryDistance();
        CachedTrajectoryEndPoint = TrajectoryComponent->GetTrajectoryEndPoint();
        TArray<FVector> TrajectoryPoints = TrajectoryComponent->GetTrajectoryPoints();
        
        // Convert to Pose Search trajectory format
        PoseSearchTrajectory.Samples.Empty();
        
        for (int32 i = 0; i < TrajectoryPoints.Num(); i++)
        {
            FPoseSearchQueryTrajectorySample Sample;
            Sample.Position = TrajectoryPoints[i];
            Sample.Facing = FQuat::Identity; // Default facing direction (can be improved later)
            Sample.AccumulatedSeconds = i * 0.1f; // Time step of 0.1 seconds
            
            PoseSearchTrajectory.Samples.Add(Sample);
        }
        
        // Determine if we have valid trajectory data
        bHasValidTrajectoryData = (CachedTrajectoryDistance > 0.0f) && (TrajectoryPoints.Num() > 0);
    }
    else
    {
        // Reset data if no trajectory component
        CachedTrajectoryDistance = 0.0f;
        CachedTrajectoryEndPoint = FVector::ZeroVector;
        PoseSearchTrajectory.Samples.Empty();
        bHasValidTrajectoryData = false;
    }
}

float ACWHeroCharacter::GetTrajectoryDistance() const
{
    return CachedTrajectoryDistance;
}

FVector ACWHeroCharacter::GetTrajectoryEndPoint() const
{
    return CachedTrajectoryEndPoint;
}

bool ACWHeroCharacter::HasTrajectoryData() const
{
    return bHasValidTrajectoryData;
}

TArray<FVector> ACWHeroCharacter::GetTrajectoryPoints() const
{
    TArray<FVector> Points;
    for (const FPoseSearchQueryTrajectorySample& Sample : PoseSearchTrajectory.Samples)
    {
        Points.Add(Sample.Position);
    }
    return Points;
}

const FPoseSearchQueryTrajectory& ACWHeroCharacter::GetPoseSearchTrajectory() const
{
    return PoseSearchTrajectory;
}

UCharacterMovementComponent* ACWHeroCharacter::GetCharacterMovement() const
{
	return GetCWMovementComponent();
}

UFUNCTION(BlueprintCallable, Category = "Character Components")
UCWCharacterMovementComponent* ACWHeroCharacter::GetCWMovementComponent() const { return Cast<UCWCharacterMovementComponent>(GetCharacterMovement()); }
