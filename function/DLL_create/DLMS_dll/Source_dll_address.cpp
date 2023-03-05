#include <math.h>
__declspec(dllexport) int GetAddress(long value, unsigned long &address, int &size)
{
	if (value < 0xff)
	{
		address = (unsigned char)(value << 1 | 1);
		size = 1;
		return 0;
	}
	else if (value < 0xffff)
	{
		address = (unsigned short)((value / (long)pow(16, 2) << 1) * 16 * 16 + (value % ((long)pow(16, 2)) << 1 | 1));
		size = 2;
	}

	else
	{
		return 258; // DLMS_ERROR_CODE_INVALID_PARAMETER;
	}
	return 0; // DLMS_ERROR_CODE_OK;
}
