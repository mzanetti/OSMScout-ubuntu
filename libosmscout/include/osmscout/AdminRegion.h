#ifndef OSMSCOUT_ADMINREGION_H
#define OSMSCOUT_ADMINREGION_H

/*
  This source is part of the libosmscout library
  Copyright (C) 2009  Tim Teulings

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include <osmscout/Reference.h>

namespace osmscout {
  /**
    A named administrative region. It is used to build up hierachical, structured
    containment information like "Streets in City".
  */
  struct AdminRegion
  {
    Reference   reference; //! Reference to the object defining the region
    FileOffset  offset;    //! Offset into the region datafile
    std::string name;      //! name of the region
    std::string hash;      //! internal has for the name of the region
    std::string path;      //! Containment path
  };
}

#endif