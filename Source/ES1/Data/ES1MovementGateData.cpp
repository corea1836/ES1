#include "Data/ES1MovementGateData.h"

const FES1MovementGroup* UES1MovementGateData::GetMovementGroup(const EES1MovementGate movementEnum) const
{
	return MovementGroupMap.Find(movementEnum);
}
