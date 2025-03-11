// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGameplayTags.h"
#include "GameplayTagsManager.h"//

FDungeonGameplayTags FDungeonGameplayTags::GameplayTags;

void FDungeonGameplayTags::InitializeNativeGameplayTags()
{
    /*Base Attribute*/
    GameplayTags.Attribute_Base_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Vitality"),
        FString("Increase Max Health"));

    GameplayTags.Attribute_Base_Power = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Power"),
        FString("Increase Physical Damage"));

    GameplayTags.Attribute_Base_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Dexterity"),
        FString("Increase Physical Damage"));

    GameplayTags.Attribute_Base_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Intelligence"),
        FString("Increase Magic Damage and MaxMana"));

    GameplayTags.Attribute_Base_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Endurance"),
        FString("Increase MaxEnergy"));

    GameplayTags.Attribute_Base_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Resilience"),
        FString("Increase Various Resistances and Anti Interruption"));

    GameplayTags.Attribute_Base_Defence = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Base.Defence"),
        FString("Reduce Damage"));
    /*Base Attribute*/

    /*Derived Attribute*/
    GameplayTags.Attribute_Derived_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Derived.MaxHealth"),
        FString("Maximum of Health"));

    GameplayTags.Attribute_Derived_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Derived.MaxMana"),
        FString("Maximum of Mana"));

    GameplayTags.Attribute_Derived_MaxEnergy = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Derived.MaxEnergy"),
        FString("Maximum of Energy"));
    /*Derived Attribute*/

    /*Vital*/
    GameplayTags.Attribute_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Vital.Health"),
        FString("Food"));

    GameplayTags.Attribute_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Vital.Mana"),
        FString("Water"));

    GameplayTags.Attribute_Vital_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Vital.Energy"),
        FString("Food"));

    GameplayTags.Attribute_Vital_CurrentResilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Vital.CurrentResilience"),
        FString("CurrentResilience"));

    GameplayTags.Attribute_Vital_Food = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attribute.Vital.Food"),
        FString("Food"));
    /*Vital*/

    /*Input*/
    GameplayTags.Input_Async_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.Async.R"),
        FString("Key R for Async Event"));

    GameplayTags.Input_Async_F = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.Async.F"),
        FString("Key F for Async Event"));

    GameplayTags.Input_LM = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.LM"),
        FString("Key LM"));

    GameplayTags.Input_RM = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.RM"),
        FString("Key RM"));

    GameplayTags.Input_HeavyAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.HeavyAttack"),
        FString("Key Shift+LM"));

    GameplayTags.Input_SpecialSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.SpecialSkill"),
        FString("Key Shift+RM"));

    GameplayTags.Input_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.E"),
        FString("Key E"));

    GameplayTags.Input_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.R"),
        FString("Key R"));

    GameplayTags.Input_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.Q"),
        FString("Key Q"));

    GameplayTags.Input_X = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.X"),
        FString("Key X"));

    GameplayTags.Input_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.Space"),
        FString("Key Space"));

    GameplayTags.Input_Shift = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.Shift"),
        FString("Key Shift"));

    GameplayTags.Input_RollUp = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.RollUp"),
        FString("Roll Up"));

    GameplayTags.Input_RollDown = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.RollDown"),
        FString("Roll Down"));

    GameplayTags.Input_ShiftRollDown = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Input.ShiftRollDown"),
        FString("ShiftRollDown"));
    /*Input*/

    /*Item Type*/
    GameplayTags.ItemType_Consume = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("ItemType.Consume"),
        FString("Item To Consume"));

    GameplayTags.ItemType_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("ItemType.Weapon"),
        FString("Weapon"));

    GameplayTags.ItemType_Accessory = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("ItemType.Accessory"),
        FString("Accessory"));
    /*Item Type*/

    /*WeaponType*/
    GameplayTags.Weapon_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Weapon.Melee"),
        FString("Melee Weapon"));

    GameplayTags.Weapon_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Weapon.Range"),
        FString("Range Weapon"));

    GameplayTags.Weapon_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Weapon.Magic"),
        FString("Magic Weapon"));
    /*WeaponType*/

    /*Handheld Type*/
    GameplayTags.Handheld_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.None"),
        FString("None"));

    GameplayTags.Handheld_Body = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Body"),
        FString("Body"));

    GameplayTags.Handheld_Body_Helmet = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Body.Helmet"),
        FString("Helmet"));

    GameplayTags.Handheld_Body_Accessory = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Body.Accessory"),
        FString("Accessory"));

    GameplayTags.Handheld_1H = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.1H"),
        FString("1H"));

    GameplayTags.Handheld_2H = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.2H"),
        FString("2H"));

    GameplayTags.Handheld_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Secondary"),
        FString("Secondary"));

    GameplayTags.Handheld_Secondary_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Secondary.Shield"),
        FString("Shield Secondary Weapon"));

    GameplayTags.Handheld_Secondary_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Secondary.Magic"),
        FString("Magic Secondary Weapon"));

    GameplayTags.Handheld_Secondary_Magic_Enhance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Handheld.Secondary.Magic.Enhance"),
        FString("Magic Secondary Weapon_Enhance"));
    /*Handheld Type*/

    /*Ability*/
    GameplayTags.Ability_Execution_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Execution.Melee"),
        FString("Melee"));

    GameplayTags.Ability_Execution_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Execution.Range"),
        FString("Range"));

    GameplayTags.Ability_Execution_Combat = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Execution.Combat"),
        FString("Combat"));

    GameplayTags.MeleeAbility_Combo_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("MeleeAbility.Combo.1"),
        FString("Melee Attack Combo 1"));

    GameplayTags.MeleeAbility_Combo_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("MeleeAbility.Combo.2"),
        FString("Melee Attack Combo 2"));

    GameplayTags.MeleeAbility_Combo_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("MeleeAbility.Combo.3"),
        FString("Melee Attack Combo 3"));

    GameplayTags.MeleeAbility_Heavy = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("MeleeAbility.Heavy"),
        FString("Melee Attack Heavy"));

    GameplayTags.MeleeAbility_Spinning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("MeleeAbility.Spinning"),
        FString("2H Spinning Skill"));

    GameplayTags.RangeAbility_Shoot = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("RangeAbility.Shoot"),
        FString("Shoot"));

    GameplayTags.RangeAbility_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("RangeAbility.Aim"),
        FString("Aim"));

    GameplayTags.Ability_Enhance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Enhance"),
        FString("Enhance Magic"));

    GameplayTags.Ability_Blocking = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Blocking"),
        FString("Block Ability"));

    GameplayTags.Ability_Break = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Breal"),
        FString("Break Ability"));

    GameplayTags.Ability_Throw = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Throw"),
        FString("Throw Ability"));

    GameplayTags.Ability_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Dodge"),
        FString("Dodge Ability"));

    GameplayTags.Ability_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Jump"),
        FString("Jump Ability"));

    GameplayTags.Ability_Run = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Run"),
        FString("Run Ability"));

    GameplayTags.Ability_Run_End = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Run.End"),
        FString("End Run Ability"));

    GameplayTags.Ability_Interrupt = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Interrupt"),
        FString("Interrupt"));

    GameplayTags.Ability_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.HitReact"),
        FString("HitReact"));

    GameplayTags.Ability_Awake = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Awake"),
        FString("Awake"));

    GameplayTags.Ability_Pickup = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Pickup"),
        FString("Pickup"));
    /*Ability*/

    /*Passive*/
    GameplayTags.Passive_Helmet_Kight = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Passive.Helmet.Kight"),
        FString("Kight Helmet"));
    /*Passive*/

    /*Cooldown*/
    GameplayTags.Cooldown_CommonAction = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Cooldown.CommonAction"),
        FString("CommonAction Cooldown"));

    GameplayTags.Cooldown_Throw = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Cooldown.Throw"),
        FString("Throw Cooldown"));

    GameplayTags.Cooldown_Switch = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Cooldown.Switch"),
        FString("Switch Cooldown"));
    /*Cooldown*/

    /*Montage*/
    GameplayTags.Montage_Shoot = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.Shoot"),
        FString("Montage To Shoot"));

    GameplayTags.Montage_Land = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.Land"),
        FString("Montage To Land"));

    GameplayTags.Montage_Spinning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.Spinning"),
        FString("Montage To Spining"));

    GameplayTags.Montage_EndSpinning = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.EndSpinning"),
        FString("Montage To EndSpining"));

    GameplayTags.Montage_Throw = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.Throw"),
        FString("Montage To Throw"));

    GameplayTags.Montage_MagicEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.MagicEffect"),
        FString("Montage To MagicEffect"));

    GameplayTags.Montage_Pickup = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Montage.Pickup"),
        FString("Montage To Pickup"));
    /*Montage*/

    /*Trace Check*/
    GameplayTags.TraceCheck_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("TraceCheck.Start"),
        FString("Start Trace Check"));

    GameplayTags.TraceCheck_End = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("TraceCheck.End"),
        FString("End Trace Check"));
    /*Trace Check*/

    /*Throwable*/
    GameplayTags.Throwable_BaseDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Throwable.BaseDamage"),
        FString("Throw Base Damage"));

    GameplayTags.Throwable_PowCE = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Throwable.PowCE"),
        FString("Throw PowCE"));

    GameplayTags.Throwable_DexCE = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Throwable.DexCE"),
        FString("Throw DexCE"));

    GameplayTags.Throwable_IntCE = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Throwable.IntCE"),
        FString("Throw IntCE"));

    GameplayTags.Throwable_ResilienceCut = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Throwable.ResilienceCut"),
        FString("Throw ResilienceCut"));

    GameplayTags.Throwable_Durability = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Throwable.Durability"),
        FString("Inherit Durability"));
    /*Throwable*/

    /*Character*/
    GameplayTags.Character_Player = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Character.Player"),
        FString("Player"));

    GameplayTags.Character_Enemy = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Character.Enemy"),
        FString("Enemy"));
    /*Character*/
    
    /*Hurt*/
    GameplayTags.Hurt = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Hurt"),
        FString("Hurt"));
    /*Hurt*/
}