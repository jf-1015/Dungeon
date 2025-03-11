// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ControlledPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;;
class UInputConfig;

class UCharacterAbilitySystemComponent;

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemSelected, AActor*, PickupItem, bool, bUseDirectly, bool, bSelected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputPressed, const FGameplayTag&, InputTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetOpen);

/**
 * 
 */
UCLASS()
class DUNGEON_API AControlledPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AControlledPlayerController();
	virtual void PlayerTick(float DeltaTime)override;

	AActor* GetInteractableActor();

	UPROPERTY(BlueprintAssignable)
	FOnItemSelected OnItemSelected;

	UPROPERTY(BlueprintAssignable)
	FOnInputPressed OnInputPressed;

	UPROPERTY(BlueprintAssignable)
	FOnWidgetOpen OnWidgetOpen;

	UPROPERTY(BlueprintReadWrite)
	bool IsWidgetOpen = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsSubWidgetOpen = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsShopWidgetOpen = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsTargetToItem = false;

	UPROPERTY(BlueprintReadOnly)
	bool bLineTrace = true;

	float ThrowDistance = 300.f;

protected:

	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;

private:

	/*Input*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext>InputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>CameraMoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>OpenMenuAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction>LockTargetAction;

	void Move(const FInputActionValue& InputActionValue);
	void CameraMove(const FInputActionValue& InputActionValue);
	void OpenMenu(const FInputActionValue& InputActionValue);
	void LockTarget(const FInputActionValue& InputActionValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputConfig>InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	/*Input*/

	UPROPERTY()
	TObjectPtr<UCharacterAbilitySystemComponent>PlayerASC;
	UCharacterAbilitySystemComponent* GetASC();

	/*LineTrace*/
	FHitResult LineTraceHitResult;
	void LineTrace();

	UPROPERTY()
	TObjectPtr<AActor>LastActor;
	UPROPERTY()
	TObjectPtr<AActor>ThisActor;

	UPROPERTY()
	TObjectPtr<AActor>InteractableActor;
	/*LineTrace*/
};