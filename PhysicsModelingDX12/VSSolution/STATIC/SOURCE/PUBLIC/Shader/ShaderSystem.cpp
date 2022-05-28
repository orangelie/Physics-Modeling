/*
*
* < ShaderSystem.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Shader/ShaderSystem.h"

namespace orangelie {

	namespace Shader {

		ShaderSystem::ShaderSystem() {
		}

		ShaderSystem::~ShaderSystem() {
		}

		ShaderResources ShaderSystem::Resource(const std::string& resource_name) {
			return m_ShaderResources[resource_name];
		}

		void ShaderSystem::ShaderCompileFromFile(
			const char* shader_resource_name,
			ShaderRasterizeObjects2 rasterize_objects2) {

			ShaderResources shaderRes;

			if (rasterize_objects2.VS.isDefined) {
				shaderRes.VSbinCode = Compile(rasterize_objects2.VS.filename.c_str(),
					rasterize_objects2.VS.target.c_str(),
					rasterize_objects2.VS.func.c_str(),
					rasterize_objects2.VS.macros.data());
			}

			if (rasterize_objects2.PS.isDefined) {
				shaderRes.PSbinCode = Compile(rasterize_objects2.PS.filename.c_str(),
					rasterize_objects2.PS.target.c_str(),
					rasterize_objects2.PS.func.c_str(),
					rasterize_objects2.PS.macros.data());
			}

			if (rasterize_objects2.GS.isDefined) {
				shaderRes.PSbinCode = Compile(rasterize_objects2.GS.filename.c_str(),
					rasterize_objects2.GS.target.c_str(),
					rasterize_objects2.GS.func.c_str(),
					rasterize_objects2.GS.macros.data());
			}

			if (rasterize_objects2.CS.isDefined) {
				shaderRes.PSbinCode = Compile(rasterize_objects2.CS.filename.c_str(),
					rasterize_objects2.CS.target.c_str(),
					rasterize_objects2.CS.func.c_str(),
					rasterize_objects2.CS.macros.data());
			}

			if (rasterize_objects2.HS.isDefined) {
				shaderRes.PSbinCode = Compile(rasterize_objects2.HS.filename.c_str(),
					rasterize_objects2.HS.target.c_str(),
					rasterize_objects2.HS.func.c_str(),
					rasterize_objects2.HS.macros.data());
			}

			if (rasterize_objects2.DS.isDefined) {
				shaderRes.PSbinCode = Compile(rasterize_objects2.DS.filename.c_str(),
					rasterize_objects2.DS.target.c_str(),
					rasterize_objects2.DS.func.c_str(),
					rasterize_objects2.DS.macros.data());
			}

			m_ShaderResources[shader_resource_name] = shaderRes;
		}

		ComPtr<ID3DBlob> ShaderSystem::Compile(
			const wchar_t* filename,
			const char* target,
			const char* func,
			D3D_SHADER_MACRO* macros) {

			ComPtr<ID3DBlob> ppCode = nullptr, ppErrorMsgs = nullptr;

			UINT flag = 0;

#if defined(DEBUG) | defined(_DEBUG)
			flag = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif

			HRESULT hResult = 0;
			hResult = D3DCompileFromFile(
				filename,
				macros,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				func,
				target,
				flag,
				0,
				ppCode.GetAddressOf(),
				ppErrorMsgs.GetAddressOf());

			if (ppErrorMsgs != nullptr) {
				MessageBoxA(0, (char*)ppErrorMsgs->GetBufferPointer(), "Compile Error", MB_ICONWARNING);
			}

			HR(hResult);

			return ppCode;
		}
	}
}