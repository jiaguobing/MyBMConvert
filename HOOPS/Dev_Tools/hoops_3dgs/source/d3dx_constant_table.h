#pragma once

#pragma warning (disable:4530)	// C++ exception handler used, but unwind semantics are not enabled.

#include <sstream>
#include <map>
#include <vector>

class HD3DXConstantTable : public CMO_Pooled
{
private:

	struct Param;	
	typedef std::vector<Param*, POOL_Allocator<Param*>> ParamArray;
	
	typedef std::basic_string<char, std::char_traits<char>, CMO_Allocator<char>> String;
	typedef std::basic_istringstream<char, std::char_traits<char>, CMO_Allocator<char>> StringStream;

public:			

	static HD3DXConstantTable * Create(Memory_Pool * pool, ID3DBlob * compiled_shader, H3D_Constant_Table table_type)
	{
		ID3DBlob * blob = NULL;
		HRESULT hr = D3DDisassemble(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), 0, 0, &blob);				

		if (FAILED(hr))
			return null;

		Auto_Buffer<char> disassembly(blob->GetBufferSize(), pool);			
		memcpy(&disassembly[0], blob->GetBufferPointer(),blob->GetBufferSize());
		H_SAFE_RELEASE(blob);		

		const char * disassembly_ptr = &disassembly[0];

		//char disassembly_filename[4096];
		//sprintf(disassembly_filename,"%s.diss",dump_filename);
		//H3DShader::DumpHLSL(disassembly_filename, disassembly_ptr);

		HD3DXConstantTable * table = POOL_NEW(pool, HD3DXConstantTable)(table_type, pool);

		table->ParseParameters(disassembly_ptr);

		return table;
	}	

	int Release()
	{
		delete this;
		return 0;
	}

	void SetInt(IDirect3DDevice9 * device, D3DXHANDLE handle, int n) 
	{
		Param const * param = reinterpret_cast<Param const *>(handle);
		
		if (param->reg_type == 's')
			return;

		ASSERT(param->reg_type == 'c');

		SetFloat(device, handle, static_cast<float>(n));
	}

	void SetFloat(IDirect3DDevice9 * device, D3DXHANDLE handle, float f)
	{
		float const vec4 [] = {f, 0, 0, 0};		

		Param const * param = reinterpret_cast<Param const *>(handle);

		if (table_type == H3D_VSConstant_Table)
		{
			device->SetVertexShaderConstantF(param->offset, vec4, 1);
		}

		else if (table_type == H3D_PSConstant_Table)
		{
			device->SetPixelShaderConstantF(param->offset, vec4, 1);
		}
	}

	void SetFloatArray(IDirect3DDevice9 * device, D3DXHANDLE handle, float const * in_ptr, int count)
	{
		Param const * param = reinterpret_cast<Param const *>(handle);

		typedef std::vector<Plane, POOL_Allocator<Plane>> PlaneVector;
		POOL_Allocator<Plane*> a(pool);

		switch (param->type)
		{
			case Param::Float3x3:
			{
				float matrix[12];
				memcpy(&matrix[0],&in_ptr[0],3*sizeof(float));
				memcpy(&matrix[4],&in_ptr[3],3*sizeof(float));
				memcpy(&matrix[8],&in_ptr[6],3*sizeof(float));

				if (table_type == H3D_VSConstant_Table)
				{
					device->SetVertexShaderConstantF(param->offset, matrix, 3);
				}

				else if (table_type == H3D_PSConstant_Table)
				{
					device->SetPixelShaderConstantF(param->offset, matrix, 3);
				}
			}break;

			case Param::Float2:
			{
				int const num_vectors = count / 2;
				PlaneVector buffer(a);
				buffer.resize(num_vectors);

				for (int i = 0 ; i < num_vectors ; i++)
				{
					memcpy(&buffer[i], &in_ptr[2*i], 2*sizeof(float));
				}

				SetVectorArray(device, handle, &buffer[0], num_vectors);

			}break;

			case Param::Float3:
			{
				int const num_vectors = count / 3;
				PlaneVector buffer(a);
				buffer.resize(num_vectors);

				for (int i = 0 ; i < num_vectors ; i++)
				{
					memcpy(&buffer[i], &in_ptr[3*i], 3*sizeof(float));
				}

				SetVectorArray(device, handle, &buffer[0], num_vectors);

			}break;

			default:
			{
				int const padded = (count + 3) & ~0x03;
				PlaneVector buffer(a);
				buffer.resize(padded/4);

				memcpy(&buffer[0], in_ptr, count * sizeof(float));		

				SetVectorArray(device, handle, &buffer[0], static_cast<int>(buffer.size()));
				
			}break;
		}		
	}

	void SetIntArray(IDirect3DDevice9 * , D3DXHANDLE , int const *, int )
	{
		ASSERT(0);
	}

	void SetVector(IDirect3DDevice9 * device , D3DXHANDLE handle, D3DXVECTOR4 const * vector)
	{
		Param const * param = reinterpret_cast<Param const *>(handle);

		if (table_type == H3D_VSConstant_Table)
		{
			device->SetVertexShaderConstantF(param->offset, reinterpret_cast<float const*>(vector), 1);
		}

		else if (table_type == H3D_PSConstant_Table)
		{
			device->SetPixelShaderConstantF(param->offset, reinterpret_cast<float const*>(vector), 1);
		}
	}

	void SetMatrix(IDirect3DDevice9 * device, D3DXHANDLE handle, D3DXMATRIX const * ptr)
	{
		Param const * param = reinterpret_cast<Param const *>(handle);

		if (table_type == H3D_VSConstant_Table)
		{
			device->SetVertexShaderConstantF(param->offset, reinterpret_cast<float const*>(ptr), 4);
		}

		else if (table_type == H3D_PSConstant_Table)
		{
			device->SetPixelShaderConstantF(param->offset, reinterpret_cast<float const*>(ptr), 4);
		}
	}

	void SetVectorArray(IDirect3DDevice9 * device, D3DXHANDLE handle , D3DXVECTOR4 const * vectors , int count)
	{
		Param const * param = reinterpret_cast<Param const *>(handle);

		// don't write past the "real" size (according to the compiler) of the parameter
		count = Min(param->size, count);

		if (table_type == H3D_VSConstant_Table)
		{
			device->SetVertexShaderConstantF(param->offset, reinterpret_cast<float const*>(vectors), count);
		}

		else if (table_type == H3D_PSConstant_Table)
		{
			device->SetPixelShaderConstantF(param->offset, reinterpret_cast<float const*>(vectors), count);
		}
	}	

	int GetSamplerIndex(D3DXHANDLE handle)
	{		
		Param * param = (Param*)handle;
		ASSERT(param->type == Param::Sampler);
		return param->offset;
	}

	D3DXHANDLE GetConstantByName(D3DXHANDLE parent_handle, const char * name)
	{
		Param * parent = (Param*)parent_handle;

		if (parent != null)
		{
			Param * child = parent->children[name];

			// If this parameter is located past the end of its parent struct, 
			// this means it is unused and was optimized away by the compiler.
			if (child->offset >= parent->offset + parent->size)
				return null;

			return (D3DXHANDLE)child;		
		}

		return (D3DXHANDLE)top_level_params[name];
	}

private:

	struct Param;
	typedef POOL_Allocator<Param*> ParamAllocator;

	class ParamCollection
	{

	public:		
		
		void Add(Param * param)
		{
			ASSERT(!Contains(param->name));

			params.push_back(param);
			hash.insert(std::make_pair(param->name,param));
		}

		ParamCollection(ParamAllocator const & a) : params(a)
		{

		}

		~ParamCollection()
		{
			for (size_t i = 0 ; i < params.size() ; i++)
			{
				delete params[i];
			}
		}

		size_t Count() const
		{
			return params.size();
		}

		bool Contains(String const & name) const
		{
			return hash.count(name) > 0;
		}

		Param * operator[](String const & name)
		{
			auto it = hash.find(name);
			return it != hash.end() ? it->second : null;
		}

		Param const * operator[](String const & name) const
		{
			auto it = hash.find(name);
			return it != hash.cend() ? it->second : null;
		}

		Param * operator[](size_t index)
		{
			return params[index];
		}

		Param const * operator[](size_t index) const
		{
			return params[index];
		}

	private:

		typedef std::map<String,Param*,std::less<String>, CMO_Allocator<std::pair<String,Param*>>> ParamMap;

		ParamArray params;
		ParamMap hash;		
	};

	struct ArrayInfo
	{
		String name;
		int count;

		ArrayInfo() : count(0)
		{

		}

		ArrayInfo(String const & name, int count) : name(name), count(count)
		{

		}
	};

	typedef std::map<String,ArrayInfo> ArrayMap;

	struct Param : public CMO_Pooled
	{
		enum Type
		{			
			Float2,
			Float3,
			Float4,
			Float3x3,
			Float4x4,
			Sampler,
			Other,
		};
		
		ParamCollection children;				
		String name;		
		Memory_Pool * pool;
		int size;
		int offset;
		char reg_type;
		Type type;

		Param(Memory_Pool * pool) : children(ParamAllocator(pool)), pool(pool), size(0), offset(0), type(Other)
		{

		}

		Param * Clone()
		{
			Param * clone = POOL_NEW(pool, Param)(pool);
			clone->name = name;
			clone->size = size;
			clone->type = type;

			for (size_t i = 0 ; i < children.Count() ; i++)
			{				
				clone->children.Add(children[i]->Clone());
			}

			return clone;
		}

		static void ParseSomething(StringStream & stream, String const & line, ParamArray & params, ArrayMap & arrays, Memory_Pool * pool)
		{
			if (line.find("struct") != String::npos)
			{ 
				ParseStruct(stream, params, arrays, pool);				
			}

			else if (line.find(";") != String::npos)
			{					
				ParseNotStruct(line, params, arrays, pool);				
			}			

			for (size_t i = 0 ; i < params.size() ; i++)
			{
				Param * param = params[i];
				for (size_t j = 0 ; j < param->children.Count() ; j++)
				{	
					Param * child = param->children[j];
					param->size += child->size;					
				}
			}			
		}

		static void ParseStruct(StringStream & stream, ParamArray & params, ArrayMap & arrays, Memory_Pool * pool)
		{
			ParamAllocator a(pool);
			ParamArray children(a);
			Param * param = POOL_NEW(pool, Param)(pool); // the struct param

			// the "struct" line was already consumed by ParseSomething			
			String line;
			while (std::getline(stream, line)) 
			{	
				if (line.find(";") != String::npos && line.find("}") != String::npos)
				{	
					char name_buff[256];					

					// array
					if (line.find("[") != String::npos)
					{	
						int count = -1;
						sscanf(line.c_str(), "// } %[^[][%d]",name_buff,&count);
						arrays.insert(std::make_pair(name_buff,ArrayInfo(name_buff,count)));						
					}

					// not an array
					else
					{
						sscanf(line.c_str(), "// } %[^;]",name_buff);
					}
					
					param->name = name_buff;					

					break; // end of struct
				}

				else
				{	
					ParseSomething(stream, line, children, arrays, pool);
				}				
			}

			for (size_t i = 0 ; i < children.size() ; i++)
			{
				param->children.Add(children[i]);
			}

			params.push_back(param);
		}

		static Param * CreateBasicTypeParam(String const & type, Memory_Pool * pool)
		{
			Param * param = POOL_NEW(pool, Param)(pool);

			if (type == "float4x4")
			{
				param->size = 4;
				param->type = Float4x4;
			}

			else if (type == "float4")
			{
				param->size = 1;
				param->type = Float4;
			}

			else if (type == "float3")
			{
				param->size = 1;
				param->type = Float3;
			}

			else if (type == "float2")
			{
				param->size = 1;
				param->type = Float2;
			}

			else if (type == "float3x3")
			{
				param->size = 3;
				param->type = Float3x3;
			}			

			else if (type == "sampler2D")
			{				
				param->type = Sampler;
			}

			else
			{
				param->size = 1;
				param->type = Other;
			}

			return param;
		}

		static void ParseNotStruct(String const & line, ParamArray & params, ArrayMap & arrays, Memory_Pool * pool)
		{
			char name_buff[256];
			char type_buff[256];	
			int count = 1;

			if (line.find("[") != String::npos)
			{	
				sscanf(line.c_str(), "// %s %[^[][%d]",type_buff, name_buff,&count);
				arrays.insert(std::make_pair(name_buff,ArrayInfo(name_buff,count)));						
			}

			else if (line.find("row_major") != String::npos)
			{				
				sscanf(line.c_str(), "// row_major %s %[^;]",type_buff,name_buff);
			}

			else
			{
				sscanf(line.c_str(), "// %s %[^;]",type_buff,name_buff);
			}

			String const type = type_buff;

			// First we have to record the top-level name (with no brackets)
			Param * param = CreateBasicTypeParam(type, pool);
			param->name = name_buff;						

			params.push_back(param);			
		}
	};

	void PropagateOffsets(Param * param, int base_offset, char reg_type)
	{
		param->offset = base_offset;
		param->reg_type = reg_type;

		int offset = base_offset;
		for (size_t i = 0 ; i < param->children.Count() ; i++)
		{
			Param * child = param->children[i];
			PropagateOffsets(child, offset, reg_type);
			offset += child->size;
		}
	}

	void ParseParameters(const char * source)
	{
		const char * parameters = strstr(source, "// Parameters:");

		if (parameters == 0)
			return;	

		StringStream stream(parameters);

		const String REGISTERS = "// Registers:";

		ArrayMap arrays;

		String line;
		while (std::getline(stream, line)) 
		{
			if (line == REGISTERS)
			{
				break;
			}

			else
			{
				ParamAllocator a(pool);
				ParamArray params(a);
				Param::ParseSomething(stream, line, params, arrays, pool);

				for (size_t i = 0 ; i < params.size() ; i++)
				{
					top_level_params.Add(params[i]);					
				}
			}
		}

		ASSERT(line == REGISTERS);
		std::getline(stream, line); //
		std::getline(stream, line); //   Name          Reg   Size
		std::getline(stream, line); //   ------------- ----- ----

		size_t const topLevelCount = top_level_params.Count();
		for (size_t i = 0 ; i < topLevelCount ; i++)
		{
			std::getline(stream, line);

			char name_buff[256];
			char reg_type;
			int reg = -1;
			int size = -1; 

			sscanf(line.c_str(), "// %s %c%i %i",name_buff, &reg_type, &reg, &size);			

			ASSERT(top_level_params.Contains(name_buff));
			Param * param = top_level_params[name_buff];	
			
			PropagateOffsets(param, reg, reg_type);
			
			// We need to record array parameters as one top level name, and also as individual elements. Because somebody may
			// try to set the whole (or part of the) array using just the name, or they may try to set individual elements by index.
			// So we're going to clone the parameter that has the name of the array for each element and dump it back into the top-level parameter list
			if (arrays.count(name_buff) == 1)
			{
				//Param * param = top_level_params[name_buff];
				int offset = reg;

				ArrayInfo const & ai = arrays[name_buff];
				for (int i = 0 ; i < ai.count ; i++)
				{
					Param * elt = param->Clone();
					char name[256];
					sprintf(name, "%s[%i]",name_buff,i);
					elt->name = name;
					top_level_params.Add(elt);

					PropagateOffsets(elt,offset, reg_type);
					offset += elt->size;
				}

				//param->size *= ai.count;
			}	

			// For top-level params, record the size given to us by the disassembly, not the calculated size of all child parameters.
			// The compile may overlap parameters that are not actually used in the shader and we need to be able to detect this in GetConstantByName.
			param->size = size;
		}
	}

	HD3DXConstantTable(H3D_Constant_Table table_type, Memory_Pool * pool) : top_level_params(ParamAllocator(pool)), pool(pool), table_type(table_type)
	{

	}

	ParamCollection top_level_params;
	Memory_Pool * pool;
	H3D_Constant_Table table_type;	
};

