#ifndef __TOKENIZE__LOCATION_H
#define __TOKENIZE__LOCATION_H

#include <tokenize/StreamPosition.h>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Location
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Stream;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Location
{
	public:
		Location(Stream &stream);
		Location(Stream &stream, StreamPosition position);

		const char *sectionStart() const;
		const char *sectionEnd() const;

		StreamPosition rowStart() const;
		StreamPosition rowEnd() const;

		StreamPosition columnStart() const;
		StreamPosition columnEnd() const;

	private:
		void initializeLazily() const;

		Stream &m_stream;
		const StreamPosition m_position;

		mutable bool m_isInitialized{false};

		mutable const char *m_sectionStart{nullptr};
		mutable const char *m_sectionEnd{nullptr};

		mutable StreamPosition m_rowStart{InvalidStreamPosition};
		mutable StreamPosition m_rowEnd{InvalidStreamPosition};

		mutable StreamPosition m_columnStart{InvalidStreamPosition};
		mutable StreamPosition m_columnEnd{InvalidStreamPosition};
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
