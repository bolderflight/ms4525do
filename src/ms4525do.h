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

#ifndef MS4525_SRC_MS4525_H_  // NOLINT
#define MS4525_SRC_MS4525_H_

#if defined(ARDUINO)
#include "Arduino.h"
#include "Wire.h"
#else
#include "core/core.h"
#endif

namespace bfs {

class Ms4525do {
 public:
  enum OutputType {
    OUTPUT_TYPE_A,
    OUTPUT_TYPE_B
  };
  enum Status : uint8_t {
    STATUS_GOOD = 0x00,
    STATUS_STALE_DATA = 0x02,
    STATUS_FAULT = 0x03
  };
  Ms4525do() {}
  Ms4525do(TwoWire *bus, const uint8_t addr, const float p_max,
           const float p_min, const OutputType type = OUTPUT_TYPE_A);
  void Config(TwoWire *bus, const uint8_t addr, const float p_max,
              const float p_min, const OutputType type = OUTPUT_TYPE_A);
  bool Begin();
  bool Read();
  inline float pres_pa() const {return pres_pa_;}
  inline float die_temp_c() const {return temp_c_;}
  inline Status status() const {return static_cast<Status>(status_);}

 private:
  /* Communication interface */
  TwoWire *bus_;
  uint8_t addr_;
  static constexpr int MAX_TRIES_ = 10;
  /* Data conversion */
  float p_max_, p_min_;
  float c_, d_;
  static constexpr int16_t P_CNT_ = 16383;
  static constexpr int16_t T_CNT_ = 2047;
  static constexpr float T_MAX_ = 150;
  static constexpr float T_MIN_ = -50;
  /* Data */
  uint8_t buf_[4];
  uint8_t bytes_rx_;
  uint8_t status_;
  uint16_t pres_cnts_, temp_cnts_;
  float pres_psi_, temp_;
  float pres_pa_, temp_c_;
};

}  // namespace bfs

#endif  // MS4525_SRC_MS4525_H_ NOLINT
