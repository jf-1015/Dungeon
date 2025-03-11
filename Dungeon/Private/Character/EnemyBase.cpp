// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"
#include "AbilitySystem/CharacterAbilitySystemComponent.h"//
#include "AbilitySystem/CharacterAttributeSet.h"//
#include "AI/EnemyAIController.h"//
#include "BehaviorTree/BehaviorTree.h"//
#include "BehaviorTree/BlackboardComponent.h"//
#include "AbilitySystem/DungeonAbilitySystemLibrary.h"//
#include "GameFramework/CharacterMovementComponent.h"//
#include "Actor/PickupItems.h"//
#include "DungeonGameplayTags.h"//
#include "Interaction/PickupInterface.h"//
#include "Interaction/PlayerInterface.h"//
#include "Data/EnemyEquipmentInfo.h"//
#include "Components/SphereComponent.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "Components//CapsuleComponent.h"//

AEnemyBase::AEnemyBase()
{
	CharacterTag = FDungeonGameplayTags::Get().Character_Enemy;

	CharacterASC = CreateDefaultSubobject<UCharacterAbilitySystemComponent>("EnemyASC");
	CharacterASC->SetIsReplicated(true);
	CharacterASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CharacterAS = CreateDefaultSubobject<UCharacterAttributeSet>("EnemyAS");

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AwakeRange = CreateDefaultSubobject<USphereComponent>("AwakeRange");
	AwakeRange->SetupAttachment(GetRootComponent());

	AwakeRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::AwakeRangeEvent);

	AwakeRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	AwakeRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EnemyAIController = Cast<AEnemyAIController>(NewController);

	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BehaviorTree);

	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReact"), false);
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), bDead);
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsAwake"), IsAwake);

	InitializeHandheld();

	/*Initial Handheld*/
	IEnemyInterface::Execute_SetEnemyHandheldItem(this, WeaponInfo, false);
	IEnemyInterface::Execute_SetEnemyHandheldItem(this, SecondaryWeaponInfo, true);
	/*Initial Handheld*/
}

void AEnemyBase::LoadActor_Implementation()
{
	if (bDead)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (IsValid(EnemyAIController))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
		}
	}

	IEnemyInterface::Execute_SetIsAwake(this, IsAwake);

	IEnemyInterface::Execute_SetEnemyHandheldItem(this, WeaponInfo, false);
	IEnemyInterface::Execute_SetEnemyHandheldItem(this, SecondaryWeaponInfo, true);
}

bool AEnemyBase::ShouldLoadTransform_Implementation()
{
	return true;
}

void AEnemyBase::SetTargetActor_Implementation(AActor* InActor)
{
	TargetActor = InActor;
}

AActor* AEnemyBase::GetTargetActor_Implementation()
{
	return TargetActor;
}

void AEnemyBase::SetEnemyHandheldItem_Implementation(const FGameItemInfo& InInfo, bool IsSecondary)
{
	const FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();


	if (InInfo.ItemIndex != -1)
	{
		if (InInfo.ItemType != DungeonTag.ItemType_Weapon)return;

		if (InInfo.HandheldType.MatchesTag(DungeonTag.Handheld_Secondary))
		{
			SecondaryWeaponInfo = InInfo;

			if (WeaponInfo.HandheldType == DungeonTag.Handheld_2H)
			{
				UnusedWeapon->SetStaticMesh(InInfo.ItemMeshOnUnusedSocket);
				ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), DungeonTag.Input_RM, true);
			}
			else
			{
				SecondaryWeapon->SetStaticMesh(InInfo.ItemMesh);
				ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, InInfo, DungeonTag.Input_RM, false);
			}
			
			if (WeaponInfo.ItemIndex == -1)
			{
				E_HandheldType = EHandheldType::OneHand;
			}
		}
		else
		{
			WeaponInfo = InInfo;

			Weapon->SetStaticMesh(InInfo.ItemMesh);

			if (SecondaryWeaponInfo.ItemIndex != -1 && WeaponInfo.HandheldType == DungeonTag.Handheld_2H)
			{
				UnusedWeapon->SetStaticMesh(SecondaryWeaponInfo.ItemMeshOnUnusedSocket);
				ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), DungeonTag.Input_RM, true);
			}

			ICharacterInterface::Execute_SetWeaponAttachment(this, InInfo);
			ICharacterInterface::Execute_SetCurrentWeaponType(this, InInfo.WeaponType);
			
			ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, InInfo, DungeonTag.Input_LM, false);

			if (InInfo.WeaponType == DungeonTag.Weapon_Range && InInfo.IsLoaded)
			{
				if (IsValid(EnemyAIController) && IEnemyInterface::Execute_CheckAmmunitionIsEnough(this))
				{
					EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsAmmunitionEnough"), true);
				}
			}

			if (InInfo.HandheldType == DungeonTag.Handheld_1H)
			{
				E_HandheldType = EHandheldType::OneHand;
			}
			else
			{
				E_HandheldType = EHandheldType::TwoHands;
			}
		}
	}
	else
	{
		if (IsSecondary)
		{
			SecondaryWeaponInfo = FGameItemInfo();

			SecondaryWeapon->SetStaticMesh(nullptr);

			ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), DungeonTag.Input_RM, true);

			if (WeaponInfo.ItemIndex == -1)
			{
				E_HandheldType = EHandheldType::None;
			}
		}
		else
		{
			if (WeaponInfo.HandheldType == DungeonTag.Handheld_2H)
			{
				UnusedWeapon->SetStaticMesh(nullptr);
				SecondaryWeapon->SetStaticMesh(SecondaryWeaponInfo.ItemMesh);
				ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), DungeonTag.Input_RM, true);
			}

			WeaponInfo = FGameItemInfo();

			Weapon->SetStaticMesh(nullptr);

			ICharacterInterface::Execute_SetCurrentWeaponType(this, DungeonTag.Weapon_Melee);

			ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), DungeonTag.Input_LM, true);
			ICharacterInterface::Execute_GiveWeaponAbilityByTag(this, FGameItemInfo(), DungeonTag.Input_LM, false);

			if (SecondaryWeaponInfo.ItemIndex == -1)
			{
				E_HandheldType = EHandheldType::None;
			}
			else
			{
				E_HandheldType = EHandheldType::OneHand;
			}
		}
	}
}

APickupItems* AEnemyBase::FindNearestWeapon_Implementation(float Radius)
{
	FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();

	bool IsWeaponEmpty = (WeaponInfo.ItemIndex == -1);
	bool IsSecondaryWeaponEmpty = (SecondaryWeaponInfo.ItemIndex == -1);

	if (!IsWeaponEmpty && !IsSecondaryWeaponEmpty)return nullptr;

	FCollisionQueryParams SphereParams;

	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult>OverlapResults;

		World->OverlapMultiByObjectType(
			OverlapResults,
			GetActorLocation(),
			FQuat(),
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams);

		if (!IsWeaponEmpty && IsSecondaryWeaponEmpty)
		{
			if (TargetActor != nullptr && (TargetActor->GetActorLocation() - GetActorLocation()).Length() <= 200)return nullptr;

			if (CurrentWeaponType == DungeonTag.Weapon_Melee || CurrentWeaponType == DungeonTag.Weapon_Range)
			{
				return FilterItemByTag(OverlapResults, DungeonTag.Handheld_Secondary_Shield);
			}
			else if (CurrentWeaponType == DungeonTag.Weapon_Magic)
			{
				return FilterItemByTag(OverlapResults, DungeonTag.Handheld_Secondary_Magic);
			}		
		}
		else
		{
			if (SecondaryWeaponInfo.ItemIndex == -1)
			{
				return FilterItemByTag(OverlapResults, DungeonTag.Handheld_1H, DungeonTag.Handheld_2H);
			}
			else
			{
				return FilterItemByTag(OverlapResults, DungeonTag.Handheld_1H);
			}
		}	
	}
	return nullptr;
}

APickupItems* AEnemyBase::FindNearestConsumable_Implementation(float Radius, int32 ItemIndex)
{
	FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();

	if (CurrentWeaponType != DungeonTag.Weapon_Range)return nullptr;

	float TargetDistance = 9999;
	APickupItems* NearestAmmunition = nullptr;
	int32 ConsumableIndex = 0;

	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult>OverlapResults;
		FCollisionQueryParams SphereParams;

		World->OverlapMultiByObjectType(
			OverlapResults,
			GetActorLocation(),
			FQuat(),
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams);

		for (FOverlapResult& Result : OverlapResults)
		{
			if (Result.GetActor()->Implements<UPickupInterface>())
			{
				APickupItems* InActor = Cast<APickupItems>(Result.GetActor());

				ConsumableIndex = ItemIndex;

				if (InActor->ItemInfo.ItemType == DungeonTag.ItemType_Consume && InActor->ItemInfo.ItemIndex == ConsumableIndex)
				{
					FHitResult HitResult;
					FCollisionQueryParams LineParam;
					LineParam.AddIgnoredActor(InActor);
					LineParam.AddIgnoredActor(this);

					World->LineTraceSingleByChannel(HitResult, GetActorLocation(), InActor->GetActorLocation(), ECollisionChannel::ECC_Visibility, LineParam);

					if (HitResult.GetActor() == nullptr)
					{
						if ((InActor->GetActorLocation() - GetActorLocation()).Length() < TargetDistance)
						{
							TargetDistance = (InActor->GetActorLocation() - GetActorLocation()).Length();
							NearestAmmunition = InActor;
						}
					}
				}
			}
		}
	}

	return NearestAmmunition;
}

TMap<int32, UAnimMontage*> AEnemyBase::GetCountToMontageByHandheldType_Implementation()
{
	FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();

	if (CurrentWeaponType == DungeonTag.Weapon_Melee)
	{
		if (WeaponInfo.ItemIndex == -1)
		{
			return EquipmentAbilitiesAndMontages.CountToMontageNone;
		}
		else if (E_HandheldType == EHandheldType::OneHand)
		{
			return EquipmentAbilitiesAndMontages.CountToMontage1H;
		}
		else if (E_HandheldType == EHandheldType::TwoHands)
		{
			return EquipmentAbilitiesAndMontages.CountToMontage2H;
		}
	}
	return TMap<int32, UAnimMontage*>();
}

UAnimMontage* AEnemyBase::GetShootMontage_Implementation(bool InBool)
{
	FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();

	if (CurrentWeaponType == DungeonTag.Weapon_Magic)
	{
		if (InBool)
		{
			return EquipmentAbilitiesAndMontages.Spellcast_LongMontage;
		}
		return EquipmentAbilitiesAndMontages.SpellcastMontage;
	}
	else if (CurrentWeaponType == DungeonTag.Weapon_Range)
	{
		if (E_HandheldType == EHandheldType::TwoHands)
		{
			return EquipmentAbilitiesAndMontages.ShootMontage_2H;
		}
		return EquipmentAbilitiesAndMontages.ShootMontage;
	}
	return nullptr;
}

UAnimMontage* AEnemyBase::GetReloadMontage_Implementation()
{
	return EquipmentAbilitiesAndMontages.ReloadMontage;
}

UAnimMontage* AEnemyBase::GetPickupMontage_Implementation()
{
	return PickupMontage;
}

bool AEnemyBase::GetIsBlocking_Implementation()
{
	return IsBlocking;
}

void AEnemyBase::SetIsBlocking_Implementation(bool InBool)
{
	IsBlocking = InBool;
}

bool AEnemyBase::GetIsInterrupt_Implementation()
{
	return IsInterrupt;
}

void AEnemyBase::SetIsInterrupt_Implementation(bool InBool)
{
	IsInterrupt = InBool;
}

void AEnemyBase::SetIsAwake_Implementation(bool InBool)
{
	IsAwake = InBool;
	if (IsValid(EnemyAIController))
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsAwake"), IsAwake);
	}
}

bool AEnemyBase::GetIsAwake_Implementation()
{
	return IsAwake;
}

UAnimMontage* AEnemyBase::GetAwakeMontage_Implementation(int32 InIndex)
{
	if(IsInactive_Floor)
	{
		return AwakeMontages_Floor[InIndex];
	}
	else
	{
		return AwakeMontage_Stand;
	}
}

bool AEnemyBase::GetInactiveState_Implementation()
{
	return IsInactive_Floor;
}

bool AEnemyBase::GetIsLoaded_Implementation()
{
	return WeaponInfo.IsLoaded;
}

void AEnemyBase::SetIsLoaded_Implementation(bool InBool)
{
	WeaponInfo.IsLoaded = InBool;

	if (!IEnemyInterface::Execute_CheckAmmunitionIsEnough(this) && !InBool)
	{
		if (IsValid(EnemyAIController))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsAmmunitionEnough"), false);
		}
	}

	if(InBool)
	{
		IEnemyInterface::Execute_SetAmountOfAmmunition(this, -1);
	}
}

bool AEnemyBase::CheckAmmunitionIsEnough_Implementation()
{
	if (WeaponInfo.IsLoaded)
	{
		return true;
	}

	return AmountOfArrows > 0 ? true : false;
}

void AEnemyBase::SetAmountOfAmmunition_Implementation(int32 InNum)
{
	AmountOfArrows += InNum;

	if (AmountOfArrows <= 0 && !WeaponInfo.IsLoaded)
	{
		if (IsValid(EnemyAIController))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsAmmunitionEnough"), false);
		}
	}
	else
	{
		if (IsValid(EnemyAIController))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsAmmunitionEnough"), true);
		}
	}
}

void AEnemyBase::DicardCurrentWeapon_Implementation(bool bSecondary)
{
	FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();

	if (!IsValid(ItemClass))return;

	FTransform Transform;
	FVector BaseLocation = GetActorLocation() + GetActorForwardVector() * 15;

	BaseLocation = BaseLocation.RotateAngleAxis(FMath::RandRange(-5.f, 5.f), FVector::UpVector);

	Transform.SetLocation(BaseLocation);
	Transform.SetRotation(GetActorQuat());

	if(bSecondary)
	{
		APickupItems* Item = GetWorld()->SpawnActorDeferred<APickupItems>(ItemClass->GetSubclassByItemIndex(SecondaryWeaponInfo.ItemIndex), Transform);

		Item->ItemInfo.Durability = SecondaryWeaponInfo.Durability;

		Item->FinishSpawning(Transform);

		IEnemyInterface::Execute_SetEnemyHandheldItem(this, FGameItemInfo(), true);
	}
	else
	{
		APickupItems* Item = GetWorld()->SpawnActorDeferred<APickupItems>(ItemClass->GetSubclassByItemIndex(WeaponInfo.ItemIndex), Transform);	
		Item->ItemInfo.Durability = WeaponInfo.Durability;
		if (Item->ItemInfo.WeaponType == DungeonTag.Weapon_Range)
		{
			Item->ItemInfo.IsLoaded = WeaponInfo.IsLoaded;
		}
		Item->FinishSpawning(Transform);

		IEnemyInterface::Execute_SetEnemyHandheldItem(this, FGameItemInfo(), false);
	}
}

void AEnemyBase::SetWeaponInfo_Implementation(const FGameItemInfo& InInfo)
{
	WeaponInfo = InInfo;
}

FGameItemInfo AEnemyBase::GetWeaponInfo_Implementation()
{
	return WeaponInfo;
}

void AEnemyBase::SetSecondaryWeaponInfo_Implementation(const FGameItemInfo& InInfo)
{
	SecondaryWeaponInfo = InInfo;
}

FGameItemInfo AEnemyBase::GetSecondaryWeaponInfo_Implementation()
{
	return SecondaryWeaponInfo;
}

APickupItems* AEnemyBase::FilterItemByTag(TArray<FOverlapResult>& OverlapResults, FGameplayTag FilterTag_1, FGameplayTag FilterTag_2)
{
	FDungeonGameplayTags DungeonTag = FDungeonGameplayTags::Get();
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);

	float TargetDistance = 9999;
	APickupItems* NearestWeapon = nullptr;

	for (FOverlapResult& Result : OverlapResults)
	{
		if (Result.GetActor()->Implements<UPickupInterface>())
		{
			APickupItems* InActor = Cast<APickupItems>(Result.GetActor());

			if (InActor->ItemInfo.Durability == 0)continue;

			//If TargetActor is valid and in the Range, but not have enough Ammunition, ignore the Range Weapon of corresponding Ammunition
			if (TargetActor != nullptr && (TargetActor->GetActorLocation() - GetActorLocation()).Length() <= 200)
			{
				UEnemyEquipmentInfo* EnemyEquipmentInfomation = UDungeonAbilitySystemLibrary::GetEnemyEquipmentInfomation(this);
				FEnemyWeaponInfo EnemyWeaponInfo = EnemyEquipmentInfomation->GetEnemyWeaponInfoForItemIndex(InActor->ItemInfo.ItemIndex);

				if (InActor->ItemInfo.WeaponType == DungeonTag.Weapon_Range)
				{
					if (AmountOfArrows == 0)
					{
						continue;
					}
				}
			}

			if (InActor->ItemInfo.ItemType == DungeonTag.ItemType_Weapon 
				&& 
				(InActor->ItemInfo.HandheldType.MatchesTag(FilterTag_1)|| InActor->ItemInfo.HandheldType.MatchesTag(FilterTag_2)))
			{
				FHitResult HitResult;
				FCollisionQueryParams LineParam;
				LineParam.AddIgnoredActor(InActor);
				LineParam.AddIgnoredActor(this);

				World->LineTraceSingleByChannel(HitResult, GetActorLocation(), InActor->GetActorLocation(), ECollisionChannel::ECC_Visibility, LineParam);

				if (HitResult.GetActor() == nullptr)
				{
					if ((InActor->GetActorLocation() - GetActorLocation()).Length() < TargetDistance)
					{
						TargetDistance = (InActor->GetActorLocation() - GetActorLocation()).Length();
						NearestWeapon = InActor;
					}
				}
			}
		}
	}
	return NearestWeapon;
}

int32 AEnemyBase::GetWeaponIndex_Implementation()
{
	return WeaponInfo.ItemIndex;
}

int32 AEnemyBase::GetSecondaryWeaponIndex_Implementation()
{
	return SecondaryWeaponInfo.ItemIndex;
}

void AEnemyBase::SetCurrentWeaponType_Implementation(FGameplayTag InType)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	CurrentWeaponType = InType;

	if (CurrentWeaponType == DungeonTags.Weapon_Melee)
	{
		if(IsValid(EnemyAIController))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsMelee"), true);
		}
	}
	else
	{
		if (IsValid(EnemyAIController))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsMelee"), false);

			if (CurrentWeaponType == DungeonTags.Weapon_Range)
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsNeedAmmunition"), true);
			}
			else
			{
				EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsNeedAmmunition"), false);
			}
		}
	}

	OnWeaponTypeChange.Broadcast(InType);
}

FGameplayTag AEnemyBase::GetSecondaryWeaponHandheldType_Implementation()
{
	return SecondaryWeaponInfo.HandheldType;
}

void AEnemyBase::GiveWeaponAbilityByTag_Implementation(const FGameItemInfo& Info, FGameplayTag InputTag, bool bClear)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	UEnemyEquipmentInfo* EnemyEquipmentInfomation = UDungeonAbilitySystemLibrary::GetEnemyEquipmentInfomation(this);
	FEnemyWeaponInfo EnemyWeaponInfo = EnemyEquipmentInfomation->GetEnemyWeaponInfoForItemIndex(Info.ItemIndex);

	if (IsValid(EnemyWeaponInfo.WeaponAbility) && !bClear)
	{
		Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(InputTag);

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(EnemyWeaponInfo.WeaponAbility, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		CharacterASC->GiveAbility(AbilitySpec);
	}
	else
	{
		Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(InputTag);
	}

	if (InputTag == DungeonTags.Input_LM)
	{
		if (IsValid(EnemyWeaponInfo.SpecialSkill))
		{
			Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(DungeonTags.Input_SpecialSkill);

			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(EnemyWeaponInfo.SpecialSkill, 1);
			Spec.DynamicAbilityTags.AddTag(DungeonTags.Input_SpecialSkill);
			CharacterASC->GiveAbility(Spec);
		}
		else
		{
			Cast<UCharacterAbilitySystemComponent>(CharacterASC)->RmoveAbilityByInputTag(DungeonTags.Input_SpecialSkill);
		}
	}
}

void AEnemyBase::Die_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bDead = true;

	if (IsValid(EnemyAIController))
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	}

	if (WeaponInfo.ItemIndex != -1)
	{
		IEnemyInterface::Execute_DicardCurrentWeapon(this, false);
	}
	if (SecondaryWeaponInfo.ItemIndex != -1)
	{
		IEnemyInterface::Execute_DicardCurrentWeapon(this, true);
	}
}

void AEnemyBase::SetDurability_Implementation(float InNum, bool bSecondary)
{
	if (bSecondary)
	{
		SecondaryWeaponInfo.Durability += InNum;
		if (SecondaryWeaponInfo.Durability == 0)
		{
			IEnemyInterface::Execute_DicardCurrentWeapon(this, true);
		}
	}
	else
	{
		WeaponInfo.Durability += InNum;
		if (WeaponInfo.Durability == 0)
		{
			IEnemyInterface::Execute_DicardCurrentWeapon(this, false);
		}
	}
}

float AEnemyBase::GetDurability_Implementation(bool bSecondary)
{
	if (bSecondary)
	{
		return SecondaryWeaponInfo.Durability;
	}
	else
	{
		return WeaponInfo.Durability;
	}
}

void AEnemyBase::SaveEnemyVitalAttribute_Implementation(FSavedEnemyVitalAttribute& InAttributeInfo)
{
	InAttributeInfo.Health = UCharacterAttributeSet::GetHealthAttribute().GetNumericValue(GetAttributeSet());
	InAttributeInfo.CurrentResilience = UCharacterAttributeSet::GetCurrentResilienceAttribute().GetNumericValue(GetAttributeSet());
	InAttributeInfo.Energy = UCharacterAttributeSet::GetEnergyAttribute().GetNumericValue(GetAttributeSet());
}

void AEnemyBase::LoadAttribute_Implementation(FSavedEnemyVitalAttribute InAttributeInfo)
{
	SavedVitalAttribute = InAttributeInfo;
	bVitalIsSet = true;
}

void AEnemyBase::InitializeHandheld()
{
	if (!IsValid(ItemClass))return;

	if (WeaponInfo.ItemIndex != -1)
	{
		TSubclassOf<APickupItems> Class = ItemClass->GetSubclassByItemIndex(WeaponInfo.ItemIndex);
		APickupItems* Item = Cast<APickupItems>(Class->GetDefaultObject());

		WeaponInfo = Item->ItemInfo;
	}
	if (SecondaryWeaponInfo.ItemIndex != -1)
	{
		TSubclassOf<APickupItems> Class = ItemClass->GetSubclassByItemIndex(SecondaryWeaponInfo.ItemIndex);
		APickupItems* Item = Cast<APickupItems>(Class->GetDefaultObject());

		SecondaryWeaponInfo = Item->ItemInfo;
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	/*Ability*/
	UDungeonAbilitySystemLibrary::GiveDefaultAbilities(this, CharacterClass, CharacterASC);
	/*Ability*/

	/*HitReact*/
	CharacterASC->RegisterGameplayTagEvent(FDungeonGameplayTags::Get().Ability_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyBase::HitReactTagChanged);
	/*HitReact*/
}

void AEnemyBase::InitAbilityActorInfo()
{
	CharacterASC->InitAbilityActorInfo(this, this);
	UDungeonAbilitySystemLibrary::InitialCharacterAttribute(this, CharacterClass, Level, CharacterASC, bVitalIsSet, SavedVitalAttribute);
}

void AEnemyBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bool bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact ? 0 : BaseWalkSpeed;

	if(IsValid(EnemyAIController))
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReact"), bHitReact);
	}
}

void AEnemyBase::AwakeRangeEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	if (OtherActor->Implements<UPlayerInterface>())
	{
		if (IsAwake)
		{
			AwakeRange->OnComponentBeginOverlap.RemoveAll(this);
			return;
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, DungeonTags.Ability_Awake, FGameplayEventData());
		AwakeRange->OnComponentBeginOverlap.RemoveAll(this);
	}
}