#pragma once

#include "./../shading/uniform_buffer.hpp"

namespace CEngine {

	/// <summary>
	///		Interface for any renderable object in the scene
	/// </summary>
	class ISceneRenderable
	{
	public:
		/// <summary>
		///		Binds the uniform blocks for global matrices and global lights
		/// </summary>
		/// <param name="matricesUBO">	Buffer containing the "View" and "Proj" matrices	</param>
		/// <param name="lightsUBO">	Buffer containinf all global lights' data			</param>
		virtual void Initialize(const UBO& matricesUBO, const UBO& lightsUBO) = 0;
		/// <summary>
		///		Custom rendering logic
		/// </summary>
		virtual void Render() = 0;
	};
}