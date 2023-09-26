// Author: Caleb Heydon <probablyme@caleb.engineer>
// License: GPL-3.0

#pragma once

#include <frc/TimedRobot.h>

class Robot : public frc::TimedRobot
{
public:
    void RobotInit() override;
    void DisabledInit() override;
    void AutonomousInit() override;
    void TeleopInit() override;
};