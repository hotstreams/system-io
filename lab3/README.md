# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** получить знания и навыки разработки драйверов сетевых интерфейсов для операционной системы

## Описание функциональности драйвера

Драйвер создает виртуальный интерфейс, который перехватывает UDP пакеты родительского интерфейса, адресуемые на конкретный порт. Информация выводится в кольцевой буфер, статистика в /proc/lab3.

## Инструкция по сборке

```
make
```

## Инструкция пользователя

```
sudo insmod lab3.ko
sudo rmmod lab3.ko
sudo ifconfig
traceroute --udp -p port ip
```

## Примеры использования

```
ifconfig
vni0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.0.147  netmask 255.255.255.0  broadcast 192.168.0.255
        inet6 fe80::9800:6785:8698:9471  prefixlen 64  scopeid 0x20<link>
        ether b8:86:87:57:74:1f  txqueuelen 1000  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

traceroute --udp -p 53 yandex.ru

cat /proc/lab3
Package count: 844
Passed count: 227


```
