#include "headers/calibdata.hpp"
#include <stdio.h>

int CalibData::save() const
{
	FILE* f;
	if (fopen_s(&f, CALIB_DATA_FILE_PATH, "wb") != 0) {
		perror("save:fopen_s");
		return 0;
	}
	fwrite(this, sizeof(CalibData), 1, f);
	fclose(f);
}

int CalibData::load(CalibData* out)
{
	FILE* f;
	if (fopen_s(&f, CALIB_DATA_FILE_PATH, "rb") != 0) {
		perror("load:fopen_s");
		return 0;
	}
	fread(out, sizeof(CalibData), 1, f);
	fclose(f);
}