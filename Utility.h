#pragma once

#define NullPtr 0

#define IsNullPtr(__var__) __var__ == NullPtr

#define Not(__expression__) !__expression__

#define stageMachine(__stage__) switch(__stage__)
//#define stage case

#define Pi 3.14159265358979323846264338327950288419716939937510582
#define Circumference(X) X * Pi

constexpr long double operator "" _in (long double inches) { return inches; }
constexpr long double operator "" _ft (long double feet) { return feet * 12.0; }

constexpr long double operator "" _rpm (long double rpm) { return rpm; }
constexpr long double operator "" _rps (long double rps) { return rps * 60.0; }

namespace MOLib{
	namespace Utility{
		unsigned int typedef uint;

		long double Round(long double value, uint places){
			double off = pow(10, places);
			return (int)(value * off) / off;

		}

		int Sign(long double value) {
			if (value < 0) return -1;
			if (value > 0) return 1;
			return 0;
		}

		long double DegToRad(long double degrees){ return degrees * (Pi / 180.0); }
		long double RadToDeg(long double radians){ return radians * (180.0 / Pi); }

		void Print(std::string message) { std::cout << ">>" << message << std::endl; }
		void Print(long double message) { std::cout << ">>" << message << std::endl; }
	}
}

//Modify Scope
using namespace MOLib::Utility;
