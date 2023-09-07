// Copyright Epic Games, Inc. All Rights Reserved.

#include "WakOfSpyCharacter.h"
#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyAnimInstance.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" // ����� ������ �׸��� ���� �ʿ��� ��� ����
#include <iostream>
#include <cstdlib>
#include <ctime>



//////////////////////////////////////////////////////////////////////////
// AWakOfSpyCharacter

AWakOfSpyCharacter::AWakOfSpyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);


	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.55f;
	GetCharacterMovement()->MaxWalkSpeed = nomalWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//CollisionComponent = GetCapsuleComponent();
	//CollisionComponent->OnComponentHit.AddDynamic(this, &AWakOfSpyCharacter::OnBulletHit);

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	CollisionComponent->SetCollisionProfileName(TEXT("Player"));
	CollisionComponent->InitCapsuleSize(40.f, 96.0f);
	CollisionComponent->SetupAttachment(RootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWakOfSpyCharacter::OnOverlapBegin);


	

	 
}

void AWakOfSpyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// �ִϸ��̼� �������Ʈ �Ҵ�
	anim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void AWakOfSpyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the OtherActor has the desired tag
	if (OtherActor && OtherActor->ActorHasTag("Bullet"))
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor has the desired tag"));
		anim->PlayhitMontage();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor does not have the desired tag"));
	}
}





//////////////////////////////////////////////////////////////////////////
// Input

void AWakOfSpyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWakOfSpyCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWakOfSpyCharacter::Look);

		//Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AWakOfSpyCharacter::StartSprinting);

		//Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AWakOfSpyCharacter::StopSprinting);

		//SlowWalking
		EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Triggered, this, &AWakOfSpyCharacter::StartSlowWalking);

		//StopRunning
		EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Completed, this, &AWakOfSpyCharacter::StopSlowWalking);

		EnhancedInputComponent->BindAction(AmSalAction, ETriggerEvent::Triggered, this, &AWakOfSpyCharacter::AmSalGo);
		EnhancedInputComponent->BindAction(AmSalAction, ETriggerEvent::Completed, this, &AWakOfSpyCharacter::AmSalGo2);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWakOfSpyCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AWakOfSpyCharacter::Attack2);

	}

}
void AWakOfSpyCharacter::AmSalGo()
{
	pressAmsal = true;
}
void AWakOfSpyCharacter::AmSalGo2()
{
	pressAmsal = false;
}
void AWakOfSpyCharacter::Attack()
{
	pressAttack = true;
}
void AWakOfSpyCharacter::Attack2()
{
	pressAttack = false;
}
void AWakOfSpyCharacter::StartSprinting()
{
	bIsSprinting = true;
	SetWalkSpeed();
	// �޸��� ������ �����ϴ� ������ �ۼ��մϴ�.
}

void AWakOfSpyCharacter::StopSprinting()
{
	bIsSprinting = false;
	SetWalkSpeed();
	// �޸��� ������ ���ߴ� ������ �ۼ��մϴ�.
}

void AWakOfSpyCharacter::StartSlowWalking()
{
	bIsSlowWalking = true;
	SetWalkSpeed();
	// õõ�� ������ �����ϴ� ������ �ۼ��մϴ�.
}

void AWakOfSpyCharacter::StopSlowWalking()
{
	bIsSlowWalking = false;
	SetWalkSpeed();
	// õõ�� ������ ���ߴ� ������ �ۼ��մϴ�.
}
void AWakOfSpyCharacter::SetWalkSpeed()
{
	if (bIsSprinting)
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	
	if (bIsSlowWalking)
		GetCharacterMovement()->MaxWalkSpeed = slowWalkSpeed;
	
	if(!bIsSprinting && !bIsSlowWalking)
		GetCharacterMovement()->MaxWalkSpeed = nomalWalkSpeed;
	anim->isSlowWalking = bIsSlowWalking;
}

void AWakOfSpyCharacter::Move(const FInputActionValue& Value)
{
	if (isAmSal) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		
	}
	
}

void AWakOfSpyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void AWakOfSpyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PerformRaycast();
	PerformRaycast2();
}

void AWakOfSpyCharacter::PerformRaycast()
{
	// ���� ���� ��������
	UWorld* World = GetWorld();
	if (!World) return;

	// Raycast�� �߻��� ���� ��ġ�� ���� ����
	FVector StartLocation = GetActorLocation();
	FVector DownVector = -GetActorUpVector();
	FVector EndLocation = StartLocation + (DownVector * 160.0f); // Raycast�� �ִ� �Ÿ� ����

	// Raycast�� ������ ä�� ����
	ECollisionChannel TraceChannel = ECC_Visibility;

	// �浹 ����� ������ ����
	FHitResult HitResult;

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this); // �ڱ� �ڽ��� �����մϴ�.

	// Raycast ����
	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel, queryParams);

	if (bHit)
	{

		// �浹�� ������ ��� �΋H�� ���� ���
		FVector HitLocation = HitResult.Location;
		float height = StartLocation.Z - HitLocation.Z - 92.0f;
		height = height / 65.0f * 100.0f;
		anim->rayHeight = height;
		// �΋H�� ������ �������� �߰����� ���� ����
		// ...
	}
	else
	{
		anim->rayHeight = 100.0f;
		
	}

	// ����� ���� �׸���
	//DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, false, 2.0f);
}
void AWakOfSpyCharacter::PerformRaycast2()
{
	// ���� ���� ��������
	UWorld* World = GetWorld();
	if (!World) return;

	// Raycast�� �߻��� ���� ��ġ�� ���� ����
	FVector StartLocation = GetActorLocation();
	FVector FowordVector = GetActorForwardVector();
	FVector EndLocation = StartLocation + (FowordVector* 100.0f); // Raycast�� �ִ� �Ÿ� ����

	// Raycast�� ������ ä�� ����
	ECollisionChannel TraceChannel = ECC_Visibility;

	// �浹 ����� ������ ����
	FHitResult HitResult;

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this); // �ڱ� �ڽ��� �����մϴ�.

	// Raycast ����
	bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel, queryParams);

	if (bHit)
	{
		// Check if the OtherActor has the desired tag

		if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Enemy"))
		{
			canAmSal = true;
			if (pressAmsal)
			{
				HitResult.GetActor();
				AEnemy* Enemy = Cast<AEnemy>(HitResult.GetActor());
				if (Enemy)
				{
					

					// 1���� 2������ ���� �߻�
					int randomNumber = std::rand() % 2 + 1;
					Enemy->AmSal(randomNumber); // AWakOfSpyCharacter Ŭ������ ����� ����
					//UE_LOG(LogClass, Log, TEXT("%d"), randomNumber);
					if(randomNumber == 1)
						anim->PlayAmSal();
					else
						anim->PlayAmSal2();
				}
			}
			if (pressAttack)
			{
				anim->PlayPunch();
			}
		}
		else
		{
			canAmSal = false;
		}
	}
	else
	{
		canAmSal = false;
	}

	// ����� ���� �׸���
	//DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, false, 2.0f);
}





