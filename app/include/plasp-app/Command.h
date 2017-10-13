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

		std::tuple<OptionGroups...> m_optionGroups;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
