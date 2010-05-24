#ifndef OSMSCOUT_DATAFILE_H
#define OSMSCOUT_DATAFILE_H

/*
  This source is part of the libosmscout library
  Copyright (C) 2010  Tim Teulings

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

#include <set>
#include <vector>

#include <osmscout/Cache.h>
#include <osmscout/FileScanner.h>
#include <osmscout/NumericIndex.h>

namespace osmscout {

  template <class N>
  class DataFile
  {
  private:
    typedef NumericIndex<Id,N> DataIndex;

    typedef Cache<FileOffset,N> DataCache;

    struct DataCacheValueSizer : public DataCache::ValueSizer
    {
      size_t GetSize(const N& value) const
      {
        return sizeof(value);
      }
    };

  private:
    bool                isOpen;        //! If true,the data file is opened
    std::string         datafile;      //! Basename part fo the data file name
    std::string         datafilename;  //! complete filename for data file
    mutable DataCache   cache;         //! Entry cache
    mutable FileScanner scanner;       //! File stream to the data file
    DataIndex           index;         //! Index

  public:
    DataFile(const std::string& datafile,
             const std::string& indexfile,
             size_t cacheSize);

    bool Open(const std::string& path);
    bool Close();

    bool Get(const std::vector<FileOffset>& offsets,
             std::vector<N>& data) const;

    bool Get(const std::set<FileOffset>& offsets,
             std::vector<N>& data) const;

    bool Get(const std::vector<Id>& ids,
             std::vector<N>& data) const;

    bool Get(const std::set<Id>& ids,
             std::vector<N>& data) const;

    bool Get(const Id& id, N& entry) const;

    void DumpStatistics() const;
  };

  template <class N>
  DataFile<N>::DataFile(const std::string& datafile,
                        const std::string& indexfile,
                        size_t cacheSize)
  : isOpen(false),
    datafile(datafile),
    cache(cacheSize),
    index(indexfile)
  {
    // no code
  }

  template <class N>
  bool DataFile<N>::Open(const std::string& path)
  {
    datafilename=path+"/"+datafile;

    isOpen=scanner.Open(datafilename) && index.Load(path);

    return isOpen;
  }

  template <class N>
  bool DataFile<N>::Close()
  {
    bool success=true;

    if (scanner.IsOpen()) {
      if (!scanner.Close()) {
        success=false;
      }
    }

    isOpen=false;

    return success;
  }

  template <class N>
  bool DataFile<N>::Get(const std::vector<FileOffset>& offsets,
                        std::vector<N>& data) const
  {
    assert(isOpen);

    if (!scanner.IsOpen()) {
      if (!scanner.Open(datafilename)) {
        std::cerr << "Error while opening " << datafilename << " for reading!" << std::endl;
        return false;
      }
    }

    data.reserve(data.size()+offsets.size());

    typename DataCache::CacheRef cacheRef;

    for (std::vector<FileOffset>::const_iterator offset=offsets.begin();
         offset!=offsets.end();
         ++offset) {
      if (!cache.GetEntry(*offset,cacheRef)) {
        typename DataCache::CacheEntry cacheEntry(*offset);

        cacheRef=cache.SetEntry(cacheEntry);

        scanner.SetPos(*offset);
        cacheRef->value.Read(scanner);

        if (scanner.HasError()) {
          std::cerr << "Error while reading data from offset " << *offset << " of file " << datafilename << "!" << std::endl;
          // TODO: Remove broken entry from cache
          scanner.Close();
          return false;
        }
      }

      data.push_back(cacheRef->value);
    }

    return true;
  }

  template <class N>
  bool DataFile<N>::Get(const std::set<FileOffset>& offsets,
                        std::vector<N>& data) const
  {
    assert(isOpen);

    if (!scanner.IsOpen()) {
      if (!scanner.Open(datafilename)) {
        std::cerr << "Error while opening " << datafilename << " for reading!" << std::endl;
        return false;
      }
    }

    data.reserve(data.size()+offsets.size());

    typename DataCache::CacheRef cacheRef;

    for (std::set<FileOffset>::const_iterator offset=offsets.begin();
         offset!=offsets.end();
         ++offset) {
      if (!cache.GetEntry(*offset,cacheRef)) {
        typename DataCache::CacheEntry cacheEntry(*offset);

        cacheRef=cache.SetEntry(cacheEntry);

        scanner.SetPos(*offset);
        cacheRef->value.Read(scanner);

        if (scanner.HasError()) {
          std::cerr << "Error while reading data from offset " << *offset << " of file " << datafilename << "!" << std::endl;
          // TODO: Remove broken entry from cache
          scanner.Close();
          return false;
        }
      }

      data.push_back(cacheRef->value);
    }

    return true;
  }

  template <class N>
  bool DataFile<N>::Get(const std::vector<Id>& ids,
                        std::vector<N>& data) const
  {
    assert(isOpen);

    std::vector<FileOffset> offsets;

    if (!index.GetOffsets(ids,offsets)) {
      std::cerr << "Ids not found in index" << std::endl;
      return false;
    }

    return Get(offsets,data);
  }

  template <class N>
  bool DataFile<N>::Get(const std::set<Id>& ids,
                        std::vector<N>& data) const
  {
    assert(isOpen);

    std::vector<Id> i;

    i.reserve(ids.size());
    for (std::set<Id>::const_iterator id=ids.begin();
         id!=ids.end();
         ++id) {
      i.push_back(*id);
    }

    std::vector<FileOffset> offsets;

    if (!index.GetOffsets(i,offsets)) {
      std::cerr << "Ids not found in index" << std::endl;
      return false;
    }

    return Get(offsets,data);
  }

  template <class N>
  bool DataFile<N>::Get(const Id& id, N& entry) const
  {
    assert(isOpen);

    std::vector<Id> ids;
    std::vector<N>  data;

    ids.push_back(id);

    if (Get(ids,data) && data.size()==1) {
      entry=data.front();
      return true;
    }
    else {
      return false;
    }
  }

  template <class N>
  void DataFile<N>::DumpStatistics() const
  {
    cache.DumpStatistics(datafile.c_str(),DataCacheValueSizer());
    index.DumpStatistics();
  }
}

#endif