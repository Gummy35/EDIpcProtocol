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
}

bool EDGameVariablesClass::IsDocked()
{
    return (StatusFlags1 & 0x00000001) == 0x00000001;
}

bool EDGameVariablesClass::IsLanded()
{
    return (StatusFlags1 & 0x00000002) == 0x00000002;
}

bool EDGameVariablesClass::IsLandingGearDown()
{
    return (StatusFlags1 & 0x00000004) == 0x00000004;
}

bool EDGameVariablesClass::IsShieldsUp()
{
    return (StatusFlags1 & 0x00000008) == 0x00000008;
}

bool EDGameVariablesClass::IsSupercruise()
{
    return (StatusFlags1 & 0x00000010) == 0x00000010;
}

bool EDGameVariablesClass::IsFlightAssistOff()
{
    return (StatusFlags1 & 0x00000020) == 0x00000020;
}

bool EDGameVariablesClass::IsHardpointsDeployed()
{
    return (StatusFlags1 & 0x00000040) == 0x00000040;
}

bool EDGameVariablesClass::IsInWing()
{
    return (StatusFlags1 & 0x00000080) == 0x00000080;
}

bool EDGameVariablesClass::IsLightsOn()
{
    return (StatusFlags1 & 0x00000100) == 0x00000100;
}

bool EDGameVariablesClass::IsCargoScoopDeployed()
{
    return (StatusFlags1 & 0x00000200) == 0x00000200;
}

bool EDGameVariablesClass::IsSilentRunning()
{
    return (StatusFlags1 & 0x00000400) == 0x00000400;
}

bool EDGameVariablesClass::IsScoopingFuel()
{
    return (StatusFlags1 & 0x00000800) == 0x00000800;
}

bool EDGameVariablesClass::IsSrvHandbrake()
{
    return (StatusFlags1 & 0x00001000) == 0x00001000;
}

bool EDGameVariablesClass::IsSrvTurret()
{
    return (StatusFlags1 & 0x00002000) == 0x00002000;
}

bool EDGameVariablesClass::IsSrvUnderShip()
{
    return (StatusFlags1 & 0x00004000) == 0x00004000;
}

bool EDGameVariablesClass::IsSrvDriveAssist()
{
    return (StatusFlags1 & 0x00008000) == 0x00008000;
}

bool EDGameVariablesClass::IsFsdMassLocked()
{
    return (StatusFlags1 & 0x00010000) == 0x00010000;
}

bool EDGameVariablesClass::IsFsdCharging()
{
    return (StatusFlags1 & 0x00020000) == 0x00020000;
}

bool EDGameVariablesClass::IsFsdCooldown()
{
    return (StatusFlags1 & 0x00040000) == 0x00040000;
}

bool EDGameVariablesClass::IsLowFuel()
{
    return (StatusFlags1 & 0x00080000) == 0x00080000;
}

bool EDGameVariablesClass::IsOverheating()
{
    return (StatusFlags1 & 0x00100000) == 0x00100000;
}

bool EDGameVariablesClass::IsHasLatLong()
{
    return (StatusFlags1 & 0x00200000) == 0x00200000;
}

bool EDGameVariablesClass::IsInDanger()
{
    return (StatusFlags1 & 0x00400000) == 0x00400000;
}

bool EDGameVariablesClass::IsBeingInterdicted()
{
    return (StatusFlags1 & 0x00800000) == 0x00800000;
}

bool EDGameVariablesClass::IsInMainShip()
{
    return (StatusFlags1 & 0x01000000) == 0x01000000;
}

bool EDGameVariablesClass::IsInFighter()
{
    return (StatusFlags1 & 0x02000000) == 0x02000000;
}

bool EDGameVariablesClass::IsInSRV()
{
    return (StatusFlags1 & 0x04000000) == 0x04000000;
}

bool EDGameVariablesClass::IsHudAnalysisMode()
{
    return (StatusFlags1 & 0x08000000) == 0x08000000;
}

bool EDGameVariablesClass::IsNightVision()
{
    return (StatusFlags1 & 0x10000000) == 0x10000000;
}

bool EDGameVariablesClass::IsAltitudeFromAverageRadius()
{
    return (StatusFlags1 & 0x20000000) == 0x20000000;
}

bool EDGameVariablesClass::IsFsdJump()
{
    return (StatusFlags1 & 0x40000000) == 0x40000000;
}

bool EDGameVariablesClass::IsSrvHighBeam()
{
    return (StatusFlags1 & 0x80000000) == 0x80000000;
}

bool EDGameVariablesClass::IsOnFoot()
{
    return (StatusFlags2 & 0x00000001) == 0x00000001;
}

bool EDGameVariablesClass::IsInTaxi()
{
    return (StatusFlags2 & 0x00000002) == 0x00000002;
}

bool EDGameVariablesClass::IsInMulticrew()
{
    return (StatusFlags2 & 0x00000004) == 0x00000004;
}

bool EDGameVariablesClass::IsOnFootInStation()
{
    return (StatusFlags2 & 0x00000008) == 0x00000008;
}

bool EDGameVariablesClass::IsOnFootOnPlanet()
{
    return (StatusFlags2 & 0x00000010) == 0x00000010;
}

bool EDGameVariablesClass::IsAimDownSight()
{
    return (StatusFlags2 & 0x00000020) == 0x00000020;
}

bool EDGameVariablesClass::IsLowOxygen()
{
    return (StatusFlags2 & 0x00000040) == 0x00000040;
}

bool EDGameVariablesClass::IsLowHealth()
{
    return (StatusFlags2 & 0x00000080) == 0x00000080;
}

bool EDGameVariablesClass::IsCold()
{
    return (StatusFlags2 & 0x00000100) == 0x00000100;
}

bool EDGameVariablesClass::IsHot()
{
    return (StatusFlags2 & 0x00000200) == 0x00000200;
}

bool EDGameVariablesClass::IsVeryCold()
{
    return (StatusFlags2 & 0x00000400) == 0x00000400;
}

bool EDGameVariablesClass::IsVeryHot()
{
    return (StatusFlags2 & 0x00000800) == 0x00000800;
}

bool EDGameVariablesClass::IsGlideMode()
{
    return (StatusFlags2 & 0x00001000) == 0x00001000;
}

bool EDGameVariablesClass::IsOnFootInHanger()
{
    return (StatusFlags2 & 0x00002000) == 0x00002000;
}

bool EDGameVariablesClass::IsOnFootSocialSpace()
{
    return (StatusFlags2 & 0x00004000) == 0x00004000;
}

bool EDGameVariablesClass::IsOnFootExterior()
{
    return (StatusFlags2 & 0x00008000) == 0x00008000;
}

bool EDGameVariablesClass::IsBreathableAtmosphere()
{
    return (StatusFlags2 & 0x00010000) == 0x00010000;
}

EDGameVariablesClass EDGameVariables;