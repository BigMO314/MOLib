#pragma once
#include "Utilities.h"

namespace MOLib {
	namespace Dashboard {
		class Base {
		public:
			virtual void Delete(){ SmartDashboard::Delete(m_Label); }
		protected:
			std::string m_Label;
			virtual ~Base(){}
		};

		class Number: public Base {
		public:
			Number(std::string ref_Label, long double value = 0.0) {
				m_Label = ref_Label;
				m_DefaultVariable = value;
				Set(value);
			}
			long double Get() const {
				return SmartDashboard::GetNumber(m_Label, m_DefaultVariable);
			}
			void Set(long double value) {
				SmartDashboard::PutNumber(m_Label, value);
			}
		private:
			long double m_DefaultVariable;
		};

		class String : public Base {
		public:
			String(std::string ref_Label, std::string expression = "") {
				m_Label = ref_Label;
				m_DefaultValue = expression;
				Set(expression);
			}
			std::string Get(){
				return SmartDashboard::GetString(m_Label,m_DefaultValue);
			}
			void Set(std::string expression){
				SmartDashboard::PutString(m_Label, expression);
			}
		private:
			std::string m_DefaultValue;
		};
		class Boolean : public Base {
		public:
			Boolean(std::string ref_Label, bool state = false) {
				m_Label = ref_Label;
				m_DefaultValue = state;
				Set(state);
			}
			bool Get(){
				return SmartDashboard::GetBoolean(m_Label,m_DefaultValue);
			}
			void Set(bool state){
				SmartDashboard::PutBoolean(m_Label, state);
			}
		private:
			bool m_DefaultValue;
		};
		class Checkbox : public Boolean {
		public:
			Checkbox(std::string ref_Label, bool state = false) {
				m_Label = ref_Label;
				Boolean (ref_Label, state);
			}
			void Check() { Set(true); }
			void Uncheck() { Set(false); }
		};
		class Indicator : public Boolean {
			Indicator(std::string ref_Label, bool state = false){
				m_Label = ref_Label;
				Boolean(ref_Label, state);
			}
			void TurnOn() { Set(true); }
			void TurnOff() { Set(false); }
		};
	}
}

