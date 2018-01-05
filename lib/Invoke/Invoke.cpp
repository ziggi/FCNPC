/*
    SA-MP Streamer Plugin v2.5.2
    Copyright © 2010 Incognito

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Invoke.h"

Invoke * g_Invoke;

int Invoke::callNative(const PAWN::Native * native, ...)
{
	if (amx_list.empty() || amx_map.find(native->name) == amx_map.end())
	{
		return 0;
	}

	unsigned int amx_addr = amx_map[native->name], count = strlen(native->data), variables = 0;
	cell * params = new cell[count + 1], * physAddr[6];
	params[0] = count * sizeof(cell);
	va_list input;
	va_start(input, native);
	for (unsigned int i = 0; i < count; ++i)
	{
		switch (native->data[i])
		{
			case 'd':
			case 'i':
			{
				params[i + 1] = va_arg(input, int);
			}
			break;
			case 'f':
			{
				float value = (float)va_arg(input, double);
				params[i + 1] = amx_ftoc(value);
			}
			break;
			case 's':
			{
				char * string = va_arg(input, char *);
				amx_Allot(amx_list.front(), strlen(string) + 1, &params[i + 1], &physAddr[variables++]);
				amx_SetString(physAddr[variables - 1], string, 0, 0, strlen(string) + 1);
			}
			break;
			case 'v':
			{
				va_arg(input, void *);
				amx_Allot(amx_list.front(), 1, &params[i + 1], &physAddr[variables++]);
			}
			break;
			case 'c':
			{
				va_arg(input, void *);
				amx_Allot(amx_list.front(), 1, &params[i + 1], &physAddr[variables++]);
			}
			break;
			case 'p':
			{
				va_arg(input, void *);
				int size = va_arg(input, int);
				amx_Allot(amx_list.front(), size, &params[++i], &physAddr[variables++]);
				params[i + 1] = size;
			}
			break;
		}
	}
	va_end(input);
	amx_Function_t amx_Function = (amx_Function_t)amx_addr;
	int value = amx_Function(amx_list.front(), params);
	if (variables)
	{
		variables = 0;
		va_start(input, native);
		for (unsigned int i = 0; i < count; ++i)
		{
			switch (native->data[i])
			{
				case 'd':
				case 'i':
				{
					va_arg(input, int);
				}
				break;
				case 'f':
				{
					va_arg(input, double);
				}
				break;
				case 's':
				{
					amx_Release(amx_list.front(), params[i + 1]);
				}
				break;
				case 'v':
				{
					unsigned int *val = va_arg(input, unsigned int *), *returnValue = (unsigned int *)physAddr[variables++];
					*val = *returnValue;
					amx_Release(amx_list.front(), params[i + 1]);
				}
				break;
				case 'c':
				{
					unsigned int *returnValue = (unsigned int *)physAddr[variables++];
					float *val = (float *)va_arg(input, double *);
					*val = amx_ctof(*returnValue);
					amx_Release(amx_list.front(), params[i + 1]);
				}
				break;
				case 'p':
				{
					char * text = va_arg(input, char *);
					int size = va_arg(input, int);
					amx_GetString(text, physAddr[variables++], 0, size);
					amx_Release(amx_list.front(), params[++i]);
				}
				break;
			}
		}
		va_end(input);
	}
	delete [] params;
	return value;
}

int Invoke::getAddresses()
{
	if (gotAddresses)
	{
		return 1;
	}
	AMX_HEADER * amx_hdr = (AMX_HEADER *)(amx_list.back())->base;
	std::size_t size = sizeof(PAWN::names) / sizeof(const char *);
	for (std::size_t i = 0; i < size; ++i)
	{
		amx_FindNative(amx_list.back(), PAWN::names[i], &amx_idx);
		if (amx_idx != std::numeric_limits<int>::max())
		{
			unsigned int amx_addr = (unsigned int)((AMX_FUNCSTUB *)((char *)amx_hdr + amx_hdr->natives + amx_hdr->defsize * amx_idx))->address;
			if (amx_addr)
			{
				if (amx_map.find(PAWN::names[i]) == amx_map.end())
				{
					amx_map.insert(std::make_pair(PAWN::names[i], amx_addr));
				}
			}
		}
	}
	if (amx_map.size() == size)
	{
		gotAddresses = true;
		return 1;
	}
	return 0;
}
