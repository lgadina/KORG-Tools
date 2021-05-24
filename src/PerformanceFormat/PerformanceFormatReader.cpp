/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of KORG-Tools.
 *
 * KORG-Tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KORG-Tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KORG-Tools.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "PerformanceFormatReader.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
PerformanceObject *PerformanceFormatReader::TakePerformanceResult()
{
	ASSERT_EQUALS(1, this->perfIndex);

	return new PerformanceObject(Move(this->generalPerformanceData), Move(this->keyboardSettings[0]));
}

SingleTouchSettings *PerformanceFormatReader::TakeSTSResult()
{
	ASSERT_EQUALS(4, this->perfIndex);

	return new SingleTouchSettings(this->max9version, Move(this->generalPerformanceData), Move(this->keyboardSettings));
}