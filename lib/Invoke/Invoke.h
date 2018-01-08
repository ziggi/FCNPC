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

#pragma once

#include <bitset>
#include <list>
#include <map>
#include <stdarg.h>
#include <cstring>
#include <limits>

#define HAVE_STDINT_H
#include <sdk/plugin.h>

namespace PAWN {
	struct Native {
		const char * name;
		const char * data;
	};

	static const char * const names[] = {
		"CA_RayCastLine",
		"MapAndreas_FindZ_For2DCoord"
	};

	static const Native CA_RayCastLine = { "CA_RayCastLine", "ffffffccc" };
	static const Native MapAndreas_FindZ_For2DCoord = { "MapAndreas_FindZ_For2DCoord", "ffc" };
};

class Invoke {
public:
	Invoke()
	{
		gotAddresses = false;
		amx_idx = 0;
	}

	int CA_RayCastLine(float fStartX, float fStartY, float fStartZ, float fEndX, float fEndY, float fEndZ, float *fX, float *fY, float *fZ) {
		return this->callNative(&PAWN::CA_RayCastLine, fStartX, fStartY, fStartZ, fEndX, fEndY, fEndZ, fX, fY, fZ);
	}

	int MapAndreas_FindZ_For2DCoord(float fX, float fY, float *fZ) {
		return this->callNative(&PAWN::MapAndreas_FindZ_For2DCoord, fX, fY, fZ);
	}

	int callNative(const PAWN::Native * native, ...);
	int getAddresses();

	int amx_idx;

	std::list<AMX *> amx_list;
private:
	bool gotAddresses;

	std::map<std::string, unsigned int> amx_map;
};

typedef int(*amx_Function_t)(AMX * amx, cell * params);

extern Invoke *g_Invoke;
