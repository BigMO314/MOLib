#pragma once
#include "Utilities.h"

namespace MOLib {
		namespace Lights {
			enum class LightState { kOn , kOff };
			class ToggleLight {
			public:
				ToggleLight(uint port): sol_Light(port) {}
				bool IsOn() {
					return m_State == LightState::kOn;
				}
				void Set(LightState state) {
					tmr_Interval.Stop();
					m_Blinking = false;
					m_State = state;
				}
				void Set(bool IsOn) {
					tmr_Interval.Stop();
					m_Blinking = false;
					m_State = (IsOn() ? LightState::kOn : LightState::kOff);
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
							m_State = (!IsOn() ? LightState::kOn : LightState::kOff);
						}
					}
					sol_Light.Set(IsOn());
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
