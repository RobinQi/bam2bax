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

#ifndef _BAM2BAXINTERNAL_H_
#define _BAM2BAXINTERNAL_H_

//namespace internal
namespace internal {
    /// \name \{

    /// Map enum QVs defined in PacBio::BAM to strings.
    static const std::map<PacBio::BAM::BaseFeature, std::string> QVEnumToString = {
        {PacBio::BAM::BaseFeature::DELETION_QV,      PacBio::GroupNames::deletionqv},
        {PacBio::BAM::BaseFeature::DELETION_TAG,     PacBio::GroupNames::deletiontag},
        {PacBio::BAM::BaseFeature::INSERTION_QV,     PacBio::GroupNames::insertionqv},
        {PacBio::BAM::BaseFeature::MERGE_QV,         PacBio::GroupNames::mergeqv},
        {PacBio::BAM::BaseFeature::SUBSTITUTION_QV,  PacBio::GroupNames::substitutionqv}, 
        {PacBio::BAM::BaseFeature::SUBSTITUTION_TAG, PacBio::GroupNames::substitutiontag},
        {PacBio::BAM::BaseFeature::IPD,              PacBio::GroupNames::prebaseframes},
        {PacBio::BAM::BaseFeature::PULSE_WIDTH,      PacBio::GroupNames::widthinframes}
    };

    /// Map string QVs defined in PacBio::BAM to enums.
    static const std::map<std::string, PacBio::BAM::BaseFeature> QVStringToEnum = {
        {PacBio::GroupNames::deletionqv,      PacBio::BAM::BaseFeature::DELETION_QV},
        {PacBio::GroupNames::deletiontag,     PacBio::BAM::BaseFeature::DELETION_TAG},     
        {PacBio::GroupNames::insertionqv,     PacBio::BAM::BaseFeature::INSERTION_QV},
        {PacBio::GroupNames::mergeqv,         PacBio::BAM::BaseFeature::MERGE_QV},
        {PacBio::GroupNames::substitutionqv,  PacBio::BAM::BaseFeature::SUBSTITUTION_QV}, 
        {PacBio::GroupNames::substitutiontag, PacBio::BAM::BaseFeature::SUBSTITUTION_TAG},
        {PacBio::GroupNames::prebaseframes,   PacBio::BAM::BaseFeature::IPD},
        {PacBio::GroupNames::widthinframes,   PacBio::BAM::BaseFeature::PULSE_WIDTH}
    };

    /// \returns a vector of QV strings contained by read group rg, such as 'DeletionQV', 'DeletionTag'.
    inline std::vector<std::string> QVNamesInReadGroup(const PacBio::BAM::ReadGroupInfo & rg) {
        std::vector<std::string> ret;
        for (auto it = internal::QVStringToEnum.begin(); it != internal::QVStringToEnum.end(); it++) {
            if (rg.HasBaseFeature(it->second)) {
                ret.push_back(it->first);
            }
        }
        return ret;
    }
    /// \}
};

#endif
