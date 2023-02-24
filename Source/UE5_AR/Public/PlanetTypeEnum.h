#pragma once

#include "PlanetTypeEnum.generated.h"

UENUM()
enum class EPlanetTypeEnum : uint8
{
     FlatGround = 0 UMETA(DisplayName = "FlatGround"),
     SpherePlanet = 1  UMETA(DisplayName = "SpherePlanet"),
     PlanetType_Max UMETA(Hidden),
};