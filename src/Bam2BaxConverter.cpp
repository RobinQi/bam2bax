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

#include <iostream>
#include "Bam2BaxConverter.h"

const unsigned int Bam2BaxConverter::Bax_ScanData_NumFrames = 0;
const std::string Bam2BaxConverter::Bax_ScanData_RunCode = "Bam2Bax_Run_Code";
const std::string Bam2BaxConverter::Bax_ScanData_BaseMap = PacBio::AttributeValues::ScanData::DyeSet::basemap;
const std::vector<std::string> Bam2BaxConverter::Bax_Regions_RegionTypes = PacBio::AttributeValues::Regions::regiontypes;


bool Bam2BaxConverter::ConvertFile(void) {

    std::string outfn = settings_.outputBaxFilename;

    PacBio::BAM::BamFile bamfile(settings_.subreadsBamFilename);
    PacBio::BAM::BamHeader bamheader = bamfile.Header();

    if (bamheader.ReadGroups().size() != 1) {
        AddErrorMessage("Bam file must contain reads from exactly one SMRTCell.");
        return 0;
    }
    PacBio::BAM::ReadGroupInfo rg = bamheader.ReadGroups()[0];

    //settings_.movieName = rg.MovieName();

    ScanData scandata;
    scandata.PlatformID(Springfield) // only Springfield is supported.
            .MovieName(rg.MovieName())
            .WhenStarted(rg.Date())
            .RunCode(Bam2BaxConverter::Bax_ScanData_RunCode)  // bam does not contain RunCode
            .NumFrames(Bam2BaxConverter::Bax_ScanData_NumFrames) // bam does not contain NumFrames
            .FrameRate(strtof(rg.FrameRateHz().c_str(), NULL))
            .SequencingKit(rg.SequencingKit())
            .BindingKit(rg.BindingKit())
            .BaseMap(Bam2BaxConverter::Bax_ScanData_BaseMap);

    // /Regions attribute RegionTypes, which defines supported region types in ORDER.
    std::vector<RegionType> regionTypes = RegionTypeAdapter::ToRegionTypes(Bam2BaxConverter::Bax_Regions_RegionTypes);

    HDFBaxWriter writer(outfn, 
                        scandata, 
                        rg.BasecallerVersion(), 
                        internal::QVNamesInReadGroup(rg),
                        Bam2BaxConverter::Bax_Regions_RegionTypes);

    PacBio::BAM::VirtualPolymeraseReader reader(settings_.subreadsBamFilename,
                                                settings_.scrapsBamFilename);
    while(reader.HasNext()) {
        PacBio::BAM::VirtualPolymeraseBamRecord record = reader.Next();
        SMRTSequence smrt;
        smrt.Copy(record, true);
        std::vector<RegionAnnotation> ras = RegionsAdapter::ToRegionAnnotations(record, regionTypes);

        /*
        std::cout<< record.FullName() << std::endl;
        for (std::vector<RegionAnnotation>::iterator it = ras.begin(); it != ras.end(); it++) {
            std::cout << *it << endl;
        }*/
        writer.WriteOneZmw(smrt, ras);
        if (writer.Errors().size() != 0)
            break;
    }
    for (auto error: writer.Errors()) {
        AddErrorMessage(error);
    }
        
    return errors_.size() == 0;
}
