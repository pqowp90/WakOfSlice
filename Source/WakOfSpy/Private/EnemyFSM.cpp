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

	// 월드에서 TPSCharacter 가져오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AWakOfSpyCharacter::StaticClass());
	target = Cast<AWakOfSpyCharacter>(actor);

	// 소유 객체 가져오기
	me = Cast<AEnemy>(GetOwner());



	// 애니메이션 블루프린트 할당
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
    queryParams.AddIgnoredActor(me); // 자기 자신은 무시합니다.
    bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

    if (bHit && hitResult.GetActor() == target)
    {
        // 플레이어 캐릭터가 시야에 있을 경우, 시야 각도 및 거리 제한을 확인합니다.
        FVector toTarget = (target->GetActorLocation() - me->GetActorLocation()).GetSafeNormal();
        FVector forwardVector = me->GetActorForwardVector().GetSafeNormal();
        float dotProduct = FVector::DotProduct(toTarget, forwardVector);
        float angleToTarget = FMath::Acos(dotProduct) * 180.0f / PI;
        float distanceToTarget = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

        if (angleToTarget <= yourDesiredAngle && distanceToTarget <= yourDesiredDistance)
		{
			// 플레이어 캐릭터가 시야 각도 내에 있을 경우, 이동 상태로 전환합니다.
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

    // 랜덤 이동 로직
    if (currentTime >= 7.0f)
    {
        // 10초마다 랜덤한 방향 설정
        randomMoveDirection = FMath::VRand();
        randomMoveDirection.Z = 0.0f;
        randomMoveDirection.Normalize();
        currentTime = 0.0f;
    }

    // 이동 방향 설정
    FVector moveDirection = randomMoveDirection;

	if (currentTime < 3.0f)
    // 이동 방향으로 이동
		me->AddMovementInput(moveDirection, moveSpeed);


	// 적의 현재 회전값
	FRotator currentRotation = me->GetActorRotation();

	// 목표 회전값
	FRotator targetRotation = FRotationMatrix::MakeFromX(moveDirection).Rotator();

	// 현재 회전값과 목표 회전값의 차이 계산
	FRotator deltaRotation = targetRotation - currentRotation;

	// 적의 회전속도 (Yaw 속도를 조절하여 회전 속도 조절)
	float rotationSpeed = 180.0f;

	// 회전 방향 및 회전량 계산
	float yawRotation = deltaRotation.Yaw;

	// 목표 회전값 적용
	FRotator newRotation = currentRotation + FRotator(0.0f, yawRotation, 0.0f);
	me->SetActorRotation(newRotation);

	FVector start = me->GetActorLocation() + headOffset;
	FVector end = target->GetActorLocation() + headOffset;
	FHitResult hitResult;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(me); // 자기 자신은 무시합니다.
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

	if (bHit && hitResult.GetActor() == target)
	{
		// 플레이어 캐릭터가 시야에 있을 경우, 시야 각도 및 거리 제한을 확인합니다.
		FVector toTarget = (target->GetActorLocation() - me->GetActorLocation()).GetSafeNormal();
		FVector forwardVector = me->GetActorForwardVector().GetSafeNormal();
		float dotProduct = FVector::DotProduct(toTarget, forwardVector);
		float angleToTarget = FMath::Acos(dotProduct) * 180.0f / PI;
		float distanceToTarget = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

		if (angleToTarget <= yourDesiredAngle && distanceToTarget <= yourDesiredDistance)
		{
			// 플레이어 캐릭터가 시야 각도 내에 있을 경우, 이동 상태로 전환합니다.
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

	// 타겟 목적지
	FVector dest = target->GetActorLocation();
	// 방향
	FVector dir = dest - me->GetActorLocation();
	// 방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal(), moveSpeed);

	// 적의 현재 회전값
	FRotator currentRotation = me->GetActorRotation();

	// 목표 회전값
	FRotator targetRotation = FRotationMatrix::MakeFromX(dir).Rotator();

	// 현재 회전값과 목표 회전값의 차이 계산
	FRotator deltaRotation = targetRotation - currentRotation;

	// 적의 회전속도 (Yaw 속도를 조절하여 회전 속도 조절)
	float rotationSpeed = 180.0f;

	// 회전 방향 및 회전량 계산
	float yawRotation = deltaRotation.Yaw;

	// 목표 회전값 적용
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

	// 플레이어 캐릭터가 시야에 있는지 확인
	FVector start = me->GetActorLocation() + headOffset;
	FVector end = target->GetActorLocation() + headOffset;
	FHitResult hitResult;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(me); // 자기 자신은 무시합니다.
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, queryParams);

	if (bHit && !(hitResult.GetActor() == target))
	{
		// 플레이어 캐릭터가 시야에 없을 경우, 아이들 상태로 유지합니다.
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
		// 공격하기
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

	// 타겟 목적지
	FVector dest = target->GetActorLocation();
	// 방향
	FVector dir = dest - me->GetActorLocation();

	// 적의 현재 회전값
	FRotator currentRotation = me->GetActorRotation();

	// 목표 회전값
	FRotator targetRotation = FRotationMatrix::MakeFromX(dir).Rotator();

	// 현재 회전값과 목표 회전값의 차이 계산
	FRotator deltaRotation = targetRotation - currentRotation;

	// 적의 회전속도 (Yaw 속도를 조절하여 회전 속도 조절)
	float rotationSpeed = 180.0f;

	// 회전 방향 및 회전량 계산
	float yawRotation = deltaRotation.Yaw;

	// 목표 회전값 적용
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
	// 등속운동 공식 : P = P0 + vt
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
	// 체력 감소
	hp--;

	if (hp > 0)
	{
		mState = EEnemyState::Damage;


		// 피격 애니메이션 재생 함수 호출
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
		// 캡슐 충돌체 비활성화
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

