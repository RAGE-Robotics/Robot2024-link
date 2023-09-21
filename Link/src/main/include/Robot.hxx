// Author: Caleb Heydon <probablyme@caleb.enginee>
// License: GPL-3.0

#pragma once

#include <frc/TimedRobot.h>

class Robot : public frc::TimedRobot
{
public:
    void RobotInit() override;
    void RobotPeriodic() override;
};
