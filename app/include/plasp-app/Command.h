#ifndef __PLASP_APP__COMMAND_H
#define __PLASP_APP__COMMAND_H

#include <tuple>

#include <cxxopts.hpp>

#include <plasp-app/Utils.h>
#include <plasp-app/Version.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived, class... OptionGroups>
class Command
{
	public:
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

	protected:
		Command()
		:	m_options(std::string("plasp ") + Derived::Name, std::string(Derived::Description) + ".")
		{
			forEach(m_optionGroups,
				[&](auto &optionGroup)
		        {
					optionGroup.addTo(m_options);
				});
		}

		void parseOptions(int argc, char **argv)
		{
			const auto parseResult = m_options.parse(argc, argv);

			forEach(m_optionGroups,
				[&](auto &optionGroup)
		        {
					optionGroup.read(parseResult);
				});
		}

		static void printVersion()
		{
			std::cout << Version << std::endl;
		}

		cxxopts::Options m_options;
		std::tuple<OptionGroups...> m_optionGroups;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
