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

//#include "../../../ICqListener.hpp"
//#include "../../../CqListener.hpp"
#include "../ICqListener.hpp"
#include "SafeConvert.hpp"
using namespace System;

//using namespace Apache::Geode::Client;

namespace Apache
{
  namespace Geode
  {
    namespace Client
    {

      generic<class TKey, class TResult>
      public ref class CqListenerGeneric : Apache::Geode::Client::ICqListener<Object^, Object^>
      {
        private:

          ICqListener<TKey, TResult>^ m_listener;

        public:

          virtual void AddCqListener(ICqListener<TKey, TResult>^ listener)
          {
            m_listener = listener;
          }

          virtual void OnEvent( Apache::Geode::Client::CqEvent<Object^, Object^>^ ev) 
	        {
						//TODO:split---Done
            CqEvent<TKey, TResult> gevent(ev->GetNative());
            m_listener->OnEvent(%gevent);
          }

          virtual void OnError(Apache::Geode::Client::CqEvent<Object^, Object^>^ ev)
	        {
						//TODO::split--Done
	          CqEvent<TKey, TResult> gevent(ev->GetNative());
            m_listener->OnError(%gevent);
          }
        
	        virtual void Close() 
	        {
	          m_listener->Close();
          }   
      };
    }  // namespace Client
  }  // namespace Geode
}  // namespace Apache

