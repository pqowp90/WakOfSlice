// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "EnemyFSM.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <WakOfSpy/WakOfSpyCharacter.h>
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"
/**
 * 
 */
UCLASS()
class UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// ���� �ӽ� ���� ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	EEnemyState animState;
	
	UEnemyFSM *enemyFsm;
	// ���� ���� ������� ���� �Ǵ� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bDiePlay = false;

	// ���� �ִϸ��̼� ������ �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndAttackAnimation();

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndDieAnimation();
	// �ǰ� �ִϸ��̼� ��� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
	void PlayDamageAnim(FName sectionName);

public:
	UFUNCTION(BlueprintCallable, Category = Pigeuk)
	void OnHitEvent();
public:
	void PlayShoottingMontage();
	void PlayAmSal();
	void PlayAmSal2();
public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* ShoottingMontage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* AmSal1Montage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* AmSal2Montage;
};
