#pragma once

#include <vector>

#include "GLTFNode.h"
#include "GLTFObject.h"

namespace GLTF {
	class Scene : public GLTF::Object {
	public:
		std::vector<GLTF::Node*> nodes;
		~Scene()
		{
			for (auto _node : nodes)
			{
				delete _node;
			}
			nodes.clear();
		}
		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
}
