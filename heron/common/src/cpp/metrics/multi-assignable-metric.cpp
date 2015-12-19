/*
 * Copyright 2015 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//////////////////////////////////////////////////////////////////////////////
//
// multi-assignable-metric.cpp
//
// Please see multi-assignable-metric.h for details
//////////////////////////////////////////////////////////////////////////////
#include "proto/messages.h"
#include "basics/basics.h"
#include "errors/errors.h"
#include "threads/threads.h"
#include "network/network.h"

#include "metrics/imetric.h"
#include "metrics/assignable-metric.h"
#include "metrics/multi-assignable-metric.h"

namespace heron { namespace common {

MultiAssignableMetric::MultiAssignableMetric()
{
}

MultiAssignableMetric::~MultiAssignableMetric()
{
  std::map<sp_string, AssignableMetric*>::iterator iter;
  for (iter = value_.begin(); iter != value_.end(); ++iter) {
    delete iter->second;
  }
}

AssignableMetric* MultiAssignableMetric::scope(const sp_string& _key)
{
  std::map<sp_string, AssignableMetric*>::iterator iter;
  iter = value_.find(_key);
  if (iter == value_.end()) {
    AssignableMetric* m = new AssignableMetric(0);
    value_[_key] = m;
    return m;
  } else {
    return iter->second;
  }
}

void MultiAssignableMetric::GetAndReset(const sp_string& _prefix,
                      proto::system::MetricPublisherPublishMessage* _message)
{
  std::map<sp_string, AssignableMetric*>::iterator iter;
  for (iter = value_.begin(); iter != value_.end(); ++iter) {
    iter->second->GetAndReset(_prefix + "/" + iter->first, _message);
  }
}

}} // end namespace
