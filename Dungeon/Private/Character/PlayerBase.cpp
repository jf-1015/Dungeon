// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerBase.h"
#include "ControlledPlayerState.h"//
#include "ControlledPlayerController.h"//
#include "UI/HUD/PlayerHUD.h"//
#include "AbilitySystem/CharacterAbilitySystemComponent.h"//
#include "AbilitySystem/CharacterAttributeSet.h"//
#include "Data/DungeonItemInfo.h"//
#include "Kismet/GameplayStatics.h"//
#include "GameFramework/CharacterMovementComponent.h"//
#include "Data/DetailedEquipmemtInfo.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "Camera/CameraComponent.h"//
#include "GameFramework/SpringArmComponent.h"//
#include "Components/SplineComponent.h"//
#include "GameMode/GlobalGameModeBase.h"//
#include "DungeonGameInstance.h"//
#include "SaveGame/DungeonSaveGame.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "DungeonGameInstance.h"//
#include "Components//CapsuleComponent.h"//

APlayerBase::APlayerBase()
{
	CharacterTag = FDungeonGameplayTags::Get().Character_Player;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());

	AimingBoom = CreateDefaultSubobject<USpringArmComponent>("AimingBoom");
	AimingBoom->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = true;

	OnWeaponTypeChange.AddDynamic(this, &APlayerBase::WeaponTypeChanged);

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	ThrowPoint = CreateDefaultSubobject<UStaticMeshComponent>("ThrowPoint");
	ThrowPoint->SetupAttachment(GetMesh());
	ThrowPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
}

FGameItemInfo APlayerBase::GetEquipedWeaponInfo_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();	
	return  ControlledPS->GetCurrentWeaponInfo();
}

FGameItemInfo APlayerBase::GetEquipedSecondaryWeaponInfo_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return  ControlledPS->GetCurrentSecondaryWeaponInfo();
}

FGameItemInfo APlayerBase::GetEquipedConsumableInfo_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return  ControlledPS->GetCurrentConsumableInfo();
}

bool APlayerBase::GetIsAttacking_Implementation()
{
	return IsAttacking;
}

void APlayerBase::SetIsAttacking_Implementation(bool InBool)
{
	IsAttacking = InBool;
}

bool APlayerBase::GetIsLocking_Implementation()
{
	return IsLocking;
}

void APlayerBase::SetIsLocking_Implementation(bool InBool)
{
	IsLocking = InBool;
}

float APlayerBase::GetDirection_Implementation()
{
	return Direction;
}

void APlayerBase::SetDirection_Implementation(float InDirection)
{
	Direction = InDirection;
}

void APlayerBase::AddToBackpack_Implementation(UPARAM(ref) FGameItemInfo& InItemInfo)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	ControlledPS->AddToBackpack(InItemInfo);
	Cast<AControlledPlayerController>(GetController())->OnItemSelected.Broadcast(nullptr, false, false);
}

void APlayerBase::SetTargetActor_Implementation(AActor* InActor)
{
	LockTarget = InActor;
}

AActor* APlayerBase::GetTargetActor_Implementation()
{
	return LockTarget;
}

UCharacterMovementComponent* APlayerBase::GetCharacterMovementComponent_Implementation()
{
	return GetCharacterMovement();
}

bool APlayerBase::CanEquip_Implementation(FGameplayTag HandheldType)
{
	FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	
	if (ControlledPS->CheckBackpackIsFull())return false;

	if (HandheldType == DungeonTags.Handheld_None)
	{
		return true;
	}
	if (HandheldType.MatchesTag(DungeonTags.Handheld_1H)|| HandheldType.MatchesTag(DungeonTags.Handheld_2H))
	{
		if (WeaponIsEmpty)
		{
			return true;
		}
		return false;
	}
	if (HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
	{
		if (SecondaryWeaponIsEmpty)
		{
			return true;
		}	
	}
	if (HandheldType.MatchesTag(DungeonTags.Handheld_Body))
	{
		if (HandheldType == DungeonTags.Handheld_Body_Helmet && ControlledPS->CheckHelmet())return false;

		if (ControlledPS->GetEmptyAccessorySlot() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool APlayerBase::CheckBackpackIsFull_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	if (ControlledPS->CheckBackpackIsFull())
	{
		return true;
	}
	return false;
}

void APlayerBase::SetUnusedSocket_Implementation(UStaticMesh* InMesh)
{
	UnusedWeapon->SetStaticMesh(InMesh);
}

void APlayerBase::ChangeCurrentEquippedWeaponSlot_Implementation(int32 Slot)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	ControlledPS->ChangeCurrentEquippedWeaponSlot(Slot);
}

void APlayerBase::ChangeCurrentEquippedSecondaryWeaponSlot_Implementation(int32 Slot)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	ControlledPS->ChangeCurrentEquippedSecondaryWeaponSlot(Slot);
}

void APlayerBase::ChangeCurrentEquippedConsumableSlot_Implementation(int32 Slot)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	ControlledPS->ChangeCurrentEquippedConsumableSlot(Slot);
}

int32 APlayerBase::GetCurrentEquippedWeaponSlot_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return ControlledPS->GetCurrentEquippedWeaponSlot();
}

int32 APlayerBase::GetCurrentEquippedSecondaryWeaponSlot_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return ControlledPS->GetCurrentEquippedSecondaryWeaponSlot();
}

int32 APlayerBase::GetCurrentEquippedConsumableSlot_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return ControlledPS->GetCurrentEquippedConsumableSlot();
}

void APlayerBase::ExpendItem_Implementation(int32 ItemIndex, int32 Num)
{
	GetPlayerState<AControlledPlayerState>()->ExpendItem(ItemIndex, Num);
}

bool APlayerBase::CheckCanAfford_Implementation(int32 ItemIndex, int32 RequiredNum)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return ControlledPS->CheckCanAfford(ItemIndex, RequiredNum);
}

void APlayerBase::SetIsTargetToItem_Implementation(bool InBool)
{
	if (GetController())
	{
		Cast<AControlledPlayerController>(GetController())->IsTargetToItem = InBool;
	}	
}

bool APlayerBase::GetIsTargetToItem_Implementation()
{
	if (GetController())
	{
		return Cast<AControlledPlayerController>(GetController())->IsTargetToItem;
	}
	return false;
}

void APlayerBase::SetLineTrace_Implementation(bool InBool)
{
	Cast<AControlledPlayerController>(GetController())->bLineTrace = InBool;
}

bool APlayerBase::GetIsLoaded_Implementation()
{
	return GetPlayerState<AControlledPlayerState>()->GetIsLoaded();
}

void APlayerBase::SetIsLoaded_Implementation(bool InBool)
{
	GetPlayerState<AControlledPlayerState>()->SetIsLoaded(InBool);
}

bool APlayerBase::CanLoad_Implementation()
{
	return GetPlayerState<AControlledPlayerState>()->CanLoad();
}

bool APlayerBase::GetIsAiming_Implementation()
{
	return IsAiming;
}

void APlayerBase::SetIsAiming_Implementation(bool InBool)
{
	IsAiming = InBool;
	if (InBool)
	{
		Camera->AttachToComponent(AimingBoom, FAttachmentTransformRules::KeepRelativeTransform);
		bUseControllerRotationYaw = true;
	}
	else
	{
		Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
		bUseControllerRotationYaw = false;
	}
}

void APlayerBase::SetUseControllerRotationYaw_Implementation(bool InBool)
{
	bUseControllerRotationYaw = InBool;
}

AActor* APlayerBase::GetInteractableActor_Implementation()
{
	return Cast<AControlledPlayerController>(GetController())->GetInteractableActor();
}

void APlayerBase::UseCurrentConsumable_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	ControlledPS->UseCurrentConsumable();
}

bool APlayerBase::IsCurrentConsumableEmpty_Implementation()
{
	return GetPlayerState<AControlledPlayerState>()->IsCurrentConsumableEmpty();
}

void APlayerBase::SetWidgetOpen_Implementation(bool InBool, FName WidgetName)
{
	if (WidgetName == FName("Menu"))
	{
		if (IsValid(GetController()))
		{
			Cast<AControlledPlayerController>(GetController())->IsWidgetOpen = InBool;
		}
	}
	else if(WidgetName==FName("Shop"))
	{
		if(IsValid(GetController()))
		{
			Cast<AControlledPlayerController>(GetController())->IsShopWidgetOpen = InBool;
		}
	}
}

bool APlayerBase::GetIsWidgetOpen_Implementation(FName WidgetName)
{
	if (WidgetName == FName("Menu"))
	{
		return Cast<AControlledPlayerController>(GetController())->IsWidgetOpen;

	}
	else if (WidgetName == FName("Shop"))
	{
		return Cast<AControlledPlayerController>(GetController())->IsShopWidgetOpen;
	}
	return false;
}

bool APlayerBase::GetShowAuxSpline_Implementation()
{
	return bShowAuxSpline;
}

void APlayerBase::SetShowAuxSpline_Implementation(bool InBool)
{
	bShowAuxSpline = InBool;
}

FVector APlayerBase::GetThrowVelocity_Implementation()
{
	return Cast<AControlledPlayerController>(GetController())->ThrowDistance * ThrowPoint->GetForwardVector();
}

FTransform APlayerBase::GetThrowPointTransform_Implementation()
{
	FTransform Transform;
	Transform.SetLocation(ThrowPoint->GetComponentTransform().GetLocation());
	Transform.SetRotation(ThrowPoint->GetComponentTransform().GetRotation());
	return Transform;
}

void APlayerBase::SaveProgress_Implementation()
{
	AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this));
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();

	FString CurrentMapAssetName = GetWorld()->GetMapName();
	CurrentMapAssetName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (GM && ControlledPS)
	{
		UDungeonSaveGame* SaveData = GM->RetrieveInGameSaveData();

		SaveData->bFirstLoadIn = false;
		
		/*Items*/
		ControlledPS->SaveItemsToSaveGame(SaveData);

		/*Slot*/
		SaveData->WeaponSlot = ControlledPS->GetCurrentEquippedWeaponSlot();
		SaveData->SecondaryWeaponSlot = ControlledPS->GetCurrentEquippedSecondaryWeaponSlot();
		SaveData->ConsumableSlot = ControlledPS->GetCurrentEquippedConsumableSlot();

		/*Transform*/
		SaveData->PlayerLocation = GetActorLocation();
		SaveData->PlayerRotation = GetActorRotation();

		/*Attribute*/
		ControlledPS->RemoveAccessoryEffect();
		
		SaveData->Vitality = UCharacterAttributeSet::GetVitalityAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Power = UCharacterAttributeSet::GetPowerAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Dexterity = UCharacterAttributeSet::GetDexterityAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UCharacterAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Endurance = UCharacterAttributeSet::GetEnduranceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UCharacterAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Defence = UCharacterAttributeSet::GetDefenceAttribute().GetNumericValue(GetAttributeSet());

		SaveData->Health = UCharacterAttributeSet::GetHealthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Mana = UCharacterAttributeSet::GetManaAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Energy = UCharacterAttributeSet::GetEnergyAttribute().GetNumericValue(GetAttributeSet());
		SaveData->CurrentResilience = UCharacterAttributeSet::GetCurrentResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Food = UCharacterAttributeSet::GetFoodAttribute().GetNumericValue(GetAttributeSet());

		GM->SaveInGameProgressData(SaveData);
	}
}

void APlayerBase::SaveAndQuit_Implementation()
{
	if (AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		const UWorld* World = GetWorld();
		FString MapAssetName = World->GetMapName();
		MapAssetName.RemoveFromStart(World->StreamingLevelsPrefix);

		IPlayerInterface::Execute_SaveProgress(this);
		GM->SaveWorldState(GetWorld(), MapAssetName);
	}
}

void APlayerBase::LoadToMap_Implementation()
{
	if (bFirstLoadIn)
	{
		bFirstLoadIn = false;
		return;
	}
	if (AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		UWorld* World = GetWorld();

		LoadProgress();
		GM->LoadWorldState(World);
	}
}

void APlayerBase::SetFoodConsumeRate_Implementation(float InFloat)
{
	FoodConsumeRate = InFloat;
}

float APlayerBase::GetFoodConsumeRate_Implementation()
{
	return FoodConsumeRate;
}

bool APlayerBase::IsFoodEmpty_Implementation()
{
	float CurrentFood = Cast<UCharacterAttributeSet>(GetAttributeSet())->GetFood();
	if (CurrentFood <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APlayerBase::LoadProgress()
{
	AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this));
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();

	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	if (GM && ControlledPS)
	{
		UDungeonSaveGame* SaveData = GM->RetrieveInGameSaveData();

		if (SaveData == nullptr)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
			return;
		}

		if (SaveData->bFirstLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			AddCharacterAbilities();

			/*Initialize BackpackItems*/
			for (FGameItemInfo ItemInfo : SaveData->BackpackItems)
			{
				InitializeItems(ItemInfo, SaveData);
			}

			/*Initialize Attributes*/
			UCharacterClassInfo* CharacterClassInfo = UDungeonAbilitySystemLibrary::GetCharacterClassInfomation(this);

			FGameplayEffectContextHandle ContextHandle = CharacterASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle BaseSpecHandle = CharacterASC->MakeOutgoingSpec(CharacterClassInfo->BaseAttribute_SetByCaller, 1.f, ContextHandle);

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Vitality, SaveData->Vitality);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Power, SaveData->Power);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Dexterity, SaveData->Dexterity);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Intelligence, SaveData->Intelligence);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Endurance, SaveData->Endurance);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Resilience, SaveData->Resilience);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(BaseSpecHandle, DungeonTags.Attribute_Base_Defence, SaveData->Defence);

			CharacterASC->ApplyGameplayEffectSpecToSelf(*BaseSpecHandle.Data.Get());

			AppliedEffectToSelf(DerivedAttribute);

			FGameplayEffectSpecHandle VitalSpecHandle = CharacterASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttribute_SetByCaller, 1.f, ContextHandle);
			
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Health, SaveData->Health);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Mana, SaveData->Mana);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Energy, SaveData->Energy);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_CurrentResilience, SaveData->CurrentResilience);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Food, SaveData->Food);

			CharacterASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
		}
	}
}

bool APlayerBase::Use_Implementation(UPARAM(ref) FGameItemInfo& Info)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();

	IPlayerInterface::Execute_SetLineTrace(this, false);

	if (Info.ItemType == DungeonTags.ItemType_Consume)
	{
		AppliedEffectToSelf(Info.ItemEffect);
		Cast<AControlledPlayerController>(GetController())->OnItemSelected.Broadcast(nullptr, false, false);
		return true;
	}

	if (Info.ItemType == DungeonTags.ItemType_Weapon)
	{
		if (Info.HandheldType == DungeonTags.Handheld_1H)
		{
			if (WeaponIsEmpty)
			{
				if (IsValid(CharacterASC))
				{
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), FDungeonGameplayTags::Get().Input_LM, true);
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, Info, FDungeonGameplayTags::Get().Input_LM, false);
				}

				ICharacterInterface::Execute_SetWeaponAttachment(this, Info);
				ICharacterInterface::Execute_SetCurrentWeaponType(this, Info.WeaponType);

				Weapon->SetStaticMesh(Info.ItemMesh);
				WeaponIsEmpty = false;
				Info.IsEquiped = true;
				Info.EquipSlotIndex = ControlledPS->GetCurrentEquippedWeaponSlot();
				E_HandheldType = EHandheldType::OneHand;
				IPlayerInterface::Execute_AddToBackpack(this, Info);
				return true;
			}
		}
		if (Info.HandheldType == DungeonTags.Handheld_2H)
		{
			if (WeaponIsEmpty)
			{
				if (!SecondaryWeaponIsEmpty)
				{
					IPlayerInterface::Execute_SetUnusedSocket(this, ControlledPS->GetCurrentSecondaryWeaponInfo().ItemMeshOnUnusedSocket);
					ICharacterInterface::Execute_SetHandheldStaticMeshByType(this, nullptr, FDungeonGameplayTags::Get().Handheld_Secondary);		
				}
				if (IsValid(CharacterASC))
				{
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), FDungeonGameplayTags::Get().Input_LM, true);
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, Info, FDungeonGameplayTags::Get().Input_LM, false);

					Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(FDungeonGameplayTags::Get().Input_RM);
				}

				ICharacterInterface::Execute_SetWeaponAttachment(this, Info);
				ICharacterInterface::Execute_SetCurrentWeaponType(this, Info.WeaponType);

				Weapon->SetStaticMesh(Info.ItemMesh);
				WeaponIsEmpty = false;
				Info.IsEquiped = true;
				Info.EquipSlotIndex = ControlledPS->GetCurrentEquippedWeaponSlot();
				E_HandheldType = EHandheldType::TwoHands;
				IPlayerInterface::Execute_AddToBackpack(this, Info);
				return true;
			}
		}
		if (Info.HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
		{
			if (SecondaryWeaponIsEmpty)
			{
				if (E_HandheldType != EHandheldType::TwoHands)
				{
					SecondaryWeapon->SetStaticMesh(Info.ItemMesh);
					E_HandheldType = EHandheldType::OneHand;
					if (IsValid(CharacterASC))
					{
						ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, Info, FDungeonGameplayTags::Get().Input_RM, false);
					}			
				}
				else
				{
					UnusedWeapon->SetStaticMesh(Info.ItemMeshOnUnusedSocket);
				}
				SecondaryWeaponIsEmpty = false;
				Info.IsEquiped = true;
				Info.EquipSlotIndex = ControlledPS->GetCurrentEquippedSecondaryWeaponSlot();
				IPlayerInterface::Execute_AddToBackpack(this, Info);
				return true;
			}
		}	
	}
	if (Info.HandheldType.MatchesTag(DungeonTags.Handheld_Body))
	{
		if (Info.HandheldType.MatchesTag(DungeonTags.Handheld_Body_Helmet))
		{
			ICharacterInterface::Execute_SetHandheldStaticMeshByType(this, Info.ItemMesh, FDungeonGameplayTags::Get().Handheld_Body_Helmet);		
		}
		Info.IsEquiped = true;
		Info.EquipSlotIndex = ControlledPS->GetEmptyAccessorySlot();

		ControlledPS->SetAccessorySlotStatus(Info.EquipSlotIndex, false);
		ControlledPS->EquipAccessory(Info);

		IPlayerInterface::Execute_AddToBackpack(this, Info);
		return true;

	}
	return false;
}

void APlayerBase::InitializeItems(FGameItemInfo& ItemInfo, UDungeonSaveGame* SaveData)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();

	ControlledPS->SetCurrentEquippedWeaponSlot(SaveData->WeaponSlot);
	ControlledPS->SetCurrentEquippedSecondaryWeaponSlot(SaveData->SecondaryWeaponSlot);
	ControlledPS->SetCurrentEquippedConsumableSlot(SaveData->ConsumableSlot);

	if (ItemInfo.ItemType == DungeonTags.ItemType_Consume)
	{
		if (ItemInfo.IsEquiped && ItemInfo.EquipSlotIndex == SaveData->ConsumableSlot)
		{
			ControlledPS->GiveItemAbility(ItemInfo, DungeonTags.Input_R);
		}
		ControlledPS->AddToBackpack(ItemInfo);
	}

	if (ItemInfo.ItemType == DungeonTags.ItemType_Weapon)
	{
		if (ItemInfo.IsEquiped && ItemInfo.EquipSlotIndex == SaveData->WeaponSlot)
		{
			if (ItemInfo.HandheldType == DungeonTags.Handheld_1H)
			{
				ControlledPS->SetCurrentEquippedWeaponSlot(ItemInfo.EquipSlotIndex);

				if (IsValid(CharacterASC))
				{
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), FDungeonGameplayTags::Get().Input_LM, true);
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, ItemInfo, FDungeonGameplayTags::Get().Input_LM, false);
				}

				ICharacterInterface::Execute_SetWeaponAttachment(this, ItemInfo);
				ICharacterInterface::Execute_SetCurrentWeaponType(this, ItemInfo.WeaponType);

				Weapon->SetStaticMesh(ItemInfo.ItemMesh);
				WeaponIsEmpty = false;
				E_HandheldType = EHandheldType::OneHand;
			}

			if (ItemInfo.HandheldType == DungeonTags.Handheld_2H)
			{
				ControlledPS->SetCurrentEquippedWeaponSlot(ItemInfo.EquipSlotIndex);

				if (!SecondaryWeaponIsEmpty)
				{
					IPlayerInterface::Execute_SetUnusedSocket(this, ControlledPS->GetCurrentSecondaryWeaponInfo().ItemMeshOnUnusedSocket);
					ICharacterInterface::Execute_SetHandheldStaticMeshByType(this, nullptr, FDungeonGameplayTags::Get().Handheld_Secondary);
				}
				if (IsValid(CharacterASC))
				{
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), FDungeonGameplayTags::Get().Input_LM, true);
					ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, ItemInfo, FDungeonGameplayTags::Get().Input_LM, false);

					Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(FDungeonGameplayTags::Get().Input_RM);
				}

				ICharacterInterface::Execute_SetWeaponAttachment(this, ItemInfo);
				ICharacterInterface::Execute_SetCurrentWeaponType(this, ItemInfo.WeaponType);

				Weapon->SetStaticMesh(ItemInfo.ItemMesh);
				WeaponIsEmpty = false;
				E_HandheldType = EHandheldType::TwoHands;
			}
		}
		if (ItemInfo.IsEquiped && ItemInfo.EquipSlotIndex == SaveData->SecondaryWeaponSlot)
		{
			if (ItemInfo.HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
			{
				ControlledPS->SetCurrentEquippedSecondaryWeaponSlot(ItemInfo.EquipSlotIndex);

				if (SecondaryWeaponIsEmpty)
				{
					if (E_HandheldType != EHandheldType::TwoHands)
					{
						SecondaryWeapon->SetStaticMesh(ItemInfo.ItemMesh);
						E_HandheldType = EHandheldType::OneHand;
						if (IsValid(CharacterASC))
						{
							ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, ItemInfo, FDungeonGameplayTags::Get().Input_RM, false);
						}
					}
					else
					{
						UnusedWeapon->SetStaticMesh(ItemInfo.ItemMeshOnUnusedSocket);
					}
					SecondaryWeaponIsEmpty = false;
				}
			}
		}
		ControlledPS->AddToBackpack(ItemInfo);
	}

	if (ItemInfo.HandheldType.MatchesTag(DungeonTags.Handheld_Body))
	{
		if (ItemInfo.IsEquiped)
		{
			if (ItemInfo.HandheldType.MatchesTag(DungeonTags.Handheld_Body_Helmet))
			{
				ICharacterInterface::Execute_SetHandheldStaticMeshByType(this, ItemInfo.ItemMesh, FDungeonGameplayTags::Get().Handheld_Body_Helmet);
			}

			ControlledPS->SetAccessorySlotStatus(ItemInfo.EquipSlotIndex, false);
			ControlledPS->EquipAccessory(ItemInfo);
		}
		ControlledPS->AddToBackpack(ItemInfo);
	}
}

void APlayerBase::SetHandheldStaticMeshByType_Implementation(UStaticMesh* InMesh, FGameplayTag HandheldType)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	if (HandheldType != DungeonTags.Handheld_None)
	{
		if (HandheldType.MatchesTag(DungeonTags.Handheld_Secondary))
		{
			SecondaryWeapon->SetStaticMesh(InMesh);
			return;
		}
		if (HandheldType.MatchesTag(DungeonTags.Handheld_1H) || HandheldType.MatchesTag(DungeonTags.Handheld_2H))
		{
			Weapon->SetStaticMesh(InMesh);
			return;
		}
		if (HandheldType.MatchesTag(DungeonTags.Handheld_Body_Helmet))
		{
			Helmet->SetStaticMesh(InMesh);
			return;
		}
	}
}

int32 APlayerBase::GetWeaponIndex_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return  ControlledPS->GetCurrentWeaponInfo().ItemIndex;
}

int32 APlayerBase::GetSecondaryWeaponIndex_Implementation()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return  ControlledPS->GetCurrentSecondaryWeaponInfo().ItemIndex;
}

FGameplayTag APlayerBase::GetSecondaryWeaponHandheldType_Implementation()
{
	if (SecondaryWeapon->GetStaticMesh() != nullptr || UnusedWeapon->GetStaticMesh() != nullptr)
	{
		FGameItemInfo Info = IPlayerInterface::Execute_GetEquipedSecondaryWeaponInfo(this);
		return Info.HandheldType;
	}
	return FGameplayTag();
}

void APlayerBase::GiveWeaponAbilityByTag_Implementation(const FGameItemInfo& Info, FGameplayTag InputTag, bool bClear)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	UDetailedEquipmemtInfo* EquipmentInfomation = UDungeonAbilitySystemLibrary::GetEquipmentInfomation(this);
	FWeaponInfo WeaponInfo = EquipmentInfomation->GetWeaponInfoForItemIndex(Info.ItemIndex);

	if (IsValid(WeaponInfo.WeaponAbility) && !bClear)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(WeaponInfo.WeaponAbility, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		CharacterASC->GiveAbility(AbilitySpec);
	}
	else
	{
		Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(InputTag);
	}

	if (InputTag == DungeonTags.Input_LM)
	{
		if(IsValid(WeaponInfo.SpecialSkill))
		{
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(WeaponInfo.SpecialSkill, 1);
			Spec.DynamicAbilityTags.AddTag(DungeonTags.Input_SpecialSkill);
			CharacterASC->GiveAbility(Spec);
		}
		else
		{
			Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(DungeonTags.Input_SpecialSkill);
		}
	}
}

void APlayerBase::Die_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bDead = true;

	Die_BlueprintImplement();
}

void APlayerBase::SetDurability_Implementation(float InNum, bool bSecondary)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	ControlledPS->SetDurability(InNum, bSecondary);
}

float APlayerBase::GetDurability_Implementation(bool bSecondary)
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	return ControlledPS->GetDurability(bSecondary);
}

void APlayerBase::HandleDie()
{
	UDungeonGameInstance* GI = Cast<UDungeonGameInstance>(GetGameInstance());
	AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this));

	GM->DeleteSlot(GI->LoadSlotName, GI->LoadSlotIndex);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, GM->StartupMap);
}

void APlayerBase::WeaponTypeChanged(FGameplayTag WeaponType)
{
}

int32 APlayerBase::TickForThrow(FVector StartPosition, FVector Velocity)
{
	Spline->ClearSplinePoints();

	FPredictProjectilePathParams Params;
	Params.StartLocation = StartPosition;
	Params.LaunchVelocity = Velocity;
	Params.ProjectileRadius = 3.f;
	Params.ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	Params.DrawDebugTime = 1.f;
	Params.SimFrequency = 15.f;
	Params.MaxSimTime = 5.f;

	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(this, Params, Result);

	int32 Index = 0;
	for (const FPredictProjectilePathPointData& PointData : Result.PathData)
	{
		Spline->AddSplinePointAtIndex(PointData.Location, Index++, ESplineCoordinateSpace::World, true);
	}
	return Result.PathData.Num();
}

void APlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ICharacterInterface::Execute_SetCurrentWeaponType(this, IPlayerInterface::Execute_GetEquipedWeaponInfo(this).WeaponType);

	InitAbilityActorInfo();

	LoadProgress();

	if (AGlobalGameModeBase* GM = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->LoadWorldState(GetWorld());
	}
}

bool APlayerBase::ShouldLoadTransform_Implementation()
{
	return true;
}

void APlayerBase::InitAbilityActorInfo()
{
	AControlledPlayerState* ControlledPS = GetPlayerState<AControlledPlayerState>();
	check(ControlledPS);

	ControlledPS->GetAbilitySystemComponent()->InitAbilityActorInfo(ControlledPS, this);

	CharacterASC = ControlledPS->GetAbilitySystemComponent();
	CharacterAS = ControlledPS->GetAttributeSet();

	CharacterASC->RegisterGameplayTagEvent(FDungeonGameplayTags::Get().Ability_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerBase::HitReactTagChanged);
	CharacterASC->RegisterGameplayTagEvent(FDungeonGameplayTags::Get().Ability_Run, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerBase::RunTagChanged);

	if (AControlledPlayerController* ControlledPC = Cast<AControlledPlayerController>(GetController()))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(ControlledPC->GetHUD()))
		{
			PlayerHUD->InitOverlay(CharacterASC, CharacterAS, ControlledPS, ControlledPC);
		}
	}
}

void APlayerBase::InitializeDefaultAttributes()
{
	AppliedEffectToSelf(BaseAttribute);
	AppliedEffectToSelf(DerivedAttribute);
	AppliedEffectToSelf(VitalAttribute);
}

void APlayerBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bool bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact ? 0 : BaseWalkSpeed;
}

void APlayerBase::RunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		BaseWalkSpeed = RunSpeed;
	}
	else
	{
		BaseWalkSpeed = 100.f;
	}

	if (!CharacterASC->HasMatchingGameplayTag(FDungeonGameplayTags::Get().Ability_HitReact))
	{
		GetCharacterMovement()->MaxWalkSpeed = NewCount ? RunSpeed : 100.f;
	}
}