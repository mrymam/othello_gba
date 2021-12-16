## othello in GBA

## コンパイル
```
$ /opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc  -mthumb -mthumb-interwork -c hello.c
$ /opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc -specs=gba.specs -mthumb -mthumb-interwork hello.o -o hello.elf
$ /opt/devkitpro/devkitARM/bin/arm-none-eabi-objcopy -O binary hello.elf hello.gba
$ gbafix hello.gba
```

```
$ sudo -E make
```

## Env

```
export DEVKITARM=<path to>devkitARM
export DEVKITPRO=<path to>devkitPro
```