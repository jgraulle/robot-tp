Robot - Embedded: TP1
=====================

Setup
=====

- For Windows only:
  - from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads download
    CP210x_Universal_Windows_Driver.zip
  - Unzip this file, right clic on "silabser.inf" then "Install"
  - Do not forget to follow "epitaGitlabSshKey.pdf" and execute

    `git clone git@gitlab.cri.epita.fr:jeremie.graulle/esp-idf-cxx.git` from a git console
- For Personal Linux (not Epita computer):
  - VS code installation: (add PPA and install from

    https://doc.ubuntu-fr.org/visual_studio_code#via_ppa_de_microsoft or manual download from https://code.visualstudio.com/download and install)
  - To have access to the USB:
    ```
      echo -e '# CP210X USB UART\nATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", ' \
        'MODE:="0666", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"' \
        | sudo tee /etc/udev/rules.d/97-cp210x.rules
    ```
- Run Visual Studio Code
- In menu "View" option "Extensions", search and install the extension `Espressif IDF`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Configure ESP-IDF extension`.
- Choose `Express`
- Leave `Github` in "Select download server"
- Select `v5.4 (release version)` in "Select ESP-IDF version:"
- Leave `/home/<user>/esp` in "Enter ESP-IDF container directory"
- Leave `/home/<user>/.espressif` in "Enter ESP-IDF Tools directory (IDF_TOOLS_PATH)"
- Leave `/usr/bin/python3` in "Select Python version:"
- Clic on `install`
- Search and install the extension `C/C++ Extension Pack`

1. First C Project example
==========================

Purpose: To be able to import an ESP-IDF example, build, flash and see debug message.

Step 1.1
--------
- In menu "View" option "Command Palette", type and select `ESP-IDF: Show Examples Projects`.
- Select `esp-idf/get-started/blink`
- Clic on `Create project using example blink`
- Select your workspace folder then clik on `Open`
- In menu "View" option "Command Palette", type and select `ESP-IDF: SDK Configuration editor`
- In `Serial flasher config/Flash size` select `4MB`
- In `Example Configuration/Blink GPIO number` type `12`
- Clic `Save`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Build your project`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Select port to use` select
`/dev/ttyUSB0`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Flash your project` then `UART`
- In menu "View" option "Command Palette", type and select `ESP-IDF: Monitor your device`
- Use Ctrl+T then x to exit from monitor (usefull when reboot in loop)

Step 1.2
--------
- Update the period by menuconfig
- Open the main.c and updated period directly in C source

2. First C++ Project example
============================

Purpose: To be able to import an ESP-IDF module example and use C++ instead of C.
Definition: a ESP-IDF module is an external lib can be easilly add to a ESP-IDF project.

Step 2.1
--------
- From console in your workspace, `code . &`
- In menu "View" option "Command Palette", type and select `ESP-IDF: open ESP-IDF Terminal`
- execute `idf.py create-project-from-example espressif/esp-idf-cxx=1.0.0-beta:blink_cxx`
- In menu "File" option "Folder" Select `blink_cxx`
- In "Explorer" open `main/main.cpp` and update `GPIONum(26)` with `GPIONum(12)`
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

Step 2.2
--------
- Open the main.cpp and updated period directly in C++ source

3. First new C++ Project
========================

Purpose: To be able to create a new project and import a custom ESP-IDF module dedicated for this
robot.

Step 3.1
--------
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
  - remplace `espressif/esp-idf-cxx:` by `jgraulle/esp-idf-cxx-tp1:`
  - remplace `version: 1.0.1-beta` by `git: git@gitlab.cri.epita.fr:jeremie.graulle/esp-idf-cxx-tp1.git`
  - remove `pre_release: true`
- remove `main/main.c` and in `main/CMakeLists.txt` replace `main.c` by `main.cpp`
- Do the same menuconfig as `blink_cxx`
- Do Build, Flash and Monitor like previous project

Step 3.2
--------
- Open the main.cpp and updated period directly in C++ source
- Create new personnal private project `ssie-robot-embedded` on the Epita gitlab.
- Add your two-person team and "jeremie.graulle" as "maintainer"
- Clone or add remote this project in local
- Do not commit generated files. Create a .gitignore file and add:
   - build/
   - sdkconfig
   - sdkconfig.old
   - .vscode
   - managed_components
   - dependencies.lock
- Commit all sources files of this project in main branch.

4. Add new hardware component for ADC
=====================================

Step 4.1
--------

- Download robot-embedded-TP1-adc-header.tar.gz and extract into the project created in previous
step "First new C++ Project" next to "main.cpp".
- Create a new C++ body C++ file adc_cxx.cpp and add stub (empty body function) for
each header function to make it compile again (do not forget to add body in CMakeLists.txt)

Step 4.2
--------

- Create a `tp1` branche to commit this step
- From official SDK help: https://docs.espressif.com/projects/esp-idf
- Select the stable version (v5.1.1)
- Goto "API Reference" and look for ADC
- Update file adc_cxx.cpp and adc_cxx.hpp to have reel implementation. You can have a look in other
hardware C++ class in `jgraulle/esp-idf-cxx-tp1:` (include/gptimer_cxx.hpp,
include/pulse_counter_cxx.hpp) to have the same behavior (use CHECK_THROW to convert return code
to exception).
- Update main to add a new thread, in this thread you will read battery voltage every seconds and
print it into serial console (use ADC on GPIO 36).
- Commit and push your modifications
- Create a merge request from tp1 branch to main.
- Add jeremie.graulle as reviewer of this MR
