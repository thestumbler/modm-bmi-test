#ifndef _BMI_HPP
#define _BMI_HPP

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

#define EOL "\r\n"

#include <modm/driver/inertial/bmi088.hpp>

#include <modm/board.hpp>
#include <atomic>

using namespace Board;

class BMI {
  private:
  public:
  
  using Spi = SpiMaster2_Dma<Dma1::Channel0, Dma1::Channel1>;
  using CsGyro = GpioC0;
  using CsAcc = GpioD6;
  using Mosi = GpioC3;
  using Miso = GpioC2;
  using Sck = GpioD3;
  
  using AccInt1 = GpioC8;
  using GyroInt3 = GpioC9;
  
  using Transport = modm::Bmi088SpiTransport<Spi, CsAcc, CsGyro>;
  using Imu = modm::Bmi088<Transport>;

	std::atomic_bool accReady = false;
	std::atomic_bool gyroReady = false;
  bool initialized = false;
  uint32_t nacc_reads = 0;
  uint32_t ngyro_reads = 0;

  Imu imu;

  BMI();

  void initialize(void);
  bool check_acc(void);
  bool check_gyro(void);

};

#endif // _BMI_HPP
