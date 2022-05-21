/*
*
* < ShaderSystem.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include <HEADER/PUBLIC/Utility/DxgiUtil.h>

namespace orangelie {

	namespace Shader {

		struct ShaderRasterizeObjects1 {
			std::wstring filename;
			std::string target;
			std::string func;
			std::vector<D3D_SHADER_MACRO> macros;
			bool isDefined = false;
		};

		struct ShaderRasterizeObjects2 {
			ShaderRasterizeObjects1 VS;
			ShaderRasterizeObjects1 PS;
			ShaderRasterizeObjects1 GS;
			ShaderRasterizeObjects1 CS;
			ShaderRasterizeObjects1 HS;
			ShaderRasterizeObjects1 DS;
		};

		struct ShaderResources {
			ComPtr<ID3DBlob> VSbinCode = nullptr;
			ComPtr<ID3DBlob> PSbinCode = nullptr;
			ComPtr<ID3DBlob> GSbinCode = nullptr;
			ComPtr<ID3DBlob> CSbinCode = nullptr;
			ComPtr<ID3DBlob> HSbinCode = nullptr;
			ComPtr<ID3DBlob> DSbinCode = nullptr;
		};

		class ShaderSystem {
		public:
			ShaderSystem();
			ShaderSystem(const ShaderSystem&) = delete;
			ShaderSystem operator=(const ShaderSystem&) = delete;
			~ShaderSystem();

			ShaderResources Resource(const std::string& resource_name);

			void ShaderCompileFromFile(
				const char* shader_resource_name,
				ShaderRasterizeObjects2 rasterize_objects2);

		private:
			ComPtr<ID3DBlob> Compile(
				const wchar_t* filename,
				const char* target,
				const char* func,
				D3D_SHADER_MACRO* macros);

		private:
			std::unordered_map<std::string, ShaderResources> m_ShaderResources;

		};
	}
}