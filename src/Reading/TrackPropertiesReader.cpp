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
#include "TrackPropertiesReader.hpp"
//Local
#include "PerformanceDataFormat.hpp"
//Namespaces
using namespace libKORG;
using namespace StdXX;

//Public methods
template<uint32 N>
void TrackPropertiesReader<N>::Read(const ChunkHeader &header, DataReader &dataReader)
{
	if((header.version.major == 0) && (header.version.minor == 0))
	{
		this->ReadVersion00Chunk(dataReader);
	}
	else if((header.version.major == 1) && (header.version.minor == 0))
	{
		this->ReadVersion10Chunk(dataReader);
	}
	else if((header.version.major == 1) && (header.version.minor == 1))
	{
		this->ReadVersion11Chunk(dataReader);
	}
	else
		NOT_IMPLEMENTED_ERROR;
}

//Private methods
template<uint32 N>
void TrackPropertiesReader<N>::ReadVersion00Chunk(DataReader& dataReader)
{
	uint32 trackNumber = dataReader.ReadUInt32();

	TrackProperties& atp = this->trackProperties[trackNumber];

	auto unknown1 = atp.unknown1.CreateOutputStream();
	dataReader.InputStream().FlushTo(*unknown1, 8);

	uint8 msb = dataReader.ReadByte();
	uint8 lsb = dataReader.ReadByte();
	uint8 soundSetType = dataReader.ReadByte();
	uint8 pc = dataReader.ReadByte();

	ASSERT(
			(soundSetType == (uint8)SoundSetType::KORG)
			|| (soundSetType == (uint8)SoundSetType::XG), String::Number(soundSetType));


	atp.soundProgramChangeSeq = ProgramChangeSequence(static_cast<SoundSetType>(soundSetType), msb, lsb, pc);

	atp.volume = dataReader.ReadByte();
	atp.pan = dataReader.ReadByte() - c_knob_offset;
	atp.detune = dataReader.ReadByte() - c_knob_offset;

	atp.octaveTranspose = (int8)dataReader.ReadByte() / 12;

	auto unknown2 = atp.unknown2.CreateOutputStream();
	dataReader.InputStream().FlushTo(*unknown2, 11);

	atp.fxMaster1 = dataReader.ReadByte();
	atp.fxMaster2 = dataReader.ReadByte();

	ASSERT_EQUALS(0, dataReader.ReadUInt16());

	atp.dry = dataReader.ReadByte();
	ASSERT(atp.dry <= 1, "??");

	ASSERT_EQUALS(0, dataReader.ReadByte());
	ASSERT_EQUALS(0, dataReader.ReadByte());

	atp.unknown12 = dataReader.ReadByte();
	atp.unknown13 = dataReader.ReadByte();

	atp.pbSensitivity = dataReader.ReadByte();

	atp.unknown182 = dataReader.ReadByte();
	atp.unknown181 = dataReader.ReadByte();

	atp.lowGainTimes2 = dataReader.ReadByte();
	atp.midGainTimes2 = dataReader.ReadByte();
	atp.highGainTimes2 = dataReader.ReadByte();

	auto unknown3 = atp.unknown3.CreateOutputStream();
	dataReader.InputStream().FlushTo(*unknown3);
}

template<uint32 N>
void TrackPropertiesReader<N>::ReadVersion10Chunk(DataReader &dataReader)
{
	this->ReadVersion00Chunk(dataReader);
}

template<uint32 N>
void TrackPropertiesReader<N>::ReadVersion11Chunk(DataReader &dataReader)
{
	this->ReadVersion10Chunk(dataReader);
}

//Explicit instantiation
template class TrackPropertiesReader<4>;
template class TrackPropertiesReader<8>;