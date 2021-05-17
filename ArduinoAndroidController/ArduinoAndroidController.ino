//https://www.arduino.cc/en/Guide/MKRWiFi1010/connecting-to-wifi-network
#include <WiFiNINA.h>


char ssid[] = "Arduino";        // your network SSID (name)
char pass[] = "binozoworks";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
  while (!Serial && millis() < 5000); //Warte bis der Serial Port verbunden ist
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Boote TCP System...");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Booten fehlgeschlagen: Kommunikation mit WiFi Modul fehlgeschlagen");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Info: Es gibt ein neues Firmware upgrade");
  }
  
  // print the network name (SSID);
  Serial.print("Es wurde ein Access Point auf Port 8140 erstellt: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Booten fehlgeschlagen: Creating access point failed");
    // don't continue
    while (true);
  }

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}

void loop() {
  // compare the previous status to the current status
  if(status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if(status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        String text = client.readString();
        //char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        //currentLine += c;
        if(text.equals("led an")){
          Serial.write("LED an\n");
          client.print(1);
          digitalWrite(LED_BUILTIN, HIGH);
        }
        if(text.equals("led aus")){
          Serial.write("LED aus\n");
          client.print(1);
          digitalWrite(LED_BUILTIN, LOW);
        }
        if(text.equals("Client bestaetigt.")){
          Serial.write("CLient verbunden\n");
          client.print(1);
          
        }
        client.println('Ok');
        /*if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

            int randomReading = analogRead(A1);
            client.print("Random reading from analog pin: ");
            client.print(randomReading);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }*/

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //digitalWrite(led, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          //digitalWrite(led, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("\nclient disconnected");
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
