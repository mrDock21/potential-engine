#pragma once

#include <string>

#include "shading/Shader.hpp"

namespace CEngine {

	class IMaterialProperty {
	public:
		/**
		* Updates this uniform in the shader
		*/
		virtual void Set(const std::string&, Shader&) = 0;
	};

	class MaterialPropertyInt : public IMaterialProperty {
	public:
		MaterialPropertyInt(const int& val) : value(val) { }
		void Value(const int& i) { value = i; }
		// updates this uniform in shader
		void Set(const std::string& name, Shader& s) override {
			s.SetUniform(name, value); 
		}

	private:
		int value;
	};

	class MaterialPropertyFloat : public IMaterialProperty {
	public:
		MaterialPropertyFloat(const float& val) : value(val) { }

		void Value(const float& f) { value = f; }
		// updates this uniform in shader
		void Set(const std::string& name, Shader& s) override { 
			s.SetUniform(name, value); 
		}

	private:
		float value;
	};

	class MaterialPropertyVector3 : public IMaterialProperty {
	public:
		MaterialPropertyVector3(const std::string& name, const Vector3& val) : value(val) { }

		void Value(const Vector3& f) { value = f; }
		// updates this uniform in shader
		void Set(const std::string& name, Shader& s) override { 
			s.SetUniform(name, value); 
		}

	private:
		Vector3 value;
	};

	class MaterialPropertyVector4 : public IMaterialProperty {
	public:
		MaterialPropertyVector4(const std::string& name, const Vector4& val) : value(val) { }

		void Value(const Vector4& f) { value = f; }
		// updates this uniform in shader
		void Set(const std::string& name, Shader& s) override { 
			s.SetUniform(name, value); 
		}

	private:
		Vector4 value;
	};

	class MaterialPropertyMatrix4 : public IMaterialProperty {
	public:
		MaterialPropertyMatrix4(const std::string& name, const Matrix4& val) : value(val) { }

		void Value(const Matrix4& f) { value = f; }
		// updates this uniform in shader
		void Set(const std::string& name, Shader& s) override { 
			s.SetUniform(name, value); 
		}

	private:
		Matrix4 value;
	};

	class MaterialPropertyColor : public IMaterialProperty {
	public:
		MaterialPropertyColor(const std::string& name, const Color& val) : value(val) { }

		void Value(const Color& f) { value = f; }
		// updates this uniform in shader
		void Set(const std::string& name, Shader& s) override { 
			s.SetUniform(name, value); 
		}

	private:
		Color value;
	};
}

