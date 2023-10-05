// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <mutex>

#include "Constants.hxx"

class Link
{
private:
    std::string m_sLinkIp;
    int m_nTcpFd;
    int m_nUdpFd;

    std::mutex m_mutexSend;
    bool m_bDestroy = false;

    std::thread m_threadTcp;
    std::thread m_threadUdp;

public:
    enum Mode
    {
        Unknown,
        Disabled,
        Autonomous,
        Teleop
    };

    enum TalonType
    {
        Srx,
        Fx
    };

    enum DriveMode
    {
        OpenLoop,
        Velocity,
        Position
    };

    Link(std::string sLinkIp = Constants::sLinkIp);

    void Reset();
    void Destroy();

    void InitNavX();
    void InitPwmInput(int nChannel);
    void InitTalon(enum TalonType talonType, int nCanId, bool bInvert = false, bool bInvertEncoder = false, std::vector<std::tuple<int, double, double, double, double>> *pvecPidfs = nullptr);

    void UpdateTalon(int nCanId, enum DriveMode driveMode, double dSetpoint, bool bBreakMode = false);

    enum Mode GetMode();
    double GetNavXHeading();
    double GetPwmInput(int nChannel);
    double GetTalonVelocity(int nCanId);
    double GetTalonPosition(int nCanId);
};
