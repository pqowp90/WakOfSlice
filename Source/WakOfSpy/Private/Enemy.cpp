// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include "Bullet.h"
#include "EnemyAnim.h"
#include "Components/StaticMeshComponent.h"
#include "CanvasTypes.h"
#include <WakOfSpy/WakOfSpyGameMode.h>
//#include "C:\Users\USER\Downloads\TPSProject_5_3\WakOfSpy\Source\WakOfSpy\WakOfSpyGameMode.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메시 데이터 로드	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/MyAnimation/Meshs2/11_Bryan_Chimp.11_Bryan_Chimp'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}
	MyCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	MyCapsuleComp->InitCapsuleSize(40.f, 96.0f);;
	MyCapsuleComp->SetCollisionProfileName(TEXT("Enemy"));
	MyCapsuleComp->SetupAttachment(RootComponent);

	MyCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);



	// EnemyFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// 애니메이션 블루프린트 클래스 할당
	/*ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/MyAnimation/ABP_Enemy.ABP_Enemy'"));

	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}*/
	// 애니메이션 블루프린트 할당
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	
}




// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();


	AWakOfSpyGameMode* YourGameMode = Cast<AWakOfSpyGameMode>(GetWorld()->GetAuthGameMode());

	if (YourGameMode != nullptr)
	{
		YourGameMode->EnemyCount++;
	}
}
void AEnemy::Shoot()
{
	if (bulletFactory)
	{

		ABullet *bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, fireTrans.GetLocation(), fireTrans.Rotator());

		

		
	}

	float Radius = 50.0f; // 구의 반지름
	FColor Color(255, 0, 0); // 구의 색상 (빨간색)

	//DrawDebugSphere(GetWorld(), fireTrans.GetLocation(), Radius, 12, Color, false, 1.0f);
}
void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	fsm->CheckPlayer();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemy::AmSal(int num)
{
	fsm->AmSal(num);
}

