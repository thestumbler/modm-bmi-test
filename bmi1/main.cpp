/*
 * Copyright (c) 2023, Christopher Durand
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include "bmi.hpp"

int main()
{
	Board::initialize();
	Leds::setOutput();

  BMI bmi;

	bmi.initialize();

	while (true)
	{
    if( bmi.check_acc() ) {
      bmi.nacc_reads++;
    }
    if( bmi.check_gyro() ) {
      bmi.ngyro_reads++;
    }
	}

	return 0;
}
