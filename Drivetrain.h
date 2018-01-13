/**
 * File Description
 * @author
 * @since
 */

#pragma once

#include "WPIScope.h"
#include "Utility.h"

#include "PID.h"
#include "Vision.h"

namespace MOLib {
	namespace Drivetrain {
		enum class ShiftState { kHighSpeed, kLowSpeed };
		namespace TankScheme {
			namespace Motors {
				class MotorSetup {
				public:
					MotorSetup(
							WPILib::SpeedController* ref_L_Drive_1, WPILib::SpeedController* ref_L_Drive_2, WPILib::SpeedController* ref_L_Drive_3,
							WPILib::SpeedController* ref_R_Drive_1, WPILib::SpeedController* ref_R_Drive_2, WPILib::SpeedController* ref_R_Drive_3) {
						this->mtr_L_Drive_1 = ref_L_Drive_1;
						this->mtr_L_Drive_2 = ref_L_Drive_2;
						this->mtr_L_Drive_3 = ref_L_Drive_3;
						this->mtr_R_Drive_1 = ref_R_Drive_1;
						this->mtr_R_Drive_2 = ref_R_Drive_2;
						this->mtr_R_Drive_3 = ref_R_Drive_3;
					}

					void SetScale(float lScale, float rScale) {
						m_LForwardScale = lScale; m_RForwardScale = rScale;
						m_LReverseScale = lScale; m_RReverseScale = rScale;
					}

					void SetScale(
							float lForwardScale, float rForwardScale,
							float lReverseScale, float rReverseScale) {
						m_LForwardScale = lForwardScale; m_RForwardScale = rForwardScale;
						m_LReverseScale = lReverseScale; m_RReverseScale = rReverseScale;
					}

					void SetDrive(float lPower, float rPower) { m_LPower = lPower; m_RPower = rPower; }

					void Update() {
						if (mtr_L_Drive_1) mtr_L_Drive_1->Set(m_LPower * (m_LPower > 0 ? m_LForwardScale : m_LReverseScale));
						if (mtr_L_Drive_2) mtr_L_Drive_2->Set(m_LPower * (m_LPower > 0 ? m_LForwardScale : m_LReverseScale));
						if (mtr_L_Drive_3) mtr_L_Drive_3->Set(m_LPower * (m_LPower > 0 ? m_LForwardScale : m_LReverseScale));
						if (mtr_R_Drive_1) mtr_R_Drive_1->Set(m_RPower * (m_RPower > 0 ? m_RForwardScale : m_RReverseScale));
						if (mtr_R_Drive_2) mtr_R_Drive_2->Set(m_RPower * (m_RPower > 0 ? m_RForwardScale : m_RReverseScale));
						if (mtr_R_Drive_3) mtr_R_Drive_3->Set(m_RPower * (m_RPower > 0 ? m_RForwardScale : m_RReverseScale));
					}
				private:
					WPILib::SpeedController* mtr_L_Drive_1;
					WPILib::SpeedController* mtr_L_Drive_2;
					WPILib::SpeedController* mtr_L_Drive_3;
					WPILib::SpeedController* mtr_R_Drive_1;
					WPILib::SpeedController* mtr_R_Drive_2;
					WPILib::SpeedController* mtr_R_Drive_3;

					float m_LForwardScale	= 1.0;
					float m_RForwardScale	= 1.0;
					float m_LReverseScale	= 1.0;
					float m_RReverseScale	= 1.0;

					float m_LPower			= 0.0;
					float m_RPower			= 0.0;
				};
			}
			namespace Shifters {
				class ShifterSetup {
				public:
					virtual ~ShifterSetup() {}
					ShifterSetup(ShiftState extendedState){
						this->m_ExtendedState = extendedState;
						this->m_State = (m_ExtendedState == ShiftState::kHighSpeed ? ShiftState::kLowSpeed : ShiftState::kHighSpeed);
					}
					ShiftState Get() const { return m_State; }
					void Set(ShiftState state) { m_State = state; }

					virtual void Update() {}
				protected:
					ShiftState m_State;
					ShiftState m_ExtendedState;
				};

				/**	SingleShifter
				 *	One single solenoid with spring loaded retraction
				 */
				class SingleShifter : public ShifterSetup {
				public:
					virtual ~SingleShifter() {}
					SingleShifter(WPILib::Solenoid* ref_Shifter, ShiftState extendedState)
					:ShifterSetup(extendedState) {
						this->sol_Shifter = ref_Shifter;
						this->m_ExtendedState = extendedState;
					}

					void Update() override { sol_Shifter->Set(m_State == m_ExtendedState); }
				private:
					WPILib::Solenoid* sol_Shifter;
				};

				/**	OneDoubleShifter
				 *	One double solenoid to control both directions
				 */
				class DoubleShifter : public ShifterSetup {
				public:
					virtual ~DoubleShifter() {}
					DoubleShifter(WPILib::DoubleSolenoid* ref_Shifter, ShiftState extendedState)
					:ShifterSetup(extendedState) {
						this->dbs_Shifter = ref_Shifter;
					}

					void Update() override {
						dbs_Shifter->Set(m_State == m_ExtendedState ? WPILib::DoubleSolenoid::kForward : WPILib::DoubleSolenoid::kReverse);
					}
				private:
					WPILib::DoubleSolenoid* dbs_Shifter;
				};
			}
			class TankDrivetrain {
			public:
//---------------------------------------------------------------------------------------------------------------------------------------------------|
				//No shifters
				TankDrivetrain(
						WPILib::SpeedController* ref_L_Drive_1, WPILib::SpeedController* ref_L_Drive_2, WPILib::SpeedController* ref_L_Drive_3,
						WPILib::SpeedController* ref_R_Drive_1, WPILib::SpeedController* ref_R_Drive_2, WPILib::SpeedController* ref_R_Drive_3,
						WPILib::Encoder* ref_DriveDistance = NullPtr, WPILib::GyroBase* ref_DriveAngle = NullPtr, MOLib::Vision::Target* ref_Target = NullPtr) {
					drv_Motors = new Motors::MotorSetup(ref_L_Drive_1, ref_L_Drive_2, ref_L_Drive_3, ref_R_Drive_1, ref_R_Drive_2, ref_R_Drive_3);
					drv_Shifters = NullPtr;

					enc_DriveDistance = ref_DriveDistance;
					gyr_DriveAngle = ref_DriveAngle;
					vis_Target = ref_Target;

					CreatePIDs();
				}

				//Single Solenoid shifter
				TankDrivetrain(
						WPILib::SpeedController* ref_L_Drive_1, WPILib::SpeedController* ref_L_Drive_2, WPILib::SpeedController* ref_L_Drive_3,
						WPILib::SpeedController* ref_R_Drive_1, WPILib::SpeedController* ref_R_Drive_2, WPILib::SpeedController* ref_R_Drive_3,
						WPILib::Solenoid* ref_Shifter, ShiftState extendedState,
						WPILib::Encoder* ref_DriveDistance = NullPtr, WPILib::GyroBase* ref_DriveAngle = NullPtr, MOLib::Vision::Target* ref_Target = NullPtr) {
					drv_Motors = new Motors::MotorSetup(ref_L_Drive_1, ref_L_Drive_2, ref_L_Drive_3, ref_R_Drive_1, ref_R_Drive_2, ref_R_Drive_3);
					drv_Shifters = new Shifters::SingleShifter(ref_Shifter, extendedState);

					enc_DriveDistance = ref_DriveDistance;
					gyr_DriveAngle = ref_DriveAngle;
					vis_Target = ref_Target;
					CreatePIDs();
				}

				//Double Solenoid shifter
				TankDrivetrain(
						WPILib::SpeedController* ref_L_Drive_1, WPILib::SpeedController* ref_L_Drive_2, WPILib::SpeedController* ref_L_Drive_3,
						WPILib::SpeedController* ref_R_Drive_1, WPILib::SpeedController* ref_R_Drive_2, WPILib::SpeedController* ref_R_Drive_3,
						WPILib::DoubleSolenoid* ref_Shifter, ShiftState extendedState,
						WPILib::Encoder* ref_DriveDistance = NullPtr, WPILib::GyroBase* ref_DriveAngle = NullPtr, MOLib::Vision::Target* ref_Target = NullPtr) {
					drv_Motors = new Motors::MotorSetup(ref_L_Drive_1, ref_L_Drive_2, ref_L_Drive_3, ref_R_Drive_1, ref_R_Drive_2, ref_R_Drive_3);
					drv_Shifters = new Shifters::DoubleShifter(ref_Shifter, extendedState);

					enc_DriveDistance = ref_DriveDistance;
					gyr_DriveAngle = ref_DriveAngle;
					vis_Target = ref_Target;

					CreatePIDs();
				}

//---------------------------------------------------------------------------------------------------------------------------------------------------|
				MOLib::PIDLoop* pid_DriveDistance = NullPtr;
				MOLib::PIDLoop* pid_DriveStraight = NullPtr;

				bool IsDistancePIDEnabled() const {
					if(UsingEnc()) {
						if(UsingGyro()) { return pid_DriveDistance->IsEnabled() && pid_DriveStraight->IsEnabled(); }
						return pid_DriveDistance->IsEnabled();
					}
					return false;
				}

				bool IsAtDistance() const {
					if(UsingEnc()) {
						if(UsingGyro()) { return pid_DriveDistance->OnTarget() && pid_DriveStraight->OnTarget(); }
						return pid_DriveDistance->OnTarget();
					}
					return false;
				}

				void GoToDistance(double inches, bool resetDistance = true) {
					StopAnglePID();
					StopVisionPID();

					if(UsingEnc()) {
						if(!pid_DriveDistance->IsEnabled()){
							pid_DriveDistance->SetSetpoint(inches);
							pid_DriveDistance->Enable();
							if(resetDistance) { pid_DriveDistance->ResetSource(); }

							if(UsingGyro()){
								if(!pid_DriveStraight->IsEnabled()) {
									pid_DriveStraight->SetSetpoint(0);
									pid_DriveStraight->Enable();
									if(resetDistance) { pid_DriveStraight->ResetSource(); }
								}
							}
						}
					}
				}

				void StopDistancePID() {
					if(UsingEnc()) pid_DriveDistance->Disable();
					if(UsingGyro()) pid_DriveStraight->Disable();
				}

//---------------------------------------------------------------------------------------------------------------------------------------------------|
				MOLib::PIDLoop* pid_DriveAngle = NullPtr;

				bool IsAnglePIDEnabled() const { if(UsingGyro()) return pid_DriveAngle->IsEnabled(); return false; }

				bool IsAtAngle() const { if(UsingGyro()) return pid_DriveAngle->OnTarget(); return false; }

				bool GoToAngle(double degrees, bool resetAngle = true) {
					StopDistancePID();
					StopVisionPID();
					if(UsingGyro()) {
						if(resetAngle) pid_DriveAngle->ResetSource();
						pid_DriveAngle->SetSetpoint(degrees);
						pid_DriveAngle->Enable();
					}
					return IsAtAngle();
				}

				void StopAnglePID() { if(UsingGyro()) pid_DriveAngle->Disable(); }

//---------------------------------------------------------------------------------------------------------------------------------------------------|
				MOLib::PIDLoop* pid_GoalAngle = NullPtr;

				bool IsVisionPIDEnabled() const { if(UsingVision()) return pid_GoalAngle->IsEnabled(); return false; }

				bool IsVisionPIDAlignedToGoal() const { if(UsingVision()) return pid_GoalAngle->OnTarget(); return false; }

				bool GetVisionPIDError() const { if (UsingVision()) return pid_GoalAngle->GetError(); return false; }

				bool AlignToGoal() {
					StopDistancePID();
					StopAnglePID();
					if(UsingVision()) {
						//std::cout << "m_VisionStep: " << m_VisionStep << " Vision PID Enabled: " << pid_GoalAngle->IsEnabled() << "Vision Angle Calculated" << vis_Target->GetAnglePrint() << "Vision Target" << pid_GoalAngle->GetSetpoint() << std::endl;
						//if(m_VisionStep == -1){ m_VisionStep = 0; }
						if(!pid_GoalAngle->IsEnabled()){ vis_Target->RequestValue(); pid_GoalAngle->Enable();}
						pid_GoalAngle->SetSetpoint(0.0);
						pid_GoalAngle->Enable();
					}
					return IsVisionPIDAlignedToGoal();
				}

				void SetAutoAlignDrive(float power){ AlignToGoal(); SetArcadeDrive(power, pid_GoalAngle->Get()); }

				void StopVisionPID() { if(UsingVision()) pid_GoalAngle->Disable(); }

//---------------------------------------------------------------------------------------------------------------------------------------------------|

				void SetTankDrive(float lPower, float rPower) { drv_Motors->SetDrive(lPower, rPower); }

				void SetArcadeDrive(float throttle, float steering) { drv_Motors->SetDrive(throttle+steering, throttle-steering); }

//				float GetScale() const { return drv_Motors->GetScale(); }
				void SetScale(float lscale, float rscale) { drv_Motors->SetScale(lscale, rscale); }
				void SetScale(float lForwardScale, float rForwardScale, float lReverseScale, float rReverseScale) { drv_Motors->SetScale(lForwardScale, rForwardScale, lReverseScale, rReverseScale); }

				ShiftState GetShift() const { return (UsingShifters() ? drv_Shifters->Get() : ShiftState::kLowSpeed); }
				void SetShift(ShiftState state) { if(UsingShifters()) drv_Shifters->Set(state); }

				void ResetDistance() { if(UsingEnc()) enc_DriveDistance->Reset(); }

				void ResetAngle() { if(UsingGyro()) gyr_DriveAngle->Reset(); }
				void CalibrateAngle() { if(UsingGyro()) gyr_DriveAngle->Calibrate(); }

				double GetDistance() const { return (UsingEnc() ? enc_DriveDistance->GetDistance() : 0.0); }
				double GetAngle() const { return (UsingGyro() ? gyr_DriveAngle->GetAngle() : 0.0); }

				void SetWheelDiameter(double diameter) {
					m_WheelDiameter = diameter;
					if(UsingEnc()) enc_DriveDistance->SetDistancePerPulse(((m_WheelDiameter * Pi) * m_GearRatio) / 2048);
				}

				void SetGearRatio(double ratio) {
					m_GearRatio = ratio;
					if(UsingEnc()) enc_DriveDistance->SetDistancePerPulse(((m_WheelDiameter * Pi) * m_GearRatio) / 2048);
				}

				void Update() {
					if(UsingGyro()) {
						if(pid_DriveAngle->IsEnabled()){
							if(pid_DriveAngle->OnTarget()) pid_DriveAngle->Disable();
							drv_Motors->SetDrive(pid_DriveAngle->Get(), -pid_DriveAngle->Get());
						}
					}
					if(UsingEnc() && !UsingGyro()) {
						if(pid_DriveDistance->IsEnabled()){//TODO Finish all UsingEnc / UsingGyro including this one
							if(pid_DriveDistance->OnTarget()) pid_DriveDistance->Disable();
							drv_Motors->SetDrive(pid_DriveDistance->Get(), -pid_DriveDistance->Get());
						}
					}
					if(UsingEnc() && UsingGyro()) {
						if(pid_DriveDistance->IsEnabled() && pid_DriveStraight->IsEnabled()) {
							if(pid_DriveDistance->OnTarget() && pid_DriveStraight->OnTarget()) { StopDistancePID(); }
							SetArcadeDrive(pid_DriveDistance->Get(), pid_DriveStraight->Get());
						}
					}
					if(UsingVision()) {
						/*switch(m_VisionStep){
						case 0:
							vis_Target->RequestValue();
							m_VisionStep = 1;
							break;
						case 1:
							if(vis_Target->ValuesReady()){
								pid_GoalAngle->Enable();
								pid_GoalAngle->ResetSource();
								pid_GoalAngle->SetSetpoint(vis_Target->GetAngle());
								m_VisionStep = 2;
							}
							break;
						case 2:
							if(pid_GoalAngle->OnTarget()){ pid_GoalAngle->Disable(); m_VisionStep = -1; }
							break;
						}*/
						if(pid_GoalAngle->IsEnabled()){
							vis_Target->RequestValue();
							//if(vis_Target->ValuesReady()){  }
							//if(pid_GoalAngle->OnTarget()) pid_GoalAngle->Disable();
							//drv_Motors->SetDrive(pid_GoalAngle->Get(), -pid_GoalAngle->Get());
						}
					}

					drv_Motors->Update();
					if(UsingShifters()) drv_Shifters->Update();
				}

			private:
				//int m_VisionStep = -1;
				Motors::MotorSetup* drv_Motors;
				Shifters::ShifterSetup* drv_Shifters;

				WPILib::Encoder* enc_DriveDistance;
				WPILib::GyroBase* gyr_DriveAngle;
				MOLib::Vision::Target* vis_Target;

				double m_WheelDiameter = 0.0;
				double m_GearRatio = 1.0;

//-------------------------------------------------------------------------------------------------|

				bool UsingShifters() const { return !IsNullPtr(drv_Shifters); }

				bool UsingEnc() const { return !IsNullPtr(enc_DriveDistance); }
				bool UsingGyro() const { return !IsNullPtr(gyr_DriveAngle); }
				bool UsingVision() const { return !IsNullPtr(vis_Target); }

//-------------------------------------------------------------------------------------------------|

				void CreatePIDs() {
					if(UsingEnc()) {
						pid_DriveDistance = new MOLib::PID::EncLoop(enc_DriveDistance);
						pid_DriveDistance->SetOutputRange(-1.0, 1.0);
						pid_DriveDistance->SetTargetTime(0.1);
						pid_DriveDistance->SetAbsoluteTolerance(1.0);
					}

					if(UsingGyro()) {
						pid_DriveStraight = new MOLib::PID::GyrLoop(gyr_DriveAngle);
						pid_DriveStraight->SetTargetTime(0.1);
						pid_DriveStraight->SetAbsoluteTolerance(1.0);

						pid_DriveAngle = new MOLib::PID::GyrLoop(gyr_DriveAngle);
						pid_DriveAngle->SetOutputRange(-0.75, 0.75);
						pid_DriveAngle->SetTargetTime(0.1);
						pid_DriveAngle->SetAbsoluteTolerance(1.5);
					}

					if(UsingVision()) {
						pid_GoalAngle = new MOLib::PIDLoop(vis_Target);
						pid_GoalAngle->SetOutputRange(-0.18, 0.18);
						pid_GoalAngle->SetTargetTime(0.1);
						pid_GoalAngle->SetAbsoluteTolerance(0.01);
					}
				}
			};
		}
	}
}

//Modify Scope
namespace MOLib{
	using Drivetrain::ShiftState;
	using Drivetrain::TankScheme::TankDrivetrain;
}
