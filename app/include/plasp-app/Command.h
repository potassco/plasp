#ifndef __PLASP_APP__COMMAND_H
#define __PLASP_APP__COMMAND_H

#include <tuple>

#include <cxxopts.hpp>

#include <plasp-app/Utils.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class... OptionGroups>
class Command
{
	protected:
		void addOptionGroupsTo(cxxopts::Options &options)
		{
			forEach(m_optionGroups,
				[&](auto &optionGroup)
		        {
					optionGroup.addTo(options);
				});
		}

		void parseOptionGroups(cxxopts::Options &options)
		{
			forEach(m_optionGroups,
				[&](auto &optionGroup)
		        {
					optionGroup.parse(options);
				});
		}

		void printHelp(cxxopts::Options &options)
		{
			const auto numberOfOptionGroups = std::tuple_size<std::decay_t<decltype(m_optionGroups)>>();

			std::vector<std::string> optionGroupNames;
			optionGroupNames.reserve(numberOfOptionGroups + 1);
			optionGroupNames.emplace_back("");

			forEach(m_optionGroups,
				[&](auto &optionGroup)
				{
					optionGroupNames.emplace_back(optionGroup.Name);
				});

			std::cout << options.help(optionGroupNames) << std::endl;
		}

		std::tuple<OptionGroups...> m_optionGroups;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
