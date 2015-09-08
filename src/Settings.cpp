// Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//  * Neither the name of Pacific Biosciences nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
// BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// Author: Yuan Li

#include "Settings.h"
#include "OptionParser.h"
#include "StringUtils.hpp"

#define DEBUG_SETTINGS

using namespace std;

namespace internal {

static inline
bool StartsWith(const string& input, const string& query)
{ return input.find(query) != string::npos; }

static
std::string GetMovienameFromFilename(const std::string & filename) {
    std::vector<std::string> tokens; 
    Splice(filename, ".", tokens);
    std::string tmp = tokens.front();
    Splice(tmp, "/", tokens);
    return tokens.back();
}

} // namespace internal

const char* Settings::Option::input_        = "input";
const char* Settings::Option::output_       = "output";
const char* Settings::Option::baseMode_     = "base";
const char* Settings::Option::pulseMode_    = "pulse";

Settings::Settings(void)
    : mode(Settings::BaseMode)
{}

Settings Settings::FromCommandLine(optparse::OptionParser& parser,
                                   int argc,
                                   char *argv[])
{
    Settings settings;

    // general program info
    settings.program = parser.prog();
    settings.description = parser.description();
    settings.version = parser.version();
    for (int i = 1; i < argc; ++i) {
        settings.args.append(argv[i]);
        settings.args.append(" ");
    }

    const optparse::Values options = parser.parse_args(argc, argv);

    // input
    settings.inputBamFilenames = parser.args();
    if (settings.inputBamFilenames.size() != 2)
        settings.errors_.push_back("missing input subreads.bam or scraps.bam.");
    else {
        settings.subreadsBamFilename = settings.inputBamFilenames[0];
        settings.scrapsBamFilename   = settings.inputBamFilenames[1];
    }

    if (settings.subreadsBamFilename.find("subreads.bam") == std::string::npos)
        settings.errors_.push_back("missing input subreads.bam.");
    if (settings.scrapsBamFilename.find("scraps.bam") == std::string::npos)
        settings.errors_.push_back("missing input scraps.bam.");

    // output 
    settings.outputBaxPrefix = options[Settings::Option::output_];
    settings.outputBaxFilename = settings.outputBaxPrefix + ".bax.h5";
    settings.outputRgnFilename = settings.outputBaxPrefix + ".rgn.h5";

    // movie
    settings.movieName = internal::GetMovienameFromFilename(settings.subreadsBamFilename);

    // mode
    const bool isBaseMode = 
        options.is_set(Settings::Option::baseMode_) ? options.get(Settings::Option::baseMode_) : false;

    const bool isPulseMode = 
        options.is_set(Settings::Option::pulseMode_) ? options.get(Settings::Option::pulseMode_) : false;

    int modeCount = 0;
    if (isBaseMode)  modeCount++;
    if (isPulseMode) modeCount++;

    if (modeCount == 0)
        settings.mode = Settings::BaseMode;
    else if (modeCount == 1) 
        if (isBaseMode)  
            settings.mode = Settings::BaseMode;
        else 
            settings.mode = Settings::PulseMode;
    else
        settings.errors_.push_back("Unknown modes selected.");

#ifdef DEBUG_SETTINGS
    string modeString = "Unknown";
    if (settings.mode == Settings::BaseMode)
        modeString = "base";
    else if (settings.mode == Settings::PulseMode)
        modeString = "pulse";

    cerr << "CommandLine: " << settings.program << " " << settings.args << endl
         << "Description: " << settings.description << endl
         << "Version:     " << settings.version << endl
         << "Mode:        " << modeString << endl
         << "Input files: " << settings.subreadsBamFilename << endl
         << "             " << settings.scrapsBamFilename << endl
         << "Output file: " << settings.outputBaxFilename << endl;
         //<< "             " << settings.outputRgnFilename << endl
         //<< "Movie name : " << settings.movieName << endl;

#endif

    return settings;
}
