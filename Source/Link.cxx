// Author: Caleb Heydon <probablyme@caleb.engineer>
// License: GPL-3.0

#include "Link.hxx"

Link::Link(std::string sLinkIp)
{
}

void Link::InitNavX()
{
}

void Link::InitPwmInput(int nChannel)
{
}

void Link::InitTalon(enum TalonType talonType, int nCanId, bool bInvertEncoder, std::vector<std::tuple<int, double, double, double, double>> *pvecPidfs)
{
}

void Link::UpdateTalon(int nCanId, enum Link::DriveMode driveMode, double dSetpoint, bool bBreakMode)
{
}

double Link::GetNavXHeading()
{
    return 0;
}

double Link::GetPwmInput(int nChannel)
{
    return 0;
}

double Link::GetTalonVelocity(int nCanId)
{
    return 0;
}

double Link::GetTalonPosition(int nCanId)
{
    return 0;
}
