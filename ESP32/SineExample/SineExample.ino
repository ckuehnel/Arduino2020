#include <EloquentTinyML.h>
// sine_model.h contains the array you exported from the previous step with xxd or tinymlgen
#include "sine_model.h"

#define NUMBER_OF_INPUTS 1
#define NUMBER_OF_OUTPUTS 1
// in future projects you may need to tweek this value: it's a trial and error process
#define TENSOR_ARENA_SIZE 2*1024

Eloquent::TinyML::TfLite<NUMBER_OF_INPUTS, NUMBER_OF_OUTPUTS, TENSOR_ARENA_SIZE> ml(sine_model);

#define PLOT 0

void setup() {
    Serial.begin(115200);
}

void loop() {
    // pick up a random x and predict its sine
    float x = PI * random(100) / 100;
    float y = sin(x);
    float input[1] = { x };
    float predicted = ml.predict(input);

    if (PLOT)
    {
      Serial.print("Deviation [%] ");
      Serial.println((predicted - y)*100);
      delay(200);
    }
    else
    {
      Serial.print("sin(");
      Serial.print(x);
      Serial.print(") = ");
      Serial.print(y);
      Serial.print("\t predicted: ");
      Serial.print(predicted);
      Serial.print("\t deviation: ");
      Serial.println(predicted - y, 4);
      delay(1000);
    }
    
}
