#pragma once
#include "Utilities.h"

namespace MOLib {
namespace Vision {
class LimeLight : public PIDSource{
public :
	LimeLight(int ref_Pipeline) { table->PutNumber("pipeline", ref_Pipeline); }

	double PIDGet() override { return GetHorizontalOffset(); }

	//0 is Vision processor. 1 is Driver camera
	void SetVisionMode(int mode) { table->PutNumber("camMode", mode); }

	//0 is on. 1 is off. 2 is blink

	void EnableLight() { table->PutNumber("ledMode", 0); }
	void DisableLight() { table->PutNumber("ledMode", 1); }

	float GetHorizontalOffset() { return table->GetNumber("tx", 0); }
	float GetVerticalOffset() { return table->GetNumber("ty", 0); }
	float GetTargetArea() { return table->GetNumber("ta", 0);}
	float GetTargetSkew() { return table->GetNumber("ts", 0); }

private:
	std::shared_ptr<NetworkTable> table = 	NetworkTable::GetTable("LimeLight");
};
}
}
