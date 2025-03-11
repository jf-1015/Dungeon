// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DungeonAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"//
#include "ControlledPlayerState.h"//
#include "AbilitySystemComponent.h"//
#include "UI/HUD/PlayerHUD.h"//
#include "UI/WidgetController/AttributeMenuWidgetController.h"//
#include "UI/WidgetController/BackpackWidgetController.h"//
#include "GameMode/GlobalGameModeBase.h"//
#include "Interaction/CharacterInterface.h"//
#include "Interaction/EnemyInterface.h"//
#include "DungeonGameplayTags.h"//
#include "AbilitySystem/DungeonAbilityTypes.h"//
#include "AbilitySystemBlueprintLibrary.h"//

UAttributeMenuWidgetController* UDungeonAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			AControlledPlayerState* ControlledPS = PC->GetPlayerState<AControlledPlayerState>();
			UAbilitySystemComponent* ASC = ControlledPS->GetAbilitySystemComponent();
			UAttributeSet* AS = ControlledPS->GetAttributeSet();

			FWidgetControllerParams Param;
			Param.PlayerController = PC;
			Param.PlayerState = ControlledPS;
			Param.AbilitySystemComponent = ASC;
			Param.AttributeSet = AS;

			return PlayerHUD->GetAttributeMenuWidgetController(Param);

		}

	}
	return nullptr;
}

UBackpackWidgetController* UDungeonAbilitySystemLibrary::GetBackpackWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			AControlledPlayerState* ControlledPS = PC->GetPlayerState<AControlledPlayerState>();
			UAbilitySystemComponent* ASC = ControlledPS->GetAbilitySystemComponent();
			UAttributeSet* AS = ControlledPS->GetAttributeSet();

			FWidgetControllerParams Param;
			Param.PlayerController = PC;
			Param.PlayerState = ControlledPS;
			Param.AbilitySystemComponent = ASC;
			Param.AttributeSet = AS;

			return PlayerHUD->GetBackpackWidgetController(Param);

		}

	}
	return nullptr;
}

UEquippedWidgetController* UDungeonAbilitySystemLibrary::GetEquippedWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			AControlledPlayerState* ControlledPS = PC->GetPlayerState<AControlledPlayerState>();
			UAbilitySystemComponent* ASC = ControlledPS->GetAbilitySystemComponent();
			UAttributeSet* AS = ControlledPS->GetAttributeSet();

			FWidgetControllerParams Param;
			Param.PlayerController = PC;
			Param.PlayerState = ControlledPS;
			Param.AbilitySystemComponent = ASC;
			Param.AttributeSet = AS;

			return PlayerHUD->GetEquippedWidgetController(Param);

		}

	}
	return nullptr;
}

UShopWidgetController* UDungeonAbilitySystemLibrary::GetShopWidgetController(const UObject* WorldContextObject, AActor* InActor)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			AControlledPlayerState* ControlledPS = PC->GetPlayerState<AControlledPlayerState>();
			UAbilitySystemComponent* ASC = ControlledPS->GetAbilitySystemComponent();
			UAttributeSet* AS = ControlledPS->GetAttributeSet();

			FWidgetControllerParams Param;
			Param.PlayerController = PC;
			Param.PlayerState = ControlledPS;
			Param.AbilitySystemComponent = ASC;
			Param.AttributeSet = AS;

			return PlayerHUD->GetShopWidgetController(InActor, Param);
		}
	}
	return nullptr;
}

bool UDungeonAbilitySystemLibrary::IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDungeonGameplayEffectContext* DungeonContext = static_cast<const FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DungeonContext->IsBlocked();
	}

	return false;
}

bool UDungeonAbilitySystemLibrary::ShoudlReactBlock(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDungeonGameplayEffectContext* DungeonContext = static_cast<const FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DungeonContext->ShouldReactBlock();
	}

	return false;
}

bool UDungeonAbilitySystemLibrary::IsThrowable(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDungeonGameplayEffectContext* DungeonContext = static_cast<const FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DungeonContext->IsThrowable();
	}
	return false;
}

float UDungeonAbilitySystemLibrary::GetResilienceCut(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDungeonGameplayEffectContext* DungeonContext = static_cast<const FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DungeonContext->GetResilienceCut();
	}

	return 0.f;
}

float UDungeonAbilitySystemLibrary::GetEnduranceCut(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDungeonGameplayEffectContext* DungeonContext = static_cast<const FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DungeonContext->GetEnduranceCut();
	}

	return 0.f;
}

void UDungeonAbilitySystemLibrary::SetIsBlocked(FGameplayEffectContextHandle& EffectContextHandle, bool InBool)
{
	if (FDungeonGameplayEffectContext* DungeonContext = static_cast<FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DungeonContext->SetIsBlocked(InBool);
	}
}

void UDungeonAbilitySystemLibrary::SetReactBlock(FGameplayEffectContextHandle& EffectContextHandle, bool InBool)
{
	if (FDungeonGameplayEffectContext* DungeonContext = static_cast<FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DungeonContext->SetReactBlock(InBool);
	}
}

void UDungeonAbilitySystemLibrary::SetIsThrowable(FGameplayEffectContextHandle& EffectContextHandle, bool InBool)
{
	if (FDungeonGameplayEffectContext* DungeonContext = static_cast<FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DungeonContext->SetIsThrowable(InBool);
	}
}

void UDungeonAbilitySystemLibrary::SetResilienceCut(FGameplayEffectContextHandle& EffectContextHandle, float InFloat)
{
	if (FDungeonGameplayEffectContext* DungeonContext = static_cast<FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DungeonContext->SetResilienceCut(InFloat);
	}
}

void UDungeonAbilitySystemLibrary::SetEnduranceCut(FGameplayEffectContextHandle& EffectContextHandle, float InFloat)
{
	if (FDungeonGameplayEffectContext* DungeonContext = static_cast<FDungeonGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DungeonContext->SetEnduranceCut(InFloat);
	}
}

UDetailedEquipmemtInfo* UDungeonAbilitySystemLibrary::GetEquipmentInfomation(const UObject* WorldContextObject)
{
	AGlobalGameModeBase* GameMode = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr)return nullptr;
	return GameMode->EquipmentInfomation;
}

UEnemyEquipmentInfo* UDungeonAbilitySystemLibrary::GetEnemyEquipmentInfomation(const UObject* WorldContextObject)
{
	AGlobalGameModeBase* GameMode = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr)return nullptr;
	return GameMode->EnemyEquipmentInfomation;
}

UCharacterClassInfo* UDungeonAbilitySystemLibrary::GetCharacterClassInfomation(const UObject* WorldContextObject)
{
	;AGlobalGameModeBase* GameMode = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr)return nullptr;
	return GameMode->CharacterClassInfomation;
}

UItemClass* UDungeonAbilitySystemLibrary::GetItemClass(const UObject* WorldContextObject)
{
	AGlobalGameModeBase* GameMode = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr)return nullptr;
	return GameMode->ItemClass;
}

UProjectileClass* UDungeonAbilitySystemLibrary::GetProjectileClass(const UObject* WorldContextObject)
{
	AGlobalGameModeBase* GameMode = Cast<AGlobalGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr)return nullptr;
	return GameMode->ProjectileClass;
}

void UDungeonAbilitySystemLibrary::InitialCharacterAttribute(const UObject* WorldContextObject, 
	ECharacterClass InClass, float Level, UAbilitySystemComponent* InASC, 
	bool bVitalIsSet, FSavedEnemyVitalAttribute InSavedAttribute)
{
	const FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfomation(WorldContextObject);
	if (CharacterClassInfo == nullptr)return;
	FSPCharacterInfo SPCharacterInfo = CharacterClassInfo->FindSPInfoForCharacterClass(InClass);

	AActor* AvatarActor = InASC->GetAvatarActor();

	FGameplayEffectContextHandle BaseAttributeContextHandle = InASC->MakeEffectContext();
	BaseAttributeContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle BaseAttributeSpecHandle = InASC->MakeOutgoingSpec(SPCharacterInfo.BaseAttribute, Level, BaseAttributeContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*BaseAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle DerivedAttributeContextHandle = InASC->MakeEffectContext();
	DerivedAttributeContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle DerivedAttributeSpecHandle = InASC->MakeOutgoingSpec(CharacterClassInfo->BaseDerivedAttribute, Level, DerivedAttributeContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*DerivedAttributeSpecHandle.Data.Get());

	if(!bVitalIsSet)
	{
		FGameplayEffectContextHandle VitalAttributeContextHandle = InASC->MakeEffectContext();
		VitalAttributeContextHandle.AddSourceObject(AvatarActor);
		FGameplayEffectSpecHandle VitalAttributeSpecHandle = InASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttribute, Level, VitalAttributeContextHandle);
		InASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
	}
	else
	{
		FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
		ContextHandle.AddSourceObject(AvatarActor);
		FGameplayEffectSpecHandle VitalSpecHandle = InASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttribute_SetByCaller, 1.f, ContextHandle);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Health, InSavedAttribute.Health);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Mana, 100.f);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Energy, InSavedAttribute.Energy);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_CurrentResilience, InSavedAttribute.CurrentResilience);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(VitalSpecHandle, DungeonTags.Attribute_Vital_Food, 100.f);

		InASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
	}
}

void UDungeonAbilitySystemLibrary::GiveDefaultAbilities(const UObject* WorldContextObject, ECharacterClass InClass, UAbilitySystemComponent* InASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfomation(WorldContextObject);
	if (CharacterClassInfo == nullptr)return;

	for (TSubclassOf<UGameplayAbility>CommonAbility : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		InASC->GiveAbility(AbilitySpec);
	}
}

void UDungeonAbilitySystemLibrary::GetLiveCharactersInRadiusByTag(const UObject* WorldContextObject,
	TArray<AActor*>& OutActors,
	const TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector SphereOrigin,
	const FGameplayTag TargetTag)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult>OverlapResults;

		World->OverlapMultiByObjectType(
			OverlapResults,
			SphereOrigin,
			FQuat(),
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams);

		for (FOverlapResult& Result : OverlapResults)
		{
			if (Result.GetActor()->Implements<UCharacterInterface>() && !ICharacterInterface::Execute_IsDead(Result.GetActor()))
			{
				if (ICharacterInterface::Execute_GetCharacterTag(Result.GetActor()) == TargetTag)
				{
					if (TargetTag == FDungeonGameplayTags::Get().Character_Enemy)
					{
						if (!IEnemyInterface::Execute_GetIsAwake(Result.GetActor()))continue;
					}
					OutActors.AddUnique(Result.GetActor());
				}
			}
		}
	}
}

void UDungeonAbilitySystemLibrary::GetClosestCharacterInSightByTag(const UObject* WorldContextObject,
	AActor*& OutActor,
	const TArray<AActor*>& ActorsToIgnore,
	float Sight,
	const FVector Origin,
	const FGameplayTag TargetTag)
{
	TArray<AActor*>ActorsInSight;
	GetLiveCharactersInRadiusByTag(WorldContextObject, ActorsInSight, ActorsToIgnore, Sight, Origin, TargetTag);

	float TargetDistance = 9999;

	for (AActor* Actor : ActorsInSight)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			FHitResult HitResult;
			FCollisionQueryParams LineParam;
			LineParam.AddIgnoredActor(Actor);
			LineParam.AddIgnoredActors(ActorsToIgnore);
			World->LineTraceSingleByChannel(HitResult, Origin, Actor->GetActorLocation(), ECollisionChannel::ECC_Visibility, LineParam);

			if (HitResult.GetActor() == nullptr)
			{
				if ((Actor->GetActorLocation() - Origin).Length() < TargetDistance)
				{
					TargetDistance = (Actor->GetActorLocation() - Origin).Length();
					OutActor = Actor;
				}
			}

		}
	}
}

FGameplayEffectContextHandle UDungeonAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	FDungeonGameplayTags DungeonTags = FDungeonGameplayTags::Get();
	AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	SetIsBlocked(EffectContextHandle, Params.bBlcoked);
	SetReactBlock(EffectContextHandle, Params.bReactBlock);
	SetIsThrowable(EffectContextHandle, Params.bThrowable);

	FGameplayEffectSpecHandle SpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(Params.DamageEffectClass, 1.f, EffectContextHandle);

	if(Params.bThrowable)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DungeonTags.Throwable_BaseDamage, Params.BaseDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DungeonTags.Throwable_PowCE, Params.PowCE);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DungeonTags.Throwable_DexCE, Params.DexCE);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DungeonTags.Throwable_IntCE, Params.IntCE);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DungeonTags.Throwable_ResilienceCut, Params.ResilienceCut);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DungeonTags.Throwable_Durability, Params.Durability);
	}

	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return EffectContextHandle;
}