#pragma once
#include "Utilities.h"

namespace MOLib {
		namespace Lights {
			enum class LightState { kOn , kOff };
			class ToggleLight {
			public:
				ToggleLight(uint port): sol_Light(port) {}
				LightState Get() {
					return m_State;
				}
				void Set(LightState state) {
					tmr_Interval.Stop();
					m_Blinking = false;
					m_State = state;
				}
				void Blink(double seconds) {
					m_Interval = seconds;
					if (!m_Blinking) {
						tmr_Interval.Reset();
						tmr_Interval.Start();
						m_State = LightState::kOff;
						m_Blinking = true;
					}
				}
				void Update(){
					if (m_Blinking) {
						if (tmr_Interval.Get() >= m_Interval) {
							tmr_Interval.Reset();
							m_State = (m_State == LightState::kOn ? LightState::kOff : LightState::kOff);
						}
					}
					sol_Light.Set(m_State == LightState::kOn);
				}
			private:
				WPILib::Solenoid sol_Light;
				WPILib::Timer tmr_Interval;

				LightState m_State = LightState::kOff;

				bool m_Blinking = false;
				double m_Interval = 1.0;
				};
			};
		}
