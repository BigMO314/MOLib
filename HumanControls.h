#pragma once

#include "WPIScope.h"
#include "Utility.h"

namespace MOLib{
	namespace HumanControls{
		class XBoxController: public WPILib::XboxController{
		public:
			XBoxController(uint port, double deadband = 0.15): WPILib::XboxController(port), m_Deadband(deadband) {}
			virtual ~XBoxController() {}
			double GetX(JoystickHand hand) const override { return ApplyDeadband(-WPILib::XboxController::GetX(hand)); }
			double GetY(JoystickHand hand) const override { return ApplyDeadband(-WPILib::XboxController::GetY(hand)); }
			double GetTriggerAxis(JoystickHand hand) const override { return ApplyDeadband(WPILib::XboxController::GetTriggerAxis(hand)); }

			void SetDeadband(double value){ m_Deadband = value; }

		private:
			double m_Deadband;
			double ApplyDeadband(double value) const {
				if(fabs(value) < m_Deadband) value = 0.0;
				return value;
			}
		};
	}
}

//Modify Scope
namespace MOLib{
	using namespace HumanControls;
}
