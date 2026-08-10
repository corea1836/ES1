#include "Data/ES1WeaponLocomotionData.h"

const FES1WeaponAnimInstanceGroup* UES1WeaponLocomotionData::GetAnimInstanceGroup(const EES1EquipmentType WeaponEnum)
{
	return AnimInstanceGroupMap.Find(WeaponEnum);
}
