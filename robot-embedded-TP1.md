Setup
=====

- For Windows only:
  - from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads download CP210x_Universal_Windows_Driver.zip
  - Unzip this file, right clic on "silabser.inf" then "Install"
  - Do not forget to follow "epitaGitlabSshKey.pdf" and execute `git clone git@gitlab.cri.epita.fr:jeremie.graulle/esp-idf-cxx.git` from a git console
- For Personal Linux:
  - VS code installation: (add PPA and install from https://doc.ubuntu-fr.org/visual_studio_code#via_ppa_de_microsoft or manual download from https://code.visualstudio.com/download and install)
  - To have access to the USB: `echo -e '# CP210X USB UART\nATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"' | sudo tee /etc/udev/rules.d/97-cp210x.rules`
- Run Visual Studio Code
- In menu "View" option "Extensions", search and install the extension `Espressif IDF`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Configure ESP-IDF extension`.
- Select `Global` for "Select where to save these settings:"
- Choose `Express`
- Select `v5.1.1 (release version)` in "Select ESP-IDF version:"
- Leave `/home/<user>/esp` in "Enter ESP-IDF container directory"
- Leave `/home/<user>/.espressif` in "Enter ESP-IDF Tools directory (IDF_TOOLS_PATH)"
- Leave `/usr/bin/python3` in "Select Python version:"
- Clic on `install`
- Search and install the extension `C/C++ Extension Pack`

First C Project example
=======================

Purpose: To be able to import an ESP-IDF example, build, flash and see debug message.

Step 1
------

Connect a LED to the ESP32 GPIO33 in a breadboard with a resistor like in the following diagram:

![LED ESP32 diagram](build/ledDiagram.png)

Step 2
------
- In menu "View" option "Command Palette", type and select `ESP-IDF: Show Examples Projects`.
- Select `esp-idf/get-started/blink`
- Clic on `Create project using example blink`
- Select your workspace folder then clik on `Open`
- In menu "View" option "Command Palette", type and select `ESP-IDF: SDK Configuration editor`
- In `Serial flasher config/Flash size` select `4MB`
- In `Example Configuration/Blink GPIO number` type `33`
- Clic `Save`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Build your project`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Select port to use` select
`/dev/ttyUSB0`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Flash your project` then `UART`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Monitor your device`
- Use Ctrl+T then x to exit from monitor (usefull when reboot in loop)

Step 3
------
- Update the period by menuconfig
- Open the main.c and updated period directly in C source

First C++ Project example
=========================

Purpose: To be able to import an ESP-IDF module example and use C++ instead of C.
Definition: a ESP-IDF module is an external lib can be easilly add to a ESP-IDF project.

Step 1
------
- From console in your workspace, `code . &`
- In menu "View" option "Command Palette", type and select `ESP-IDF: open ESP-IDF Terminal`
- execute `idf.py create-project-from-example espressif/esp-idf-cxx=1.0.0-beta:blink_cxx`
- In menu "File" option "Folder" Select `blink_cxx`
- In "Explorer" open `main/main.cpp` and update `GPIONum(26)` with `GPIONum(33)`
- In "Explorer" open `main/idf_component.yml`:
  - update `version: ^1.0.0` with `version: 1.0.1-beta`
  - add `pre_release: true` just below
  - remove `override_path: ../../../`
- In menu "View" option "Command Palette", type and select `ESP-IDF: SDK Configuration editor`
- In `Serial flasher config/Flash size` select `4MB`
- In `Compiler options/Enable C++ exceptions` check `Enable C++ exceptions` and
`Enable C++ run-time type info (RTTI)`
- Clic `Save`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Build your project`
- In menu "View" option "Command Palette", type and select
`ESP-IDF: Add vscode configuration folder`
- Do Flash and Monitor like previous project

Step 2
------
- Open the main.cpp and updated period directly in C++ source

First new C++ Project
=====================

Purpose: To be able to create a new project and import a custom ESP-IDF module dedicated for this
robot.

Step 1
------
- In menu "View" option "Command Palette", type and select `ESP-IDF: new Project`
- In "Project Name" type `robot-esp-idf`
- In "Enter Project directory" select your workspace
- In "Choose ESP-IDF Board" select `Custom board`
- In "Choose ESP-IDF Target" select `ESP32 module`
- In "Choose serial port" select `/dev/ttyUSB0`
- Click on `Choose Template`
- Select `template-app`
- Click on `Create project using template template-app`
- On popup "Project robot-esp-idf has been created. Open project in a new window?" Clic `Yes`
- From `blink_cxx` copy file `main/idf_component.yml` and `main/main.cpp`
- in `main/idf_component.yml`:
  - remplace `espressif/esp-idf-cxx:` by `jgraulle/esp-idf-cxx:`
  - remplace `version: 1.0.1-beta` by `git: git@gitlab.cri.epita.fr:jeremie.graulle/esp-idf-cxx.git`
  - remove `pre_release: true`
- remove `main/main.c` and in `main/CMakeLists.txt` replace `main.c` by `main.cpp`
- Do the same menuconfig as `blink_cxx`
- Do Build, Flash and Monitor like previous project

Step 2
------
- Open the main.cpp and updated period directly in C++ source

Step 3
------
- Create a new personal project on Epita gitlab and make 1 clean commit on a tp1 branch. Do
not commit generated files (update .gitignore file).
- Add jeremie.graulle as Maintainer of this project.

Add new hardware component for ADC
==================================

Step 1
------

Connect the ESP32 GPIO26 (DAC compatible) to the ESP32 GPIO34 (input only ADC compatible) throw a
resistor in a breadboard like in the following diagram:

![DAC/ADC ESP32 diagram](build/dacAdcDiagram.png)

Step 2
------

- Download robot-embedded-TP1-dac-adc-header.tar.gz and extract into the project created in previous
step "First new C++ Project" next to "main.cpp".
- Create a new C++ body C++ file dac_cxx.cpp for dac_cxx.hpp and add stub (empty body function) for
each header function to make it compile again (do not forget to add body in CMakeLists.txt)
- Make 1 clean commit on a tp1 branch

Step 3
------

- From official SDK help: https://docs.espressif.com/projects/esp-idf
- Select the stable version (v5.1.1)
- Goto "API Reference" and look for DAC
- Update file dac_cxx.cpp and dac_cxx.hpp to have reel implementation. You can have a look in other
hardware C++ class in `jgraulle/esp-idf-cxx:` (include/gptimer_cxx.hpp,
include/pulse_counter_cxx.hpp) to have the same behavior (use CHECK_THROW to convert return code
to exception).
- Update main to using DAC_2 (GPIO 26) with this class.
- Test on the breadboard with a voltmeter your code
- Make 1 clean commit on a tp1 branch

Step 4
------

- From official SDK help in "API Reference" and look for ADC
- Update file adc_cxx.cpp and adc_cxx.hpp to have reel implementation.
- Update main to add ADC1_6 (GPIO34) with this class to read the value set by the DAC and display
on console
- Test on the breadboard your code
- Make 1 clean commit on a tp1 branch

Step 5
------

- Create a merge request from tp1 branch to main.
