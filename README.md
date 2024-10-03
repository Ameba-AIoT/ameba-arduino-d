# Ameba Arduino SDK (ambd_arduino)


## 1. About `dev` & `master` branch

|Branch|Usage|json links|
|:-----|:-----|:-----|
|`master` | stable release| https://github.com/Ameba-AIoT/ameba-arduino-d/raw/master/Arduino_package/package_realtek_amebad_index.json|
|`dev`| merge `Pull Request` & early release| https://github.com/Ameba-AIoT/ameba-arduino-d/raw/dev/Arduino_package/package_realtek_amebad_early_index.json|


For all `release` information, please visit this [link](https://github.com/ambiot/ambd_arduino/releases).

</br>

## 2. How to add Realtek Ameba Arduino package to `Arduino IDE`:

> You can refer to this link to get started: http://www.amebaiot.com/ameba-arduino-getting-started/
> , or follow the steps below.

1. **Add Ameba Arduino SDK link to *Arduino IDE Additional Boards Manager***

    > Arduino IDE 1.6.5 and above versions support third party hardware so please make sure to use the latest Arduino IDE for better experience.

    Therefore, you need to add Ameba Arduino SDK link in 
    
    **"File" -> "Preferences" -> "Additional Boards Manager URLs:"**

    Copy and paste the following link into the field and click "OK",

    https://github.com/Ameba-AIoT/ameba-arduino-d/raw/master/Arduino_package/package_realtek_amebad_index.json

    > We also suggest to enable "Show verbose output" options on "compilation" and "upload" in Preference for easier debugging.
    
    </br>

2. **Install Ameba board in *"Board Manager"***

    Open **"Tools" -> "Board" -> "Board Manager"**, wait for it to update additional hardware configurations, then type **"ameba"** in the search bar, you will see Realtek Ameba in the list.

    Press **"Install"** to start the installation.
    
    </br>

3. **Select your Ameba model in *"Tools" -> "Board" -> "AmebaD ARM (32-bits) Boards"***

    > Make sure you select the correct model for your board, otherwise your program might not work properly 

    Now you are **ready** to develop and upload firmware onto Ameba.

    For more information, please refer to https://www.amebaiot.com/en/ameba-arduino-summary/
    
    </br>

## 3. Contributing to Ameba

Please spend 5 mins to read the **Coding Style** and **Contribution Guideline** at [here](https://github.com/Ameba-AIoT/ameba-arduino-d/wiki/Contributing-to-Ameba) before you contribute.
