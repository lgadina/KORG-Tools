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
#include <libkorg.hpp>
using namespace libKORG;
using namespace StdXX;

struct BankSelection
{
	FileSystem::Path setPath;
	uint8 bankNumber;
};

struct BankSelectionWithModel : BankSelection
{
	const Model* model;
};

UniquePointer<SingleTouchSettings> MapSTS(const SingleTouchSettings& sts, const Set& targetSet, const Model* model)
{
	if(model && (sts.Version() > model->GetMaximumPerformanceVersion()))
	{
		PerformanceV2ToV1Converter converter(targetSet);
		return new SingleTouchSettings(converter.Convert(sts));
	}

	return new SingleTouchSettings(sts);
}

void ConvertStyleBank(const BankSelection& source, const BankSelectionWithModel& target)
{
	Set sourceSet(source.setPath);

	if(!FileSystem::File(target.setPath).Exists())
		Set::Create(target.setPath);
	Set targetSet(target.setPath);

	if(!targetSet.StyleBanks().Contains(target.bankNumber))
		targetSet.StyleBanks().Insert(target.bankNumber, {});

	const auto& entries = sourceSet.StyleBanks()[source.bankNumber].Objects();
	for(const auto& entry : entries)
	{
		const String& styleName = entry.value.Get<0>();
		const FullStyle& fullStyle = *entry.value.Get<1>();

		UniquePointer<Style> mappedStyle = new Style(fullStyle.Style());
		UniquePointer<SingleTouchSettings> mappedSTS = MapSTS(fullStyle.STS(), targetSet, target.model);

		SharedPointer<FullStyle> mappedFullStyle = new FullStyle(Move(mappedStyle), Move(mappedSTS));

		targetSet.StyleBanks()[target.bankNumber].AddObject(styleName, entry.key, mappedFullStyle);
	}
	targetSet.Save();
}

int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::PathArgument sourceSetPathArg(u8"source-set-path", u8"Path to the source set");
	parser.AddPositionalArgument(sourceSetPathArg);

	CommandLine::StringArgument sourceBankArg(u8"source-bank", u8"Source bank name");
	parser.AddPositionalArgument(sourceBankArg);

	CommandLine::PathArgument targetSetPathArg(u8"target-set-path", u8"Path to the target set");
	parser.AddPositionalArgument(targetSetPathArg);

	CommandLine::StringArgument targetBankArg(u8"target-bank", u8"Target bank name");
	parser.AddPositionalArgument(targetBankArg);

	CommandLine::OptionWithArgument targetModelOpt(u8'm', u8"target-model", u8"The model that the target set is for");
	parser.AddOption(targetModelOpt);

	if(!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	BankSelection source;
	source.setPath = sourceSetPathArg.Value(result);
	source.bankNumber = ParseStyleBankName(sourceBankArg.Value(result));

	BankSelectionWithModel target;
	target.setPath = targetSetPathArg.Value(result);
	target.bankNumber = ParseStyleBankName(targetBankArg.Value(result));
	target.model = nullptr;
	if(result.IsActivated(targetModelOpt))
	{
		target.model = FindModel(result.Value(targetModelOpt));
		if(target.model == nullptr)
		{
			stdErr << u8"Unknown model: " << result.Value(targetModelOpt) << endl;
			return EXIT_FAILURE;
		}
	}

	ConvertStyleBank(source, target);

	return EXIT_SUCCESS;
}