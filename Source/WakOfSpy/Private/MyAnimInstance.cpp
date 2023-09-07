// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <WakOfSpy/WakOfSpyCharacter.h>


void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// ���� �� ��� ����
	auto ownerPawn = TryGetPawnOwner();

	// �÷��̾�� ĳ����
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

void UMyAnimInstance::PlayPunch()
{
	UE_LOG(LogTemp, Warning, TEXT("ddddddddddddddddd"));
	combo++;
	if (combo > 3)
		combo = 1;
	switch (combo)
	{
	case 1:
		if (LeftPunch)
			Montage_Play(LeftPunch, 0.2f);
		break;
	case 2:
		if (RightPunch)
			Montage_Play(RightPunch, 0.2f);
		break;
	case 3:
		if (ComboPunch)
			Montage_Play(ComboPunch, 0.2f);
		break;
	default:
		break;
	}
}
