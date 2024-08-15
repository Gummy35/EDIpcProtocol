#pragma once
#include <Arduino.h>

enum class STATUS_FLAGS1 : uint32_t
{
    SF_ISDOCKED = 0x00000001,
    SF_ISLANDED = 0x00000002,
    SF_ISLANDINGGEARDOWN = 0x00000004,
    SF_ISSHIELDSUP = 0x00000008,
    SF_ISSUPERCRUISE = 0x00000010,
    SF_ISFLIGHTASSISTOFF = 0x00000020,
    SF_ISHARDPOINTSDEPLOYED = 0x00000040,
    SF_ISINWING = 0x00000080,
    SF_ISLIGHTSON = 0x00000100,
    SF_ISCARGOSCOOPDEPLOYED = 0x00000200,
    SF_ISSILENTRUNNING = 0x00000400,
    SF_ISSCOOPINGFUEL = 0x00000800,
    SF_ISSRVHANDBRAKE = 0x00001000,
    SF_ISSRVTURRET = 0x00002000,
    SF_ISSRVUNDERSHIP = 0x00004000,
    SF_ISSRVDRIVEASSIST = 0x00008000,
    SF_ISFSDMASSLOCKED = 0x00010000,
    SF_ISFSDCHARGING = 0x00020000,
    SF_ISFSDCOOLDOWN = 0x00040000,
    SF_ISLOWFUEL = 0x00080000,
    SF_ISOVERHEATING = 0x00100000,
    SF_ISHASLATLONG = 0x00200000,
    SF_ISINDANGER = 0x00400000,
    SF_ISBEINGINTERDICTED = 0x00800000,
    SF_ISINMAINSHIP = 0x01000000,
    SF_ISINFIGHTER = 0x02000000,
    SF_ISINSRV = 0x04000000,
    SF_ISHUDANALYSISMODE = 0x08000000,
    SF_ISNIGHTVISION = 0x10000000,
    SF_ISALTITUDEFROMAVERAGERADIUS = 0x20000000,
    SF_ISFSDJUMP = 0x40000000,
    SF_ISSRVHIGHBEAM = 0x80000000
};

enum class STATUS_FLAGS2 : uint32_t
{
    SF_ISONFOOT = 0x00000001,
    SF_ISINTAXI = 0x00000002,
    SF_ISINMULTICREW = 0x00000004,
    SF_ISONFOOTINSTATION = 0x00000008,
    SF_ISONFOOTONPLANET = 0x00000010,
    SF_ISAIMDOWNSIGHT = 0x00000020,
    SF_ISLOWOXYGEN = 0x00000040,
    SF_ISLOWHEALTH = 0x00000080,
    SF_ISCOLD = 0x00000100,
    SF_ISHOT = 0x00000200,
    SF_ISVERYCOLD = 0x00000400,
    SF_ISVERYHOT = 0x00000800,
    SF_ISGLIDEMODE = 0x00001000,
    SF_ISONFOOTINHANGER = 0x00002000,
    SF_ISONFOOTSOCIALSPACE = 0x00004000,
    SF_ISONFOOTEXTERIOR = 0x00008000,
    SF_ISBREATHABLEATMOSPHERE = 0x00010000
};

enum class LOADOUT_FLAGS1 : uint32_t
{
    LF_ISCHAFFLAUNCHERENABLED = 0x0000000F,
    LF_ISHEATSINKLAUNCHERENABLED = 0x000000F0,
    LF_ISSHIELDCELLBANKENABLED = 0x00000F00,
    LF_ISECMENABLED = 0x0000F000,
};

enum class LOADOUT_FLAGS2 : uint32_t
{
    LF_ISHYPERDRIVEENABLED = 0x0000000F,
    LF_ISENGINEENABLED = 0x000000F0,
    LF_ISLIFESUPPORTENABLED = 0x00000F00,
    LF_ISPOWERDISTRIBUTORENABLED = 0x0000F000,
    LF_ISSENSORSENABLED = 0x000F0000,
    LF_ISSHIELDGENERATORENABLED = 0x00F00000
};

enum class GUIFOCUS_MODE : uint8_t
{
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
    uint8_t GuiFocus;
    uint8_t FireGroup;
    uint8_t FireGroupCount;
    uint32_t LoadoutFlags1 = 0xFFFFFFFF;
    uint32_t LoadoutFlags2 = 0xFFFFFFFF;

    bool IsShutdown = false;
};

extern EDGameVariablesClass EDGameVariables;