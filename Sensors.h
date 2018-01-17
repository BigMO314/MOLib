#pragma once
#include "Utilities.h"
namespace MOLib{
	namespace Sensors{
		class Dial {
		public:
			Dial(uint port): pot_Source(port) {}
			void ConfigPositions(uint maxPositions) { m_Positions = maxPositions; }
			uint GetPosition() {
				uint pos;
				double curVoltage = pot_Source.Get();
				for(pos = 1; pos < m_Positions; pos ++)
					if(curVoltage >= (m_MaxVoltage / (double)m_Positions) * (double)(pos - 1) && curVoltage < (m_MaxVoltage / (double)m_Positions) * (double)(pos))
						break;
				return pos;
			}
		private:
			WPILib::AnalogPotentiometer pot_Source;
			uint m_Positions = 1;
			const double m_MaxVoltage = 1.0;
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
