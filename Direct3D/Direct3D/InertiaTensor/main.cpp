#include "InertiaTensor.h"

int main(int argc, char** argv)
{

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


	try {
		std::unique_ptr<orangelie::InertiaTensor> iTensor(new orangelie::InertiaTensor);

		orangelie::InertiaTensor::InitializeInertiaTensor(orangelie::gPointElements, gPointMaxSize);
		auto InertialTensor = orangelie::InertiaTensor::CalcInertiaTensor(orangelie::gPointElements, gPointMaxSize);
		orangelie::InertiaTensor::PrintInertiaTensor(InertialTensor);

	}
	catch (const std::exception& e) {
		MessageBoxA(0, e.what(), "## Exception Occured!! ##", MB_ICONWARNING);
		return -1;
	}

	BLOCK;
	return 0;
}