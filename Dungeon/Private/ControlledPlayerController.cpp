// Fill out your copyright notice in the Description page of Project Settings.

#include "ControlledPlayerController.h"

#include "EnhancedInputSubsystems.h"//
#include "Input/PlayerInputComponent.h"//
#include "Interaction/PickupInterface.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "Interaction/InteractableInterface.h"//
#include "Actor/PickupItems.h"//
#include "AbilitySystem/CharacterAbilitySystemComponent.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "Actor/PickupItems.h"//
#include "Actor/InteractableActor.h"//
#include "Kismet/KismetMathLibrary.h"//
#include "GameFramework/CharacterMovementComponent.h"//

AControlledPlayerController::AControlledPlayerController()
{
}

void AControlledPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	LineTrace();
}

AActor* AControlledPlayerController::GetInteractableActor()
{
	return InteractableActor;
}

void AControlledPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
}

void AControlledPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(InputComponent);

	PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AControlledPlayerController::Move);
	PlayerInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &AControlledPlayerController::CameraMove);
	PlayerInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &AControlledPlayerController::OpenMenu);
	PlayerInputComponent->BindAction(LockTargetAction, ETriggerEvent::Started, this, &AControlledPlayerController::LockTarget);

	PlayerInputComponent->BindAbilityActions(InputConfig, this, &AControlledPlayerController::AbilityInputTagPressed, &AControlledPlayerController::AbilityInputTagReleased, &AControlledPlayerController::AbilityInputTagHeld);
}

void AControlledPlayerController::Move(const FInputActionValue& InputActionValue)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FDungeonGameplayTags::Get().Ability_Interrupt);
	TagContainer.AddTag(FDungeonGameplayTags::Get().Ability_Dodge);
	TagContainer.AddTag(FDungeonGameplayTags::Get().RangeAbility_Aim);
	TagContainer.AddTag(FDungeonGameplayTags::Get().RangeAbility_Shoot);
	TagContainer.AddTag(FDungeonGameplayTags::Get().Ability_Throw);

	if (GetASC() && GetASC()->HasAnyMatchingGameplayTags(TagContainer))return;
	if (IsSubWidgetOpen)return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X);
	}
}

void AControlledPlayerController::CameraMove(const FInputActionValue& InputActionValue)
{
	if (IPlayerInterface::Execute_GetIsLocking(GetPawn()))return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxisVector.X);
		ControlledPawn->AddControllerPitchInput(-InputAxisVector.Y);

		ThrowDistance += InputAxisVector.Y * 36;
		ThrowDistance = FMath::Clamp(ThrowDistance, 200.f, 600.f);
	}
}

void AControlledPlayerController::OpenMenu(const FInputActionValue& InputActionValue)
{
	OnWidgetOpen.Broadcast();
}

void AControlledPlayerController::LockTarget(const FInputActionValue& InputActionValue)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetPawn(), FDungeonGameplayTags::Get().Input_Q, FGameplayEventData());
}

void AControlledPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC())GetASC()->AbilityInputTagPressed(InputTag);
	OnInputPressed.Broadcast(InputTag);
}

void AControlledPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())GetASC()->AbilityInputTagReleased(InputTag);
}

void AControlledPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC())GetASC()->AbilityInputTagHeld(InputTag);
}

UCharacterAbilitySystemComponent* AControlledPlayerController::GetASC()
{
	if (PlayerASC == nullptr)
	{
		PlayerASC = Cast<UCharacterAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return PlayerASC;
}

void AControlledPlayerController::LineTrace()
{
	if (!bLineTrace)
	{
		OnItemSelected.Broadcast(nullptr, false, false);
		ThisActor = nullptr;
		LastActor = nullptr;
		IsTargetToItem = true;
		return;
	}

	int32 SizeX;
	int32 SizeY;
	FVector WorldLocation;
	FVector WorldDirection;
	GetViewportSize(SizeX, SizeY);
	DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, WorldLocation, WorldDirection);

	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

	//
	DrawDebugLine(GetWorld(), CameraLocation, CameraLocation + WorldDirection * 175, FColor::Red, false, 0.1);
	//

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());
	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, CameraLocation, CameraLocation + WorldDirection * 175, ECC_Visibility, Params);

	/*bBlockingHit*/
	if (!LineTraceHitResult.bBlockingHit)
	{
		if (IsValid(InteractableActor))IInteractableInterface::Execute_SetInteractIconVisibility(InteractableActor, false);
		
		InteractableActor = nullptr;

		if (IsValid(LastActor))IPickupInterface::Execute_SetInteractIconVisibility(LastActor, false);	
		if (IsValid(ThisActor))IPickupInterface::Execute_SetInteractIconVisibility(ThisActor, false);
		
		OnItemSelected.Broadcast(nullptr, false, false);

		ThisActor = nullptr;
		LastActor = nullptr;
		IsTargetToItem = false;
		return;
	}
	/*bBlockingHit*/

	LastActor = ThisActor;

	/*Interact Actor*/
	if (IsValid(LineTraceHitResult.GetActor()) && LineTraceHitResult.GetActor()->Implements<UInteractableInterface>())
	{
		if (IsValid(InteractableActor) && InteractableActor != LineTraceHitResult.GetActor())
		{
			IInteractableInterface::Execute_SetInteractIconVisibility(InteractableActor, false);
		}
		InteractableActor = LineTraceHitResult.GetActor();
		IInteractableInterface::Execute_SetInteractIconVisibility(InteractableActor, true);
	}
	else
	{
		if (IsValid(InteractableActor))
		{
			IInteractableInterface::Execute_SetInteractIconVisibility(InteractableActor, false);
		}
		InteractableActor = nullptr;
	}
	/*Interact Actor*/

	/*Pickup Actor*/
	if (IsValid(LineTraceHitResult.GetActor()) && LineTraceHitResult.GetActor()->Implements<UPickupInterface>())
	{
		ThisActor = LineTraceHitResult.GetActor();
		IsTargetToItem = true;
	}
	else
	{
		ThisActor = nullptr;
		IsTargetToItem = false;
	}

	if (ThisActor != LastActor)
	{
		if (IsValid(LastActor) && LastActor->Implements<UPickupInterface>())
		{
			IPickupInterface::Execute_SetInteractIconVisibility(LastActor, false);
			OnItemSelected.Broadcast(nullptr, false, false);
		}

		if (IsValid(ThisActor) && ThisActor->Implements<UPickupInterface>())
		{
			IPickupInterface::Execute_SetInteractIconVisibility(ThisActor, true);
			OnItemSelected.Broadcast(ThisActor, Cast<APickupItems>(ThisActor)->ItemInfo.bUsedDirectly, true);
		}	
	}

	if (ThisActor == nullptr && LastActor == nullptr)
	{
		OnItemSelected.Broadcast(nullptr, false, false);
	}
	/*Pickup Actor*/
}