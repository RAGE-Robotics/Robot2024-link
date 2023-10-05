// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include "Constants.hxx"

class Link
{
private:
    std::string m_sLinkIp;
    int m_nTcpFd = -1;
    int m_nUdpFd = -1;

    std::mutex m_mutexSend;
    std::atomic<bool> m_bDestroy;

    std::thread m_threadTcp;
    std::thread m_threadUdp;

    std::function<void(struct GamepadState)> m_funcGamepadCallback;

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

    struct GamepadState
    {
        int nId;
        bool bA, bB, bX, bY;
        int nDPad;
        bool bShoulderLeft, bShoulderRight;
        double dTriggerLeft, dTriggerRight;
        std::tuple<double, double> tJoystickLeft, tJoystickRight;
    };

    Link(std::string sLinkIp = Constants::sLinkIp);

    void Reset();
    void Destroy();

    void InitNavX();
    void InitPwmInput(int nChannel);
    void InitTalon(enum TalonType talonType, int nCanId, bool bInvert = false, bool bInvertEncoder = false, std::vector<std::tuple<int, double, double, double, double>> *pvecPidfs = nullptr);
    void InitGamepad(int nId);

    void SetGamepadCallback(std::function<void(struct GamepadState)>);

    void UpdateTalon(int nCanId, enum DriveMode driveMode, double dSetpoint, bool bBreakMode = false);

    enum Mode GetMode();
    double GetNavXHeading();
    double GetPwmInput(int nChannel);
    double GetTalonVelocity(int nCanId);
    double GetTalonPosition(int nCanId);
    struct GamepadState GetGamepadState(int nId);
};
