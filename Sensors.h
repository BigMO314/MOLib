#pragma once

#include "WPIScope.h"
#include "Utility.h"

namespace MOLib{
	namespace Sensors{
		class Dial{
		public:
			Dial(uint port): pot_Source(port){}
		private:
			WPILib::AnalogPotentiometer pot_Source;
			uint m_Positions = 1;
		};

		class AnalogGyro : public WPILib::AnalogGyro{
		public:
			AnalogGyro(int channel)
			:WPILib::AnalogGyro(channel) {}

			void SetAngleScale(double scale) { m_Scale = scale; }
			double GetAngle() const override { return WPILib::AnalogGyro::GetAngle() * m_Scale; }
		private:
			double m_Scale = 1.0;
		};
	}
}

//Modify Scope
namespace MOLib{
	using Sensors::AnalogGyro;
}
