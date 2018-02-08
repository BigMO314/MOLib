#pragma once

#include "WPILib.h"
#include "Utility.h"

#include "Dashboard.h"

namespace MOLib{
	namespace Vision{
		class Target: public frc::PIDSource{
		public:
			virtual ~Target() {};
			Target(std::string ref_Target){

				m_VisionX += ref_Target;
				m_VisionY += ref_Target;
				m_VisionW += ref_Target;
				m_VisionH += ref_Target;
				dsh_X = new MOLib::Dashboard::Number(m_VisionX, 0);
				dsh_Y = new MOLib::Dashboard::Number(m_VisionY, 0);
				dsh_W = new MOLib::Dashboard::Number(m_VisionW, 0);
				dsh_H = new MOLib::Dashboard::Number(m_VisionH, 0);

				dsh_RequestValues = new MOLib::Dashboard::Boolean("Robot requesting", false);
				dsh_RequestComplete = new MOLib::Dashboard::Boolean("Request complete", false);
				dsh_VisionProcessingTime = new MOLib::Dashboard::Number("Vision Processing Time", 0);

				tmr_Vision.Start();
			}

			double PIDGet() override { return GetX(); }

			//TODO Change to get ground distance
			double GetDistance() {
				//if(GetW() == 0){ width = 0.001; }
				//else{ width = GetW(); }
				return (m_TargetWidth * m_XRes) / (2 * GetW() * tan(DegToRad(m_XFOV / 2)));
			}

			double GetAnglePrint() {
				//if(GetW() == 0){ width = 0.001; }
				//else{ width = GetW() }
				return (m_TargetWidth * GetX()) / (GetDistance() * GetW());
			}

			double GetAngle() { return RadToDeg(asin(GetAnglePrint())); }

			bool ValuesReady() { return dsh_RequestComplete->Get(); }

			void SetCameraOffset(double offset) { m_CameraOffset = offset; }

			void SetResoloution(uint xRes, uint yRes){ m_XRes = xRes; m_YRes = yRes; }
			void SetFOV(double xFOV, double yFOV){ m_XFOV = xFOV; m_YFOV = yFOV; }
			void SetTargetSize(double width, double height){ m_TargetWidth = width; m_TargetHeight = height; }

			double GetX() const { return (-1*dsh_X->Get())+m_CameraOffset; }//negative
			double GetY() const { return dsh_Y->Get(); }
			double GetW() const { return dsh_W->Get(); }
			double GetH() const { return dsh_H->Get(); }

			void RequestValue() {
				//std::cout << "m_TargetWidth: " << GetAnglePrint() << std::endl;
				dsh_RequestValues->Set(true);
				dsh_RequestComplete->Set(false);
				tmr_Vision.Reset();
				tmr_Vision.Start();
			}

			bool IsUpdated() { return dsh_RequestComplete->Get(); }

			void Update() {
				if(IsUpdated()) tmr_Vision.Stop();
				dsh_VisionProcessingTime->Set(tmr_Vision.Get());
			}

		private:

			std::string m_VisionX = "Vision X";
			std::string m_VisionY = "Vision Y";
			std::string m_VisionW = "Vision W";
			std::string m_VisionH = "Vision H";

			MOLib::Dashboard::Number* dsh_X;
			MOLib::Dashboard::Number* dsh_Y;
			MOLib::Dashboard::Number* dsh_W;
			MOLib::Dashboard::Number* dsh_H;

			MOLib::Dashboard::Boolean* dsh_RequestValues;
			MOLib::Dashboard::Boolean* dsh_RequestComplete;
			MOLib::Dashboard::Number* dsh_VisionProcessingTime;

			WPILib::Timer tmr_Vision;

			double width = 0.001;

			uint m_XRes				= 0;
			uint m_YRes				= 0;
			double m_XFOV			= 0;
			double m_YFOV			= 0;
			double m_TargetWidth	= 0;
			double m_TargetHeight	= 0;
			double m_CameraOffset	= 0;
		};
	}
}
