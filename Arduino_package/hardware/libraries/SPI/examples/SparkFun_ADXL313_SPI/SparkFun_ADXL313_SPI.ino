/******************************************************************************
  SparkFun_ADXL313_SPI.ino

  Read values of x/y/z axis of the ADXL313 (via SPI), print them to terminal.
  This uses default configuration (1G range, full resolution, 100Hz datarate).

  SparkFun ADXL313 Arduino Library
  Pete Lewis @ SparkFun Electronics
  Original Creation Date: September 19, 2020
  https://github.com/sparkfun/SparkFun_ADXL313_Arduino_Library

  7/12/2022 Pete Lewis
  This is a modified version of the "Example8_SPI" in the library,
  made to work with the AzureWave AW-CU488 Thing Plus
  (as part of the AmebaD ARM (32-bit) boards package)

  Do you like this library? Help support SparkFun. Buy a board!

    SparkFun 3-Axis Digital Accelerometer Breakout - ADXL313 (Qwiic)
    https://www.sparkfun.com/products/17241  

  Development environment specifics:

	IDE: Arduino 1.8.19
	Hardware Platform: SparkFun AzureWave AW-CU488 Thing Plus
	SparkFun 3-Axis Digital Accelerometer Breakout - ADXL313 (Qwiic) Version: 1.0

  Hardware Connections:
  Note, Some of the pins on the ADXL313 are used for both I2C and SPI.
  The top side of the board has the pins labeled for I2C.
  The bottom side of the board has the pins labeled for SPI (CS/SDO/SCLK/SDI)

  Also note, this example uses 4-wire connection SPI.
  
  Wire up the connections like so:

  THINGPLUS    -->   ADXL313
  8            -->   CS
  2 (POCI)     -->   SDO
  0 (SCK)      -->   SCLK
  1 (PICO)     -->   SDI   
  3.3V         -->   3.3V
  GND          -->   GND


  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <SPI.h>
#include <SparkFunADXL313.h> //Click here to get the library: http://librarymanager/All#SparkFun_ADXL313
ADXL313 myAdxl;

void setup()
{
  Serial.begin(115200);
  Serial.println("Example 8 - Reading values from ADXL313 over SPI");

  if (myAdxl.beginSPI(8) == false) //Begin communication over SPI with CS pin of 8. note, beginSPI() returns the result of checkPartID().
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
  Serial.print("Sensor is connected properly.");

  // For using SPI1, 
  // Send SPI1 object as a second argument to beginSPI().
  // Note, SPI1 has CS on the AsureWave Thing Plus Pin 17

  // if (myAdxl.beginSPI(17, SPI1) == false) //Begin communication over SPI1 with CS pin of 17. note, beginSPI() returns the result of checkPartID().
  // {
  //   Serial.println("The sensor did not respond. Please check wiring.");
  //   while(1); //Freeze
  // }
  // Serial.print("Sensor is connected properly.");

  myAdxl.measureModeOn(); // wakes up the sensor from standby and puts it into measurement mode
}

void loop()
{
  if(myAdxl.dataReady()) // check data ready interrupt, note, this clears all other int bits in INT_SOURCE reg
  {
    myAdxl.readAccel(); // read all 3 axis, they are stored in class variables: myAdxl.x, myAdxl.y and myAdxl.z
    Serial.print("x: ");
    Serial.print(myAdxl.x);
    Serial.print("\ty: ");
    Serial.print(myAdxl.y);
    Serial.print("\tz: ");
    Serial.print(myAdxl.z);
    Serial.println();
  }
  else
  {
    Serial.println("Waiting for dataReady.");
  }  
  delay(50);
}