#pragma once
#include "Utilities.h"

namespace MOLib {
namespace Lights {


	class ToggleLight{
	public:
		ToggleLight(uint port) { this->sol_Controller = new WPILib::Solenoid(port); }

		void Enable() { sol_Controller->Set(true); }
		void Disable() { sol_Controller->Set(false); }
		void Toggle() { sol_Controller->Set(!sol_Controller->Get()); }
	private:
		WPILib::Solenoid* sol_Controller;
	};

	class Blinkin{
	public:
		Blinkin(uint port){ this->controller = new WPILib::Spark(port); }
		void SetMode(double mode){ controller->Set(mode); }

		struct ColorMode {
			struct Pattern {
				struct FixedPalette {
					struct Rainbow {
						static constexpr double
						FullColor			=-0.99,
						Party				=-0.97,
						Ocean				=-0.95,
						Lava				=-0.93,
						Forest				=-0.91,
						Glitter				=-0.89;
					};

					static constexpr double
					Confetti				=-0.87;

					struct Shot {
						static constexpr double
						Red					=-0.85,
						Blue				=-0.83,
						White				=-0.81;
					};

					struct Sinelon {
						static constexpr double
						Rainbow				=-0.79,
						Party				=-0.77,
						Ocean				=-0.75,
						Lava				=-0.73,
						Forest				=-0.71;
					};

					struct BeatsPerMinute {
						static constexpr double
						Rainbow				=-0.69,
						Party				=-0.67,
						Ocean				=-0.65,
						Lava				=-0.63,
						Forest				=-0.61;
					};

					struct Fire {
						static constexpr double
						Medium				=-0.59,
						Large				=-0.57;
					};

					struct Twinkles {
						static constexpr double
						Rainbow				=-0.55,
						Party				=-0.53,
						Ocean				=-0.51,
						Lava				=-0.49,
						Forest				=-0.47;
					};

					struct ColorWaves {
						static constexpr double
						Rainbow				=-0.45,
						Party				=-0.43,
						Ocean				=-0.41,
						Lava				=-0.39,
						Forest				=-0.37;
					};

					struct LarsonScanner {
						static constexpr double
						Red					=-0.35,
						Grey				=-0.33;
					};

					struct LightChase {
						static constexpr double
						Red					=-0.31,
						Blue				=-0.29,
						Grey				=-0.27;
					};

					struct HeartBeat {
						static constexpr double
						Red					=-0.25,
						Blue				=-0.23,
						White				=-0.21,
						Grey				=-0.19;
					};

					struct Breath {
						static constexpr double
						Red					=-0.17,
						Blue				=-0.15,
						Grey				=-0.13;
					};

					struct Strobe {
						static constexpr double
						Red					=-0.11,
						Blue				=-0.09,
						Gold				=-0.07,
						White				=-0.05;
					};
				};

				struct Color1 {
					static constexpr double
					EndToEndBlendToBlack	=-0.03,
					LarsonScanner			=-0.01,
					LightChase				= 0.01;

					struct HeartBeat {
						static constexpr double
						Slow				= 0.03,
						Medium				= 0.05,
						Fast				= 0.07;
					};

					struct Breath {
						static constexpr double
						Slow				= 0.09,
						Fast				= 0.11;
					};

					static constexpr double
					Shot					= 0.13,
					Strobe					= 0.15;
				};

				struct Color2 {
					static constexpr double
					EndToEndBlendToBlack	= 0.17,
					LarsonScanner			= 0.19,
					LightChase				= 0.21;

					struct HeartBeat {
						static constexpr double
						Slow				= 0.23,
						Medium				= 0.25,
						Fast				= 0.27;
					};

					struct Breath {
						static constexpr double
						Slow				= 0.29,
						Fast				= 0.31;
					};

					static constexpr double
					Shot					= 0.33,
					Strobe					= 0.35;
				};

				struct Color1and2 {
					struct Sparkle {
						static constexpr double
						Color1OnColor2		= 0.37,
						Color2OnColor1		= 0.39;
					};

					static constexpr double
					Gradient				= 0.41,
					BeatsPerMinute			= 0.43;

					struct EndToEndBlend {
						static constexpr double
						Color1ToColor2		= 0.45,
						Color2ToColor1		= 0.47;
					};

					static constexpr double
					EndToEndNoBlend			= 0.49,
					Twinkle					= 0.51,
					ColorWave				= 0.53,
					Sinelon					= 0.55;
				};
			};

			struct SolidColor {
				static constexpr double
				HotPink						= 0.57,
				DarkRed						= 0.59,
				Red							= 0.61,
				RedOrange					= 0.63,
				Orange						= 0.65,
				Gold						= 0.67,
				Yellow						= 0.69,
				LawnGreen					= 0.71,
				Lime						= 0.73,
				DarkGreen					= 0.75,
				Green						= 0.77,
				BlueGreen					= 0.79,
				Aqua						= 0.81,
				SkyBlue						= 0.83,
				DarkBlue					= 0.85,
				Blue						= 0.87,
				BlueViolet					= 0.89,
				Violet						= 0.91,
				White						= 0.93,
				Grey						= 0.95,
				DarkGrey					= 0.97,
				Black						= 0.99;
			};
		};
	private:
		WPILib::Spark* controller;
	};
}
}
