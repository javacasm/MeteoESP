# MeteoESP

Estación meteorológica con ESP Wemos D1 Mini

## A tener en cuenta....

# ¡¡¡Funciona a 3.3V!!!

## V1

LCD I2C 16x2 [Librería](https://github.com/HobbytronicsPK/ESP8266-I2C-LCD1602) ([renombrando la clase](https://github.com/javacasm/ESP8266-I2C-LCD1602) para evitar conflictos)

Sensores:
* [MQ7](https://www.sparkfun.com/products/9403): ¿3.3V?, Concentración de CO (analógico) ([Ejemplo](https://learn.sparkfun.com/tutorials/hazardous-gas-monitor?_ga=2.44603748.1795368896.1497390302-1920885004.1497390302))
* DHT22: 5V, temperatura y humedad



## V2

LCD Nokia 5110 [Librería](https://github.com/mcauser/WeMos-D1-mini-Nokia-5110-PCD8544)

* [BME280](https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout?view=all): 3.3V mide humedad, presión barométrica y temperatura (I2C) ([Ejemplo](https://github.com/TheAustrian/Wemos-D1-Mini-BME280-Weather-Station))

ADS1115: ADC externo


## Referencias

* [Sensores de gases](https://playground.arduino.cc/Main/MQGasSensors)
* [Estación de medida de  calidad del aire](https://biohackingitm.wordpress.com/2017/06/13/avences-en-nuestra-central-de-monitoreo-de-calidad-de-aire-de-bajo-costo-con-sensores-mq-parte-i/)
