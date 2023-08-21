// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WakOfSpyGameMode.generated.h"

UCLASS(minimalapi)
class AWakOfSpyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWakOfSpyGameMode();
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> WidgetClass; // Widget 변수 추가

	UPROPERTY()
		UUserWidget* CurrentWidget; // 현재 사용중인 Widget 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyCount = 0;
	bool GameStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GameTime;
public:
	virtual void BeginPlay() override; // BeginPlay 함수 추가
	virtual void Tick(float DeltaTime) override;
	
	void ShowWidget();
};



