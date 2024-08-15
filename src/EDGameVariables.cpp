#include <Arduino.h>
#include "EDGameVariables.h"

EDGameVariablesClass::EDGameVariablesClass()
{
    memset(LocationSystemName, 0, 21);
    memset(LocationStationName, 0, 21);
    memset(LocalAllegiance, 0, 21);
    memset(SystemSecurity, 0, 21);
    memset(Navroute1, 0, 21);
    memset(Navroute2, 0, 21);
    memset(Navroute3, 0, 21);
    memset(InfosCommanderName, 0, 21);
    memset(InfosShipName, 0, 21);
    memset(StatusLegal, 0, 21);
    memset(AlertMessageTitle, 0, 21);
    memset(AlertMessage1, 0, 21);
    memset(AlertMessage2, 0, 21);
    memset(AlertMessage3, 0, 21);
}

bool EDGameVariablesClass::IsDocked()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISDOCKED) == (uint32_t)STATUS_FLAGS1::SF_ISDOCKED;
}
bool EDGameVariablesClass::IsLanded()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISLANDED) == (uint32_t)STATUS_FLAGS1::SF_ISLANDED;
}
bool EDGameVariablesClass::IsLandingGearDown()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISLANDINGGEARDOWN) == (uint32_t)STATUS_FLAGS1::SF_ISLANDINGGEARDOWN;
}
bool EDGameVariablesClass::IsShieldsUp()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSHIELDSUP) == (uint32_t)STATUS_FLAGS1::SF_ISSHIELDSUP;
}
bool EDGameVariablesClass::IsSupercruise()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSUPERCRUISE) == (uint32_t)STATUS_FLAGS1::SF_ISSUPERCRUISE;
}
bool EDGameVariablesClass::IsFlightAssistOff()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISFLIGHTASSISTOFF) == (uint32_t)STATUS_FLAGS1::SF_ISFLIGHTASSISTOFF;
}
bool EDGameVariablesClass::IsHardpointsDeployed()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISHARDPOINTSDEPLOYED) == (uint32_t)STATUS_FLAGS1::SF_ISHARDPOINTSDEPLOYED;
}
bool EDGameVariablesClass::IsInWing()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISINWING) == (uint32_t)STATUS_FLAGS1::SF_ISINWING;
}
bool EDGameVariablesClass::IsLightsOn()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISLIGHTSON) == (uint32_t)STATUS_FLAGS1::SF_ISLIGHTSON;
}
bool EDGameVariablesClass::IsCargoScoopDeployed()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISCARGOSCOOPDEPLOYED) == (uint32_t)STATUS_FLAGS1::SF_ISCARGOSCOOPDEPLOYED;
}
bool EDGameVariablesClass::IsSilentRunning()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSILENTRUNNING) == (uint32_t)STATUS_FLAGS1::SF_ISSILENTRUNNING;
}
bool EDGameVariablesClass::IsScoopingFuel()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSCOOPINGFUEL) == (uint32_t)STATUS_FLAGS1::SF_ISSCOOPINGFUEL;
}
bool EDGameVariablesClass::IsSrvHandbrake()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSRVHANDBRAKE) == (uint32_t)STATUS_FLAGS1::SF_ISSRVHANDBRAKE;
}
bool EDGameVariablesClass::IsSrvTurret()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSRVTURRET) == (uint32_t)STATUS_FLAGS1::SF_ISSRVTURRET;
}
bool EDGameVariablesClass::IsSrvUnderShip()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSRVUNDERSHIP) == (uint32_t)STATUS_FLAGS1::SF_ISSRVUNDERSHIP;
}
bool EDGameVariablesClass::IsSrvDriveAssist()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSRVDRIVEASSIST) == (uint32_t)STATUS_FLAGS1::SF_ISSRVDRIVEASSIST;
}
bool EDGameVariablesClass::IsFsdMassLocked()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISFSDMASSLOCKED) == (uint32_t)STATUS_FLAGS1::SF_ISFSDMASSLOCKED;
}
bool EDGameVariablesClass::IsFsdCharging()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISFSDCHARGING) == (uint32_t)STATUS_FLAGS1::SF_ISFSDCHARGING;
}
bool EDGameVariablesClass::IsFsdCooldown()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISFSDCOOLDOWN) == (uint32_t)STATUS_FLAGS1::SF_ISFSDCOOLDOWN;
}
bool EDGameVariablesClass::IsLowFuel()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISLOWFUEL) == (uint32_t)STATUS_FLAGS1::SF_ISLOWFUEL;
}
bool EDGameVariablesClass::IsOverheating()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISOVERHEATING) == (uint32_t)STATUS_FLAGS1::SF_ISOVERHEATING;
}
bool EDGameVariablesClass::IsHasLatLong()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISHASLATLONG) == (uint32_t)STATUS_FLAGS1::SF_ISHASLATLONG;
}
bool EDGameVariablesClass::IsInDanger()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISINDANGER) == (uint32_t)STATUS_FLAGS1::SF_ISINDANGER;
}
bool EDGameVariablesClass::IsBeingInterdicted()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISBEINGINTERDICTED) == (uint32_t)STATUS_FLAGS1::SF_ISBEINGINTERDICTED;
}
bool EDGameVariablesClass::IsInMainShip()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISINMAINSHIP) == (uint32_t)STATUS_FLAGS1::SF_ISINMAINSHIP;
}
bool EDGameVariablesClass::IsInFighter()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISINFIGHTER) == (uint32_t)STATUS_FLAGS1::SF_ISINFIGHTER;
}
bool EDGameVariablesClass::IsInSRV()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISINSRV) == (uint32_t)STATUS_FLAGS1::SF_ISINSRV;
}
bool EDGameVariablesClass::IsHudAnalysisMode()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISHUDANALYSISMODE) == (uint32_t)STATUS_FLAGS1::SF_ISHUDANALYSISMODE;
}
bool EDGameVariablesClass::IsNightVision()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISNIGHTVISION) == (uint32_t)STATUS_FLAGS1::SF_ISNIGHTVISION;
}
bool EDGameVariablesClass::IsAltitudeFromAverageRadius()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISALTITUDEFROMAVERAGERADIUS) == (uint32_t)STATUS_FLAGS1::SF_ISALTITUDEFROMAVERAGERADIUS;
}
bool EDGameVariablesClass::IsFsdJump()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISFSDJUMP) == (uint32_t)STATUS_FLAGS1::SF_ISFSDJUMP;
}
bool EDGameVariablesClass::IsSrvHighBeam()
{
    return (StatusFlags1 & (uint32_t)STATUS_FLAGS1::SF_ISSRVHIGHBEAM) == (uint32_t)STATUS_FLAGS1::SF_ISSRVHIGHBEAM;
}
bool EDGameVariablesClass::IsOnFoot()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISONFOOT) == (uint32_t)STATUS_FLAGS2::SF_ISONFOOT;
}
bool EDGameVariablesClass::IsInTaxi()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISINTAXI) == (uint32_t)STATUS_FLAGS2::SF_ISINTAXI;
}
bool EDGameVariablesClass::IsInMulticrew()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISINMULTICREW) == (uint32_t)STATUS_FLAGS2::SF_ISINMULTICREW;
}
bool EDGameVariablesClass::IsOnFootInStation()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISONFOOTINSTATION) == (uint32_t)STATUS_FLAGS2::SF_ISONFOOTINSTATION;
}
bool EDGameVariablesClass::IsOnFootOnPlanet()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISONFOOTONPLANET) == (uint32_t)STATUS_FLAGS2::SF_ISONFOOTONPLANET;
}
bool EDGameVariablesClass::IsAimDownSight()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISAIMDOWNSIGHT) == (uint32_t)STATUS_FLAGS2::SF_ISAIMDOWNSIGHT;
}
bool EDGameVariablesClass::IsLowOxygen()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISLOWOXYGEN) == (uint32_t)STATUS_FLAGS2::SF_ISLOWOXYGEN;
}
bool EDGameVariablesClass::IsLowHealth()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISLOWHEALTH) == (uint32_t)STATUS_FLAGS2::SF_ISLOWHEALTH;
}
bool EDGameVariablesClass::IsCold()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISCOLD) == (uint32_t)STATUS_FLAGS2::SF_ISCOLD;
}
bool EDGameVariablesClass::IsHot()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISHOT) == (uint32_t)STATUS_FLAGS2::SF_ISHOT;
}
bool EDGameVariablesClass::IsVeryCold()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISVERYCOLD) == (uint32_t)STATUS_FLAGS2::SF_ISVERYCOLD;
}
bool EDGameVariablesClass::IsVeryHot()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISVERYHOT) == (uint32_t)STATUS_FLAGS2::SF_ISVERYHOT;
}
bool EDGameVariablesClass::IsGlideMode()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISGLIDEMODE) == (uint32_t)STATUS_FLAGS2::SF_ISGLIDEMODE;
}
bool EDGameVariablesClass::IsOnFootInHanger()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISONFOOTINHANGER) == (uint32_t)STATUS_FLAGS2::SF_ISONFOOTINHANGER;
}
bool EDGameVariablesClass::IsOnFootSocialSpace()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISONFOOTSOCIALSPACE) == (uint32_t)STATUS_FLAGS2::SF_ISONFOOTSOCIALSPACE;
}
bool EDGameVariablesClass::IsOnFootExterior()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISONFOOTEXTERIOR) == (uint32_t)STATUS_FLAGS2::SF_ISONFOOTEXTERIOR;
}
bool EDGameVariablesClass::IsBreathableAtmosphere()
{
    return (StatusFlags2 & (uint32_t)STATUS_FLAGS2::SF_ISBREATHABLEATMOSPHERE) == (uint32_t)STATUS_FLAGS2::SF_ISBREATHABLEATMOSPHERE;
}
bool EDGameVariablesClass::IsChaffLauncherEnabled()
{
    return (LoadoutFlags1 & (uint32_t)LOADOUT_FLAGS1::LF_ISCHAFFLAUNCHERENABLED) > 0;
}

bool EDGameVariablesClass::IsHeatsinkLauncherEnabled()
{
    return (LoadoutFlags1 & (uint32_t)LOADOUT_FLAGS1::LF_ISHEATSINKLAUNCHERENABLED) > 0;
}

bool EDGameVariablesClass::IsShieldCellbankEnabled()
{
    return (LoadoutFlags1 & (uint32_t)LOADOUT_FLAGS1::LF_ISSHIELDCELLBANKENABLED) > 0;
}

bool EDGameVariablesClass::IsEcmEnabled()
{
    return (LoadoutFlags1 & (uint32_t)LOADOUT_FLAGS1::LF_ISECMENABLED) > 0;
}

bool EDGameVariablesClass::IsHyperdriveEnabled()
{
    return (LoadoutFlags2 & (uint32_t)LOADOUT_FLAGS2::LF_ISHYPERDRIVEENABLED) > 0;
}

bool EDGameVariablesClass::IsEngineEnabled()
{
    return (LoadoutFlags2 & (uint32_t)LOADOUT_FLAGS2::LF_ISENGINEENABLED) > 0;
}

bool EDGameVariablesClass::IsLifeSupportEnabled()
{
    return (LoadoutFlags2 & (uint32_t)LOADOUT_FLAGS2::LF_ISLIFESUPPORTENABLED) > 0;
}

bool EDGameVariablesClass::IsPowerDistributorEnabled()
{
    return (LoadoutFlags2 & (uint32_t)LOADOUT_FLAGS2::LF_ISPOWERDISTRIBUTORENABLED) > 0;
}

bool EDGameVariablesClass::IsSensorsEnabled()
{
    return (LoadoutFlags2 & (uint32_t)LOADOUT_FLAGS2::LF_ISSENSORSENABLED) > 0;
}

bool EDGameVariablesClass::IsShieldGeneratorEnabled()
{
    return (LoadoutFlags2 & (uint32_t)LOADOUT_FLAGS2::LF_ISSHIELDGENERATORENABLED) > 0;
}

/// @brief FSA (Full system analysis) = ACS (Analyse complete du systeme) = FSS (Full spectrum scanner)
/// @return
bool EDGameVariablesClass::IsInFSAMode()
{
    return (GuiFocus == (uint8_t)GUIFOCUS_MODE::GF_FSS);
}

/// @brief SAA (Surface area analysis) = DSD (Détecteur de surface détaillée)
/// @return
bool EDGameVariablesClass::IsInDSDMode()
{
    return (GuiFocus == (uint8_t)GUIFOCUS_MODE::GF_SAA);
}

EDGameVariablesClass EDGameVariables;