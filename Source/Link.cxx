// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include "Link.hxx"

Link::Link(std::string sLinkIp) : m_sLinkIp(sLinkIp)
{
    m_bStop = true;

    Reset();
}

void Link::Reset()
{
    if (!m_bStop)
    {
        m_bStop = true;

        if (m_threadTcp.joinable())
            m_threadTcp.join();
        if (m_threadUdp.joinable())
            m_threadUdp.join();
    }
    m_bStop = false;

    m_threadTcp = std::thread([this]
                              {while (!m_bStop); });
    m_threadUdp = std::thread([this]
                              {while (!m_bStop); });
}

void Link::Destroy()
{
    m_bStop = true;

    if (m_threadTcp.joinable())
        m_threadTcp.join();
    if (m_threadUdp.joinable())
        m_threadUdp.join();
}

void Link::InitNavX()
{
    m_mutexInits.lock();
    m_bNavX = true;
    m_mutexInits.unlock();
}

void Link::InitPwmInput(int nChannel)
{
    m_mutexInits.lock();
    m_vecPwms.push_back(nChannel);
    m_mutexInits.unlock();
}

void Link::InitTalon(enum TalonType talonType, int nCanId, bool bInvert, bool bInvertEncoder, std::vector<std::tuple<int, double, double, double, double>> vecPidfs)
{
    m_mutexInits.lock();
    m_vecTalons.push_back(std::tuple<int, int, bool, bool, std::vector<std::tuple<int, double, double, double, double>>>(talonType, nCanId, bInvert, bInvertEncoder, vecPidfs));
    m_mutexInits.unlock();
}

void Link::InitGamepad(int nId)
{
    m_mutexInits.lock();
    m_vecGamepads.push_back(nId);
    m_mutexInits.unlock();
}

void Link::SetGamepadCallback(std::function<void(struct Link::GamepadState)> funcGamepadCallback)
{
    m_mutexGamepadCallback.lock();
    m_funcGamepadCallback = funcGamepadCallback;
    m_mutexGamepadCallback.unlock();
}

void Link::UpdateTalon(int nCanId, enum Link::DriveMode driveMode, double dSetpoint, bool bBreakMode)
{
}

enum Link::Mode Link::GetMode()
{
    enum Link::Mode mode;
    m_mutexTcpReceive.lock();
    mode = m_mode;
    m_mutexTcpReceive.unlock();
    return mode;
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

struct Link::GamepadState Link::GetGamepadState(int nId)
{
    struct Link::GamepadState gamepadState;
    gamepadState.nId = nId;

    return gamepadState;
}
