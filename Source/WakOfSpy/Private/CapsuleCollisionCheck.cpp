// Fill out your copyright notice in the Description page of Project Settings.


#include "CapsuleCollisionCheck.h"
#include <Components/CapsuleComponent.h>

// Sets default values
ACapsuleCollisionCheck::ACapsuleCollisionCheck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void ACapsuleCollisionCheck::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACapsuleCollisionCheck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

