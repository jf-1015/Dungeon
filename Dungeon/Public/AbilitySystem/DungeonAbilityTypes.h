#pragma once

#include "GameplayEffectTypes.h"
#include "DungeonAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject>WorldContext = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect>DamageEffectClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>SourceAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>TargetAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bBlcoked = false;

	UPROPERTY(BlueprintReadWrite)
	bool bReactBlock = true;

	UPROPERTY(BlueprintReadWrite)
	bool bThrowable = false;

	/*Throwable*/
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float PowCE = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DexCE = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float IntCE = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float ResilienceCut = 0.f;

	UPROPERTY(BlueprintReadWrite)
	int32 Durability = 1;
	/*Throwable*/
};

USTRUCT(BlueprintType)
struct FDungeonGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsBlocked()const { return bBlocked; }
	bool ShouldReactBlock()const { return bReactBlock; }
	bool IsThrowable()const { return bThrowable; }
	float GetResilienceCut()const { return ResilienceCut; }
	float GetEnduranceCut()const { return EnduranceCut; }

	void SetIsBlocked(bool InBool) { bBlocked = InBool; }
	void SetReactBlock(bool InBool) { bReactBlock = InBool; }
	void SetIsThrowable(bool InBool) { bThrowable = InBool; }
	void SetResilienceCut(float Infloat) { ResilienceCut = Infloat; }
	void SetEnduranceCut(float Infloat) { EnduranceCut = Infloat; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FDungeonGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FDungeonGameplayEffectContext* Duplicate() const
	{
		FDungeonGameplayEffectContext* NewContext = new FDungeonGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bBlocked = false;

	UPROPERTY()
	bool bReactBlock = false;

	UPROPERTY()
	bool bThrowable = false;

	UPROPERTY()
	float ResilienceCut = 0.f;

	UPROPERTY()
	float EnduranceCut = 0.f;

};

template<>
struct TStructOpsTypeTraits<FDungeonGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FDungeonGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};

};