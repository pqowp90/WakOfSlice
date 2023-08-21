// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "WakOfSpyCharacter.generated.h"
#include "GameFramework/Character.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollsionComp"));
	if (collisionComp)
	{
		collisionComp->SetCollisionProfileName(TEXT("BlockALL"));
		collisionComp->SetSphereRadius(13);
		RootComponent = collisionComp;
	}



	// 발사체 이동 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	if (movementComp)
	{
		// 발사체 컴포넌트가 갱신시킬 컴포넌트 셋팅
		movementComp->SetUpdatedComponent(collisionComp);

		movementComp->InitialSpeed = 1000;
		movementComp->MaxSpeed = 1000;
		movementComp->bShouldBounce = true;
		movementComp->Bounciness = 0.3f;
	}

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 7.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::Die()
{
	Destroy();
}



