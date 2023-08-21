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
	// 상태 머신 정보 기억 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	EEnemyState animState;
	
	UEnemyFSM *enemyFsm;
	// 공격 상태 재생할지 여부 판단 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bDiePlay = false;

	// 공격 애니메이션 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndAttackAnimation();

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndDieAnimation();
	// 피격 애니메이션 재생 이벤트 함수
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
