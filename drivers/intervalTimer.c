#include "intervalTimer.h"
#include <xil_io.h>
#include <xparameters.h>

#define CASC_BIT 0x00000800
#define ENT_BIT 0x00000080
#define LOAD_BIT 0x20
#define UDT_BIT 0x02
#define ARTH_BIT 0x10
#define ENIT_BIT 0x40
#define TCSR1_OFFSET 0x10
#define TCR0_OFFSET 0x08
#define TCR1_OFFSET 0x18
#define TLR0_OFFSET 0x04
#define TLR1_OFFSET 0x14
#define T0INT_BIT 0x100

/*Reads from the ZYBO registers and returns the register value as a 32-bit
 * integer*/
static uint32_t readRegister(uint32_t baseAddress, uint32_t offset) {
  return Xil_In32(baseAddress + offset);
}

/*Writes a value to the specified ZYBO register*/
static void writeRegister(uint32_t baseAddress, uint32_t offset,
                          uint32_t value) {
  Xil_Out32(baseAddress + offset, value);
}

/*Finds the base address based on the given timerNumber and returns it as a
 * 32bit integer*/
uint32_t baseAddressLocator(uint32_t timerNumber) {
  uint32_t baseAddress;

  /*These if statements determine the address*/
  if (timerNumber == 0) {
    baseAddress = XPAR_AXI_TIMER_0_BASEADDR;
  } else if (timerNumber == 1) {
    baseAddress = XPAR_AXI_TIMER_1_BASEADDR;
  } else {
    baseAddress = XPAR_AXI_TIMER_2_BASEADDR;
  }

  return baseAddress;
}

/*This will OR one bit so that only that value is set to one, leaving the others
 * unchanged. Returns the correct register value to write*/
uint32_t orOneBit(uint32_t baseAddress, uint32_t offset, uint32_t bitToEdit) {
  uint32_t prevVal = readRegister(baseAddress, offset);
  uint32_t valToWrite = prevVal | bitToEdit;
  return valToWrite;
}

/*Initialize the count up functionality for the timer*/
void intervalTimer_initCountUp(uint32_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);

  writeRegister(baseAddress, 0, CASC_BIT);
  writeRegister(baseAddress, TCSR1_OFFSET, 0);
  writeRegister(baseAddress, TLR0_OFFSET, 0);
  writeRegister(baseAddress, TLR1_OFFSET, 0);
  intervalTimer_reload(timerNumber);
}

/*Initialize the count down functionality of the timer*/
void intervalTimer_initCountDown(uint32_t timerNumber, double period) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);
  uint64_t ticks = XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ * period;
  uint32_t upper = ticks >> 32;
  uint32_t lower = ticks;

  writeRegister(baseAddress, 0, 0x812); // CASC_BIT | UDT_BIT | ARTH_BIT
  writeRegister(baseAddress, TCSR1_OFFSET, 0x012); // UDT_BIT | ARTH_BIT
  writeRegister(baseAddress, TLR0_OFFSET, lower);
  writeRegister(baseAddress, TLR1_OFFSET, upper);
  intervalTimer_reload(timerNumber);
}

/*Starts the timer*/
void intervalTimer_start(uint32_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);

  uint32_t bitToEdit = orOneBit(baseAddress, 0, ENT_BIT);
  writeRegister(baseAddress, 0, bitToEdit);

  bitToEdit = orOneBit(baseAddress, TCSR1_OFFSET, ENT_BIT);
  writeRegister(baseAddress, TCSR1_OFFSET, bitToEdit);
}

/*Stops the timer*/
void intervalTimer_stop(uint32_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);
  uint32_t prevVal = readRegister(baseAddress, 0);
  uint32_t valToEdit = ~ENT_BIT & prevVal;
  writeRegister(baseAddress, 0, valToEdit);
}

/*Reloads values into the timer*/
void intervalTimer_reload(uint32_t timerNumer) {
  uint32_t baseAddress = baseAddressLocator(timerNumer);

  uint32_t valToWrite = orOneBit(baseAddress, 0, LOAD_BIT);
  uint32_t prevVal = readRegister(baseAddress, 0);
  writeRegister(baseAddress, 0, valToWrite);
  writeRegister(baseAddress, 0, prevVal);

  prevVal = readRegister(baseAddress, TCSR1_OFFSET);
  valToWrite = orOneBit(baseAddress, TCSR1_OFFSET, LOAD_BIT);
  writeRegister(baseAddress, TCSR1_OFFSET, valToWrite);
  writeRegister(baseAddress, TCSR1_OFFSET, prevVal);
}

/*Gets the duration of the timer in seconds. Returns a double representing this
 * value*/
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);
  uint32_t lower = readRegister(baseAddress, TCR0_OFFSET);
  uint64_t upper = readRegister(baseAddress, TCR1_OFFSET);

  double returnVal = ((upper << 32) + lower);
  return returnVal / XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
}

/*Enables Interrupt for the timer*/
void intervalTimer_enableInterrupt(uint8_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);

  uint32_t valToWrite = orOneBit(baseAddress, 0, ENIT_BIT);
  writeRegister(baseAddress, 0, valToWrite);
}

/*Disables interrupt for the timer*/
void intervalTimer_disableInterrupt(uint8_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);
  uint32_t prevVal = readRegister(baseAddress, 0);
  uint32_t valToEdit = ~ENIT_BIT & prevVal;
  writeRegister(baseAddress, 0, valToEdit);
}

/*Acknowledges the interrupt and sets the bit value to 0*/
void intervalTimer_ackInterrupt(uint8_t timerNumber) {
  uint32_t baseAddress = baseAddressLocator(timerNumber);
  uint32_t valToWrite = orOneBit(baseAddress, 0, T0INT_BIT);
  writeRegister(baseAddress, 0, valToWrite);
}
