#pragma once
#include <Arduino.h>

enum class GUIFOCUS_MODE: uint32_t {
    GF_NOFOCUS = 0,
    GF_INTERNALPANEL = 1,
    GF_EXTERNALPANEL = 2,
    GF_COMMSPANEL = 3,
    GF_ROLEPANEL = 4,
    GF_STATIONSERVICES = 5,
    GF_GALAXYMAP = 6,
    GF_SYSTEMMAP = 7,
    GF_ORRERY = 8,
    GF_FSS = 9,
    GF_SAA = 10,
    GF_CODEX = 11
};

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

    bool IsChaffLauncherEnabled();
    bool IsHeatsinkLauncherEnabled();
    bool IsShieldCellbankEnabled();
    bool IsEcmEnabled();
    bool IsHyperdriveEnabled();
    bool IsEngineEnabled();
    bool IsLifeSupportEnabled();
    bool IsPowerDistributorEnabled();
    bool IsSensorsEnabled();
    bool IsShieldGeneratorEnabled();

    bool IsInFSAMode();
    bool IsInDSDMode();

    char LocationSystemName[21];
    char LocationStationName[21];
    char LocalAllegiance[21];
    char SystemSecurity[21];
    char Navroute1[21];
    char Navroute2[21];
    char Navroute3[21];
    char InfosCommanderName[21];
    char InfosShipName[21];
    char StatusLegal[21];
    char AlertMessageTitle[21];
    char AlertMessage1[21];
    char AlertMessage2[21];
    char AlertMessage3[21];

    uint32_t AlertDuration;
    uint32_t StatusFlags1;
    uint32_t StatusFlags2;
    uint32_t GuiFocus;
    uint32_t LoadoutFlags1 = 0xFFFFFFFF;
    uint32_t LoadoutFlags2 = 0xFFFFFFFF;
};

extern EDGameVariablesClass EDGameVariables;