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
		Command(cxxopts::Options &&options)
		:	m_options{options}
		{
			forEach(m_optionGroups,
				[&](auto &optionGroup)
		        {
					optionGroup.addTo(m_options);
				});
		}

		void parseOptions(int argc, char **argv)
		{
			m_options.parse(argc, argv);

			forEach(m_optionGroups,
				[&](auto &optionGroup)
		        {
					optionGroup.parse(m_options);
				});
		}

		void printHelp()
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

			std::cout << m_options.help(optionGroupNames) << std::endl;
		}

		cxxopts::Options m_options;
		std::tuple<OptionGroups...> m_optionGroups;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif