char ssid[]            = "SSID";
char pass[]            = "PASSWORD";
char auth[]            = "AUTH";
char pumpauth[]        = "AUTH2";
//char serveraddr[]          = "blynk-cloud.com";

IPAddress serveraddr  (172,   24,   1,   2);
unsigned int port      = 1234; //use your own port of the server

// If you dont want to use DHCP 
//IPAddress arduino_ip ( 192,  168,   0,  56);
//IPAddress dns_ip     ( 192,  168,   0,   1);
//IPAddress gateway_ip ( 192,  168,   0,   1);
//IPAddress subnet_mask(255, 255, 255,     0);

char targetCode1[9]  = "00000001"; // Button A
char targetCode2[9]  = "00000002"; // Button B
char targetCode3[9]  = "00000003"; // Button C
char targetCode4[9]  = "00000004"; // Button D
char targetCode5[9]  = "00000005"; // Flood 1
bool flood1 = 0;
char targetCode6[9]  = "00000006"; // Flood 2
bool flood2 = 0;
char targetCode7[9]  = "00000007"; // Flood 3
bool flood3 = 0;
