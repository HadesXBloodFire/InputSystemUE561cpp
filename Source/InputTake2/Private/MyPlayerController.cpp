// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AMyPlayerController::AMyPlayerController()
{
    bShowMouseCursor = false;
    DefaultMouseCursor = EMouseCursor::Default;
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    SetShowMouseCursor(false);

    if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (DefaultMappingContext)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (EnhancedInputComponent)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleMove);

            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleLook);

            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::HandleJumpStarted);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayerController::HandleJumpReleased);

            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyPlayerController::HandleSprintStarted);
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyPlayerController::HandleSprintStopped);

            EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMyPlayerController::HandleCrouchToggle);
        }
    }
}

void AMyPlayerController::HandleMove(const FInputActionValue &Value)
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        FVector2D MovementVector = Value.Get<FVector2D>();

        const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        ControlledCharacter->MoveInDirection(ForwardDirection, MovementVector.Y);
        ControlledCharacter->MoveInDirection(RightDirection, MovementVector.X);
    }
}

void AMyPlayerController::HandleLook(const FInputActionValue &Value)
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        FVector2D LookAxisVector = Value.Get<FVector2D>();

        ControlledCharacter->LookUpDown(-LookAxisVector.Y);
        ControlledCharacter->LookLeftRight(LookAxisVector.X);
    }
}

void AMyPlayerController::HandleJumpStarted()
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        ControlledCharacter->StartJump();
    }
}

void AMyPlayerController::HandleJumpReleased()
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        ControlledCharacter->StopJump();
    }
}

void AMyPlayerController::HandleSprintStarted()
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        ControlledCharacter->StartSprint();
    }
}

void AMyPlayerController::HandleSprintStopped()
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        ControlledCharacter->StopSprint();
    }
}

void AMyPlayerController::HandleCrouchToggle()
{
    if (AMyCharacter *ControlledCharacter = Cast<AMyCharacter>(GetPawn()))
    {
        ControlledCharacter->ToggleCrouch();
    }
}
