// Copyright Epic Games, Inc. All Rights Reserved.


#include "WakOfSpyGameMode.h"
#include "WakOfSpyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include<Blueprint/UserWidget.h>

AWakOfSpyGameMode::AWakOfSpyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AWakOfSpyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AWakOfSpyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GameTime += GetWorld()->DeltaTimeSeconds;
	//UE_LOG(LogClass, Log, TEXT("remain_enemy%d"), EnemyCount);
	if (GameTime > 5)
	{
		GameStart = true;
	}
	
	if (EnemyCount == 0 && GameStart == true)
	{
		ShowWidget();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		APlayerController& PlayerControllerRef = *GetWorld()->GetFirstPlayerController();
		PlayerControllerRef.SetShowMouseCursor(true);
	}
	
}
void AWakOfSpyGameMode::ShowWidget()
{
	// 위젯을 생성하고 Viewport에 추가합니다.
	if (WidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}


