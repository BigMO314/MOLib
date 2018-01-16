#pragma once

#include "Utilities.h"

namespace MOLib {
	namespace Drivetrain {
		enum class ShiftState { kHighSpeed, kLowSpeed };
		namespace TankScheme {
			namespace Motors {
				class Base {
				public:
					virtual void Update() {}
					void SetScale(double lScale, double rScale) {
						m_Scale.LF = lScale; m_Scale.LR = lScale;
						m_Scale.RF = rScale; m_Scale.RR = rScale;
					}
					void SetScale(double lfScale, double lrScale, double rfScale, double rrScale) {
						m_Scale.LF = lfScale; m_Scale.LR = lrScale;
						m_Scale.RF = rfScale; m_Scale.RR = rrScale;
					}
					void SetDrive(double lPower, double rPower) {
						m_Power.L = lPower;
						m_Power.R = rPower;
					}
					void SetRestriction(double newRestriction){
						m_Restriction = newRestriction;
					}
				protected:
					virtual ~Base() {}
					struct { double LF, LR, RF, RR; } m_Scale{ 1.0, 1.0, 1.0, 1.0 };
					struct { double L, R; } m_Power{ 0.0, 0.0 };
					double m_Restriction = 1.0;
				};

				class TwoTalon : public Base {
				public:
					TwoTalon(CTRLib::TalonSRX *mtr_L_Drive, CTRLib::TalonSRX *mtr_R_Drive) {
						this->mtr_L_Drive = mtr_L_Drive;
						this->mtr_R_Drive = mtr_L_Drive;
					}
					virtual void Update() override {
						mtr_L_Drive->Set(CTRLib::ControlMode::PercentOutput, m_Power.L * (m_Power.L > 0 ? m_Scale.LF : m_Scale.LR) * m_Restriction);
						mtr_R_Drive->Set(CTRLib::ControlMode::PercentOutput, m_Power.R * (m_Power.R > 0 ? m_Scale.RF : m_Scale.RR) * m_Restriction);
					}
				private:
					CTRLib::TalonSRX *mtr_L_Drive;
					CTRLib::TalonSRX *mtr_R_Drive;
				};
			}
			namespace Shifters{
				class Base {
				public:
					Base(ShiftState defaultState = ShiftState::kLowSpeed) { m_Default = defaultState; m_State = defaultState; }
					virtual ~Base() {}
					void Set(ShiftState newState) { m_State = newState; }
					virtual void Update() {}
				protected:
					ShiftState m_Default;
					ShiftState m_State;
				};

				class NoShifter : public Base {
				public:
					NoShifter() {}
				};

				class SingleShifter : public Base {
				public:
					SingleShifter(WPILib::Solenoid *sol_Shifter, ShiftState defaultState)
					:Base(defaultState) { this->sol_Shifter = sol_Shifter; }
					virtual void Update() override { sol_Shifter->Set(m_State != m_Default); }
				private:
					WPILib::Solenoid *sol_Shifter;
				};

				class DoubleShifter : public Base {
				public:
					DoubleShifter(WPILib::DoubleSolenoid *sol_Shifter, ShiftState defaultState)
					:Base(defaultState) { this->sol_Shifter = sol_Shifter; }
					virtual void Update() override { sol_Shifter->Set(m_State == m_Default ? WPILib::DoubleSolenoid::kReverse : WPILib::DoubleSolenoid::kForward); }
				private:
					WPILib::DoubleSolenoid *sol_Shifter;
				};
			}
			class TankDrivetrain {
			public:
				TankDrivetrain(CTRLib::TalonSRX *mtr_L_Drive, CTRLib::TalonSRX *mtr_R_Drive) {
					this->drv_Motors = new Motors::TwoTalon(mtr_L_Drive, mtr_R_Drive);
					this->drv_Shifters = new Shifters::NoShifter();
				}
				TankDrivetrain(
						CTRLib::TalonSRX *mtr_L_Drive, CTRLib::TalonSRX *mtr_R_Drive,
						WPILib::Solenoid *sol_Shifter) {
					this->drv_Motors = new Motors::TwoTalon(mtr_L_Drive, mtr_R_Drive);
					this->drv_Shifters = new Shifters::SingleShifter(sol_Shifter, ShiftState::kLowSpeed);
				}

				void SetScale(double lScale, double rScale) { drv_Motors->SetScale(lScale, rScale); }
				void SetScale(double lfScale, double lrScale, double rfScale, double rrScale) { drv_Motors->SetScale(lfScale, lrScale, rfScale, rrScale); }
				void SetRestriction(double newRestriction) { drv_Motors->SetRestriction(newRestriction); }
				void SetDrive(double lPower, double rPower) { drv_Motors->SetDrive(lPower, rPower); }
				void SetShift(ShiftState newState) { drv_Shifters->Set(newState); }

				void Update() {
					drv_Motors->Update();
					drv_Shifters->Update();
				}
			private:
				Motors::Base	*drv_Motors;
				Shifters::Base	*drv_Shifters;
			};
		}
	}
}
