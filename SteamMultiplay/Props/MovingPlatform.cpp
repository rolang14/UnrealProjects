// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/MovingPlatform.h"
#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

	ActivatedTrigger = 0;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// 시작 위치
	GlobalStartLocation = GetActorLocation();
	// 종료 위치. TargetLocation 이 로컬 축이기에 이렇게 월드 축으로 변환할 필요가 있다.
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocationOffset);

	// 서버일 경우
	if (HasAuthority())
	{
		// 이 액터를 Replicate 하고, 움직임 또한 Replicate 하여 클라이언트에 반영할 수 있도록 한다.
		// 당연히 이는 서버의 권한이므로 서버에서만 실행되는 코드여야 한다.
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 서버라면 HasAuthority 는 True 를 반환하고, 클라이언트라면 False 를 반환한다.
	// 즉 아래와 같은 코드는 서버에서만 작동하도록 하는 코드가 된다.
	// 즉, 이 플랫폼은 클라이언트에서는 실제로 움직이지 않고, 서버에서만 움직이게 되는 것이다!!
	if (HasAuthority() && ActivatedTrigger >= NumberOfTrigger)
	{
		FVector CurrentLocation = GetActorLocation();

		// 아래처럼 생각했으나, 이렇게 하더라도 만약 속도가 너무 빨라서 프레임 당 속도,
		// 그러니까 MoveSpeed * DeltaSeconds 보다 빨라지면 단일 프레임에 이를 넘어서기 때문에 체크가 안된다.
		//if (FVector::Dist(GetActorLocation(), GlobalTargetLocation) <= MoveSpeed * DeltaSeconds)
		//{
		//	//Swap(GlobalStartLocation, GlobalTargetLocation);
		//	FVector TempLocation = GlobalTargetLocation;
		//	GlobalTargetLocation = GlobalStartLocation;
		//	GlobalStartLocation = TempLocation;
		//}

		// 그래서 아래처럼 움직인 거리 자체를 비교할 필요가 있다.
		// 두 벡터 차이를 이용해서 거리를 알아낸다.
		//float ShouldMove = (GlobalTargetLocation - GlobalStartLocation).Size();
		//float MovedLength = (GetActorLocation() - GlobalStartLocation).Size();
		//if (MovedLength >= ShouldMove)
		//{
		//	FVector TempLocation = GlobalTargetLocation;
		//	GlobalTargetLocation = GlobalStartLocation;
		//	GlobalStartLocation = TempLocation;
		//}

		// 이제 월드 Target 위치에서 현재 위치 벡터를 뺸 뒤 정규화하여 방향을 구한다.
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		// 해당 방향으로 초속 MoveSpeed 만큼 이동하면 되며, 프레임레이트를 고려하여 DeltaSeconds 를 곱해서 변경한다.
		SetActorLocation(CurrentLocation + Direction * MoveSpeed * DeltaSeconds);


		// 근데 내가만든게 오히려 더 정확하더라..
		// 이렇게 하니까 속도가 일정 수치 이상 넘어가면 걍 휙 하고 튀어버리는데?
		// 내가 한건 MoveSpeed * DeltaSeconds를 비교하니까 상대적으로 더 높은 속도까지 커버가 된다.
		// 걍 원래 했던대로 하자 ㅇㅇ

		if (FVector::Dist(GetActorLocation(), GlobalTargetLocation) <= MoveSpeed * DeltaSeconds)
		{
			//Swap(GlobalStartLocation, GlobalTargetLocation);
			FVector TempLocation = GlobalTargetLocation;
			GlobalTargetLocation = GlobalStartLocation;
			GlobalStartLocation = TempLocation;
		}
	}
}

void AMovingPlatform::AddActivatedTrigger()
{
	ActivatedTrigger++;
}

void AMovingPlatform::RemoveActivatedTrigger()
{
	ActivatedTrigger--;
}
