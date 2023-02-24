#pragma once

#include "PowerUpTypeEnum.generated.h"

UENUM()
enum class EPowerUpTypeEnum : uint8
{
    None = 0 UMETA(DisplayName = "None"),
    Star = 1 UMETA(DisplayName = "Star"),
    Coin = 2 UMETA(DisplayName = "Coin"),
    SpecialCoin = 3 UMETA(DisplayName = "SpecialCoin"),
    SpeedUp = 4 UMETA(DisplayName = "SpeedUp"),
    RedCoin = 5 UMETA(DisplayName = "RedCoin"),
    PowerUpType_Max UMETA(Hidden),
};