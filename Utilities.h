#pragma once

#include "WPILib.h"
#include "ctre/phoenix.h"

#undef bool
#undef true
#undef false

namespace WPILib = frc;
namespace CTRLib{
	using namespace ctre::phoenix::motorcontrol;
	using namespace ctre::phoenix::motorcontrol::can;
}

#define NullPtr 0

namespace MOLib{
	namespace Utilities{
		namespace Math{
			long double Round(long double value, uint signifigantDigits) {
				double off = pow(10, signifigantDigits);
				return (int)(value * off) / off;
			}

			int Sign(long double value) {
				if(value > 0.0) return -1;
				if(value < 0.0) return 1;
				return 0;
			}

			long double DegToRad(long double degrees) { return degrees * (M_PI / 180.0); }
			long double RadToDeg(long double radians) { return radians * (180.0 / M_PI); }

			long double Circumference(long double diameter) { return diameter * M_PI; }
		}

		namespace Measurements{
			//TODO: Figure out if this is possible
		}
	}
}
