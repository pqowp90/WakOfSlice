// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayhitMontage();

	void PlayAmSal();

	void PlayAmSal2();

	void PlayPunch();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WalkSpeed)
	bool isSlowWalking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WalkSpeed)
	bool isFloor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WalkSpeed)
	float rayHeight;
public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* hitMontage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AmSal1Montage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* AmSal2Montage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* LeftPunch;
	UPROPERTY(EditAnywhere)
		UAnimMontage* RightPunch;
	UPROPERTY(EditAnywhere)
		UAnimMontage* ComboPunch;

	int combo = 0;

};


