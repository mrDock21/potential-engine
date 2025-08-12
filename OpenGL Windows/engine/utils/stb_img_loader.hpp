#pragma once

#include <iostream>

#include "types/types.hpp"

#include "Logger.hpp"

namespace CEngine {
	
	class STBImageLoader {
	public:
		STBImageLoader();
		STBImageLoader(const std::string&, int* const, int* const);

		~STBImageLoader();


		void LoadTexture(const std::string&, int* const, int* const);
		void LoadCubeMap(const std::string&, int* const, int* const);
		void Load(const std::string&, int* const, int* const);

		u_char* Data() const;

		bool HasData() const;

	private:

		void freeData();

		u_char* data;
	};

}
