char ssid[]            = "SSID";
char pass[]            = "PW";
char auth[]            = "AUTH";
char pumpauth[]     = "PUMPAUTH";
//char serveraddr[]          = "blynk-cloud.com";

IPAddress serveraddr  (192,   192,   0,   2);
unsigned int port      = 8445; //use your own port of the server

// If you dont want to use DHCP 
//IPAddress arduino_ip ( 192,  168,   0,  56);
//IPAddress dns_ip     ( 192,  168,   0,   1);
//IPAddress gateway_ip ( 192,  168,   0,   1);
//IPAddress subnet_mask(255, 255, 255,     0);
