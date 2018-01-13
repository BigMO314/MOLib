#pragma once

#include "WPIScope.h"
#include "Utility.h"

//Experimental Dashboard Wrapper
namespace MOLib{
	namespace Dashboard{
		class Base{
		public:
			virtual void Remove() { SmartDashboard::Delete(m_Label); }
		protected:
			std::string m_Label;
			virtual ~Base() {}
		};

		class Number: public Base{
		public:
			Number(std::string ref_Label, long double value = 0.0){ m_Label = ref_Label; m_DefaultValue = value; Set(value); }
			long double Get() const { return SmartDashboard::GetNumber(m_Label, 0.0); }
			void Set(long double value){ SmartDashboard::PutNumber(m_Label, value); }
		private:
			long double m_DefaultValue;
		};

		class String: public Base{
		public:
			String(std::string ref_Label, std::string expression = ""){ m_Label = ref_Label; m_DefaultValue = expression; Set(expression); }
			std::string Get() const { return SmartDashboard::GetString(m_Label, ""); }
			void Set(std::string expression){ SmartDashboard::PutString(m_Label, expression); }
		private:
			std::string m_DefaultValue;
		};

		class Boolean: public Base{
		public:
			Boolean(std::string ref_Label, bool state = false){ m_Label = ref_Label; m_DefaultValue = state; Set(state); }
			bool Get() const { return SmartDashboard::GetBoolean(m_Label, false); }
			void Set(bool state){ SmartDashboard::PutBoolean(m_Label, state); }
		private:
			bool m_DefaultValue;
		};

		class CheckBox: public Boolean{
		public:
			CheckBox(std::string ref_Label, bool state = false): Boolean(ref_Label, state) {}
			void Check(){ Set(true); }
			void Uncheck(){ Set(false); }
		};

		class Indicator: public Boolean{
		public:
			Indicator(std::string ref_Label, bool state = false): Boolean(ref_Label, state) {}
			void TurnOn(){ Set(true); }
			void TurnOff(){ Set(false); }
		};
	}
}

//Modify Scope
//None necessary
