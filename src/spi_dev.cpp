#include "burner_power.h"
#include "spi_dev.h"
#include <SPI.h>

SPISettings spi_dev::spisettings;
int spi_dev::chipselectpin;

static constexpr const char *TAG = "spi_dev";


spi_dev::spi_dev() {}

void spi_dev::begin(SPISettings s, int cs) {
    spisettings = s;
    chipselectpin = cs;
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
    delay(100);
    SPI.begin(SCK, MISO, MOSI, -1);
}

uint32_t spi_dev::readRegister_16(uint8_t addr)
{
    //uint8_t tx_data = (addr << 2) | 0x01;
    uint8_t tx_data = addr | 0x01;
    SPI.beginTransaction(spisettings);
    digitalWrite(chipselectpin, LOW);
    // The transfers are always a step behind, so the transfer reads the previous value/command
    SPI.write(tx_data);
    uint32_t value = (uint32_t)SPI.transfer16(0);
    digitalWrite(chipselectpin, HIGH);
    SPI.endTransaction();

    BLog_d(TAG, "Read register24 0x%02x = 0x%08x", (uint32_t)addr, value);
    //  retvalue is the PREVIOUS value stored in the register
    return value;
}

uint32_t  spi_dev::readRegister_24(uint8_t addr)
{
    uint8_t tx_data = addr | 0x01;
    uint8_t rx_data[4];

    SPI.beginTransaction(spisettings);
    digitalWrite(chipselectpin, LOW);
    SPI.write(tx_data);
    rx_data[0] = SPI.transfer(0);
    rx_data[1] = SPI.transfer(0);
    rx_data[2] = SPI.transfer(0);
    digitalWrite(chipselectpin, HIGH);
    SPI.endTransaction();

    uint32_t value = ((uint32_t)rx_data[0] << 16) | ((uint32_t)rx_data[1] << 8) | ((uint32_t)rx_data[2]);

    BLog_d(TAG, "Read register24 0x%02x = 0x%08x", (uint32_t)addr, value);

    return value;
}

uint32_t  spi_dev::writeRegister(uint8_t addr, uint16_t data)
{
    uint32_t rx_data;
    uint8_t write_val;
    
    SPI.beginTransaction(spisettings);
    digitalWrite(chipselectpin, LOW);
    // The transfers are always a step behind, so the transfer reads the previous value/command
    SPI.write(addr);
    rx_data = (uint32_t)SPI.transfer16(data);
    digitalWrite(chipselectpin, HIGH);
    SPI.endTransaction();

    BLog_d(TAG, "write register16 0x%02x = 0x%08x", (uint32_t)addr, rx_data);
    //  retvalue is the PREVIOUS value stored in the register
    return rx_data;
}
