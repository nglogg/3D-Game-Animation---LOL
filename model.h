
#ifndef MODEL_H
#define MODEL_H

class CModel
{
public:

	virtual bool Load(const char * szFile) = 0;

	virtual void Render() = 0;

};


inline void SwapEndian(unsigned int& i)
{
	unsigned int j  = (( i << 24 ) & 0xFF000000) + ((i << 8) & 0x00FF0000) + ((i >> 8) & 0x0000FF00) + ((i >> 24) & 0x000000FF);
	i = j;
}

inline void SwapEndian(unsigned short& i)
{
	unsigned short j  = (( i << 8 ) & 0xFF00) + ((i >> 8) & 0x00FF);
	i = j;
}


#endif //MODEL_H