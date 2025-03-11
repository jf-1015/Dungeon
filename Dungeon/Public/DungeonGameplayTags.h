// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"//

/**
 * 
 */

struct FDungeonGameplayTags
{
public:

	static const FDungeonGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

	/*Base Attribute*/
	FGameplayTag Attribute_Base_Vitality;
	FGameplayTag Attribute_Base_Power;
	FGameplayTag Attribute_Base_Dexterity;
	FGameplayTag Attribute_Base_Intelligence;
	FGameplayTag Attribute_Base_Endurance;
	FGameplayTag Attribute_Base_Resilience;
	FGameplayTag Attribute_Base_Defence;
	/*Base Attribute*/

	/*Derived Attribute*/
	FGameplayTag Attribute_Derived_MaxHealth;
	FGameplayTag Attribute_Derived_MaxMana;
	FGameplayTag Attribute_Derived_MaxEnergy;
	/*Derived Attribute*/

	/*Vital*/
	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;
	FGameplayTag Attribute_Vital_Energy;
	FGameplayTag Attribute_Vital_CurrentResilience;
	FGameplayTag Attribute_Vital_Food;
	/*Vital*/

	/*Input*/
	FGameplayTag Input_Async_R;
	FGameplayTag Input_Async_F;
	FGameplayTag Input_LM;
	FGameplayTag Input_RM;
	FGameplayTag Input_HeavyAttack;/*Shift + LM*/
	FGameplayTag Input_SpecialSkill;/*Shift + RM*/
	FGameplayTag Input_E;
	FGameplayTag Input_R;
	FGameplayTag Input_Q;
	FGameplayTag Input_X;
	FGameplayTag Input_Space;
	FGameplayTag Input_Shift;
	FGameplayTag Input_RollUp;
	FGameplayTag Input_RollDown;
	FGameplayTag Input_ShiftRollDown;
	/*Input*/

	/*Item Type*/
	FGameplayTag ItemType_Consume;
	FGameplayTag ItemType_Weapon;
	FGameplayTag ItemType_Accessory;
	/*Item Type*/

	/*WeaponType*/
	FGameplayTag Weapon_Melee;
	FGameplayTag Weapon_Range;
	FGameplayTag Weapon_Magic;
	/*WeaponType*/

	/*Handheld Type*/
	FGameplayTag Handheld_None;
	FGameplayTag Handheld_Body;
	FGameplayTag Handheld_Body_Helmet;
	FGameplayTag Handheld_Body_Accessory;
	FGameplayTag Handheld_1H;
	FGameplayTag Handheld_2H;
	FGameplayTag Handheld_Secondary;
	FGameplayTag Handheld_Secondary_Shield;
	FGameplayTag Handheld_Secondary_Magic;
	FGameplayTag Handheld_Secondary_Magic_Enhance;
	/*Handheld Type*/

	/*Ability*/
	FGameplayTag Ability_Execution_Melee;
	FGameplayTag Ability_Execution_Range;
	FGameplayTag Ability_Execution_Combat;

	FGameplayTag MeleeAbility_Combo_1;
	FGameplayTag MeleeAbility_Combo_2;
	FGameplayTag MeleeAbility_Combo_3;
	FGameplayTag MeleeAbility_Heavy;
	FGameplayTag MeleeAbility_Spinning;

	FGameplayTag RangeAbility_Shoot;
	FGameplayTag RangeAbility_Aim;

	FGameplayTag Ability_Dodge;
	FGameplayTag Ability_Jump;
	FGameplayTag Ability_Run;
	FGameplayTag Ability_Run_End;

	FGameplayTag Ability_Blocking;
	FGameplayTag Ability_Break;
	FGameplayTag Ability_Enhance;
	FGameplayTag Ability_Throw;

	FGameplayTag Ability_Interrupt;
	FGameplayTag Ability_HitReact;
	FGameplayTag Ability_Awake;
	FGameplayTag Ability_Pickup;
	/*Ability*/

	/*Passive*/
	FGameplayTag Passive_Helmet_Kight;
	/*Passive*/

	/*Cooldown*/
	FGameplayTag Cooldown_CommonAction;
	FGameplayTag Cooldown_Throw;
	FGameplayTag Cooldown_Switch;
	/*CoolDown*/

	/*Montage*/
	FGameplayTag Montage_Shoot;
	FGameplayTag Montage_Land;
	FGameplayTag Montage_Spinning;
	FGameplayTag Montage_EndSpinning;
	FGameplayTag Montage_Throw;
	FGameplayTag Montage_MagicEffect;
	FGameplayTag Montage_Pickup;
	/*Montage*/

	/*Trace Check*/
	FGameplayTag TraceCheck_Start;
	FGameplayTag TraceCheck_End;
	/*Trace Check*/

	/*Throwable*/
	FGameplayTag Throwable_BaseDamage;
	FGameplayTag Throwable_PowCE;
	FGameplayTag Throwable_DexCE;
	FGameplayTag Throwable_IntCE;
	FGameplayTag Throwable_ResilienceCut;
	FGameplayTag Throwable_Durability;
	/*Throwable*/

	/*Character*/
	FGameplayTag Character_Player;
	FGameplayTag Character_Enemy;
	/*Character*/

	/*Hurt*/
	FGameplayTag Hurt;
	/*Hurt*/

private:

	static FDungeonGameplayTags GameplayTags;

};