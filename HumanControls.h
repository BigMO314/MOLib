#pragma once

#include "WPIScope.h"
#include "Utility.h"

namespace MOLib{
	namespace HumanControls{
		class XboxController: public WPILib::XboxController{
		public:
			XboxController(uint port, double deadband = 0.15): WPILib::XboxController(port), m_Deadband(deadband) {}
			virtual ~XBoxController() {}
			double GetX(JoystickHand hand) const override { return ApplyDeadband(-WPILib::XboxController::GetX(hand)); }
			double GetY(JoystickHand hand) const override { return ApplyDeadband(-WPILib::XboxController::GetY(hand)); }
			double GetTriggerAxis(JoystickHand hand) const override { return ApplyDeadband(WPILib::XboxController::GetTriggerAxis(hand)); }

			void ConfigDeadband(double value){ m_Deadband = value; }

		private:
			double m_Deadband;
			double ApplyDeadband(double value) const {
				return (fabs(value)<m_Deadband ? 0 : (value<0 ? (value/(1-m_Deadband))+(1/((1-m_Deadband)/m_Deadband)) : (value/(1-m_Deadband))-(1/((1-m_Deadband)/m_Deadband))));
			}
		};
	}
}

//Modify Scope
namespace MOLib{
	using namespace HumanControls;
}
