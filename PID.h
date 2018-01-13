#pragma once

#include "WPIScope.h"
#include "Utility.h"

#include "Vision.h"

namespace MOLib {
	namespace PID {
		using WPILib::PIDSourceType;
		class BaseLoop: public WPILib::PIDController, WPILib::PIDOutput {
		public:
			virtual ~BaseLoop() {}
			BaseLoop(WPILib::PIDSource* ref_Source)
			:PIDController(0.0, 0.0, 0.0, ref_Source, this){ tmr_AtTarget = new WPILib::Timer(); }
			BaseLoop(double P, double I, double D, WPILib::PIDSource* ref_Source)
			:PIDController(P, I, D, ref_Source, this) { tmr_AtTarget = new WPILib::Timer(); }

			virtual void Enable() {
				PIDController::Enable();
				tmr_AtTarget->Start();
				tmr_AtTarget->Reset();
			}

			virtual void Disable() {
				PIDController::Disable();
				tmr_AtTarget->Stop();
			}

			virtual bool OnTarget() const override {
				return PIDController::OnTarget() && tmr_AtTarget->Get() > m_AtTargetTime;
			}

			virtual double Get() const override { if(IsEnabled()) return PIDController::Get(); else return 0.0;}

			virtual void PIDWrite(double output) override {
				//Do nothing
			}


			virtual void ResetSource() {}
			virtual void CalibrateSource() {}
			
			void SetTargetTime(double seconds){ m_AtTargetTime = seconds; }

			void SetP(double P){ SetPID(P, GetI(), GetD(), GetF()); }
			void SetI(double I){ SetPID(GetP(), I, GetD(), GetF()); }
			void SetD(double D){ SetPID(GetP(), GetI(), D, GetF()); }
			void SetF(double F){ SetPID(GetP(), GetI(), GetD(), F); }

		protected:
			virtual void Calculate() {
				PIDController::Calculate();
				if(IsEnabled() && !PIDController::OnTarget()) tmr_AtTarget->Reset();
			}

		private:
			WPILib::Timer* tmr_AtTarget;
			double m_AtTargetTime = 0.1;
		};

		class EncLoop : public BaseLoop {
		public:
			virtual ~EncLoop() {}
			EncLoop(WPILib::Encoder* ref_Source)
			:BaseLoop(ref_Source){ this->enc_Source = ref_Source; }
			EncLoop(double P, double I, double D, WPILib::Encoder* ref_Source)
			:BaseLoop(P, I, D, ref_Source) { this->enc_Source = ref_Source; }
			virtual void ResetSource() override { enc_Source->Reset(); }
		private:
			WPILib::Encoder* enc_Source;
		};

		class GyrLoop : public BaseLoop {
		public:
			virtual ~GyrLoop() {}
			GyrLoop(WPILib::GyroBase* ref_Source)
			:BaseLoop(ref_Source){ this->gyr_Source = ref_Source; }
			GyrLoop(double P, double I, double D, WPILib::GyroBase* ref_Source) :BaseLoop(P, I, D, ref_Source) { this->gyr_Source = ref_Source; }
			virtual void ResetSource() override { gyr_Source->Reset(); }
			virtual void CalibrateSource() override { gyr_Source->Calibrate(); }
		private:
			WPILib::GyroBase* gyr_Source;
		};
	}
}

//Modify Scope
namespace MOLib{
	using PID::PIDSourceType;
	PID::BaseLoop typedef PIDLoop;
}
