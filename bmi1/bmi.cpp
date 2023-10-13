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

BMI::BMI() 
{}

void BMI::initialize( void ) {

	Dma1::enable();
	Spi::connect<Sck::Sck, Mosi::Mosi, Miso::Miso>();
	Spi::initialize<Board::SystemClock, 9_MHz, 10_pct>();

	AccInt1::setInput(AccInt1::InputType::PullDown);
	GyroInt3::setInput(GyroInt3::InputType::PullDown);
	constexpr bool selfTest = true;
	while (!imu.initialize(selfTest)) {
		MODM_LOG_ERROR << "Initialization failed, retrying ...\n";
		modm::delay(500ms);
	}

	bool ok = imu.setAccRate(Imu::AccRate::Rate12Hz_Bw5Hz);
	ok &= imu.setAccRange(Imu::AccRange::Range3g);

	const auto int1Config = (Imu::AccGpioConfig::ActiveHigh | Imu::AccGpioConfig::EnableOutput);
	ok &= imu.setAccInt1GpioConfig(int1Config);
	ok &= imu.setAccGpioMap(Imu::AccGpioMap::Int1DataReady);

	ok &= imu.setGyroRate(Imu::GyroRate::Rate100Hz_Bw12Hz);
	ok &= imu.setGyroRange(Imu::GyroRange::Range250dps);
	ok &= imu.setGyroGpioConfig(Imu::GyroGpioConfig::Int3ActiveHigh);
	ok &= imu.setGyroGpioMap(Imu::GyroGpioMap::Int3DataReady);

	if (!ok) {
		MODM_LOG_ERROR << "Configuration failed!\n";
	} else {
  	Exti::connect<AccInt1>(Exti::Trigger::RisingEdge, [&accReady](auto){
  		accReady = true;
  	});
  	Exti::connect<GyroInt3>(Exti::Trigger::RisingEdge, [&gyroReady](auto){
  		gyroReady = true;
  	});
    initialized = true;
  }
}

bool BMI::check_acc(void) {
  bool bret = false;
  if(initialized and accReady) {
    accReady = false;
    const std::optional accResult = imu.readAccData();
    if (accResult) {
      bret = true;
      const modm::Vector3f data = accResult->getFloat();
      MODM_LOG_INFO.printf("Acc  [mg]\tx:\t%5.1f\ty: %5.1f\tz: %5.1f   n:%ld" EOL, 
          data[0], data[1], data[2], nacc_reads );
    }
  }
  return bret;
}

bool BMI::check_gyro(void) {
  bool bret = false;
  if(initialized and gyroReady) {
    gyroReady = false;
    const std::optional gyroResult = imu.readGyroData();
    if (gyroResult) {
      bret = true;
      const modm::Vector3f data = gyroResult->getFloat();
      MODM_LOG_INFO.printf("Gyro [deg/s]\tx:\t%5.2f\ty: %5.2f\tz: %5.2f   n:%ld" EOL, 
          data[0], data[1], data[2], ngyro_reads);
    }
  }
  return bret;
}
