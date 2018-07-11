#pragma once

#include "GLTFObject.h"

namespace GLTF {
	class Buffer : public GLTF::Object {
	public:
		~Buffer()
		{

		}
		unsigned char* data = NULL;
		int byteLength;
		std::string uri;

		Buffer(unsigned char* data, int dataLength);
		
		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
};
