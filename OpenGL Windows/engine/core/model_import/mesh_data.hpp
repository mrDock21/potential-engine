#pragma once

#include <vector>
#include <string>

#include "vertex.hpp"
#include "types/types.hpp"

namespace CEngine {

	class MeshData {
	public:

		MeshData() { }

		void Vertices(const std::vector<Vertex>& data) {
			verts = data;
		}

		void AddVertex(const Vertex& v) {
			verts.push_back(v);
		}
		
		void AddIndex(const uint& vIndex) {
			indices.push_back(vIndex);
		}

		void Name(const std::string& s) {
			name = s;
		}

		std::vector<Vertex>& Vertices() {
			return verts;
		}

		std::vector<uint>& Indices() {
			return indices;
		}

		const std::string& Name() {
			return name;
		}

	private:
		std::vector<Vertex> verts;
		std::vector<uint> indices;
		std::string name;
	};
}