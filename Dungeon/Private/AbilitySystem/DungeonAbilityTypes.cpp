#include "AbilitySystem/DungeonAbilityTypes.h"

bool FDungeonGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bBlocked)
		{
			RepBits |= 1 << 7;
		}
		if (ResilienceCut != 0)
		{
			RepBits |= 1 << 8;
		}
		if (EnduranceCut != 0)
		{
			RepBits |= 1 << 9;
		}
		if (bThrowable)
		{
			RepBits |= 1 << 10;
		}
		if (bReactBlock)
		{
			RepBits |= 1 << 11;
		}

		Ar.SerializeBits(&RepBits, 12);

		if (RepBits & (1 << 0))
		{
			Ar << Instigator;
		}
		if (RepBits & (1 << 1))
		{
			Ar << EffectCauser;
		}
		if (RepBits & (1 << 2))
		{
			Ar << AbilityCDO;
		}
		if (RepBits & (1 << 3))
		{
			Ar << SourceObject;
		}
		if (RepBits & (1 << 4))
		{
			SafeNetSerializeTArray_Default<31>(Ar, Actors);
		}
		if (RepBits & (1 << 5))
		{
			if (Ar.IsLoading())
			{
				if (!HitResult.IsValid())
				{
					HitResult = TSharedPtr<FHitResult>(new FHitResult());
				}
			}
			HitResult->NetSerialize(Ar, Map, bOutSuccess);
		}
		if (RepBits & (1 << 6))
		{
			Ar << WorldOrigin;
			bHasWorldOrigin = true;
		}
		else
		{
			bHasWorldOrigin = false;
		}
		if (RepBits & (1 << 7))
		{
			Ar << bBlocked;
		}
		if (RepBits & (1 << 8))
		{
			Ar << ResilienceCut;
		}
		if (RepBits & (1 << 9))
		{
			Ar << EnduranceCut;
		}
		if (RepBits & (1 << 10))
		{
			Ar << bThrowable;
		}
		if (RepBits & (1 << 11))
		{
			Ar << bReactBlock;
		}

		if (Ar.IsLoading())
		{
			AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
		}

		bOutSuccess = true;
		return true;
	}

	return false;

}