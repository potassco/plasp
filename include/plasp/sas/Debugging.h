#ifndef __PLASP__SAS__DEBUGGING_H
#define __PLASP__SAS__DEBUGGING_H

#include <iosfwd>

#include <plasp/sas/Description.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Debugging
//
////////////////////////////////////////////////////////////////////////////////////////////////////

output::ColorStream &operator<<(output::ColorStream &ostream, const Description &description);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
