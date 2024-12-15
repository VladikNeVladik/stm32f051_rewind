Настройка среды
=================

Чтобы использовать этот репозиторий, необходимо установить следующие ниже ресурсы и утилиты.

## Предварительная установка зависимостей

### Linux

Некоторые требования к установке и использованию этого программного обеспечения следующие (для Ubuntu):

- Обновите систему (необязательный шаг):
```
sudo apt-get update
sudo apt upgrade
```

#### Установите все необходимые пакеты

- Установите git:
```
sudo apt-get install git
```

- Установите libusb:
```
sudo apt-get install libusb-1.0-0-dev
```

- Установите cmake:
```
sudo apt-get install cmake
```

### Windows

Запустите Windows PowerShell

- Установите Scoop:
```
  Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
  irm get.scoop.sh | iex
```

- Добавьте необходимые репозитории:
```
  scoop bucket add main
  scoop bucket add extras
```

- Установите основные пакеты:
```
  scoop install cmake
  scoop install git
```

## Установка инструментария разработки

### Установка ARM developer toolchain

ARM GNU Embedded Toolchain - это готовый к использованию набор инструментов с открытым исходным кодом для программирования на C, C++ и ассемблере, предназначенный для процессоров семейства ARM Cortex-M и Cortex-R.

Необходимые PPA в нашем случае предоставлены от GCC ARM Embedded.

#### Linux (Ubuntu 20.04)

Этот случай немного посложнее: перейдите на веб-сайт [GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) и установите архив с инструментами разработки последней версии (например, **gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2**).

Распакуйте архив в каталог установки, вот так:

```
cd ${INSTALL_DIR} && tar xjf gcc-arm-none-eabi-_version_-linux.tar.bz2
```

*${INSTALL_DIR}* - это директория, куда небходимо установить ваши файлы.
*\_version\_* - это версия arm toolchain.

Затем экспортируйте путь в файл \~/.bashrc (shell-скрипт, выполняемый при каждом запуске терминала):

```
echo "export PATH="$PATH:${INSTALL_DIR}/gcc-arm-none-eabi-*/bin"" >> ~/.bashrc && source ~/.bashrc
```

Вместо \* пропишите полное название вашей директории (версия gnu arm toolchain).

Чтобы проверить, что все успешно установилось, напишите в терминале следующее:

```
arm-none-eabi-gcc --version
```

Если все корректно, то вы увидете версию toolchain-а.

#### Linux (Ubuntu 18.04 и более ранние версии)

Для того, чтобы установить необходимый инструментарий разработки, пропишите следующее:

```
sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
sudo apt-get update
sudo apt-get install gcc-arm-embedded
```

#### Linux (Fedora)

Аналогично предыдущему пункту:

```
sudo yum install arm-none-eabi-gcc-cs
sudo yum install arm-none-eabi-binutils-cs
sudo yum install arm-none-eabi-newlib
```

#### Windows

```
scoop install gcc-arm-none-eabi
```

#### MacOS

Аналогично:

```
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

Если все успешно установилось, попробуйте вызвать автодополнение:

```
arm-none (press tab twice)
```

В результате увидите список установленных двоичных файлов:

```
arm-none-eabi-addr2line   arm-none-eabi-gcc-ar      arm-none-eabi-nm
arm-none-eabi-ar          arm-none-eabi-gcc-nm      arm-none-eabi-objcopy
arm-none-eabi-as          arm-none-eabi-gcc-ranlib  arm-none-eabi-objdump
arm-none-eabi-c++         arm-none-eabi-gcov        arm-none-eabi-ranlib
arm-none-eabi-c++filt     arm-none-eabi-gcov-dump   arm-none-eabi-readelf
arm-none-eabi-cpp         arm-none-eabi-gcov-tool   arm-none-eabi-size
arm-none-eabi-elfedit     arm-none-eabi-gdb         arm-none-eabi-strings
arm-none-eabi-g++         arm-none-eabi-gprof       arm-none-eabi-strip
arm-none-eabi-gcc         arm-none-eabi-ld
arm-none-eabi-gcc-7.3.1   arm-none-eabi-ld.bfd
```

### Установка QEMU

#### Краткий обзор

*QEMU* - это универсальный машинный эмулятор и виртуализатор с открытым исходным кодом. Наиболее распространенным способом является “эмуляция системы”, когда она предоставляет виртуальную модель всей машины (процессор, память и эмулируемая периферия). Так, эмулятор QEMU может быть использован даже для запуска гостевой ОС.

**xPack QEMU Arm** - кроссплатформенный двоичный дистрибутив общедоступного проекта QEMU с открытым исходным кодом, который будет использоваться в наших лабах.

Перейдите по ссылке [xPACK-QEMU-ARM releases](https://github.com/xpack-dev-tools/qemu-arm-xpack/releases/) и установите архив последней версии, соответствующей вашей операционной системе (например, **xpack-qemu-arm-7.1.0-1-linux-x64.tar.gz**).

#### Распаковка

##### Linux

Чтобы установить вручную *xPack QEMU Arm*, распакуйте архив и перейдите в `~/.local/xPacks/qemu-arm/xpack-qemu-arm-_version_`:

!!! Вместо *\_version\_* вставьте свою версию, вместо \* пропишите полное имя директории (версия xpack-qemu-arm toolchain).

```
mkdir -p ~/.local/xPacks/qemu-arm
cd ~/.local/xPacks/qemu-arm

tar xvf ~/${INSTALL_DIR}/xpack-qemu-arm-_version_.tar.gz
chmod -R -w xpack-qemu-arm-*
```

Затем экспортируйте обновлённый путь в \~/.bashrc:

```
echo "export PATH="$PATH:~/.local/xPacks/qemu-arm/xpack-qemu-arm-*/bin"" >> ~/.bashrc && source ~/.bashrc
```

##### Windows

Распакуйте установленный архив в удобное место и добавьте путь к папке bin в PATH. Это можно сделать так:
1. Зайти по порядку в "система"\"дополнительные параметры системы"\"переменные среды".
2. Найти на экране поле PATH, нажать кнопку изменить.
3. Добавить соответствующий путь к переменной PATH.

#### Проверка

Чтобы проверить, запускается ли установленный вручную QEMU, попробуйте посмотреть версию:

```
qemu-system-gnuarmeclipse --version
```

### Установка ST-link

Stlink package - это программное обеспечение для связи с программатором ST-Link, которое работает со многими другими палатами STM32. На плате STM32F051-Discovery программатор реализован на основе микроконтроллера STM32F103C8T6 (blue pill).

#### Windows

Для начала скачайте драйвер ST-Link [здесь](https://www.st.com/en/development-tools/stsw-link009.html#get-software).
Затем установите [приложение](https://www.st.com/en/development-tools/stsw-link004.html).
В дальнейшем для того чтобы, например, выгрузить код в память микроконтроллера, Вам будет необходимо использовать именно это приложение.

#### Linux (Ubuntu)

Склонируйте репозиторий st-link:

```
git clone https://github.com/texane/stlink.git
```

Все команды должны выполняться из корневого каталога репозитория:

```
cd stlink
make
```

Далее выполните следующую (**злую!**) команду с правами root:

```
sudo make install
```

По некоторым причинам дополнительная библиотека может не установиться автоматически, поэтому возможно пользователям **Linux** придётся переместить библиотеку в системный каталог.

```
cd build/Release/lib
sudo cp libstlink.so.1 /usr/lib/
```

#### Linux (Fedora)

```
sudo yum install stlink
```

##### MacOS
```
brew install stlink
```

Dunzo! Welcome aboard!
