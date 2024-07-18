// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SSPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"

// Sets default values
ASSPlayerCharacter::ASSPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 시작 모션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && StageStartMontage)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		AnimInstance->Montage_Play(StageStartMontage);

		// 몽타주 종료 콜백 델리게이트
		FOnMontageEnded LevelStartEndDelegate;
		LevelStartEndDelegate.BindUObject(this, &ASSPlayerCharacter::LevelStartEnded);
		AnimInstance->Montage_SetEndDelegate(LevelStartEndDelegate, StageStartMontage);
	}
	if (StageStartSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Stage Start Sound !!"));
		UGameplayStatics::PlaySoundAtLocation(this, StageStartSound, GetActorLocation());
	}
}

void ASSPlayerCharacter::LevelStartEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CannotFire = false;
}

// Called every frame
void ASSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASSPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2D MoveValue = InputValue.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector() * MoveValue.X);
	AddMovementInput(GetActorRightVector() * MoveValue.Y);
}

void ASSPlayerCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D LookValue = InputValue.Get<FVector2D>();
	AddControllerPitchInput(LookValue.Y * LookSpeed);
	AddControllerYawInput(LookValue.X * LookSpeed);
}

void ASSPlayerCharacter::LookController(const FInputActionValue& InputValue)
{
	const FVector2D LookValue = InputValue.Get<FVector2D>();
	AddControllerPitchInput(LookValue.Y * LookSpeed * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(LookValue.X * LookSpeed * GetWorld()->GetDeltaSeconds());
}

void ASSPlayerCharacter::Jump()
{
	Super::Jump();
}

void ASSPlayerCharacter::StopJumping()
{
	Super::StopJumping();
}

void ASSPlayerCharacter::Fire()
{
	if (CannotFire) return;

	Super::Fire();
}

