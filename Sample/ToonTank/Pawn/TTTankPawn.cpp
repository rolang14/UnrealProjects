// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/TTTankPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Controller/TTPlayerController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

ATTTankPawn::ATTTankPawn()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    Camera->SetupAttachment(SpringArm);
}

void ATTTankPawn::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = CastChecked<ATTPlayerController>(GetController());
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        // PlayerInputSubsystem = Subsystem;
        Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(InputMappingContext, 0);
        // PlayerController->SetPlayerControllerEnabled(true);
    }
}

void ATTTankPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (PlayerController)
    {
        FHitResult HitResult;
        if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult))
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, -1.f);
                RotateTurret(HitResult.ImpactPoint);
            }
        }
    }
}

// Called to bind functionality to input
void ATTTankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATTTankPawn::InputActionMove);
    EnhancedInput->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATTTankPawn::InputActionRotate);
    EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATTTankPawn::InputActionFire);
}

void ATTTankPawn::InputActionMove(const FInputActionValue& Value)
{
    FVector DeltaLocation(0.0f); // or = FVector::ZeroVector
    const FVector2D MovementInputVector = Value.Get<FVector2D>();

    DeltaLocation.X += MovementInputVector.Y * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);

    // // 액션과 매핑된 키를 가져온다. (해당 액션과 매핑되지 않은 키는 안가져옴)
    // TArray<FKey> InputKeys = PlayerInputSubsystem->QueryKeysMappedToAction(MoveAction);
    // // 컨트롤러를 가져와서
    // APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
    // // 가져온 키에 대해서 현재 눌려있는지를 검사함
    // for (FKey Key : InputKeys)
    // {
    //     if (PlayerController->IsInputKeyDown(Key))
    //     {
    //         UE_LOG(LogTemp, Display, TEXT("Moving... + %s"), *Key.GetFName().ToString());
    //     }
    // }
}

void ATTTankPawn::InputActionRotate(const FInputActionValue& Value)
{
    FRotator DeltaRotation(0.0f); // or FRotator::ZeroRotator
    const FVector2D RotateInputVector = Value.Get<FVector2D>();

    DeltaRotation.Yaw += RotateInputVector.Y * RotateSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

void ATTTankPawn::InputActionFire()
{
    Fire();
}

void ATTTankPawn::HandleDestruction()
{
    Super::HandleDestruction();

    // Do Something
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    IsPlayerDead = true;
}