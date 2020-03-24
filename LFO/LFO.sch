EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
Title "LFO"
Date ""
Rev ""
Comp "RetroLinear"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Davis Polito"
$EndDescr
Text Notes 7300 4550 0    50   ~ 0
Decoupling Capacitors
$Comp
L Device:CP C11
U 1 1 5E360101
P 9050 5150
F 0 "C11" H 9168 5196 50  0000 L CNN
F 1 "10uF" H 9168 5105 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3216-10_Kemet-I_Pad1.58x1.35mm_HandSolder" H 9088 5000 50  0001 C CNN
F 3 "~" H 9050 5150 50  0001 C CNN
	1    9050 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C13
U 1 1 5E361090
P 9550 5150
F 0 "C13" H 9668 5196 50  0000 L CNN
F 1 "10uF" H 9668 5105 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3216-10_Kemet-I_Pad1.58x1.35mm_HandSolder" H 9588 5000 50  0001 C CNN
F 3 "~" H 9550 5150 50  0001 C CNN
	1    9550 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 4950 9050 5000
Wire Wire Line
	8000 5850 8000 5600
Wire Wire Line
	8000 5600 8400 5600
Connection ~ 8000 5600
$Comp
L Regulator_Linear:L78L05_SO8 U1
U 1 1 5E24917E
P 7900 5300
F 0 "U1" H 7900 5542 50  0000 C CNN
F 1 "L78L05_SO8" H 7900 5451 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 8000 5500 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/15/55/e5/aa/23/5b/43/fd/CD00000446.pdf/files/CD00000446.pdf/jcr:content/translations/en.CD00000446.pdf" H 8100 5300 50  0001 C CNN
	1    7900 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	7900 5600 8000 5600
$Comp
L power:GND #PWR0120
U 1 1 5E264EAE
P 6550 5300
F 0 "#PWR0120" H 6550 5050 50  0001 C CNN
F 1 "GND" H 6555 5127 50  0000 C CNN
F 2 "" H 6550 5300 50  0001 C CNN
F 3 "" H 6550 5300 50  0001 C CNN
	1    6550 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5E2BE124
P 6750 5700
F 0 "C3" H 6865 5746 50  0000 L CNN
F 1 ".01uF" H 6865 5655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0201_0603Metric" H 6788 5550 50  0001 C CNN
F 3 "~" H 6750 5700 50  0001 C CNN
	1    6750 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 5300 6750 5300
Connection ~ 6750 5300
Wire Wire Line
	6750 5300 6750 5550
$Comp
L Device:C C2
U 1 1 5E2DA0EF
P 6750 5000
F 0 "C2" H 6865 5046 50  0000 L CNN
F 1 ".33uF" H 6865 4955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6788 4850 50  0001 C CNN
F 3 "~" H 6750 5000 50  0001 C CNN
	1    6750 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 5150 6750 5300
Wire Wire Line
	6750 4850 7500 4850
Wire Wire Line
	7900 5000 7900 4950
Wire Wire Line
	6750 5300 7600 5300
Wire Wire Line
	6750 5850 8000 5850
Wire Wire Line
	7500 4850 7500 4950
Wire Wire Line
	7500 4950 7900 4950
Connection ~ 7900 4950
Wire Wire Line
	7900 4950 7900 4850
$Comp
L MCU_Microchip_ATtiny:ATtiny44A-PU U2
U 1 1 5E63501A
P 1150 4350
F 0 "U2" H 621 4396 50  0000 R CNN
F 1 "ATtiny44A-PU" H 621 4305 50  0000 R CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 1150 4350 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc8183.pdf" H 1150 4350 50  0001 C CNN
	1    1150 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 5000 1750 4950
$Sheet
S 2000 3400 1100 250 
U 5E6A635F
F0 "Sheet5E6A635E" 50
F1 "PotInput.sch" 50
F2 "VCC" I L 2000 3450 50 
F3 "output" I L 2000 3600 50 
F4 "GND" I R 3100 3500 50 
$EndSheet
$Comp
L Connector:AVR-ISP-6 J1
U 1 1 5E1CE963
P 1100 2750
F 0 "J1" H 771 2846 50  0000 R CNN
F 1 "AVR-ISP-6" H 771 2755 50  0000 R CNN
F 2 "Connector:Tag-Connect_TC2030-IDC-NL_2x03_P1.27mm_Vertical" V 850 2800 50  0001 C CNN
F 3 " ~" H -175 2200 50  0001 C CNN
	1    1100 2750
	0    1    1    0   
$EndComp
$Sheet
S 2850 4600 500  150 
U 5E71F86B
F0 "Sheet5E71F86A" 50
F1 "filter.sch" 50
F2 "input" I L 2850 4650 50 
F3 "output" I R 3350 4700 50 
F4 "GND" I R 3350 4600 50 
F5 "POS" I L 2850 4750 50 
F6 "NEG" I L 2850 4700 50 
$EndSheet
$Sheet
S 2000 3650 1100 250 
U 5E721A82
F0 "sheet5E721A7E" 50
F1 "PotInput.sch" 50
F2 "VCC" I L 2000 3700 50 
F3 "output" I L 2000 3850 50 
F4 "GND" I R 3100 3750 50 
$EndSheet
$Sheet
S 2000 3900 1100 250 
U 5E721CB1
F0 "sheet5E721CAD" 50
F1 "PotInput.sch" 50
F2 "VCC" I L 2000 3950 50 
F3 "output" I L 2000 4100 50 
F4 "GND" I R 3100 4000 50 
$EndSheet
$Sheet
S 2050 4100 1100 250 
U 5E721EE0
F0 "sheet5E721EDC" 50
F1 "PotInput.sch" 50
F2 "VCC" I L 2050 4150 50 
F3 "output" I L 2050 4300 50 
F4 "GND" I R 3150 4200 50 
$EndSheet
Wire Wire Line
	2000 3600 1750 3600
Wire Wire Line
	1750 3600 1750 3750
Wire Wire Line
	1750 3850 2000 3850
Wire Wire Line
	2000 4100 1850 4100
Wire Wire Line
	1850 3950 1750 3950
Wire Wire Line
	1850 4100 1850 3950
Wire Wire Line
	1800 4050 1750 4050
Wire Wire Line
	1900 4200 1900 3950
Wire Wire Line
	1900 3450 2000 3450
Wire Wire Line
	1900 3700 2000 3700
Connection ~ 1900 3700
Wire Wire Line
	1900 3700 1900 3450
Wire Wire Line
	1900 3950 2000 3950
Connection ~ 1900 3950
Wire Wire Line
	1900 3950 1900 3700
Text GLabel 8400 5600 2    50   Input ~ 0
5VCC
Wire Wire Line
	1900 3450 1900 3150
Connection ~ 1900 3450
$Sheet
S 2850 4950 500  150 
U 5E7293B1
F0 "sheet5E7293AD" 50
F1 "filter.sch" 50
F2 "input" I L 2850 5000 50 
F3 "output" I R 3350 5050 50 
F4 "GND" I R 3350 4950 50 
F5 "POS" I L 2850 5100 50 
F6 "NEG" I L 2850 5050 50 
$EndSheet
Wire Wire Line
	2050 4200 2050 4150
Wire Wire Line
	1900 4200 2050 4200
Wire Wire Line
	1800 4050 1800 4300
Wire Wire Line
	1800 4300 2050 4300
Wire Wire Line
	1750 4450 2800 4450
Wire Wire Line
	2800 4450 2800 5000
Wire Wire Line
	2800 5000 2850 5000
Text GLabel 1900 3150 2    50   Input ~ 0
5VCC
Wire Wire Line
	1150 3450 1600 3450
Wire Wire Line
	700  2650 700  5250
Wire Wire Line
	700  5250 1150 5250
Connection ~ 1600 3450
Wire Wire Line
	1600 3450 1900 3450
Wire Wire Line
	1300 3150 1300 4250
Wire Wire Line
	1300 4250 1750 4250
Wire Wire Line
	2850 4650 1900 4650
Wire Wire Line
	1900 4650 1900 4850
Wire Wire Line
	1900 4850 1750 4850
Wire Wire Line
	1200 3150 1200 4350
Wire Wire Line
	1200 4350 1750 4350
Connection ~ 1750 4350
Wire Wire Line
	1750 4350 1800 4350
Wire Wire Line
	1100 3150 1100 4150
Wire Wire Line
	1100 4150 1750 4150
Wire Wire Line
	1000 3150 1000 5000
Wire Wire Line
	1000 5000 1750 5000
Text GLabel 3650 1650 2    50   Input ~ 0
GND
Text GLabel 3650 1550 2    50   Input ~ 0
POS
Text GLabel 3650 1750 2    50   Input ~ 0
NEG
Text GLabel 7900 4850 1    50   Input ~ 0
POS
Text GLabel 9050 5300 0    50   Input ~ 0
NEG
Text GLabel 9550 5000 0    50   Input ~ 0
POS
Wire Wire Line
	3100 3500 3200 3500
Wire Wire Line
	3200 3500 3200 3750
Wire Wire Line
	3200 3750 3100 3750
Wire Wire Line
	3200 4000 3100 4000
Wire Wire Line
	3200 3750 3200 4000
Connection ~ 3200 3750
Wire Wire Line
	3200 4000 3200 4200
Wire Wire Line
	3200 4200 3150 4200
Connection ~ 3200 4000
Wire Wire Line
	1600 2650 1600 3450
Wire Wire Line
	3350 4600 3550 4600
Wire Wire Line
	3550 4600 3550 4200
Wire Wire Line
	3550 4200 3200 4200
Connection ~ 3200 4200
Wire Wire Line
	3350 4950 3550 4950
Wire Wire Line
	3550 4950 3550 4600
Connection ~ 3550 4600
Text GLabel 1100 5250 0    50   Input ~ 0
DGND
Text GLabel 3800 3450 2    50   Input ~ 0
AGND
Wire Wire Line
	3200 3500 3800 3500
Wire Wire Line
	3800 3500 3800 3450
Connection ~ 3200 3500
Text GLabel 3400 1000 0    50   Input ~ 0
AGND
Text GLabel 3400 900  0    50   Input ~ 0
DGND
Wire Wire Line
	3400 1000 3550 1000
Wire Wire Line
	3550 900  3400 900 
Wire Wire Line
	3550 1000 3550 950 
Text GLabel 3800 950  2    50   Input ~ 0
GND
Wire Wire Line
	3800 950  3550 950 
Connection ~ 3550 950 
Wire Wire Line
	3550 950  3550 900 
$Comp
L Connector:Conn_01x03_Female J0
U 1 1 5E776BE0
P 3300 1650
F 0 "J0" H 3192 1325 50  0000 C CNN
F 1 "Conn_01x03_Female" H 3192 1416 50  0000 C CNN
F 2 "Connector:FanPinHeader_1x03_P2.54mm_Vertical" H 3300 1650 50  0001 C CNN
F 3 "~" H 3300 1650 50  0001 C CNN
	1    3300 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3500 1550 3650 1550
Wire Wire Line
	3500 1650 3650 1650
Wire Wire Line
	3500 1750 3650 1750
Text GLabel 2300 4850 0    50   Input ~ 0
POS
Text GLabel 2300 4750 0    50   Input ~ 0
NEG
Wire Wire Line
	2300 4750 2650 4750
Wire Wire Line
	2650 4750 2650 4700
Wire Wire Line
	2650 4700 2850 4700
Wire Wire Line
	2650 4750 2650 5050
Wire Wire Line
	2650 5050 2850 5050
Connection ~ 2650 4750
Wire Wire Line
	2300 4850 2750 4850
Wire Wire Line
	2750 4850 2750 4750
Wire Wire Line
	2750 4750 2850 4750
Wire Wire Line
	2750 4850 2750 5100
Wire Wire Line
	2750 5100 2850 5100
Connection ~ 2750 4850
Text GLabel 9550 5300 0    50   Input ~ 0
AGND
Text GLabel 9050 4950 0    50   Input ~ 0
AGND
$EndSCHEMATC
