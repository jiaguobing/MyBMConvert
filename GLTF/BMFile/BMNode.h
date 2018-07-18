#pragma once
#include "../../BM23DTile/base.h"
#include <vector>
#include <map>
#include <set>
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
	class  BMVertexDD
	{
	public:
		BMVertexDD(const HPoint& pt)
			:m_x((double)pt.x), m_y((double)pt.y), m_z((double)pt.z)
		{
			m_h = 1.0;
		}
		BMVertexDD& operator =(const HPoint& pt)
		{
			m_x = (double)pt.x;
			m_y = (double)pt.y;
			m_z = (double)pt.z;
			m_h = 1.0;
			//
			return *this;
		}
		BMVertexDD()
		{
			m_x = m_y = m_z = 0.0;
			m_h = 1.0;
		}
		BMVertexDD(long double x, long double y)
		{
			m_x = x;
			m_y = y;
			m_z = 0.0;
			m_h = 1.0;
		}
		BMVertexDD(long double x, long double y, long double z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = 1.0;
		}
		BMVertexDD(long double x, long double y, long double z, long double h)
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
		BMVertexDD Cross(const BMVertexDD& vecOther)const
		{
			return BMVertexDD(m_y*vecOther.m_z - m_z*vecOther.m_y,
				m_z*vecOther.m_x - m_x*vecOther.m_z,
				m_x*vecOther.m_y - m_y*vecOther.m_x);
		}
		double Dot(const BMVertexDD& vecOther)const
		{
			return m_x*vecOther.m_x + m_y*vecOther.m_y + m_z*vecOther.m_z;
		}
		BMVertexDD& Normalization()
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
		BMVertexDD operator+(const BMVertexDD& vecOther) const
		{
			return BMVertexDD(m_x + vecOther.m_x, m_y + vecOther.m_y, m_z + vecOther.m_z);
		}
		BMVertexDD operator-(const BMVertexDD& vecOther) const
		{
			return BMVertexDD(m_x - vecOther.m_x, m_y - vecOther.m_y, m_z - vecOther.m_z);
		}
		BMVertexDD operator/(double value) const
		{
			if (abs(value) < 1e-16)
				return BMVertexDD(0.0, 0.0, 0.0);
			else
				return BMVertexDD(m_x / value, m_y / value, m_z / value);
		}
		void ToMatrix(double matrix[16])
		{
			double x2 = m_x * m_x;
			double xy = m_x * m_y;
			double xz = m_x * m_z;
			double xw = m_x * m_h;
			double y2 = m_y * m_y;
			double yz = m_y * m_z;
			double yw = m_y * m_h;
			double z2 = m_z * m_z;
			double zw = m_z * m_h;
			double w2 = m_h * m_h;

			double m00 = x2 - y2 - z2 + w2;
			double m01 = 2.0 * (xy - zw);
			double m02 = 2.0 * (xz + yw);

			double m10 = 2.0 * (xy + zw);
			double m11 = -x2 + y2 - z2 + w2;
			double m12 = 2.0 * (yz - xw);

			double m20 = 2.0 * (xz - yw);
			double m21 = 2.0 * (yz + xw);
			double m22 = -x2 - y2 + z2 + w2;

			matrix[0] = m00;
			matrix[1] = m10;
			matrix[2] = m20;
			matrix[3] = 0.0;
			matrix[4] = m01;
			matrix[5] = m11;
			matrix[6] = m21;
			matrix[7] = 0.0;
			matrix[8] = m02;
			matrix[9] = m12;
			matrix[10] = m22;
			matrix[11] = 0.0;
			matrix[12] = 0.0;
			matrix[13] = 0.0;
			matrix[14] = 0.0;
			matrix[15] = 1.0;
		}
		operator HPoint() const
		{
			return HPoint((float)m_x, (float)m_y, (float)m_z);
		}
	};
	class  BMVertexD
	{
	public:
		BMVertexD(const HPoint& pt)
			:m_x((double)pt.x), m_y((double)pt.y), m_z((double)pt.z)
		{
			m_h = 1.0;
		}
		BMVertexD& operator =(const HPoint& pt)
		{
			m_x = (double)pt.x;
			m_y = (double)pt.y;
			m_z = (double)pt.z;
			m_h = 1.0;
			//
			return *this;
		}
		BMVertexD()
		{
			m_x = m_y = m_z = 0.0;
			m_h = 1.0;
		}
		BMVertexD( double x,  double y)
		{
			m_x = x;
			m_y = y;
			m_z = 0.0;
			m_h = 1.0;
		}
		BMVertexD( double x,  double y,  double z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = 1.0;
		}
		BMVertexD( double x,  double y,  double z,  double h)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_h = h;
		}
		double m_x;
		double m_y;
		double m_z;
		double m_h;
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
			if (abs(r) < 1e-8)
			{
				m_x = m_y = m_z = 0.0;
				return *this;
			}
			//
			m_x /= r;
			m_y /= r;
			m_z /= r;
			//
			if (abs(m_x) < 1e-8)
				m_x = 0.0;
			if (abs(m_y) < 1e-8)
				m_y = 0.0;
			if (abs(m_z) < 1e-8)
				m_z = 0.0;
			//
			return *this;
		}
		BMVertexD operator+(const BMVertexD& vecOther) const
		{
			return BMVertexD(m_x + vecOther.m_x, m_y + vecOther.m_y, m_z + vecOther.m_z);
		}
		BMVertexD operator-(const BMVertexD& vecOther) const
		{
			return BMVertexD(m_x - vecOther.m_x, m_y - vecOther.m_y, m_z - vecOther.m_z);
		}
		BMVertexD operator/(double value) const
		{
			if (abs(value) < 1e-8)
				return BMVertexD(0.0, 0.0, 0.0);
			else
				return BMVertexD(m_x / value, m_y / value, m_z / value);
		}
		void ToMatrix(double matrix[16])
		{
			double x2 = m_x * m_x;
			double xy = m_x * m_y;
			double xz = m_x * m_z;
			double xw = m_x * m_h;
			double y2 = m_y * m_y;
			double yz = m_y * m_z;
			double yw = m_y * m_h;
			double z2 = m_z * m_z;
			double zw = m_z * m_h;
			double w2 = m_h * m_h;

			double m00 = x2 - y2 - z2 + w2;
			double m01 = 2.0 * (xy - zw);
			double m02 = 2.0 * (xz + yw);

			double m10 = 2.0 * (xy + zw);
			double m11 = -x2 + y2 - z2 + w2;
			double m12 = 2.0 * (yz - xw);

			double m20 = 2.0 * (xz - yw);
			double m21 = 2.0 * (yz + xw);
			double m22 = -x2 - y2 + z2 + w2;

			matrix[0] = m00;
			matrix[1] = m10;
			matrix[2] = m20;
			matrix[3] = 0.0;
			matrix[4] = m01;
			matrix[5] = m11;
			matrix[6] = m21;
			matrix[7] = 0.0;
			matrix[8] = m02;
			matrix[9] = m12;
			matrix[10] = m22;
			matrix[11] = 0.0;
			matrix[12] = 0.0;
			matrix[13] = 0.0;
			matrix[14] = 0.0;
			matrix[15] = 1.0;
		}
		operator HPoint() const
		{
			return HPoint((float)m_x, (float)m_y, (float)m_z);
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
		BMTriangle()
		{
		}
		unsigned int m_index0;
		unsigned int m_index1;
		unsigned int m_index2;
	};
	//BMPrimitive在cesuim中为一个drawcall,每一个Primitive其材质相同，BatchID可以不同
	class BMPrimitive {
	public:
		std::vector<unsigned int> m_vIndexs;
		std::vector<BMVertex> m_vPositions;
		std::vector<BMVertex> m_vNormals;
		std::vector<BMCoord> m_vCoords;
		std::vector<unsigned int> m_vBatchIDs;
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
// 		std::vector<BMVertex> m_vPositions;
// 		std::vector<BMVertex> m_vNormals;
// 		std::vector<BMCoord> m_vCoords;
// 		std::vector<unsigned int> m_vBatchIDs;
		//
		std::vector<BMPrimitive> m_vPrimitives;
	};

};
