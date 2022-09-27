/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2022 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "ms4525do.h"  // NOLINT
#if defined(ARDUINO)
#include "Arduino.h"
#include "Wire.h"
#else
#include "core/core.h"
#endif

namespace bfs {

Ms4525do::Ms4525do(TwoWire *bus, const uint8_t addr, const float p_max,
                   const float p_min, const OutputType type) {
  bus_ = bus;
  addr_ = addr;
  p_min_ = p_min;
  p_max_ = p_max;
  if (type == OUTPUT_TYPE_A) {
    c_ = 0.1f;
    d_ = 0.8f;
  } else {
    c_ = 0.05f;
    d_ = 0.9f;
  }
}
void Ms4525do::Config(TwoWire *bus, const uint8_t addr, const float p_max,
                      const float p_min, const OutputType type) {
  bus_ = bus;
  addr_ = addr;
  p_min_ = p_min;
  p_max_ = p_max;
  if (type == OUTPUT_TYPE_A) {
    c_ = 0.1f;
    d_ = 0.8f;
  } else {
    c_ = 0.05f;
    d_ = 0.9f;
  }
}
bool Ms4525do::Begin() {
  /* Checking to see if we can communicate with sensor */
  for (int tries = 0; tries < MAX_TRIES_; tries++) {
    if (Read()) {
      return true;
    }
    delay(10);
  }
  return false;
}
bool Ms4525do::Read() {
  bytes_rx_ = bus_->requestFrom(addr_, sizeof(buf_));
  if (bytes_rx_ != sizeof(buf_)) {
    return false;
  }
  for (size_t i = 0; i < bytes_rx_; i++) {
    buf_[i] = bus_->read();
  }
  status_ = (buf_[0] >> 6) & 0x03;
  pres_cnts_ = static_cast<uint16_t>(buf_[0] & 0x3F) << 8 | buf_[1];
  temp_cnts_ = static_cast<uint16_t>(buf_[2]) << 3 | buf_[3] & 0xE0 >> 5;
  pres_psi_ = (static_cast<float>(pres_cnts_) - c_ * P_CNT_) *
              ((p_max_ - p_min_) / (d_ * P_CNT_)) + p_min_;
  temp_ = static_cast<float>(temp_cnts_) * (T_MAX_ - T_MIN_) / T_CNT_ + T_MIN_;
  if (temp_ > T_MAX_) {return false;}
  if (static_cast<Status>(status_) != STATUS_GOOD) {return false;}
  pres_pa_ = pres_psi_ * 0.45359237f * 9.80665f / 0.0254f / 0.0254f;
  temp_c_ = temp_;
  return true;
}

}  // namespace bfs
