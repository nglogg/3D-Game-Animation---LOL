
#include <vector>
#include <map>


struct SknMaterial
{
	static const int kSizeInFile = 0x50;
	static const int kNameLen = 0x40;

	SknMaterial() { memset(name, 0, kNameLen); }

	char name[kNameLen];
	int startVertex;
	int num_vertices;
	int startIndex;
	int num_indices;
};
struct SknVtx
{
	static const int kSizeInFile = 0x34;

	SknVtx()
	{
		for (int i = 0; i < 4; i++)
		{
			skn_indices[i] = 0;
			weights[i] = 0;
		}
	}

	float x;
	float y;
	float z;
	char skn_indices[4];
	float weights[4];
	float normal[3];
	float U;
	float V;
	int skl_indices[4];
	int uv_index;
	int dupe_data_index;
};

class SknData
{
public:
	void switchHand()
	{
		int vertices_size = static_cast<int>(vertices.size());
		for (int i = 0; i < vertices_size; i++)
		{
			SknVtx& vtx = vertices.at(i);
			vtx.x = -vtx.x;
			vtx.normal[1] = -vtx.normal[1];
			vtx.normal[2] = -vtx.normal[2];
		}
	}

	short version;
	int num_vtxs;
	int num_indices;
	int num_final_vtxs;
	std::vector<unsigned short> indices;
	std::vector<SknMaterial> materials;
	std::vector<SknVtx> vertices;
	int endTab[3];
};

