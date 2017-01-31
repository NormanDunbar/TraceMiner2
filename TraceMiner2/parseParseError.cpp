/*
 * MIT License
 *
 * Copyright (c) 2017 Norman Dunbar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** @file parseParseError.cpp
 * @brief Implementation file for the tmTraceFile.parsePARSEERROR() function.
 */

#include "tmtracefile.h"


/** @brief Parses a "PARSE ERROR" line.
 *
 * @param thisLine const string&. The trace line with "PARSE ERROR" in.
 * @return bool. Returns true if all ok. False otherwise.
 *
 * Parses a line from the trace file. The line is expected
 * to be the PARSE ERROR #cursor line.
 */
bool tmTraceFile::parsePARSEERROR(const string &thisLine) {

    // Write the replaced SQL to the report file.
    *mOfs << setw(MAXLINENUMBER) << mLineNumber << ' '
          << setw(MAXLINENUMBER) << ' ' << ' '
          << setw(MAXLINENUMBER) << ' ' << ' '
          << thisLine << ' '
          << endl;


    // Looks like a good parse.
    if (mOptions->verbose()) {
        *mDbg << "parsePARSEERROR(" << mLineNumber << "): Exit." << endl;
    }

    return true;
}

