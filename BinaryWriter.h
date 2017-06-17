#pragma once
#include <string>

typedef std::string String;

class BinaryWriter
{
	public:

		BinaryWriter(const char* Filename);
		bool IsOpen();
		void Close();
		void WriteByte(char c);
		void WriteInt32(int c);
		void WriteBool(bool c); 
		void WriteSingle(float c);
		void WriteBytes(int Count,const void* bytes);
		void WriteString(std::string& str);

	private:
		int fp;

};