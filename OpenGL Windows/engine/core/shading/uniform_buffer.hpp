#pragma once

#include <glad/glad.h>

#include <string>

#include "math_types/matrix4.hpp"
#include "types/types.hpp"

namespace CEngine {

	class UBO {
	public:

		UBO();
		/// <summary>
		///		Creates a new uniform buffer object
		/// </summary>
		/// <param name="name">			Name of the uniform block		</param>
		/// <param name="bindingPoint">	Binding point from 0 to N		</param>
		/// <param name="offset">		Offset of the data				</param>
		/// <param name="esize">		Total size of the block (bytes)	</param>
		UBO(const std::string&, const uint&, const u_long&, const u_long&);
	
		void Use();
	
		/// <summary>
		///		Uploads a matrix to the uniform buffer
		/// </summary>
		/// <param name="">	The data									</param>
		/// <param name="">	The initial offset of where the data starts	</param>
		void Upload(const Matrix4&, const unsigned long long&);

		uint ID() const;
		uint BindingIndex() const;
		std::string UniformBlockName() const;

	private:
		uint id, bindingIndex;
		bool initialized;
		std::string uniformBlockName;
	};

}