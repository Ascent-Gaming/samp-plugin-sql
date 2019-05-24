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

#include "SQL_Statement.h"

SQL_Statement::SQL_Statement(int id, AMX *amx, int connectionId) {
	this->id = id;
	this->amx = amx;
	this->connectionId = connectionId;
	flags = STATEMENT_FLAGS_NONE;
	status = STATEMENT_STATUS_NONE;
	lastResultIdx = 0;
	query = NULL;
	callback = NULL;
	format = NULL;
	error = 0;
	errorMsg = NULL;
}

SQL_Statement::~SQL_Statement() {
	free(query);
	free(callback);
	free(format);
	for (int i = 0, size = paramsArr.size(); i != size; ++i) {
		free(paramsArr[i].first);
	}
	for (int i = 0, size = paramsStr.size(); i != size; ++i) {
		free(paramsStr[i]);
	}
	for (int i = 0, size = resultSets.size(); i != size; ++i) {
		delete resultSets[i];
	}
}

int SQL_Statement::executeCallback() {
	this->status = STATEMENT_STATUS_PROCESSED;
	cell ret = 1, amx_addr = -1;
	int funcidx;
	if (this->error == 0) {
		if (!amx_FindPublic(this->amx, this->callback, &funcidx)) {
			int a_idx = this->paramsArr.size(), c_idx = this->paramsC.size(), s_idx = this->paramsStr.size();
			for (int i = strlen(this->format) - 1; i != -1; --i) {
				if ((i > 0) && (this->format[i - 1] == '&')) {
					amx_Push(this->amx, this->paramsC[--c_idx]);
					--i; // Skipping next specifier (&x).
				} else {
					cell tmp;
					switch (this->format[i]) {
						case 'a':
						case 'A':
							amx_PushArray(this->amx, &tmp, NULL, this->paramsArr[--a_idx].first, this->paramsArr[a_idx].second);
							if (amx_addr == -1) {
								amx_addr = tmp;
							}
							break;
						case 'b':
						case 'B':
						case 'c':
						case 'C':
						case 'd':
						case 'D':
						case 'i':
						case 'I':
						case 'f':
						case 'F':
							amx_Push(this->amx, (cell)this->paramsC[--c_idx]);
							break;
						case 'r':
						case 'R':
							amx_Push(this->amx, this->id);
							break;
						case 's':
						case 'S':
							amx_PushString(this->amx, &tmp, NULL, this->paramsStr[--s_idx], 0, 0);
							if (amx_addr == -1) {
								amx_addr = tmp;
							}
							break;
					}
				}
			}
			amx_Exec(this->amx, &ret, funcidx);
		}
	} else {
		ret = 0;

		if (!amx_FindPublic(this->amx, ERROR_CALLBACK, &funcidx)) {
			amx_PushString(this->amx, &amx_addr, NULL, callback, 0, 0);
			amx_PushString(this->amx, NULL, NULL, query, 0, 0);
			amx_PushString(this->amx, NULL, NULL, errorMsg, 0, 0);
			amx_Push(this->amx, error);
			amx_Push(this->amx, connectionId);
			amx_Exec(this->amx, &ret, funcidx);
		}
	}
	if (amx_addr != -1) {
		amx_Release(this->amx, amx_addr);
	}
	return ret;
}
