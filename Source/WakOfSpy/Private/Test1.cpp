// Fill out your copyright notice in the Description page of Project Settings.


#include "Test1.h"

// Sets default values
ATest1::ATest1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("경고경고경고경고경고경고경고경고경고경고"));


}

// Called when the game starts or when spawned
void ATest1::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("앙 기모띨리리리리리리이일ㅇ"));

	

	
}

// Called every frame
void ATest1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

