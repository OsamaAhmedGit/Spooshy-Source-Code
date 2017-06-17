#pragma once
#include <string>

typedef std::string String;

class BinaryReader
{
	public:
		BinaryReader(const char* Filename, int* Error);
		bool IsOpen();
		void Close();

		char  ReadByte();
		int   ReadInt32();
		bool  ReadBool();
		float ReadSingle();
		char* ReadBytes(int& bytesRead);

		std::string ReadString();

	private:
		int fp;
};