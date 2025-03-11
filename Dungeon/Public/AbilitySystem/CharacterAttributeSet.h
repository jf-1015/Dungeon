// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"//
#include "CharacterAttributeSet.generated.h"

USTRUCT()
struct FEffectProperties
{
    GENERATED_BODY();

    FEffectProperties(){}

    FGameplayEffectContextHandle ContextHandle;

    UPROPERTY()
    UAbilitySystemComponent* SourceASC = nullptr;

    UPROPERTY()
    AActor* SourceAvatarActor = nullptr;

    UPROPERTY()
    AController* SourceController = nullptr;

    UPROPERTY()
    ACharacter* SourceCharacter = nullptr;

    UPROPERTY()
    UAbilitySystemComponent* TargetASC = nullptr;

    UPROPERTY()
    AActor* TargetAvatarActor = nullptr;

    UPROPERTY()
    AController* TargetController = nullptr;

    UPROPERTY()
    ACharacter* TargetCharacter = nullptr;

};

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class DUNGEON_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

    UCharacterAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)override;

    TMap <FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>TagToAttribute;
    
    /*Vital Attribute*/
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Energy, Category = "Vital Attributes")
    FGameplayAttributeData Energy;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Energy);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_CurrentResilience, Category = "Vital Attributes")
    FGameplayAttributeData CurrentResilience;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, CurrentResilience);
    /*Vital Attribute*/

    /*Base Attribute*/
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Vitality, Category = "Base Attributes")
    FGameplayAttributeData Vitality;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Vitality);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Power, Category = "Base Attributes")
    FGameplayAttributeData Power;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Power);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Dexterity, Category = "Base Attributes")
    FGameplayAttributeData Dexterity;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Dexterity);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Intelligence, Category = "Base Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Intelligence);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Endurance, Category = "Base Attributes")
    FGameplayAttributeData Endurance;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Endurance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Resilience, Category = "Base Attributes")
    FGameplayAttributeData Resilience;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Resilience);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Defence, Category = "Base Attributes")
    FGameplayAttributeData Defence;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Defence);
    /*Base Attribute*/

    /*Base Derived Attribute*/
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_MaxHealth, Category = "Base Derived Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_MaxMana, Category = "Base Derived Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxMana);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_MaxEnergy, Category = "Base Derived Attributes")
    FGameplayAttributeData MaxEnergy;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxEnergy);
    /*Base Derived Attribute*/

    /*Meta Attribute*/
    UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
    FGameplayAttributeData IncomingDamage;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IncomingDamage);
    /*Meta Attribute*/

    /*Diet*/
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_MaxFood, Category = "Diet")
    FGameplayAttributeData MaxFood;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxFood);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_Food, Category = "Diet")
    FGameplayAttributeData Food;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Food);
    /*Diet*/

    UFUNCTION()
    void Rep_Health(const FGameplayAttributeData& OldHealth)const;

    UFUNCTION()
    void Rep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)const;

    UFUNCTION()
    void Rep_Mana(const FGameplayAttributeData& OldMana)const;

    UFUNCTION()
    void Rep_MaxMana(const FGameplayAttributeData& OldMaxMana)const;

    UFUNCTION()
    void Rep_Energy(const FGameplayAttributeData& OldEnergy)const;

    UFUNCTION()
    void Rep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)const;

    UFUNCTION()
    void Rep_Vitality(const FGameplayAttributeData& OldVitality)const;

    UFUNCTION()
    void Rep_Power(const FGameplayAttributeData& OldPower)const;

    UFUNCTION()
    void Rep_Dexterity(const FGameplayAttributeData& OldDexterity)const;

    UFUNCTION()
    void Rep_Intelligence(const FGameplayAttributeData& OldIntelligence)const;

    UFUNCTION()
    void Rep_Endurance(const FGameplayAttributeData& OldEndurance)const;

    UFUNCTION()
    void Rep_Resilience(const FGameplayAttributeData& OldResilience)const;

    UFUNCTION()
    void Rep_CurrentResilience(const FGameplayAttributeData& OldCurrentResilience)const;
  
    UFUNCTION()
    void Rep_Defence(const FGameplayAttributeData& OldDefence)const;

    UFUNCTION()
    void Rep_MaxFood(const FGameplayAttributeData& OldMaxFood)const;

    UFUNCTION()
    void Rep_Food(const FGameplayAttributeData& OldFood)const;

private:

    void SetEffectProps(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProps);

    void AddHurtTag(FEffectProperties& EffectProps);

};