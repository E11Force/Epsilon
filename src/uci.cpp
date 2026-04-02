/*
    Epsilon - Chess Engine
    Copyright (C) 2026 Fozik

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <chrono>
#include "board.hpp"

using namespace chrono;

void Board::perftStart(int depth) {
    for (int i = 0; i < depth; i++) {
        auto elapsedFrom = steady_clock::now();
        Bitboard nodes = Perft(i);
        auto elapsedTo = steady_clock::now();
        string isCorrect = nodes == perftValues[i] ? "\033[1;32m+\033[0m" : "\033[1;31mx\033[0m";

        std::cout << fixed << "depth " << i << '.' << " nodes " << nodes << ' ' << isCorrect << " nps: " << int(nodes / duration_cast<duration<float,nano>>(elapsedTo - elapsedFrom).count() * 1000000000) << endl;
    }
}