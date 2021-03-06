/**
 * Copyright (c) 2013, Dan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/lockfree/queue.hpp>

#include "../sdk/amx/amx.h"
#include "../sdk/amx/amx2.h"

#define ERROR_CALLBACK					"OnSQLError"

#define STATEMENT_FLAGS_NONE			0
#define STATEMENT_FLAGS_THREADED		1
#define STATEMENT_FLAGS_CACHED			2

#define STATEMENT_STATUS_NONE			0
#define STATEMENT_STATUS_EXECUTED		1
#define STATEMENT_STATUS_PROCESSED		2

#define WORKER_TICK_RATE				50

// SQL_Connection
class SQL_Connection;
typedef boost::unordered_map<int, class SQL_Connection*> connectionsMap_t;

// SQL_Statement
class SQL_Statement;
typedef boost::unordered_map<int, class SQL_Statement*> statementsMap_t;
typedef boost::lockfree::queue<class SQL_Statement*> statementsQueue_t;
