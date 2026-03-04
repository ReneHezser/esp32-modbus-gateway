#ifndef CONFIG_H
    #define CONFIG_H
    #include <Arduino.h>
    #include <Preferences.h>
    #define debugSerial Serial
    #ifndef MODBUS_SERIAL
    #define MODBUS_SERIAL Serial2
    #endif
    #define modbusSerial MODBUS_SERIAL
    #define DEBUG

    #ifndef HOSTNAME
    #define HOSTNAME "modbus2wifi"
    #endif

    class Config{
        private:
            Preferences *_prefs;
            int16_t _tcpPort;
            uint32_t _tcpTimeout;
            unsigned long _modbusBaudRate;
            uint32_t _modbusConfig;
            int8_t _modbusRtsPin;
            unsigned long _serialBaudRate;
            uint32_t _serialConfig;
            String _webPassword;
            String _hostname;
            int16_t _txpin;
            int16_t _rxpin;
            String _ipAdr;
            String _subnetAdr;
            String _gatewayAdr;
            String _DNSAdr;
        public:
            Config();
            void begin(Preferences *prefs);
            uint16_t getTcpPort();
            void setTcpPort(uint16_t value);
            uint32_t getTcpTimeout();
            void setTcpTimeout(uint32_t value);
            uint32_t getModbusConfig();
            unsigned long getModbusBaudRate();
            void setModbusBaudRate(unsigned long value);
            uint8_t getModbusDataBits();
            void setModbusDataBits(uint8_t value);
            uint8_t getModbusParity();
            void setModbusParity(uint8_t value);
            uint8_t getModbusStopBits();
            void setModbusStopBits(uint8_t value);
            int8_t getModbusRtsPin();
            void setModbusRtsPin(int8_t value);
            uint32_t getSerialConfig();
            unsigned long getSerialBaudRate();
            void setSerialBaudRate(unsigned long value);
            uint8_t getSerialDataBits();
            void setSerialDataBits(uint8_t value);
            uint8_t getSerialParity();
            void setSerialParity(uint8_t value);
            uint8_t getSerialStopBits();
            void setSerialStopBits(uint8_t value);
            String getWebPassword();
            void setWebPassword(String value);
            String getHostname();
            void setHostname(String value);
            uint16_t gettxpin();
            void settxpin(uint16_t value);
            uint16_t getrxpin();
            void setrxpin(uint16_t value);
            String getipAdr();
            void setipAdr(String value);
            String getSubnetAdr();
            void setSubnetAdr(String value);
            String getGatewayAdr();
            void setGatewayAdr(String value);
            String getDNSAdr();
            void setDNSAdr(String value);
    };
    #ifdef DEBUG
    #define dbg(x...) debugSerial.print(x);
    #define dbgln(x...) debugSerial.println(x);
    #else /* DEBUG */
    #define dbg(x...) ;
    #define dbgln(x...) ;
    #endif /* DEBUG */
#endif /* CONFIG_H */