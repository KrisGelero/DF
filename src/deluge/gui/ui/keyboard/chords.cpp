/*
 * Copyright (c) 2014-2024 Synthstrom Audible Limited
 * Copyright © 2024 Madeline Scyphers
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */
#include "gui/ui/keyboard/chords.h"
#include "io/debug/log.h"
#include <stdlib.h>

namespace deluge::gui::ui::keyboard {

ChordList::ChordList()
    : chords{
        {"", NoteSet({ROOT}), {{0, NONE, NONE, NONE, NONE, NONE}}}, // TODO remove when root fixed on bottom of keyboard
        kMajor,
        kMinor,
        kDim,
        kM7,
        k7,
        kMinor7,
        kMinor7b5,
        kSus4,
        kSus2,
        kAug,
        kM9,
        k9,
        kMinor9,
        kM11,
        k11,
        kMinor11,
        kM13,
        k13,
        kMinor13} {
}

Voicing ChordList::getChordVoicing(int32_t chordNo) {
	// Check if chord number is valid
	chordNo = validateChordNo(chordNo);

	int32_t voicingNo = voicingOffset[chordNo];
	bool valid;
	if (voicingNo <= 0) {
		return chords[chordNo].voicings[0];
	}
	// Check if voicing is valid
	// voicings after the first should default to 0
	// So if the voicing is all 0, we should return the voicing before it
	else if (voicingNo > 0) {
		for (int voicingN = voicingNo; voicingN >= 0; voicingN--) {
			Voicing voicing = chords[chordNo].voicings[voicingN];

			bool valid = false;
			for (int j = 0; j < kMaxChordKeyboardSize; j++) {
				if (voicing.offsets[j] != 0) {
					valid = true;
				}
			}
			if (valid) {
				return voicing;
			}
		}
		if (!valid) {
			D_PRINTLN("Voicing is invalid, returning default voicing");
			return chords[0].voicings[0];
		}
	}
	return chords[chordNo].voicings[0];
}

void ChordList::adjustChordRowOffset(int32_t offset) {
	if (offset > 0) {
		chordRowOffset = std::min<int32_t>(kOffScreenChords, chordRowOffset + offset);
	}
	else {
		chordRowOffset = std::max<int32_t>(0, chordRowOffset + offset);
	}
}

void ChordList::adjustVoicingOffset(int32_t chordNo, int32_t offset) {
	// Check if chord number is valid
	chordNo = validateChordNo(chordNo);

	if (offset > 0) {
		voicingOffset[chordNo] = std::min<int32_t>(kUniqueVoicings - 1, voicingOffset[chordNo] + offset);
	}
	else {
		voicingOffset[chordNo] = std::max<int32_t>(0, voicingOffset[chordNo] + offset);
	}
}

int32_t ChordList::validateChordNo(int32_t chordNo) {
	if (chordNo < 0) {
		D_PRINTLN("Chord number is negative, returning chord 0");
		chordNo = 0;
	}
	else if (chordNo >= kUniqueChords) {
		D_PRINTLN("Chord number is too high, returning last chord");
		chordNo = kUniqueChords - 1;
	}
	return chordNo;
}
// ChordList
const Chord kMajor = {"M",
                      NoteSet({ROOT, MAJ3, P5}),
                      {{ROOT, MAJ3, P5, NONE, NONE, NONE, NONE},
                       {ROOT, OCT + MAJ3, P5, NONE, NONE, NONE, NONE},
                       {ROOT, OCT + MAJ3, P5, -OCT, NONE, NONE, NONE}}};
const Chord kMinor = {"-",
                      NoteSet({ROOT, MIN3, P5}),
                      {{ROOT, MIN3, P5, NONE, NONE, NONE, NONE},
                       {ROOT, OCT + MIN3, P5, NONE, NONE, NONE, NONE},
                       {ROOT, OCT + MIN3, P5, -OCT, NONE, NONE, NONE}}};
const Chord kDim = {"DIM",
                    NoteSet({ROOT, MIN3, DIM5}),
                    {{ROOT, MIN3, DIM5, NONE, NONE, NONE, NONE},
                     {ROOT, OCT + MIN3, DIM5, NONE, NONE, NONE, NONE},
                     {ROOT, OCT + MIN3, DIM5, -OCT, NONE, NONE, NONE}}};
const Chord kAug = {"AUG",
                    NoteSet({ROOT, MIN3, AUG5}),
                    {{ROOT, MIN3, AUG5, NONE, NONE, NONE, NONE},
                     {ROOT, OCT + MIN3, AUG5, NONE, NONE, NONE, NONE},
                     {ROOT, OCT + MIN3, AUG5, -OCT, NONE, NONE, NONE}}};
const Chord kSus2 = {"SUS2",
                     NoteSet({ROOT, MAJ2, P5}),
                     {{ROOT, MAJ2, P5, NONE, NONE, NONE, NONE},
                      {ROOT, MAJ2 + OCT, P5, NONE, NONE, NONE, NONE},
                      {ROOT, MAJ2 + OCT, P5, -OCT, NONE, NONE, NONE}}};
const Chord kSus4 = {"SUS4",
                     NoteSet({ROOT, P4, P5}),
                     {{ROOT, P4, P5, NONE, NONE, NONE, NONE},
                      {ROOT, P4 + OCT, P5, NONE, NONE, NONE, NONE},
                      {ROOT, P4 + OCT, P5, -OCT, NONE, NONE, NONE}}};
const Chord k7 = {"7",
                  NoteSet({ROOT, MAJ3, P5, MIN7}),
                  {{ROOT, MAJ3, P5, MIN7, NONE, NONE, NONE},
                   {ROOT, MAJ3 + OCT, P5, MIN7, NONE, NONE, NONE},
                   {ROOT, MAJ3 + OCT, P5, MIN7 + OCT, NONE, NONE, NONE}}};
const Chord kM7 = {"M7",
                   NoteSet({ROOT, MAJ3, P5, MAJ7}),
                   {{ROOT, MAJ3, P5, MAJ7, NONE, NONE, NONE},
                    {ROOT, MAJ3 + OCT, P5, MAJ7, NONE, NONE, NONE},
                    {ROOT, MAJ3 + OCT, P5, MAJ7 + OCT, NONE, NONE, NONE}}};
const Chord kMinor7 = {"-7",
                       NoteSet({ROOT, MIN3, P5, MIN7}),
                       {{ROOT, MIN3, P5, MIN7, NONE, NONE, NONE},
                        {ROOT, MIN3 + OCT, P5, MIN7, NONE, NONE, NONE},
                        {ROOT, MIN3 + OCT, P5, MIN7 + OCT, NONE, NONE, NONE}}};
const Chord kMinor7b5 = {"-7flat5",
                         NoteSet({ROOT, MIN3, DIM5, MIN7}),
                         {{ROOT, MIN3, DIM5, MIN7, NONE, NONE, NONE},
                          {ROOT, MIN3 + OCT, DIM5, MIN7, NONE, NONE, NONE},
                          {ROOT, MIN3 + OCT, DIM5, MIN7 + OCT, NONE, NONE, NONE}}};
const Chord k9 = {"9",
                  NoteSet({ROOT, MAJ3, P5, MIN7, MAJ2}),
                  {{ROOT, MAJ3, P5, MIN7, MAJ9, NONE, NONE},
                   {ROOT, MAJ3 + OCT, P5, MIN7, MAJ9, NONE, NONE},
                   {ROOT, MAJ3 + OCT, P5, MIN7 + OCT, MAJ9, NONE, NONE}}};
const Chord kM9 = {"M9",
                   NoteSet({ROOT, MAJ3, P5, MAJ7, MAJ2}),
                   {{ROOT, MAJ3, P5, MAJ7, MAJ9, NONE, NONE},
                    {ROOT, MAJ3 + OCT, P5, MAJ7, MAJ9, NONE, NONE},
                    {ROOT, MAJ3 + OCT, P5, MAJ7 + OCT, MAJ9, NONE, NONE}}};
const Chord kMinor9 = {"-9",
                       NoteSet({ROOT, MIN3, P5, MIN7, MAJ2}),
                       {{ROOT, MIN3, P5, MIN7, MAJ9, NONE, NONE},
                        {ROOT, MIN3 + OCT, P5, MIN7, MAJ9, NONE, NONE},
                        {ROOT, MIN3 + OCT, P5, MIN7 + OCT, MAJ9, NONE, NONE}}};
const Chord k11 = {"11",
                   NoteSet({ROOT, MAJ3, P5, MIN7, MAJ2, P4}),
                   {{ROOT, MAJ3, P5, MIN7, MAJ9, P11, NONE},
                    {ROOT, MAJ3 + OCT, P5, MIN7, MAJ9, P11, NONE},
                    {ROOT, MAJ3 + OCT, P5, MIN7 + OCT, MAJ9, P11, NONE}}};
const Chord kM11 = {"M11",
                    NoteSet({ROOT, MAJ3, P5, MAJ7, MAJ2, P4}),
                    {{ROOT, MAJ3, P5, MAJ7, MAJ9, P11, NONE},
                     {ROOT, MAJ3 + OCT, P5, MAJ7, MAJ9, P11, NONE},
                     {ROOT, MAJ3 + OCT, P5, MAJ7 + OCT, MAJ9, P11, NONE}}};
const Chord kMinor11 = {"-11",
                        NoteSet({ROOT, MIN3, P5, MIN7, MAJ2, P4}),
                        {{ROOT, MIN3, P5, MIN7, MAJ9, P11, NONE},
                         {{ROOT, P4, MIN7, MIN3 + OCT, P5 + OCT, NONE, NONE}, "SO WHAT"},
                         {ROOT, MIN3 + OCT, P5, MIN7, MAJ9, P11, NONE},
                         {ROOT, MIN3 + OCT, P5, MIN7 + OCT, MAJ9, P11, NONE}}};
// 11th are often omitted in 13th and M13th chords because they clash with the major 3rd
// if anything, the 11th is often played as a #11
const Chord k13 = {"13",
                   NoteSet({ROOT, MAJ3, P5, MIN7, MAJ2, MAJ6}),
                   {{ROOT, MAJ3, P5, MIN7, MAJ9, MAJ13, NONE},
                    {ROOT, MAJ3 + OCT, P5, MIN7, MAJ9, MAJ13, NONE},
                    {ROOT, MAJ3 + OCT, P5, MIN7 + OCT, MAJ9, MAJ13, NONE}}};
const Chord kM13 = {"M13",
                    NoteSet({ROOT, MAJ3, P5, MAJ7, MAJ2, MAJ6}),
                    {{ROOT, MAJ3, P5, MAJ7, MAJ9, MAJ13, NONE},
                     {ROOT, MAJ3 + OCT, P5, MAJ7, MAJ9, MAJ13, NONE},
                     {ROOT, MAJ3 + OCT, P5, MAJ7 + OCT, MAJ9, MAJ13, NONE}}};
const Chord kMinor13 = {"-13",
                        NoteSet({ROOT, MIN3, P5, MIN7, MAJ2, P4, MAJ6}),
                        {{ROOT, MIN3, P5, MIN7, MAJ9, P11, MAJ13},
                         {ROOT, MIN3 + OCT, P5, MIN7, MAJ9, P11, MAJ13},
                         {ROOT, MIN3 + OCT, P5, MIN7 + OCT, MAJ9, P11, MAJ13}}};
} // namespace deluge::gui::ui::keyboard
