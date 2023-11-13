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

BMI bmi; // this does work

int main()
{
	Board::initialize();
	Leds::setOutput();
  // BMI bmi;  // this does NOT work

  stlink::Uart::initialize<SystemClock, 921600_Bd>(
      stlink::Uart::Parity::Disabled,
      stlink::Uart::WordLength::Bit8 );

  // initialize the Wiznet Ethernet module I/O pins,
  // so they don't interfere with BMI088 module
  using EthCs = GpioC6;
  using EthInt = GpioB15;
  using EthRst = GpioB8;
  EthInt::setInput(Gpio::InputType::PullDown);
  EthRst::setOutput(Gpio::OutputType::PushPull);
  EthCs::setOutput(Gpio::OutputType::PushPull);

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
