// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <WakOfSpy/WakOfSpyCharacter.h>


void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 소유 폰 얻어 오기
	auto ownerPawn = TryGetPawnOwner();

	// 플레이어로 캐스팅
	auto player = Cast<AWakOfSpyCharacter>(ownerPawn);

	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();

		FVector right = player->GetActorRightVector();

		auto movement = player->GetCharacterMovement();
		if (movement)
			isFloor = !movement->IsFalling();
	}
}
void UMyAnimInstance::PlayhitMontage()
{
	Montage_Play(hitMontage, 2.0f);
}
void UMyAnimInstance::PlayAmSal()
{
	if (AmSal1Montage)
		Montage_Play(AmSal1Montage, 1.0f);
}
void UMyAnimInstance::PlayAmSal2()
{
	if (AmSal2Montage)
		Montage_Play(AmSal2Montage, 1.0f);
}
