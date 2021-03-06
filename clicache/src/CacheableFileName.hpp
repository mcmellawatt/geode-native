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

#pragma once




#include "geode_defs.hpp"
#include "ICacheableKey.hpp"


using namespace System;

namespace Apache
{
  namespace Geode
  {
    namespace Client
    {

      /// <summary>
      /// An immutable filename wrapper that can serve as a distributable
      /// key object for caching as well as being a string value.
      /// </summary>
      public ref class CacheableFileName
        : public ICacheableKey
      {
      public:
        /// <summary>
        /// Static function to create a new instance from the given string.
        /// </summary>
        inline static CacheableFileName^ Create(String^ value)
        {
          return (value != nullptr && value->Length > 0 ?
            gcnew CacheableFileName(value) : nullptr);
        }

        /// <summary>
        /// Static function to create a new instance from the
        /// given character array.
        /// </summary>
        inline static CacheableFileName^ Create(array<Char>^ value)
        {
          return (value != nullptr && value->Length > 0 ?
            gcnew CacheableFileName(value) : nullptr);
        }

        // Region: IGeodeSerializable Members

        /// <summary>
        /// Serializes this object.
        /// </summary>
        /// <param name="output">
        /// the DataOutput object to use for serializing the object
        /// </param>
        virtual void ToData(DataOutput^ output);

        /// <summary>
        /// Deserialize this object, typical implementation should return
        /// the 'this' pointer.
        /// </summary>
        /// <param name="input">
        /// the DataInput stream to use for reading the object data
        /// </param>
        /// <returns>the deserialized object</returns>
        virtual void FromData(DataInput^ input);

        /// <summary>
        /// return the size of this object in bytes
        /// </summary>
        virtual property System::UInt64 ObjectSize
        {
          virtual System::UInt64 get();
        }

        /// <summary>
        /// Returns the classId of the instance being serialized.
        /// This is used by deserialization to determine what instance
        /// type to create and deserialize into.
        /// </summary>
        /// <returns>the classId</returns>
        virtual property System::UInt32 ClassId
        {
          virtual System::UInt32 get();
        }

        /// <summary>
        /// Return a string representation of the object.
        /// This returns the same string as <c>Value</c> property.
        /// </summary>
        virtual String^ ToString() override
        {
          return m_str;
        }

        // End Region: IGeodeSerializable Members

        // Region: ICacheableKey Members

        /// <summary>
        /// Return the hashcode for this key.
        /// </summary>
        virtual System::Int32 GetHashCode() override;

        /// <summary>
        /// Return true if this key matches other object.
        /// </summary>
        virtual bool Equals(ICacheableKey^ other);

        /// <summary>
        /// Return true if this key matches other object.
        /// </summary>
        virtual bool Equals(Object^ obj) override;

        // End Region: ICacheableKey Members

        /// <summary>
        /// Gets the string value.
        /// </summary>
        property String^ Value
        {
          inline String^ get()
          {
            return m_str;
          }
        }

      internal:
        /// <summary>
        /// Factory function to register this class.
        /// </summary>
        static IGeodeSerializable^ CreateDeserializable()
        {
          return gcnew CacheableFileName((String^)nullptr);
        }

      private:
        /// <summary>
        /// Allocates a new instance from the given string.
        /// </summary>
        inline CacheableFileName(String^ value)
          : m_str(value == nullptr ? String::Empty : value),m_hashcode(0) { }

        /// <summary>
        /// Allocates a new instance copying from the given character array.
        /// </summary>
        inline CacheableFileName(array<Char>^ value)
          : m_str(gcnew String(value)),m_hashcode(0) { }

        String^ m_str;
        int m_hashcode;
      };
    }  // namespace Client
  }  // namespace Geode
}  // namespace Apache



