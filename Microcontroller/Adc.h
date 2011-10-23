//name : initADC
//description: initializes the ADC hardware
void sense_init(void);
void timer1_init(void);
void adc_start(void);
void adc_stop(void);
void on_timer1(void);
void adc_init(void);
unsigned int get_VVoltage(void);
unsigned int get_AVoltage(void);
