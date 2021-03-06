#pragma once

#ifndef GEODE_REGIONGLOBALLOCKS_H_
#define GEODE_REGIONGLOBALLOCKS_H_

/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "LocalRegion.hpp"

namespace apache {
namespace geode {
namespace client {

class APACHE_GEODE_EXPORT RegionGlobalLocks {
 public:
  RegionGlobalLocks(LocalRegion* region, bool isFailover = true)
      : m_region(region), m_isFailover(isFailover) {
    m_region->acquireGlobals(m_isFailover);
  }

  ~RegionGlobalLocks() { m_region->releaseGlobals(m_isFailover); }

 private:
  LocalRegion* m_region;
  bool m_isFailover;
};
}  // namespace client
}  // namespace geode
}  // namespace apache

#endif  // GEODE_REGIONGLOBALLOCKS_H_
