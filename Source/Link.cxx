// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include "Link.hxx"

Link::Link(std::string sLinkIp) : m_sLinkIp(sLinkIp)
{
    Reset();
}

void Link::Reset()
{
}

void Link::Destroy()
{
}

void Link::InitNavX()
{
}

void Link::InitPwmInput(int nChannel)
{
}

void Link::InitTalon(enum TalonType talonType, int nCanId, bool bInvert, bool bInvertEncoder, std::vector<std::tuple<int, double, double, double, double>> *pvecPidfs)
{
}

void Link::UpdateTalon(int nCanId, enum Link::DriveMode driveMode, double dSetpoint, bool bBreakMode)
{
}

enum Link::Mode GetMode()
{
    return Link::Mode::Unknown;
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
