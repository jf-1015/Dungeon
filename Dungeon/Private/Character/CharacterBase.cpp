// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"//
#include "AbilitySystem/CharacterAbilitySystemComponent.h"//
#include "DungeonGameplayTags.h"//
#include "Data/DungeonItemInfo.h"//
#include "Components/BoxComponent.h"//
#include "Kismet/GameplayStatics.h"//
#include "AbilitySystemBlueprintLibrary.h"//
#include "Interaction/ProjectileInterface.h"//

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));

	Weapon->SetGenerateOverlapEvents(false);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->SetCollisionResponseToAllChannels(ECR_Ignore);

	SecondaryWeapon = CreateDefaultSubobject<UStaticMeshComponent>("SecondaryWeapon");
	SecondaryWeapon->SetupAttachment(GetMesh(), FName("SecondaryWeaponSocket"));

	UnusedWeapon = CreateDefaultSubobject<UStaticMeshComponent>("UnusedWeapon");
	UnusedWeapon->SetupAttachment(GetMesh(), FName("UnusedWeaponSocket"));

	Helmet = CreateDefaultSubobject<UStaticMeshComponent>("Helmet");
	Helmet->SetupAttachment(GetMesh(), FName("HelmetSocket"));

	WhenFallingEnd.AddDynamic(this, &ACharacterBase::FallingEnd);
}

void ACharacterBase::TickForFalling()
{
	if (ICharacterInterface::Execute_GetIsFalling(this))
	{
		FallingTime += UGameplayStatics::GetWorldDeltaSeconds(this);
	}
	else
	{
		FallingTime = 0.f;
	}
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return CharacterASC;
}

FGameplayTag ACharacterBase::GetCharacterTag_Implementation()
{
	return CharacterTag;
}

void ACharacterBase::SetCharacterTag_Implementation(FGameplayTag InTag)
{
	CharacterTag = InTag;
}

UStaticMeshComponent* ACharacterBase::GetWeaponStaticMeshComponent_Implementation()
{
	if (IsValid(Weapon->GetStaticMesh()))
	{
		return Weapon;
	}
	return nullptr;
}

UStaticMeshComponent* ACharacterBase::GetSecondaryWeaponStaticMeshComponent_Implementation()
{
	if (IsValid(SecondaryWeapon->GetStaticMesh()))
	{
		return SecondaryWeapon;
	}
	return nullptr;
}

void ACharacterBase::SetEHandheldType_Implementation(EHandheldType InE_HandheldType)
{
	E_HandheldType = InE_HandheldType;
}

EHandheldType ACharacterBase::GetEHandheldType_Implementation()
{
	return E_HandheldType;
}

bool ACharacterBase::GetWeaponIsEmpty_Implementation()
{
	return WeaponIsEmpty;
}

bool ACharacterBase::GetSecondaryWeaponIsEmpty_Implementation()
{
	return SecondaryWeaponIsEmpty;
}

bool ACharacterBase::IsDead_Implementation()
{
	return bDead;
}

bool ACharacterBase::GetIsBlocking_Implementation()
{
	if (IsValid(CharacterASC))
	{
		return CharacterASC->HasMatchingGameplayTag(FDungeonGameplayTags::Get().Ability_Blocking);
	}
	return false;
}

void ACharacterBase::SetIsBlocking_Implementation(bool InBool)
{
}

bool ACharacterBase::GetIsSpinning_Implementation()
{
	return IsSpining;
}

void ACharacterBase::SetIsSpinning_Implementation(bool InBool)
{
	IsSpining = InBool;
}

USkeletalMeshComponent* ACharacterBase::GetCharacterMesh_Implementation()
{
	return GetMesh();
}

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation(int32 Index)
{
	switch (Index)
	{
	case 1:return HitReactMontage_Normal; break;
	case 2:return HitReactMontage_Blocking; break;
	case 3:return HitReactMontage_Long; break;
	default:return HitReactMontage_Normal;break;
	}
}

void ACharacterBase::BroadcastBlockingEndDelegate_Implementation()
{
	OnBlockingEnd.Broadcast();
}

void ACharacterBase::BroadcastSpinningEndDelegate_Implementation()
{
	OnSpinningEnd.Broadcast();
}

void ACharacterBase::SetWeaponIsEmpty_Implementation(bool InBool)
{
	WeaponIsEmpty = InBool;
}

void ACharacterBase::SetSecondaryWeaponIsEmpty_Implementation(bool InBool)
{
	SecondaryWeaponIsEmpty = InBool;
}

void ACharacterBase::SetPreVelocity_Implementation(FVector InVector)
{
	PreVelocity = InVector;
}

FVector ACharacterBase::GetPreVelocity_Implementation()
{
	return PreVelocity;
}

void ACharacterBase::SetWeaponAttachment_Implementation(const FGameItemInfo& WeaponInfo)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	FGameplayTag WeaponType = WeaponInfo.WeaponType;

	ICharacterInterface::Execute_SetCurrentWeaponType(this, WeaponType);

	if (WeaponType == DungeonTags.Weapon_Melee || WeaponType == DungeonTags.Weapon_Magic)
	{	
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket"));
	}
	else if (WeaponType == DungeonTags.Weapon_Range)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("RangedWeaponSocket"));
	}	
}

FGameplayTag ACharacterBase::GetCurrentWeaponType_Implementation()
{
	return CurrentWeaponType;
}

void ACharacterBase::SetCurrentWeaponType_Implementation(FGameplayTag InType)
{
	CurrentWeaponType = InType;

	OnWeaponTypeChange.Broadcast(InType);
}

void ACharacterBase::SetIsFalling_Implementation(bool InBool)
{
	if (IsFalling == false && InBool == true)
	{
		OnBlockingEnd.Broadcast();
	}
	if (IsFalling == true && InBool == false)
	{
		WhenFallingEnd.Broadcast(FallingTime);
	}
	IsFalling = InBool;
}

bool ACharacterBase::GetIsFalling_Implementation()
{
	return IsFalling;
}

void ACharacterBase::Die_Implementation()
{
}

bool ACharacterBase::GetIsDodge_Implementation()
{
	if (CharacterASC->HasMatchingGameplayTag(FDungeonGameplayTags::Get().Ability_Dodge))
	{
		return true;
	}
	return false;
}

void ACharacterBase::BroadcastTraceCheckEndDelegate_Implementation()
{
	OnTraceCheckEnd.Broadcast();
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void ACharacterBase::RunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void ACharacterBase::DodgeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void ACharacterBase::AppliedEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitializeDefaultAttributes()
{
}

void ACharacterBase::AddCharacterAbilities()
{
	UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponent());
	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(PassiveAbilities);
}

void ACharacterBase::FallingEnd(float Time)
{
	FGameplayEventData Data;
	if (Time > 1.4f)
	{
		Data.EventMagnitude = 1.f;
	}
	else
	{
		Data.EventMagnitude = 0.f;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FDungeonGameplayTags::Get().Montage_Land, Data);
}