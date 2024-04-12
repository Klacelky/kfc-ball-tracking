#include "headers/calibdata.hpp"
#include <stdio.h>

int CalibData::save(const char* path) const
{
	FILE* f;
	if (fopen_s(&f, path, "wb") != 0) {
		perror("save:fopen_s");
		return 0;
	}
	fwrite(this, sizeof(CalibData), 1, f);
	fclose(f);
	return 1;
}

int CalibData::load(const char* path)
{
	FILE* f;
	if (fopen_s(&f, path, "rb") != 0) {
		perror("load:fopen_s");
		return 0;
	}
	fread(this, sizeof(CalibData), 1, f);
	fclose(f);
	return 1;
}