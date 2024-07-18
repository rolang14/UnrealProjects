// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Interface/ABCharacterHUDInterface.h"
// EnhancedInput ����� �߰�
#include "InputActionValue.h"
#include "ABCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase, public IABCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

// Character Control Section
protected:
	// VŰ�� ���� �Է� ��带 �ٲ�
	void ChangeCharacterControl();
	// �Է� ��� �ٲ�ɷ� ĳ���� ��Ʈ���� Set
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	// CharacterBase �� �����Ƿ� �ݵ�� override �ؾ��Ѵ�.
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;

protected:
	// �Է� ���� ���ؽ�Ʈ �Ҵ�� (�Ʒ����� DefaultMappingContext)
	virtual void BeginPlay() override;
	// ��� ����
	virtual void SetDead() override;

public:
	// �÷��̾� Input ������Ʈ�� �¾��ϴµ�, ���⼭�� UInputAction (�Է� �׼�)�� Move, Look ���� �Լ��� ���ν����ִ� ������ �ؾ� �Ѵ�.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

// For Camera
protected:
	// ������ �� : ī�޶� �������ִ� ������ ������ �ϴ� ������Ʈ
	// �Ʒ� Meta ���� ��쿡�� Private�� ����� �𸮾� ������Ʈ ��ü�� BP���� ���� �����ϵ��� ����� �ش�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// �̰Ŵ� ���� ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// For Input Section
protected:
	// �Ʒ� 3���� �츮�� ���ϴ� �׼��� �����ϱ� ���� (�⺻������ 3��Ī ���ø����� �ִ� �͵��̶� �׳� �������� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	// Shoulder, Quarter �� �ٲٸ鼭 ���� ����� �ʿ䰡 ����.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> MoveAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputAction> LookAction;

	//// InputMappingContext ����
	//// ���µ��� ��쿡�� �ٸ� �������� ��ȯ�ϱ� ���� EditAnywhere �� ������.
	// 
	// IMC���� ��쿡�� ������ �ϳ��� �� �÷��̾ �۾Ҵµ� ���� 2���� �� ������������ �ƿ� CharacterControlData �� ����.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuarterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	// ���� ĳ������ ��Ʈ�� Ÿ�� Ȯ�ο�
	ECharacterControlType CurrentCharacterControlType;

	// �׸��� ���� �� �Է� �׼ǿ� ���� ���ε� �Լ��� �����ϴµ�, ���⼭�� FInputActionValue ����ü�� �޾ƾ� �ϱ� ������
	// �̿� ���� ����� ���� �������ش�
	// + �� ���� ���� Shoulder Quarter �� ����
	//void Move(const FInputActionValue& Value);
	//void Look(const FInputActionValue& Value);

	void QuarterMove(const FInputActionValue& Value);
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void Attack();

// For HUD
protected:
	virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget) override;

};
