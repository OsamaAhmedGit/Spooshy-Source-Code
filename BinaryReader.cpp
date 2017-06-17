#include "BinaryReader.h"
#include <assert.h>

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>

BinaryReader::BinaryReader(const char* Filename, int* Error)
{
	fp = _open(Filename, O_BINARY | O_RDONLY);
	*Error = fp ;
}

bool BinaryReader::IsOpen()
{
	return fp != -1;
}

void BinaryReader::Close()
{
	assert(IsOpen() == true);

	if(fp != -1)
	{
		_close(fp);
		fp = -1;
	}
}

char BinaryReader::ReadByte()
{
	char c;

	assert(IsOpen() == true);

	if(_read(fp,&c,sizeof(c)) ==sizeof(c))
	{
		return c;
	}

	assert(0);
	return 0;
}

int BinaryReader::ReadInt32()
{
	int c;
    
	assert(IsOpen() == true);

	if(_read(fp,&c,sizeof(c)) ==sizeof(c))
	{
		return c;
	}

	assert(0);
	return 0;
}


bool  BinaryReader::ReadBool()
{
	bool c;

	assert(IsOpen() == true);

	if (_read(fp, &c, sizeof(c)) == sizeof(c))
	{
		return c;
	}

	assert(0);
	return 0;
}

float BinaryReader::ReadSingle()
{
	float c;

	assert(IsOpen() == true);

	if(_read(fp,&c,sizeof(c)) ==sizeof(c))
	{
		return c;
	}

	assert(0);
	return 0;
}

char* BinaryReader::ReadBytes(int& bytesRead)
{
	bytesRead = ReadInt32();

    char *p = new char[bytesRead];
    
	if(_read(fp,p,bytesRead) ==bytesRead)
	{
		return p;
	}

	delete[] p;
	bytesRead = 0;

	assert(0);
	return NULL;
}


std::string BinaryReader::ReadString()
{
	std::string s="";
	int len;

	char* bytes = ReadBytes(len);

	if(len > 0)
	{
		s.assign(bytes,len);

		delete[] bytes;
		return s;
	}

	delete[] bytes;
	assert(0);
	return s;
}
