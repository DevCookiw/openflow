#include <iterator>
#define MINIAUDIO_IMPLEMENTATION

#include "miniaudio.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

vector<float> audio_buffer;
mutex buffer_mutex;

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  if (pInput == nullptr)
    return;

  const float *samples = static_cast<const float *>(pInput);

  lock_guard<mutex> lock(buffer_mutex);
  for (ma_uint32 i = 0; i < frameCount; ++i) {
    audio_buffer.push_back(samples[i]);
  }

  (void)pOutput;
}
int main() {
  cout << "Audio capturing test 67" << endl;
  ma_device_config config = ma_device_config_init(ma_device_type_capture);
  config.capture.format = ma_format_f32;
  config.capture.channels = 1;
  config.sampleRate = 16000;
  config.dataCallback = data_callback;

  ma_device device;
  if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
    cerr << "Failed to initialize device" << endl;
    return 1;
  }
  ma_device_start(&device);
  cout << "Device is listening" << endl;
  while (true) {
    this_thread::sleep_for(chrono::milliseconds(3000));

    vector<float> chunk_to_process;
    {
      lock_guard<std::mutex> lock(buffer_mutex);
      chunk_to_process = audio_buffer;
      audio_buffer.clear();
    }

    if (!chunk_to_process.empty()) {
      cout << "Grabbed an audio chunk! Total samples: "
           << chunk_to_process.size() << endl;
    }
  }
}
