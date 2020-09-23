#define ENCODER_PHASE_0  0
#define ENCODER_PHASE_1  2
#define ENCODER_PHASE_2  3
#define ENCODER_PHASE_3  1

#define ENCODER_PULSES_PER_STEP  4

typedef struct {
  bool encoderRateEnabled = 0;
  int encoderMoveValue = 0;
  unsigned long lastEncoderTime = 0;
} ENCODER_Rate;

typedef enum
{
    ENCODER_DIFF_NO = 0,
    ENCODER_DIFF_CW = 1,
    ENCODER_DIFF_CCW = 2,
    ENCODER_DIFF_ENTER = 3
} ENCODER_DiffState;

class Encoder
{
private:
    unsigned char lastEncoderBits;
    unsigned long lastClickUpdate;

    unsigned long previousDepressCheckMillis = 0;
    int debounceTime = 200;
    bool previousPressed = false;

public:
    // void readEncoder();
    bool debounceReadPinDepressed();
    ENCODER_DiffState analyze();
};