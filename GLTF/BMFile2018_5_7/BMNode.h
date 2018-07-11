#pragma once
#include <vector>
#include <map>
#include <set>
#include <HTools.h>
namespace GLTF {
	class  BMVertex
	{
	public:
		BMVertex()
		{
			m_x = m_y = m_z = 0.0f;
			m_h = 1.0f;
		}
		BMVertex(float x,float y)
		{
			m_x = x;
			m_y = y;
			m_z = 0.0f;
			m_h = 1.0f;
		}
		BMVertex(float x, float y,float z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = 1.0f;
		}
		BMVertex(float x, float y, float z,float h)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = h;
		}
		// x y z h
		// r g b a
		// u v s t
		union 
		{
			float m_x;
			float m_r;
			float m_u;
		};
		union
		{
			float m_y;
			float m_g;
			float m_v;
		};
		union
		{
			float m_z;
			float m_b;
			float m_s;
		};
		union
		{
			float m_h;
			float m_a;
			float m_t;
		};
		float GetLength() const
		{
			return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
		}
		BMVertex& Normalization()
		{
			double r = GetLength();
			if (abs(r) < 1e-6)
			{
				m_x = m_y = m_z = 0.0;
				return *this;
			}
			//
			m_x /= r;
			m_y /= r;
			m_z /= r;
			//
			if (abs(m_x) < 1e-6)
				m_x = 0.0;
			if (abs(m_y) < 1e-6)
				m_y = 0.0;
			if (abs(m_z) < 1e-6)
				m_z = 0.0;
			//
			return *this;
		}
		BMVertex operator+(const BMVertex& vecOther) const
		{
			return BMVertex(m_x + vecOther.m_x, m_y + vecOther.m_y, m_z + vecOther.m_z);
		}
		BMVertex operator-(const BMVertex& vecOther) const
		{
			return BMVertex(m_x - vecOther.m_x, m_y - vecOther.m_y, m_z - vecOther.m_z);
		}
		BMVertex operator/(float value) const
		{
			if (abs(value) < 1e-6)
				return BMVertex(0.0f, 0.0f, 0.0f);
			else
				return BMVertex(m_x / value, m_y / value, m_z / value);
		}
		BMVertex Cross(const BMVertex& vecOther)const
		{
			return BMVertex(m_y*vecOther.m_z - m_z*vecOther.m_y,
				m_z*vecOther.m_x - m_x*vecOther.m_z,
				m_x*vecOther.m_y - m_y*vecOther.m_x);
		}
	};
	class  BMVertexD
	{
	public:
		BMVertexD()
		{
			m_x = m_y = m_z = 0.0;
			m_h = 1.0;
		}
		BMVertexD(long double x, long double y)
		{
			m_x = x;
			m_y = y;
			m_z = 0.0;
			m_h = 1.0;
		}
		BMVertexD(long double x, long double y, long double z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = 1.0;
		}
		BMVertexD(long double x, long double y, long double z, long double h)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = h;
		}
		long double m_x;
		long double m_y;
		long double m_z;
		long double m_h;
		float GetLength() const
		{
			return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
		}
		BMVertexD Cross(const BMVertexD& vecOther)const
		{
			return BMVertexD(m_y*vecOther.m_z - m_z*vecOther.m_y,
				m_z*vecOther.m_x - m_x*vecOther.m_z,
				m_x*vecOther.m_y - m_y*vecOther.m_x);
		}
		double Dot(const BMVertexD& vecOther)const
		{
			return m_x*vecOther.m_x + m_y*vecOther.m_y + m_z*vecOther.m_z;
		}
		BMVertexD& Normalization()
		{
			double r = GetLength();
			if (abs(r) < 1e-16)
			{
				m_x = m_y = m_z = 0.0;
				return *this;
			}
			//
			m_x /= r;
			m_y /= r;
			m_z /= r;
			//
			if (abs(m_x) < 1e-16)
				m_x = 0.0;
			if (abs(m_y) < 1e-16)
				m_y = 0.0;
			if (abs(m_z) < 1e-16)
				m_z = 0.0;
			//
			return *this;
		}
		BMVertexD operator+(const BMVertexD& vecOther) const
		{
			return BMVertexD(m_x + vecOther.m_x, m_y + vecOther.m_y, m_z + vecOther.m_z);
		}
		BMVertexD operator/(double value) const
		{
			if (abs(value) < 1e-16)
				return BMVertexD(0.0, 0.0, 0.0);
			else
				return BMVertexD(m_x / value, m_y / value, m_z / value);
		}
	};
	typedef BMVertex BMColor;
	typedef BMVertex BMCoord;
	class  BMTriangle
	{
	public:
		BMTriangle(int index0, int index1, int index2)
		{
			m_index0 = index0;
			m_index1 = index1;
			m_index2 = index2;
		}
		unsigned int m_index0;
		unsigned int m_index1;
		unsigned int m_index2;
	};
	//BMPrimitive在cesuim中为一个drawcall,每一个Primitive其材质相同，BatchID可以不同
	class BMPrimitive {
	public:
		std::vector<unsigned int> m_vIndexs;
		//
		BMColor m_Color;
		std::string m_TextureImagePath;
	};
	//每一个BMMesh对象转换为一个b3dm文件
	class BMMesh
	{
	public:
		std::vector<std::string> m_vBatchNames;
		//
		std::string m_strName;
		std::vector<BMVertex> m_vPositions;
		std::vector<BMVertex> m_vNormals;
		std::vector<BMCoord> m_vCoords;
		std::vector<unsigned int> m_vBatchIDs;
		//
		std::vector<BMPrimitive> m_vPrimitives;
	};

};
