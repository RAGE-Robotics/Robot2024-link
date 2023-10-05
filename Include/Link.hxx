// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: CC0-1.0

#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <map>

#include "Constants.hxx"

class Link
{
public:
    enum Mode
    {
        Unknown,
        Disabled,
        Autonomous,
        Teleop,
        Test
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

    Link(std::string sLinkIp = Constants::pszLinkIp);

    void Reset();
    void Destroy();

    void InitNavX();
    void InitPwmInput(int nChannel);
    void InitTalon(enum TalonType talonType, int nCanId, bool bInvert = false, bool bInvertEncoder = false, std::vector<std::tuple<int, double, double, double, double>> vecPidfs = std::vector<std::tuple<int, double, double, double, double>>());
    void InitGamepad(int nId);

    void SetModeCallback(std::function<void(enum Mode)> funcModeCallback);
    void SetGamepadCallback(std::function<void(struct GamepadState)> funcGamepadCallback);

    void UpdateTalon(int nCanId, enum DriveMode driveMode, double dSetpoint, bool bBreakMode = false);

    enum Mode GetMode();
    double GetNavXHeading();
    double GetPwmInput(int nChannel);
    double GetTalonVelocity(int nCanId);
    double GetTalonPosition(int nCanId);
    struct GamepadState GetGamepadState(int nId);

private:
    std::string m_sLinkIp;
    int m_nTcpFd;
    int m_nUdpFd;

    std::atomic<bool> m_bStop;

    std::mutex m_mutexTcpWrite;
    std::mutex m_mutexTcpReceive;
    std::mutex m_mutexUdpReceive;

    std::thread m_threadTcp;
    std::thread m_threadUdp;

    std::mutex m_mutexModeCallback;
    std::function<void(enum Mode)> m_funcModeCallback;

    std::mutex m_mutexGamepadCallback;
    std::function<void(struct GamepadState)> m_funcGamepadCallback;

    std::mutex m_mutexInits;
    bool m_bNavX = false;
    std::vector<int> m_vecPwms;
    std::vector<std::tuple<int, int, bool, bool, std::vector<std::tuple<int, double, double, double, double>>>> m_vecTalons;
    std::vector<int> m_vecGamepads;

    enum Mode m_mode = Mode::Unknown;
    double m_dNavXHeading = 0;
    std::map<int, double> m_mapPwmInputs;
    std::map<int, double> m_mapTalonVelocities;
    std::map<int, double> m_mapTalonPositions;
    std::map<int, struct GamepadState> m_mapGamepadStates;
};
