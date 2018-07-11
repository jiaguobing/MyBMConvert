#pragma once

#include <set>
#include <string>
#include <vector>

#include "GLTFCamera.h"
#include "GLTFMesh.h"
#include "GLTFObject.h"
#include "GLTFSkin.h"

namespace GLTF {
	class Node : public GLTF::Object {
	public:
		~Node()
		{
			delete mesh;
			if (transform != nullptr)
				delete transform;
		}
		class Transform {
		public:
			virtual ~Transform(){};
			enum Type {
				TRS,
				MATRIX
			};

			Type type;
		};

		class TransformTRS;
		class TransformMatrix : public Transform {
		public:
			double matrix[16];
			~TransformMatrix(){};
			TransformMatrix();
			TransformMatrix(double a00, double a01, double a02, double a03,
				double a10, double a11, double a12, double a13,
				double a20, double a21, double a22, double a23,
				double a30, double a31, double a32, double a33);

			void premultiply(TransformMatrix* transform);
			void premultiply(TransformMatrix* transform, TransformMatrix* destination);
			void scaleUniform(float scale);
			bool isIdentity();
			void getTransformTRS(TransformTRS* out);
			TransformTRS* getTransformTRS();
		};

		class TransformTRS : public Transform {
		public:
			~TransformTRS(){};
			double translation[3];
			double rotation[4];
			double scale[3];

			TransformTRS();
			bool isIdentityTranslation();
			bool isIdentityRotation();
			bool isIdentityScale();
			TransformMatrix* getTransformMatrix();
		};

		GLTF::Camera* camera = NULL;
		std::vector<GLTF::Node*> children;
		GLTF::Skin* skin = NULL;
		std::string jointName;
		GLTF::Mesh* mesh = NULL;
		GLTF::MaterialCommon::Light* light = NULL;

		Transform* transform = NULL;

		virtual std::string typeName();
		virtual GLTF::Object* clone(GLTF::Object* clone);
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
}
