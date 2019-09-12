#include "SknData.h"
#include "image.h"
using namespace std;


class SknReader
{
	CImage *texture;
public:
	bool read(istream& file, char* filename );
	SknData data_;
	
	void render();

};