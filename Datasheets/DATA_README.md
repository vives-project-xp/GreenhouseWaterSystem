# **DATASHEET OVERVIEUW**

1. Pomp

    | Connections|Description|Connected to  |
    | :---:      |    :----: |   :---:      |
    | Red        | 12V       | shakel relais|
    | Black      | Ground    | shakel relais|
    [More data...](./Recources/gebruiksaanwijzing-2386386-toolcraft-to-7159158-laagspanning-drukwaterpomp-1020-lh-12-vdc.pdf)


2. vlotter sensor

    | Connections|Description|Connected to|
    | :---:      |    :----: |   :---:    |
    | Red        | 3.3-5V    | Esp vcc    |
    | Black      | ground    | Pin ...    |
    [More data...](./Recources/vlotterSwitch.png)


3. pH-sensor

    | Connections|Description|Connected to  |
    | :---:      |    :----: |   :---:      |
    | BNC        | output    | pH-versterker| 
    [More data...](./Recources/pH_Sensor.png)


4. Versterker (pH-sensor)

    | Connections|Description                |Connected to|
    | :---:      |    :----:                 |   :---:    |
    | BNC        | input                     | pH-sensor  | 
    | TO         | Temperature Out           | /          |
    | DO         | Digital Out Trigger (3.3V)| /          |
    | PO         | 0-5V Analog Out for PH    | pin...     |
    | GND        | Ground                    | ESp ground |
    | GND        | Ground                    | /          |
    | VCC        |  5VDC Input Power         | 5V line    |
    [More data...](./Recources/pH_Sensor_versterker.png)


5. EC-sensor

    | Connections|Description|Connected to  |
    | :---:      |    :----: |   :---:      |
    | BNC  probe | output    | EC-versterker|
    | BNC        | input     | EC-probe     |
    | -          | ground    | esp ground   |
    | +          | VCC 3.3-5V| esp vcc      |
    | A          | analog out| Pin..        |
    [More data...](./Recources/EcSensor.pdf)


6. shakel relais

    | Connections|Description   |Connected to|
    | :---:      |    :----:    |   :---:    |
    | VCC        | 5V           | 5V line    |
    | GND        | ground       | esp ground |
    | IN         | input        | Pin...     |
    | COM        | common cable | red (pomp) |
    | NO         | normal open  | /          |
    | NC         | normal closed| 12V line   |
    [More data...](./Recources/schakelRelais.png) 


7. level shifter 

    | Connections|Description       |Connected to|
    | :---:      |    :----:        |   :---:    |
    | HV1        | high level 1     | pin...     |
    | HV2        | high level 2     | /          |
    | HV         | high level select| 5V line    |
    | GND        | ground           | esp ground |
    | HV3        | high level 3     | /          |
    | HV4        | high level 4     | /          |  
    | LV1        | Low  level 1     | pin...     |
    | LV2        | Low  level 2     | /          |
    | LV         | Low  level select| 3.3V esp   |
    | GND        | ground           | esp ground |
    | LV3        | Low  level 3     | /          |
    | LV3        | Low  level 4     | /          | 
    [More data...](./Recources/levelShifter.png)


8. transformator 12V

    | Connections|Description|Connected to  |
    | :---:      |    :----: |   :---:      |
    | IN         | 230VAC IN | net          |
    | GND        | ground    | net          |
    | OUT        | 12VDC     | shakel relais|
    | GND        | ground    | ground       |
    [More data...](./Recources/Transformator.png)