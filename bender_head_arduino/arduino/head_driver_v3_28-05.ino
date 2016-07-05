#include <SerialDXL.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// SERVO DXL basic config
#define SERVO_MODEL 100
#define SERVO_FIRMWARE 100

/**
 * DEVICEDXL_MIN_BUFFER = 6
 * Is the overhead for dynamixel like devices
 * Has MODEL_L, MODEL_H, FIRMWARE, ID, BAUDRATE and RETURN DELAY
 * This variables are inside DeviceDXL class
 */ 

// We add 5 UInt8 (5 bytes)
#define SERVO_MMAP_SIZE DEVICEDXL_MIN_BUFFER+4

// MMap position for command
#define SERVO_SELECT_COMMAND DEVICEDXL_MIN_BUFFER
#define SERVO_POS_COMMAND DEVICEDXL_MIN_BUFFER+1
#define LED_SELECT_COMMAND DEVICEDXL_MIN_BUFFER+2
#define LED_COLOR_COMMAND DEVICEDXL_MIN_BUFFER+3
//#define LED_UPDATE_COLOR_COMMAND DEVICEDXL_MIN_BUFFER+4

// Number of servos in the array
#define num_servos 5

// Pin numbers on Arduino Mega to control the servo devises
#define pin_dir 6
#define pin_reset 7
#define pin_servo0 8
#define pin_servo1 9
#define pin_servo2 11
#define pin_servo3 12
#define pin_servo4 13

// Pin numbers on Arduino Mega to control the LED devises
#define data_led_pin1 3
#define data_led_pin2 2
#define pixelNumber 16

/**
 * @brief SERVO control using DXL communication protocol
 * @details SERVO control using Dynamixel communication protocol over RS485.
 * This implementation uses 2 uint8_t variable to control state of 5 SERVOs in address 6 and 7
 * of memory map (MMap).
 * 
 * @param dir_pin Toggle communication pin.
 * @param reset_pin Pin for reset device
 * @param servo_pin SERVO pin.
 */
class HeadDXL: public DeviceDXL
{
  public:
    //HeadDXL(uint8_t dir_pin, uint8_t reset_pin, uint8_t numServos, uint8_t servos_pins[], Servo *servos[]):
	HeadDXL(uint8_t dir_pin, uint8_t reset_pin, uint8_t numServos, uint8_t servos_pins[]):
    DeviceDXL(SERVO_MODEL, SERVO_FIRMWARE, SERVO_MMAP_SIZE), // Call parent constructor
    dir_pin_(dir_pin),    // Direction pin for RS485
    reset_pin_(reset_pin), // Reset pin
	numServos_(numServos), // numero de servos
	//servos_pins_(servos_pins),    // SERVOS pins
	//servos_(servos),			// Puntero a objeto Servo
    servo_select_command_(SERVO_SELECT_COMMAND, MMap::Access::RW, 0U, 5U, 0U), // Servo command 1
    servo_pos_command_(SERVO_POS_COMMAND, MMap::Access::RW, 0U, 180U, 0U), // Servo command 2
	led_select_command_(LED_SELECT_COMMAND, MMap::Access::RW, 0U, 254U, 0U), // array LEDs command 1
	led_color_command_(LED_COLOR_COMMAND, MMap::Access::RW, 0U, 254U, 0U) // array LEDs command 2
//	led_update_color_command_(LED_UPDATE_COLOR_COMMAND, MMap::Access::RW, 0U, 1U, 0U) // array LEDs command 3
    {
      // Config pins
      pinMode(dir_pin_, OUTPUT);
      pinMode(reset_pin_, OUTPUT);
	  for(uint8_t i=0;i<numServos_;i++)
	  {
      	pinMode(servos_pins[i], OUTPUT);
	  }
    }

    void init(uint8_t servos_pins[], Servo servos[], Adafruit_NeoPixel LEDs[])
    {
      DEBUG_PRINTLN("INIT");
      /*
       * Register variables
       */
      DeviceDXL::init(msgBuf_, varList_);
      mmap_.registerVariable(&servo_select_command_);
      mmap_.registerVariable(&servo_pos_command_);
      mmap_.registerVariable(&led_select_command_);
      mmap_.registerVariable(&led_color_command_);
//      mmap_.registerVariable(&led_update_color_command_);
      
      /*
       * Load default values
       */
      DEBUG_PRINTLN("Load default");
      mmap_.load(); // Load values from EEPROM
      DEBUG_PRINT("data: ");DEBUG_PRINTLN(servo_select_command_.data);
      DEBUG_PRINT("data: ");DEBUG_PRINTLN(servo_pos_command_.data);
      DEBUG_PRINT("data: ");DEBUG_PRINTLN(led_select_command_.data);
      DEBUG_PRINT("data: ");DEBUG_PRINTLN(led_color_command_.data);
//      DEBUG_PRINT("data: ");DEBUG_PRINTLN(led_update_color_command_.data);
      
      /*
       * Read sensor data
       * e.g. Use ADC, check buttons, etc.
       */

	  /*
	   * Attach the Servo's variables to the servos_pins
	   */
	  for(uint8_t i=0;i<numServos_;i++)
	  {
	  	servos[i].attach(servos_pins[i]);
	  }
      // Settings for LED arrays
	  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
	  #if defined (__AVR_ATtiny85__)
		if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
	  #endif
	  // End of trinket special code
      for(uint8_t i=0;i<2;i++)
	  {
	  	LEDs[i].begin();
	    LEDs[i].setBrightness(30);
	    LEDs[i].show(); // Initialize all pixels to 'off'
	  }
    }
	void moveServoTo(Servo *servo, uint8_t pos)
	{
		servo->write(pos);
		delay(2);
	}
	void setPixelsTo(Adafruit_NeoPixel *LEDs_ring1, Adafruit_NeoPixel *LEDs_ring2, uint32_t colores[], uint8_t size)
	{
		//Serial.println(sizeof(pixelPositions));
		for(uint8_t i=0; i<size; i++)
		{
			LEDs_ring1->setPixelColor(i, colores[i]);
			LEDs_ring2->setPixelColor(i, colores[i+16]);
		}
		LEDs_ring1->show();
		delay(5);
		LEDs_ring2->show();
		delay(5);
		DEBUG_PRINTLN("LEDs_rings->show()");
	}
	uint32_t getColor(uint8_t numColor)
	{
		if(numColor==1) return 0x00000000;	//black
		else if(numColor==2) return 0x00FF0000;	//red
		else if(numColor==3) return 0x0000FF00;	//green
		else if(numColor==4) return 0x000000FF;	//blue
	}

	void updateServos(Servo servos[], uint8_t servo_select, uint8_t servo_pos)
	{
		if (servo_select == 0) moveServoTo(&servos[0], servo_pos);
		else if (servo_select == 1) moveServoTo(&servos[1], servo_pos);
		else if (servo_select == 2) moveServoTo(&servos[2], servo_pos);
		else if (servo_select == 3) moveServoTo(&servos[3], servo_pos);
		else if (servo_select == 4) moveServoTo(&servos[4], servo_pos);
	}
    void update(Servo servos[], Adafruit_NeoPixel LEDs[])
    {
		//DEBUG_PRINTLN("UPDATE");
		//DEBUG_PRINT("SERVO: ");DEBUG_PRINTLN(servo_select_command_.data);
		//Update servos
		//DEBUG_PRINT("POS: ");DEBUG_PRINTLN(servo_pos_command_.data);
		if (servo_select_command_.data == 5) updateServos(servos, servo_select, servo_pos); //when both values updated, change state
		else	//update both values, selct and position
		{
			servo_select = servo_select_command_.data;
			servo_pos = servo_pos_command_.data;
		}
		//Update LEDs
		//DEBUG_PRINT("LED SELECT: ");DEBUG_PRINTLN(led_select_command_.data);
		//DEBUG_PRINT("LED COLOR: ");DEBUG_PRINTLN(led_color_command_.data);
		if (led_select_command_.data == 0xFE){	//array of colors updated, ready to show
			setPixelsTo(&LEDs[0], &LEDs[1], colors_, 16); //show colors in LEDs
			DEBUG_PRINTLN("show command (0xFE)");
		}
		else if (led_select_command_.data == 0xFD){	//updating array of colors
			colors_[led_to_change] = getColor(new_color);
			DEBUG_PRINTLN("change color command (0xFD)");
		}
		else if (led_select_command_.data == 0xFC){	//Leds commands are not used in this case
			DEBUG_PRINTLN("Led command non used");
		}
		else //update last led number and color received
		{
			led_to_change = led_select_command_.data;
			new_color = led_color_command_.data;
			//DEBUG_PRINTLN("variables updated");
		}
    }

    inline bool onReset()
    {
      DEBUG_PRINTLN("ON RESET");
      return digitalRead(reset_pin_) == HIGH ? true : false;
    }

    inline void setTX()
    {
      digitalWrite(dir_pin_,HIGH);
    }

    inline void setRX()
    {
      digitalWrite(dir_pin_,LOW);
    }

  private:
    const uint8_t dir_pin_; // Toggle communication direction pin
    const uint8_t reset_pin_; // Reset pin
	const uint8_t numServos_; // Numero de servos
    //const uint8_t servos_pins_[5]; // SERVOs pins
	//Servo *servos_[5]; //puntero a objeto Servo
    
    // SERVOs variables
    MMap::UInt8 servo_select_command_;
    MMap::UInt8 servo_pos_command_;

    // LEDs variables
    MMap::UInt8 led_select_command_;
    MMap::UInt8 led_color_command_;
    
    // Memory map
    uint8_t msgBuf_[SERVO_MMAP_SIZE];
    MMap::VariablePtr varList_[SERVO_MMAP_SIZE];
	
	//LEDs colors
	uint32_t colors_[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t led_to_change = 0;
	uint8_t new_color = 0;
	
	//servos
	uint8_t servo_select = 0;
	uint8_t servo_pos = 0;
};

//Objetos para posicionar los servos (PWM)
Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

Servo servos[]={servo0, servo1, servo2, servo3, servo4};
uint8_t servos_pins[] = {pin_servo0, pin_servo1, pin_servo2, pin_servo3, pin_servo4};

//Objetos para manejar los LEDs
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(pixelNumber, data_led_pin1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(pixelNumber, data_led_pin2, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel LEDs[] = {strip1, strip2};

//General device
HeadDXL head_dxl(pin_dir, pin_reset, num_servos, servos_pins);
SerialDXL serialDxl;

void setup() {
  //Serial port for debug
  Serial.begin(115200);
  delay(50);
  
  // Init serial communication using Dynamixel format
  serialDxl.init(115200, &Serial3 , &head_dxl);

  head_dxl.init(servos_pins, servos, LEDs);
  head_dxl.reset();
  head_dxl.mmap_.serialize();
}

void loop() {
  // Update msg buffer
  while (Serial3.available())
    serialDxl.process(Serial3.read());

  head_dxl.mmap_.deserialize();
  head_dxl.update(servos, LEDs);
  head_dxl.mmap_.serialize();
}
