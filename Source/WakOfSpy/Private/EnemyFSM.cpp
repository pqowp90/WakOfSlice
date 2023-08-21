// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <WakOfSpy/WakOfSpyGameMode.h>
//#include "C:\Users\USER\Downloads\TPSProject_5_3\WakOfSpy\Source\WakOfSpy\WakOfSpyGameMode.h"



// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ���忡�� TPSCharacter ��������
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AWakOfSpyCharacter::StaticClass());
	target = Cast<AWakOfSpyCharacter>(actor);

	// ���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());



	// �ִϸ��̼� �������Ʈ �Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	anim->enemyFsm = this;

	attackRange = 300.0f;
	attackDelayTime = 2.0f;
	

}






void UEnemyFSM::CheckPlayer()
{
	if (mState == EEnemyState::AmSal)
	{
		return;
	}
	UE_LOG(LogClass, Log, TEXT("collision2"));
	mState = EEnemyState::Move;

	if (anim)
		anim->animState = mState;
	MoveState();
}

// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	case EEnemyState::Roming:
		RomingState();
		break;
	case EEnemyState::AmSal:
		AmSalState();
		break;
	default:
		break;
	}
}
void UEnemyFSM::AmSalState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	FVector forwardVector = me->GetActorForwardVector();
	FVector upVector = me->GetActorUpVector();
	me->SetActorLocation(target->GetActorLocation() + forwardVector * distanceOfset.Y + upVector * distanceOfset.Z);
	me->SetActorRotation(target->GetActorRotation());
	if (currentTime > amSalDeley)
	{
		target->isAmSal = false;
		AWakOfSpyGameMode* YourGameMode = Cast<AWakOfSpyGameMode>(GetWorld()->GetAuthGameMode());

		if (YourGameMode != nullptr)
		{
			YourGameMode->EnemyCount--;
		}
		me->Destroy();
	}
	
}

void UEnemyFSM::IdleState()
{
    currentTime += GetWorld()->DeltaTimeSeconds;

    FVector start = me->GetActorLocation() + headOffset;
    FVector end = target->GetActorLocation() + headOffset;
    FHitResult hitResult;
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(me); // �ڱ� �ڽ��� �����մϴ�.
    bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

    if (bHit && hitResult.GetActor() == target)
    {
        // �÷��̾� ĳ���Ͱ� �þ߿� ���� ���, �þ� ���� �� �Ÿ� ������ Ȯ���մϴ�.
        FVector toTarget = (target->GetActorLocation() - me->GetActorLocation()).GetSafeNormal();
        FVector forwardVector = me->GetActorForwardVector().GetSafeNormal();
        float dotProduct = FVector::DotProduct(toTarget, forwardVector);
        float angleToTarget = FMath::Acos(dotProduct) * 180.0f / PI;
        float distanceToTarget = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

        if (angleToTarget <= yourDesiredAngle && distanceToTarget <= yourDesiredDistance)
		{
			// �÷��̾� ĳ���Ͱ� �þ� ���� ���� ���� ���, �̵� ���·� ��ȯ�մϴ�.
			mState = EEnemyState::Move;

			if (anim)
				anim->animState = mState;
		}
    }
	if (currentTime >= 5.0f)
	{
		mState = EEnemyState::Roming;

		if (anim)
			anim->animState = mState;
		currentTime = 7.0f;
	}
}

void UEnemyFSM::RomingState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (target == nullptr || me == nullptr)
        return;

    // ���� �̵� ����
    if (currentTime >= 7.0f)
    {
        // 10�ʸ��� ������ ���� ����
        randomMoveDirection = FMath::VRand();
        randomMoveDirection.Z = 0.0f;
        randomMoveDirection.Normalize();
        currentTime = 0.0f;
    }

    // �̵� ���� ����
    FVector moveDirection = randomMoveDirection;

	if (currentTime < 3.0f)
    // �̵� �������� �̵�
		me->AddMovementInput(moveDirection, moveSpeed);


	// ���� ���� ȸ����
	FRotator currentRotation = me->GetActorRotation();

	// ��ǥ ȸ����
	FRotator targetRotation = FRotationMatrix::MakeFromX(moveDirection).Rotator();

	// ���� ȸ������ ��ǥ ȸ������ ���� ���
	FRotator deltaRotation = targetRotation - currentRotation;

	// ���� ȸ���ӵ� (Yaw �ӵ��� �����Ͽ� ȸ�� �ӵ� ����)
	float rotationSpeed = 180.0f;

	// ȸ�� ���� �� ȸ���� ���
	float yawRotation = deltaRotation.Yaw;

	// ��ǥ ȸ���� ����
	FRotator newRotation = currentRotation + FRotator(0.0f, yawRotation, 0.0f);
	me->SetActorRotation(newRotation);

	FVector start = me->GetActorLocation() + headOffset;
	FVector end = target->GetActorLocation() + headOffset;
	FHitResult hitResult;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(me); // �ڱ� �ڽ��� �����մϴ�.
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

	if (bHit && hitResult.GetActor() == target)
	{
		// �÷��̾� ĳ���Ͱ� �þ߿� ���� ���, �þ� ���� �� �Ÿ� ������ Ȯ���մϴ�.
		FVector toTarget = (target->GetActorLocation() - me->GetActorLocation()).GetSafeNormal();
		FVector forwardVector = me->GetActorForwardVector().GetSafeNormal();
		float dotProduct = FVector::DotProduct(toTarget, forwardVector);
		float angleToTarget = FMath::Acos(dotProduct) * 180.0f / PI;
		float distanceToTarget = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

		if (angleToTarget <= yourDesiredAngle && distanceToTarget <= yourDesiredDistance)
		{
			// �÷��̾� ĳ���Ͱ� �þ� ���� ���� ���� ���, �̵� ���·� ��ȯ�մϴ�.
			mState = EEnemyState::Move;

			if (anim)
				anim->animState = mState;
		}
	}
}

void UEnemyFSM::MoveState()
{
	if (target == nullptr || me == nullptr)
		return;

	// Ÿ�� ������
	FVector dest = target->GetActorLocation();
	// ����
	FVector dir = dest - me->GetActorLocation();
	// �������� �̵�
	me->AddMovementInput(dir.GetSafeNormal(), moveSpeed);

	// ���� ���� ȸ����
	FRotator currentRotation = me->GetActorRotation();

	// ��ǥ ȸ����
	FRotator targetRotation = FRotationMatrix::MakeFromX(dir).Rotator();

	// ���� ȸ������ ��ǥ ȸ������ ���� ���
	FRotator deltaRotation = targetRotation - currentRotation;

	// ���� ȸ���ӵ� (Yaw �ӵ��� �����Ͽ� ȸ�� �ӵ� ����)
	float rotationSpeed = 180.0f;

	// ȸ�� ���� �� ȸ���� ���
	float yawRotation = deltaRotation.Yaw;

	// ��ǥ ȸ���� ����
	FRotator newRotation = currentRotation + FRotator(0.0f, yawRotation, 0.0f);
	me->SetActorRotation(newRotation);

	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attack;


		if (anim)
		{
			anim->animState = mState;
			anim->bAttackPlay = true;
		}
	}
	if (target == nullptr || me == nullptr)
		return;

	// �÷��̾� ĳ���Ͱ� �þ߿� �ִ��� Ȯ��
	FVector start = me->GetActorLocation() + headOffset;
	FVector end = target->GetActorLocation() + headOffset;
	FHitResult hitResult;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(me); // �ڱ� �ڽ��� �����մϴ�.
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

	if (bHit && !(hitResult.GetActor() == target))
	{
		// �÷��̾� ĳ���Ͱ� �þ߿� ���� ���, ���̵� ���·� �����մϴ�.
		mState = EEnemyState::Idle;

		if (anim)
			anim->animState = mState;
	}
	DrawDebugLine(GetWorld(), start, end, bHit ? FColor::Green : FColor::Red, false, -1.0f, 0, 5.0f);
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
		// �����ϱ�
		currentTime = 0.0f;

		if (anim)
			anim->bAttackPlay = true;
		me->Shoot();
		anim->PlayShoottingMontage();
		
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Idle;
		if (anim)
			anim->animState = mState;
	}
	if (target == nullptr || me == nullptr)
		return;

	// Ÿ�� ������
	FVector dest = target->GetActorLocation();
	// ����
	FVector dir = dest - me->GetActorLocation();

	// ���� ���� ȸ����
	FRotator currentRotation = me->GetActorRotation();

	// ��ǥ ȸ����
	FRotator targetRotation = FRotationMatrix::MakeFromX(dir).Rotator();

	// ���� ȸ������ ��ǥ ȸ������ ���� ���
	FRotator deltaRotation = targetRotation - currentRotation;

	// ���� ȸ���ӵ� (Yaw �ӵ��� �����Ͽ� ȸ�� �ӵ� ����)
	float rotationSpeed = 180.0f;

	// ȸ�� ���� �� ȸ���� ���
	float yawRotation = deltaRotation.Yaw;

	// ��ǥ ȸ���� ����
	FRotator newRotation = currentRotation + FRotator(0.0f, yawRotation, 0.0f);
	me->SetActorRotation(newRotation);

}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;

		if (anim)
			anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	// ��ӿ ���� : P = P0 + vt
	if (anim->bDiePlay)return;
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	// ü�� ����
	hp--;

	if (hp > 0)
	{
		mState = EEnemyState::Damage;


		// �ǰ� �ִϸ��̼� ��� �Լ� ȣ��
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		if (anim)
			anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		if (anim)
			anim->bDiePlay = true;
		mState = EEnemyState::Die;
		// ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (anim)
		anim->animState = mState;
}

void UEnemyFSM::OnAttackTarget()
{
	if (target != nullptr) 
	{
		//DAMAGE
	}
}
void UEnemyFSM::AmSal(int num)
{
	if (mState == EEnemyState::AmSal)
	{
		return;
	}
	currentTime = 0;
	mState = EEnemyState::AmSal;
	if (anim)
		anim->animState = mState;
	if (num == 1) {
		anim->PlayAmSal();
		distanceOfset = FVector(0, 200.0f, 0);
		amSalDeley = 5.0f;
	}
	else {
		anim->PlayAmSal2();
		distanceOfset = FVector(0, 150.0f, 10);
		amSalDeley = 7.0f;
	}
	target->isAmSal = true;
}

