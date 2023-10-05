// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include "Link.hxx"

#include <cstring>
#include <barrier>
#include <stdexcept>

#ifdef _WIN32
#include <WinSock2.h>
#endif

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

    std::barrier barrier(3);
    m_threadTcp = std::thread([this, &barrier]
                              {
                                  m_nTcpFd = socket(AF_INET, SOCK_STREAM, 0);
                                  if (m_nTcpFd == -1)
                                      throw std::runtime_error("failed to create socket");
                                  barrier.arrive_and_wait();

                                  while (!m_bStop)
                                      ;

#ifdef _WIN32
                                  closesocket(m_nTcpFd);
#else
                                  close(m_TcpFd);
#endif
                              });
    m_threadUdp = std::thread([this, &barrier]
                              {
                                  m_nUdpFd = socket(AF_INET, SOCK_DGRAM, 0);
                                  if (m_nUdpFd == -1)
                                      throw std::runtime_error("failed to create socket");
                                  barrier.arrive_and_wait();

                                  while (!m_bStop)
                                      ;

#ifdef _WIN32
                                  closesocket(m_nTcpFd);
#else
                                  close(m_TcpFd);
#endif
                              });
    barrier.arrive_and_wait();
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

void Link::SetModeCallback(std::function<void(enum Mode)> funcModeCallback)
{
    m_mutexModeCallback.lock();
    m_funcModeCallback = funcModeCallback;
    m_mutexModeCallback.unlock();
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
    double dHeading;
    m_mutexUdpReceive.lock();
    dHeading = m_dNavXHeading;
    m_mutexUdpReceive.unlock();
    return dHeading;
}

double Link::GetPwmInput(int nChannel)
{
    double dValue = -1;
    m_mutexUdpReceive.lock();
    if (m_mapPwmInputs.find(nChannel) != m_mapPwmInputs.end())
        dValue = m_mapPwmInputs[nChannel];
    m_mutexUdpReceive.unlock();
    return dValue;
}

double Link::GetTalonVelocity(int nCanId)
{
    double dValue = -1;
    m_mutexUdpReceive.lock();
    if (m_mapTalonVelocities.find(nCanId) != m_mapTalonVelocities.end())
        dValue = m_mapTalonVelocities[nCanId];
    m_mutexUdpReceive.unlock();
    return dValue;
}

double Link::GetTalonPosition(int nCanId)
{
    double dValue = -1;
    m_mutexUdpReceive.lock();
    if (m_mapTalonPositions.find(nCanId) != m_mapTalonPositions.end())
        dValue = m_mapTalonPositions[nCanId];
    m_mutexUdpReceive.unlock();
    return dValue;
}

struct Link::GamepadState Link::GetGamepadState(int nId)
{
    struct Link::GamepadState gamepadState;
    std::memset(&gamepadState, 0, sizeof(gamepadState));
    gamepadState.nId = -1;
    gamepadState.nDPad = -1;
    gamepadState.dTriggerLeft = gamepadState.dTriggerRight = -1;
    gamepadState.tJoystickLeft = gamepadState.tJoystickRight = std::tuple<double, double>(-1, -1);

    m_mutexTcpReceive.lock();
    m_mutexUdpReceive.lock();

    if (m_mapGamepadStates.find(nId) != m_mapGamepadStates.end())
        gamepadState = m_mapGamepadStates[nId];

    m_mutexUdpReceive.unlock();
    m_mutexTcpReceive.unlock();

    return gamepadState;
}

std::string Link::GetAutonomousMode()
{
    std::string sAutonomousMode;
    m_mutexTcpReceive.lock();
    sAutonomousMode = m_sAutonomousMode;
    m_mutexTcpReceive.unlock();
    return sAutonomousMode;
}
