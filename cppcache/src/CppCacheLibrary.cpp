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

#include "CppCacheLibrary.hpp"

#include <ace/OS.h>
#include <ace/ACE.h>
#include <ace/Init_ACE.h>
#include <ace/Log_Msg.h>
#include <ace/Singleton.h>

#include "config.h"
#include "MapEntry.hpp"
#include "ExpMapEntry.hpp"
#include "LRUMapEntry.hpp"
#include "LRUExpMapEntry.hpp"
#include <geode/CacheFactory.hpp>
#include "SerializationRegistry.hpp"
#include <geode/DataOutput.hpp>
#include "TcrMessage.hpp"
#include "Utils.hpp"

#include <string>

// called during DLL initialization
void initLibDllEntry(void) {
  apache::geode::client::CppCacheLibrary::initLib();
}

extern "C" {
void DllMainGetPath(char* result, int maxLen);
}

namespace apache {
namespace geode {
namespace client {

// expect this to be called from key Library entry points, or automatically
// if we can... Probably safest to call from DistributedSystem factory method.
// impl type Unit tests may need to call this themselves to ensure the
// internals are prepared. fw_helper framework will handle this.
void CppCacheLibrary::initLib(void) { ACE::init(); }

// this closes ace and triggers the cleanup of the singleton CppCacheLibrary.
void CppCacheLibrary::closeLib(void) {
  // ACE::fini(); This should not happen..... Things might be using ace beyond
  // the life of
  // using geode.
}

// Returns pathname of product's lib directory, adds 'addon' to it if 'addon' is
// not null.
std::string CppCacheLibrary::getProductLibDir(const char* addon) {
  std::string proddir = CppCacheLibrary::getProductDir();
  proddir += "/lib/";
  if (addon != nullptr) {
    proddir += addon;
  }
  return proddir;
}

// return the directory where the library/DLL resides
std::string CppCacheLibrary::getProductLibDir() {
  // otherwise... get the DLL path, and work backwards from it.
  char buffer[PATH_MAX + 1];
  buffer[0] = '\0';
  DllMainGetPath(buffer, PATH_MAX);

  std::string path(buffer);
  std::string::size_type pos = std::string::npos;
#ifdef WIN32
  std::string cppName = PRODUCT_LIB_NAME;
  cppName += ".dll";
  pos = path.find(cppName);
  if (std::string::npos == pos) {
    std::string dotNetName = PRODUCT_DLL_NAME;
    dotNetName += ".dll";
    pos = path.find(dotNetName);
  }
#else
  std::string cppName = "lib";
  cppName += PRODUCT_LIB_NAME;
  pos = path.find(cppName);
#endif
  if (0 < pos) {
    return path.substr(0, --pos);
  }
  return std::string();
}

std::string CppCacheLibrary::getProductDir() {
  // If the environment variable is set, use it.
  std::string geodeNativeEnvironment = Utils::getEnv("GEODE_NATIVE_HOME");
  if (geodeNativeEnvironment.length() > 0) {
    return geodeNativeEnvironment;
  }

  // otherwise... get the DLL path, and work backwards from it.
  std::string productLibraryDirectoryName = getProductLibDir();
  if (productLibraryDirectoryName.size() == 0) {
    fprintf(stderr,
            "Cannot determine location of product directory.\n"
            "Please set GEODE_NATIVE_HOME environment variable.\n");
    fflush(stderr);
    throw apache::geode::client::IllegalStateException(
        "Product installation directory "
        "not found. Please set GEODE_NATIVE_HOME environment variable.");
  }
  // replace all '\' with '/' to make everything easier..
  size_t len = productLibraryDirectoryName.length() + 1;
  char* slashtmp = new char[len];
  ACE_OS::strncpy(slashtmp, productLibraryDirectoryName.c_str(), len);
  for (size_t i = 0; i < productLibraryDirectoryName.length(); i++) {
    if (slashtmp[i] == '\\') {
      slashtmp[i] = '/';
    }
  }
  productLibraryDirectoryName = slashtmp;
  delete[] slashtmp;
  slashtmp = nullptr;

  // check if it is "hidden/lib/debug" and work back from build area.
  size_t hiddenidx = productLibraryDirectoryName.find("hidden");
  if (hiddenidx != std::string::npos) {
    // make sure hidden was a whole word...
    hiddenidx--;
    if (productLibraryDirectoryName[hiddenidx] == '/' ||
        productLibraryDirectoryName[hiddenidx] == '\\') {
      // odds are high hiddenidx terminates osbuild.dir.
      std::string hiddenroute =
          productLibraryDirectoryName.substr(0, hiddenidx) + "/product";
      return hiddenroute;
    }
  }
  // check if bin on windows, and go back one...
  GF_D_ASSERT(productLibraryDirectoryName.length() > 4);
#ifdef WIN32
  std::string libpart = "bin";
#else
  std::string libpart = "lib";
#endif
  if (productLibraryDirectoryName.substr(productLibraryDirectoryName.length() -
                                         3) == libpart) {
    return productLibraryDirectoryName.substr(
        0, productLibraryDirectoryName.length() - 4);
  } else {
    return productLibraryDirectoryName;
  }
}

}  // namespace client
}  // namespace geode
}  // namespace apache
