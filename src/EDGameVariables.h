#pragma once
#include <Arduino.h>

class EDGameVariablesClass
{

public:
    EDGameVariablesClass();

    bool IsDocked();
    bool IsLanded();
    bool IsLandingGearDown();
    bool IsShieldsUp();
    bool IsSupercruise();
    bool IsFlightAssistOff();
    bool IsHardpointsDeployed();
    bool IsInWing();
    bool IsLightsOn();
    bool IsCargoScoopDeployed();
    bool IsSilentRunning();
    bool IsScoopingFuel();
    bool IsSrvHandbrake();
    bool IsSrvTurret();
    bool IsSrvUnderShip();
    bool IsSrvDriveAssist();
    bool IsFsdMassLocked();
    bool IsFsdCharging();
    bool IsFsdCooldown();
    bool IsLowFuel();
    bool IsOverheating();
    bool IsHasLatLong();
    bool IsInDanger();
    bool IsBeingInterdicted();
    bool IsInMainShip();
    bool IsInFighter();
    bool IsInSRV();
    bool IsHudAnalysisMode();
    bool IsNightVision();
    bool IsAltitudeFromAverageRadius();
    bool IsFsdJump();
    bool IsSrvHighBeam();
    bool IsOnFoot();
    bool IsInTaxi();
    bool IsInMulticrew();
    bool IsOnFootInStation();
    bool IsOnFootOnPlanet();
    bool IsAimDownSight();
    bool IsLowOxygen();
    bool IsLowHealth();
    bool IsCold();
    bool IsHot();
    bool IsVeryCold();
    bool IsVeryHot();
    bool IsGlideMode();
    bool IsOnFootInHanger();
    bool IsOnFootSocialSpace();
    bool IsOnFootExterior();
    bool IsBreathableAtmosphere();

    char LocationSystemName[21];
    char LocationStationName[21];
    char LocalAllegiance[21];
    char Navroute1[21];
    char Navroute2[21];
    char Navroute3[21];
    char InfosCommanderName[21];
    char InfosShipName[21];
    char StatusLegal[21];
    uint32_t StatusFlags1;
    uint32_t StatusFlags2;
};

extern EDGameVariablesClass EDGameVariables;