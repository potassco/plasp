#ifndef __TOKENIZE__STREAM_H
#define __TOKENIZE__STREAM_H

#include <algorithm>
#include <cassert>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <tokenize/Location.h>
#include <tokenize/StreamPosition.h>
#include <tokenize/TokenizerException.h>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Stream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
	public:
		struct Section
		{
			StreamPosition position;
			std::string name;

			std::vector<StreamPosition> newlines;
		};

	public:
		Stream()
		{
			std::setlocale(LC_NUMERIC, "C");
		}

		explicit Stream(std::string streamName, std::istream &istream)
		{
			read(streamName, istream);
		}

		~Stream() = default;

		Stream(const Stream &other) = delete;
		Stream &operator=(const Stream &other) = delete;
		Stream(Stream &&other) = default;
		Stream &operator=(Stream &&other) = default;

		void read(std::string streamName, std::istream &istream)
		{
			// Store position of new section
			m_sections.push_back({m_content.size(), streamName, {}});

			const auto contentStartIndex = m_content.size();

			try
			{
				istream.seekg(0, std::ios::end);
				const auto streamSize = istream.tellg();
				istream.seekg(0, std::ios::beg);

				m_content.reserve(m_content.size() + streamSize);
			}
			catch (const std::exception &exception)
			{
				istream.clear();
			}

			std::copy(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>(), std::back_inserter(m_content));

			for (auto i = contentStartIndex; i < m_content.size(); i++)
				if (m_content[i] == '\n')
					m_sections.back().newlines.emplace_back(i);
		}

		void read(const std::experimental::filesystem::path &path)
		{
			if (!std::experimental::filesystem::is_regular_file(path))
				throw std::runtime_error("File does not exist: “" + path.string() + "”");

			std::ifstream fileStream(path.string(), std::ios::in);

			read(path.string(), fileStream);
		}

		void reset()
		{
			m_position = 0;
		}

		void seek(StreamPosition position)
		{
			m_position = position;
		}

		StreamPosition position() const
		{
			return m_position;
		}

		Location location(StreamPosition position) const
		{
			// Find current section
			auto section = std::upper_bound(m_sections.cbegin(), m_sections.cend(), position,
				[&](const auto &lhs, const auto &rhs)
				{
					return lhs < rhs.position;
				});

			assert(section != m_sections.cbegin());

			section--;

			// Find line (row) in the file
			auto line = std::lower_bound(section->newlines.cbegin(), section->newlines.cend(), position);

			if (line == section->newlines.cbegin())
			{
				const auto row = 1;
				const auto column = static_cast<StreamPosition>(position - section->position + 1);

				return {position, section->name, section->name, row, row, column, column};
			}

			const auto row = static_cast<StreamPosition>(line - section->newlines.cbegin() + 1);
			const auto column = static_cast<StreamPosition>(position - *(line - 1));

			return {position, section->name, section->name, row, row, column, column};
		}

		Location location() const
		{
			return location(m_position);
		}

		const std::vector<Section> &sections() const
		{
			return m_sections;
		}

		char currentCharacter()
		{
			check();
			return m_content[m_position];
		}

		void advance()
		{
			check();
			m_position++;
		}

		void advanceUnchecked()
		{
			m_position++;
		}

		bool atEnd() const
		{
			return m_position >= m_content.size();
		}

		void check()
		{
			if (atEnd())
				throw TokenizerException(location(), "reading past end of file");
		}

		StreamPosition size() const
		{
			return m_content.size();
		}

		std::string &content()
		{
			return m_content;
		}

		const std::string &content() const
		{
			return m_content;
		}

	protected:
		std::string m_content;
		mutable StreamPosition m_position{0};

		std::vector<Section> m_sections;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
