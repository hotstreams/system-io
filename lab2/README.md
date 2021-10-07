# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** получить знания и навыки разработки драйверов блочных устройств для операционной системы

## Описание функциональности драйвера

- Драйвер блочного устройства создает виртуальный жесткий диск в оперативной памяти с размером 50Мбайт. 
- Созданный виртуальный диск содержит один первичный раздел размером 30Мбайт и один расширенный раздел, содержащий два логических раздела размером 10Мбайт.

## Инструкция по сборке

```
make
```

## Инструкция пользователя

```
sudo insmod lab2.ko
sudo rmmod lab2.ko
sudo parted /dev/mydisk
sudo mkfs.vfat /dev/mydisk1,5,6
```

## Примеры использования

```
sudo parted /dev/mydisk/

Number  Start   End     Size    Type      File system  Flags
 1      512B    31,5MB  31,5MB  primary
 2      31,5MB  52,4MB  21,0MB  extended               lba
 5      31,5MB  41,9MB  10,5MB  logical
 6      41,9MB  52,4MB  10,5MB  logical
 
 sudo mkfs.vfat /dev/mydisk1
 sudo mkfs.vfat /dev/mydisk5
 sudo mkfs.vfat /dev/mydisk6
 
 sudo parted /dev/mydisk/
 
 Number  Start   End     Size    Type      File system  Flags
 1      512B    31,5MB  31,5MB  primary   fat16
 2      31,5MB  52,4MB  21,0MB  extended               lba
 5      31,5MB  41,9MB  10,5MB  logical   fat16
 6      41,9MB  52,4MB  10,5MB  logical   fat16

sudo dd if=/dev/mydisk1 of=/dev/mydisk5 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,219409 s, 46,7 MB/s

sudo dd if=/dev/mydisk1 of=/dev/mydisk6 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,215202 s, 47,6 MB/s

sudo dd if=/dev/mydisk5 of=/dev/mydisk6 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,223168 s, 45,9 MB/s

sudo dd if=/dev/sda1 of=/dev/mydisk1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,475454 s, 21,5 MB/s

sudo dd if=/dev/sda1 of=/dev/mydisk5 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,21582 s, 47,4 MB/s

sudo dd if=/dev/sda1 of=/dev/mydisk6 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,207806 s, 49,3 MB/s

sudo dd if=/dev/mydisk1 of=/dev/sda1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,696882 s, 14,7 MB/s

sudo dd if=/dev/mydisk5 of=/dev/sda1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,811661 s, 12,6 MB/s

sudo dd if=/dev/mydisk6 of=/dev/sda1 bs=512 count=20000 oflag=direct
20000+0 records in
20000+0 records out
10240000 bytes (10 MB, 9,8 MiB) copied, 0,826562 s, 12,4 MB/s

```









