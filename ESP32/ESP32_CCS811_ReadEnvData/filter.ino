// Calculation of Moving Average to suppress noisy measuring values

const uint8_t WINDOW_SIZE = 5;

uint8_t idx_eCO2 = 0, idx_TVOC = 0;
uint16_t value = 0;
uint32_t sum_eCO2 = 0, sum_TVOC = 0;
uint16_t readings_eCO2[WINDOW_SIZE], readings_TVOC[WINDOW_SIZE];
uint16_t average = 0;

uint16_t movAverage_eCO2(uint16_t value)
{
  sum_eCO2 = sum_eCO2 - readings_eCO2[idx_eCO2];  // Remove the oldest entry from the sum
  readings_eCO2[idx_eCO2] = value;
  sum_eCO2 = sum_eCO2 + value;
  idx_eCO2 = (idx_eCO2 + 1) % WINDOW_SIZE;
  average = sum_eCO2 / WINDOW_SIZE;
  return average;
}

uint16_t movAverage_TVOC(uint16_t value)
{
  sum_TVOC = sum_TVOC - readings_TVOC[idx_TVOC];  // Remove the oldest entry from the sum
  readings_TVOC[idx_TVOC] = value;
  sum_TVOC = sum_TVOC + value;
  idx_TVOC = (idx_TVOC + 1) % WINDOW_SIZE;
  average = sum_TVOC / WINDOW_SIZE;
  return average;
}
