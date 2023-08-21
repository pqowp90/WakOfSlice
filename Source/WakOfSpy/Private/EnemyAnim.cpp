// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

void UEnemyAnim::OnEndAttackAnimation()
{
	bAttackPlay = false;
}
void UEnemyAnim::OnEndDieAnimation()
{
	bDiePlay = false;
}
void UEnemyAnim::OnHitEvent()
{
	enemyFsm->OnAttackTarget();
}

void UEnemyAnim::PlayShoottingMontage()
{
	Montage_Play(ShoottingMontage, 3.0f);
}
void UEnemyAnim::PlayAmSal()
{
	if(AmSal1Montage)
		Montage_Play(AmSal1Montage, 1.0f);
}
void UEnemyAnim::PlayAmSal2()
{
	if (AmSal2Montage)
		Montage_Play(AmSal2Montage, 1.0f);
}



