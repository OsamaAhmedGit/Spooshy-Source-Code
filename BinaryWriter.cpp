#include "BinaryWriter.h"
#include <assert.h>

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>

BinaryWriter::BinaryWriter(const char* Filename)
{
	fp = _open(Filename, O_CREAT | O_RDWR | O_TRUNC | O_BINARY, _S_IREAD | _S_IWRITE );

	assert(IsOpen()==true);
}

bool BinaryWriter::IsOpen()
{
	return fp !=-1;
}

void BinaryWriter::Close()
{
	_close(fp);
	fp = -1;
}

void BinaryWriter::WriteByte(char c)
{
	assert(IsOpen()==true);
	_write(fp,&c,1);
}

void BinaryWriter::WriteInt32(int c)
{
	assert(IsOpen()==true);
	_write(fp,&c,4);
}


void BinaryWriter::WriteBool(bool c)
{
	assert(IsOpen() == true);
	_write(fp, &c, 1);
}

void BinaryWriter::WriteSingle(float c)
{
	assert(IsOpen()==true);
	_write(fp,&c,4);
}

void BinaryWriter::WriteBytes(int Count, const void *c)
{
	assert(IsOpen()==true);
	WriteInt32(Count);
	_write(fp,c,Count);
}

void BinaryWriter::WriteString(std::string& str) 
{
	assert(IsOpen()==true);
	WriteBytes(str.size(),str.c_str());
}
