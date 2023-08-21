// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


// ����� ���� ����
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
	Roming,
	AmSal,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	



	

public:	
	
	void CheckPlayer();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AmSalState();

public:
	// ������
	void IdleState();
	void RomingState();
	// �̵�����
	void MoveState();
	// ���ݻ���
	void AttackState();
	// �ǰݻ���
	void DamageState();
	// ��������
	void DieState();

public:
	// �ǰ� �̺�Ʈ ó�� �Լ�
	void OnDamageProcess();
public:
	void OnAttackTarget();

	void AmSal(int num);
	FVector distanceOfset;
	float amSalDeley;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	// ��� ���� ���� ����
	// ���ð�
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2.0f;

	// �̵� ���� ���� ����
	// ���
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AWakOfSpyCharacter* target;

	// ���� ����
	class AEnemy* me;

	FVector headOffset = FVector(0.0f, 0.0f, 40.0f);
	FVector randomMoveDirection = FVector(0.0f, 0.0f, 0.0f);

	// ���� ���� ���� ����
	// ���� ����
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;



	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 2.0f;
	float soundCheckDistance;

	// �ǰ� ���� ���� ����
	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int hp = 5;

	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float moveSpeed = 0.5f;

	// ���� ���� ���� ����
	// �Ʒ��� �������� �ӵ�
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieSpeed = 50.0f;

	// ����ð�
	float currentTime = 0.0f;

	// ��� ���� �ִϸ��̼� �������Ʈ
	class UEnemyAnim* anim;
	float yourDesiredAngle = 50.0f;
	float yourDesiredDistance = 700.0f;
};
