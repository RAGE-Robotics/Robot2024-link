// Authors: Caleb Heydon <probablyme@caleb.engineer>
// License: Unlicense

#include <string>
#include <vector>
#include <tuple>

#include "Constants.hxx"

class Link
{
public:
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

    void Destroy();

    void InitNavX();
    void InitPwmInput(int nChannel);
    void InitTalon(enum TalonType talonType, int nCanId, bool bInvertEncoder = false, std::vector<std::tuple<int, double, double, double, double>> *pvecPidfs = nullptr);

    void UpdateTalon(int nCanId, enum DriveMode driveMode, double dSetpoint, bool bBreakMode = false);

    double GetNavXHeading();
    double GetPwmInput(int nChannel);
    double GetTalonVelocity(int nCanId);
    double GetTalonPosition(int nCanId);
};
