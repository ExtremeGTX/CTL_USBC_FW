## How is the code generated

Code in this folder is generated using [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html). STM3CubeMX helps in configuring the pins and peripherals. It adds relevant drivers too.


## How to re-generate code and/or update configuration

* To reconfigure pins and peripheral for this project, please open [reva-firmware.ioc](/firmware/generated/reva-firmware.ioc) in STM32CubeMX. Make sure `Keep User Code when re-generating` is set ticked, it can be found under `Project Manager` tab -> `Code Generator` menu. 

* Once you are happy with the changes made to the configuration, press `GENERATE CODE` and the changes should be reflected in this folder.

* In order to interact/add custom logic to this generated code, one can add code between `/* USER CODE BEGIN */` and `/* USER CODE END */` comment blocks. If extra configuration is added and code needs to be regenerated, code between the aforementioned blocks wont be deleted. 

