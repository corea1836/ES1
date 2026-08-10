#include "Data/ES1AnimationData.h"

const FES1MontageGroup* UES1AnimationData::GetMontageGroup(const FGameplayTag& GroupTag) const
{
	return MontageGroupMap.Find(GroupTag);
}

const FES1MAnimationGroup* UES1AnimationData::GetAnimationGroup(const FGameplayTag& GroupTag) const
{
	return AnimationGroupMap.Find(GroupTag);
}
