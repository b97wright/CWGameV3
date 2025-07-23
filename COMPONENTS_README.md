# Character Movement and Trajectory Components

This document describes the new character movement and trajectory components added to the CWGameV3 project.

## Components Overview

### 1. CWCharacterMovementComponent
A custom character movement component that extends `UCharacterMovementComponent` to provide enhanced movement functionality.

**Features:**
- Enhanced movement state management (sprint, crouch)
- Movement history tracking for trajectory prediction
- Physics-based trajectory prediction
- Movement data analysis

**Key Functions:**
- `StartSprint()` / `StopSprint()` - Control sprinting
- `ToggleCrouch()` - Toggle crouching state
- `PredictLocation(float TimeAhead)` - Predict future location
- `PredictTrajectory(float Duration, float TimeStep)` - Get trajectory points
- `GetCurrentVelocity()` / `GetCurrentSpeed()` - Get movement data

### 2. CWCharacterTrajectoryComponent
A component responsible for visualizing and analyzing character trajectory.

**Features:**
- Real-time trajectory visualization
- Trajectory recording and playback
- Target prediction and analysis
- Debug visualization

**Key Functions:**
- `ShowTrajectory(bool bShow)` - Toggle trajectory visualization
- `StartRecording()` / `StopRecording()` - Record trajectory
- `WillHitTarget(FVector TargetLocation)` - Check if trajectory hits target
- `GetTimeToTarget(FVector TargetLocation)` - Calculate time to target
- `GetTrajectoryDistance()` - Get total trajectory distance

## Usage

### Setting Up Components

The components are automatically created and attached to the `CWHeroCharacter` class:

```cpp
// In CWHeroCharacter constructor
CWMovementComponent = CreateDefaultSubobject<UCWCharacterMovementComponent>(TEXT("CWMovementComponent"));
TrajectoryComponent = CreateDefaultSubobject<UCWCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));
```

### Input Bindings

The following input actions are automatically bound:

- **Sprint** - Hold to sprint (increases movement speed)
- **Crouch** - Toggle crouching (decreases movement speed)
- **Toggle Trajectory** - Toggle trajectory visualization

### Blueprint Access

All major functions are exposed to Blueprint:

```cpp
// Movement functions
UFUNCTION(BlueprintCallable, Category = "Character Movement")
void StartSprint();

UFUNCTION(BlueprintCallable, Category = "Character Movement")
void StopSprint();

UFUNCTION(BlueprintCallable, Category = "Character Movement")
void ToggleCrouch();

// Trajectory functions
UFUNCTION(BlueprintCallable, Category = "Character Trajectory")
void ToggleTrajectoryVisualization();

UFUNCTION(BlueprintCallable, Category = "Character Trajectory")
void StartTrajectoryRecording();
```

## Configuration

### Movement Component Settings

```cpp
// In CWCharacterMovementComponent constructor
SprintSpeedMultiplier = 1.5f;    // Speed multiplier when sprinting
CrouchSpeedMultiplier = 0.5f;    // Speed multiplier when crouching
MaxHistorySize = 50;             // Number of movement history points to keep
```

### Trajectory Component Settings

```cpp
// In CWCharacterTrajectoryComponent constructor
TrajectoryDuration = 2.0f;       // How far ahead to predict (seconds)
TrajectoryTimeStep = 0.1f;       // Time step for trajectory points
TrajectoryColor = FLinearColor::Red;  // Color of trajectory visualization
TrajectoryThickness = 2.0f;      // Thickness of trajectory lines
```

## Example Usage

### Basic Movement Control

```cpp
// In your character class
void AMyCharacter::HandleSprintInput()
{
    if (CWMovementComponent)
    {
        CWMovementComponent->StartSprint();
    }
}
```

### Trajectory Analysis

```cpp
// Check if character will hit a target
void AMyCharacter::CheckTargetHit(const FVector& TargetLocation)
{
    if (TrajectoryComponent)
    {
        bool bWillHit = TrajectoryComponent->WillHitTarget(TargetLocation, 50.0f);
        if (bWillHit)
        {
            float TimeToTarget = TrajectoryComponent->GetTimeToTarget(TargetLocation);
            UE_LOG(LogTemp, Log, TEXT("Will hit target in %.2f seconds"), TimeToTarget);
        }
    }
}
```

### Trajectory Recording

```cpp
// Record character movement for analysis
void AMyCharacter::StartMovementRecording()
{
    if (TrajectoryComponent)
    {
        TrajectoryComponent->StartRecording();
        // ... character moves around ...
        TrajectoryComponent->StopRecording();
        
        TArray<FVector> RecordedPath = TrajectoryComponent->GetRecordedTrajectory();
        // Analyze the recorded path
    }
}
```

## Debug Features

### Movement Debug Info

The movement component provides debug information about:
- Current movement state (sprinting, crouching)
- Movement speed and direction
- Movement history

### Trajectory Debug Visualization

The trajectory component can display:
- Real-time trajectory prediction
- Trajectory distance and time
- Target hit prediction
- Recorded trajectory paths

Enable debug visualization by setting:
```cpp
TrajectoryComponent->SetTrajectoryColor(FLinearColor::Green);
TrajectoryComponent->ShowTrajectory(true);
```

## Performance Considerations

- Movement history is limited to `MaxHistorySize` points to prevent memory bloat
- Trajectory visualization can be toggled on/off to save performance
- Trajectory recording should be used sparingly for long periods
- Physics calculations are simplified for performance

## Future Enhancements

Potential improvements for these components:
- More sophisticated physics-based trajectory prediction
- Collision detection in trajectory prediction
- Trajectory smoothing and filtering
- Network replication for multiplayer
- Integration with AI pathfinding
- Advanced movement patterns (wall-running, sliding, etc.) 