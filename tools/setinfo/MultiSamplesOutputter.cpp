/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "MultiSamplesOutputter.hpp"

//Public methods
void MultiSamplesOutputter::Output(const MultiSamplesObject& multiSamplesObject) const
{
	{
		Section sampleEntriesSection(u8"Sample entries", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.sampleEntries.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.sampleEntries[i]);
	}

	{
		Section sampleEntriesSection(u8"Drum sample entries", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.drumSampleEntries.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.drumSampleEntries[i]);
	}

	{
		Section sampleEntriesSection(u8"Multisample entries", this->formattedOutputter);

		for(uint32 i = 0; i < multiSamplesObject.data.multiSampleEntries.GetNumberOfElements(); i++)
			this->Output(i, multiSamplesObject.data.multiSampleEntries[i]);
	}
}

//Private methods
void MultiSamplesOutputter::Output(uint32 index, const DrumSampleEntry& drumSampleEntry) const
{
	Section section(u8"Drum sample entry " + String::Number(index), this->formattedOutputter);

	for(uint32 i = 0; i < sizeof(drumSampleEntry.unknown1); i++)
		this->formattedOutputter.OutputProperty(u8"unknown1 " + String::Number(i), drumSampleEntry.unknown1[i]);

	this->formattedOutputter.OutputProperty(u8"name", drumSampleEntry.name);

	for(uint32 i = 0; i < sizeof(drumSampleEntry.unknown2); i++)
		this->formattedOutputter.OutputProperty(u8"unknown2 " + String::Number(i), drumSampleEntry.unknown2[i]);
}

void MultiSamplesOutputter::Output(uint32 index, const MultiSampleEntry& multiSampleEntry) const
{
	Section section(u8"Multisample entry " + String::Number(index), this->formattedOutputter);

	this->formattedOutputter.OutputProperty(u8"unknown1", multiSampleEntry.unknown1);
	this->formattedOutputter.OutputProperty(u8"name", multiSampleEntry.name);

	for(uint32 i = 0; i < sizeof(multiSampleEntry.unknown2); i++)
		this->formattedOutputter.OutputProperty(u8"unknown2 " + String::Number(i), multiSampleEntry.unknown2[i]);
}

void MultiSamplesOutputter::Output(uint32 index, const SampleEntry& sampleEntry) const
{
	Section section(u8"Sample entry " + String::Number(index), this->formattedOutputter);

	for(uint32 i = 0; i < sizeof(sampleEntry.unknown1); i++)
		this->formattedOutputter.OutputProperty(u8"unknown1 " + String::Number(i), sampleEntry.unknown1[i]);

	this->formattedOutputter.OutputProperty(u8"unknown6", sampleEntry.unknown6);

	for(uint32 i = 0; i < sizeof(sampleEntry.unknown2); i++)
		this->formattedOutputter.OutputProperty(u8"unknown2 " + String::Number(i), sampleEntry.unknown2[i]);

	this->formattedOutputter.OutputProperty(u8"name", sampleEntry.name);

	for(uint32 i = 0; i < sizeof(sampleEntry.unknown3); i++)
		this->formattedOutputter.OutputProperty(u8"unknown3 " + String::Number(i), sampleEntry.unknown3[i]);

	for(uint32 i = 0; i < sizeof(sampleEntry.unknown4); i++)
		this->formattedOutputter.OutputProperty(u8"unknown4 " + String::Number(i), sampleEntry.unknown4[i]);

	this->formattedOutputter.OutputProperty(u8"originalNote", sampleEntry.originalNote);
}